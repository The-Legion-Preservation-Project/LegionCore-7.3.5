/*
 * This file is part of the TrinityCore Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "ScriptMgr.h"
#include "WorldSession.h"
#include "AccountMgr.h"
#include "Player.h"
#include "PlayerDefines.h"
#include "Config.h"
#include "Chat.h"
#include "Object.h"
#include "DatabaseEnv.h"

enum IndividualXPString
{
    IXP_HELLO = 100040,
    IXP_MODULE_DISABLED,
    IXP_RATES_DISABLED,
    IXP_COMMAND_VIEW,
    IXP_MAX_RATE,
    IXP_MIN_RATE,
    IXP_COMMAND_SET,
    IXP_COMMAND_DISABLED,
    IXP_COMMAND_ENABLED,
    IXP_COMMAND_DEFAULT
};

bool moduleEnabled;
uint32 currentRate;
uint32 maxRate;
uint32 defaultRate;

// Add IndividualXP Announcer
class IndividualXP_Announce : public PlayerScript
{
  public:
    IndividualXP_Announce() : PlayerScript("IndividualXP_Announcer") {}

    void OnLogin(Player* player, bool fistLogin) override
    {
        if (sConfigMgr->GetBoolDefault("IndividualXP.Announce", true))
        {
            ChatHandler(player->GetSession()).PSendSysMessage(IXP_HELLO);
        }
    }
};

class IndividualXP : public PlayerScript
{
  public:
    IndividualXP() : PlayerScript("IndividualXP") {}

    void OnLogin(Player* player, bool fistLogin) override
    {
        QueryResult result = CharacterDatabase.Query(Trinity::StringFormat("SELECT `Rate` FROM `individual_xp` WHERE `CharacterGUID` = %u", player->GetGUID().GetCounter()).c_str());
        if (!result)
        {
            defaultRate = sConfigMgr->GetIntDefault("IndividualXP.DefaultRate", 1);
            maxRate = sConfigMgr->GetIntDefault("IndividualXP.MaxRate", 10);
        }
        else
        {
            Field* fields = result->Fetch();
            defaultRate = fields[0].GetInt32();
        }
    }

    void OnLogout(Player* player) override
    {
        if (currentRate != 0)
        {
            CharacterDatabase.Query(Trinity::StringFormat("REPLACE INTO `individual_xp` (`CharacterGUID`, `Rate`) VALUES ('%u', '%u');", player->GetGUID().GetCounter(), currentRate).c_str());
        }
    }

    void OnGiveXP(Player* player, uint32& amount, Unit* /*victim*/) override
    {
        if (moduleEnabled)
            amount *= currentRate;
    }
};

class IndividualXPCommandScript : public CommandScript
{
  public:
    IndividualXPCommandScript() : CommandScript("IndividualXPCommand") {}

    std::vector<ChatCommand> GetCommands() const override
    {
        // clang-format off
        static std::vector<ChatCommand> IndividualXPCommandTable =
        {
            { "enable",      SEC_PLAYER,        false,    &HandleEnableCommand,     "" },
            { "disable",     SEC_PLAYER,        false,    &HandleDisableCommand,    "" },
            { "view",        SEC_PLAYER,        false,    &HandleViewCommand,      "" },
            { "set",         SEC_PLAYER,        false,    &HandleSetCommand,      "" },
            { "default",     SEC_PLAYER,        false,    &HandleDefaultCommand,      "" },
            { "",            SEC_PLAYER,        false,    &HandleXPCommand,      "" },
        };
        static std::vector<ChatCommand> IndividualXPBaseTable =
        {
            { "xp",     SEC_PLAYER,    false,    nullptr,                   "", IndividualXPCommandTable },
        };
        // clang-format on
        return IndividualXPBaseTable;
    }

    static bool HandleXPCommand(ChatHandler* handler, char const* args)
    {
        if (!moduleEnabled)
        {
            handler->PSendSysMessage(IXP_MODULE_DISABLED);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (!*args)
            return false;

        return true;
    }

    static bool HandleViewCommand(ChatHandler* handler, char const* /*args*/)
    {
        if (!moduleEnabled)
        {
            handler->PSendSysMessage(IXP_MODULE_DISABLED);
            handler->SetSentErrorMessage(true);
            return false;
        }

        Player* me = handler->GetSession()->GetPlayer();
        if (!me)
            return false;

        if (me->HasFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_NO_XP_GAIN))
        {
            handler->PSendSysMessage(IXP_RATES_DISABLED);
            handler->SetSentErrorMessage(true);
            return false;
        }
        else
        {
            ChatHandler(handler->GetSession()).PSendSysMessage(handler->GetTrinityString(IXP_COMMAND_VIEW), currentRate);
        }
        return true;
    }

    // Set Command
    static bool HandleSetCommand(ChatHandler* handler, char const* args)
    {
        if (!moduleEnabled)
        {
            handler->PSendSysMessage(IXP_MODULE_DISABLED);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (!*args)
            return false;

        Player* me = handler->GetSession()->GetPlayer();
        if (!me)
            return false;

        uint32 rate = (uint32)atol(args);
        if (rate > maxRate)
        {
            handler->PSendSysMessage(IXP_MAX_RATE, maxRate);
            handler->SetSentErrorMessage(true);
            return false;
        }
        else if (rate == 0)
        {
            handler->PSendSysMessage(IXP_MIN_RATE);
            handler->SetSentErrorMessage(true);
            return false;
        }

        currentRate = rate;
        ChatHandler(handler->GetSession()).PSendSysMessage(handler->GetTrinityString(IXP_COMMAND_SET), rate);
        return true;
    }

    // Disable Command
    static bool HandleDisableCommand(ChatHandler* handler, char const* /*args*/)
    {
        if (!moduleEnabled)
        {
            handler->PSendSysMessage(IXP_MODULE_DISABLED);
            handler->SetSentErrorMessage(true);
            return false;
        }

        Player* me = handler->GetSession()->GetPlayer();
        if (!me)
            return false;

        // Turn Disabled On But Don't Change Value...
        me->SetFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_NO_XP_GAIN);
        ChatHandler(handler->GetSession()).PSendSysMessage("%s", handler->GetTrinityString(IXP_COMMAND_DISABLED));
        return true;
    }

    // Enable Command
    static bool HandleEnableCommand(ChatHandler* handler, char const* /*args*/)
    {
        if (!moduleEnabled)
        {
            handler->PSendSysMessage(IXP_MODULE_DISABLED);
            handler->SetSentErrorMessage(true);
            return false;
        }

        Player* me = handler->GetSession()->GetPlayer();
        if (!me)
            return false;

        me->RemoveFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_NO_XP_GAIN);
        ChatHandler(handler->GetSession()).PSendSysMessage("%s", handler->GetTrinityString(IXP_COMMAND_ENABLED));
        return true;
    }

    // Default Command
    static bool HandleDefaultCommand(ChatHandler* handler, char const* /*args*/)
    {
        if (!moduleEnabled)
        {
            handler->PSendSysMessage(IXP_MODULE_DISABLED);
            handler->SetSentErrorMessage(true);
            return false;
        }

        Player* me = handler->GetSession()->GetPlayer();
        if (!me)
            return false;

        currentRate = defaultRate;
        ChatHandler(handler->GetSession()).PSendSysMessage(handler->GetTrinityString(IXP_COMMAND_DEFAULT), defaultRate);
        return true;
    }
};

// Add all scripts in one
void AddModIndXPScripts()
{
    moduleEnabled = sConfigMgr->GetBoolDefault("IndividualXP.Enabled", true);
    maxRate = sConfigMgr->GetBoolDefault("IndividualXP.MaxRate", 10);
    defaultRate = sConfigMgr->GetBoolDefault("IndividualXP.DefaultRate", 1);

    new IndividualXP_Announce();
    new IndividualXP();
    new IndividualXPCommandScript();
}
