/*
SET 
@ID = 1000000,
@NAME = "Rha'Jin",
@TITLE = "The Experience Selector",
@CURSORNAME = "OpenHand", 
@DISPLAY = 70879, 
@HPMULTI = 1200, 
@POWERMULTI = 250,
@RANK = 3,
@LEVEL = 110,
@FACTION = 35,
@NPCFLAG = 129,
@SCALE = 1.2,
@UNITCLASS = 2,
@SCRIPTNAME = "npc_rate_xp_modifier",
@WEAPON1 = 140089,
@WEAPON2 = 0,
@WEAPON3 = 0;

INSERT INTO `creature_template_wdb` 
(`Entry`, 
`Name1`, 
`Title`, 
`CursorName`, 
`Displayid1`, 
`HpMulti`, 
`PowerMulti`,
`Leader`) 
VALUES 
(@ID, 
@NAME,
@TITLE,
@CURSORNAME,
@DISPLAY,
@HPMULTI,
@POWERMULTI,
@RANK);

INSERT INTO `creature_template` 
(`entry`, 
`minlevel`, 
`maxlevel`, 
`faction`, 
`npcflag`, 
`scale`, 
`unit_class`, 
`ScriptName`,
`WorldEffects`, 
`PassiveSpells`) 
VALUES 
(@ID,
@LEVEL,
@LEVEL,
@FACTION,
@NPCFLAG,
@SCALE,
@UNITCLASS,
@SCRIPTNAME,
'',
'');

INSERT INTO `creature_equip_template` 
(`CreatureID`, `ID`, `ItemID1`, `ItemID2`, `ItemID3`) 
VALUES (@ID, 1, @WEAPON1, @WEAPON2, @WEAPON3);

INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `PhaseId`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `dynamicflags`, `AiID`, `MovementID`, `MeleeID`, `isActive`, `skipClone`, `personal_size`, `isTeemingSpawn`, `unit_flags3`) VALUES (1000000, 1, 876, 876, 1, 1, '', 0, 0, 16233.3, 16299.8, 20.8442, 1.24312, 120, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);



###################################################################################################
#    Custom.XP.Rate.NPC
#        Description: This option activate the Custom XP Rate NPCs on the starting areas.
#        Default:    0 (disabled)
#                    1 (enabled)

Custom.XP.Rate.NPC = 1
Custom.XP.Rate.NPC.MAX.RATE = 3

#
###################################################################################################


*/

#include "Creature.h"
#include "Player.h"
#include "ScriptedGossip.h"
#include "ScriptMgr.h"
#include "World.h"
#include "Config.h"
#include "Chat.h"
#include "WorldSession.h"

class npc_rate_xp_modifier : public CreatureScript
{
    public:
        npc_rate_xp_modifier() : CreatureScript("npc_rate_xp_modifier") { }

		uint32 XpNpcMaxRate = (sConfigMgr->GetIntDefault("Custom.XP.Rate.NPC.MAX.RATE", 0));

        bool OnGossipHello(Player* player, Creature* creature) override
        {
            if (sConfigMgr->GetBoolDefault("Custom.XP.Rate.NPC", true))
            {
                for (uint32 i = 1; i <= XpNpcMaxRate; ++i)
                {
                    if (i == player->GetPersonnalXpRate())
                        continue;

                    //if (i == sWorld->getRate(RATE_XP_KILL))
                   //     continue;

                    std::ostringstream gossipText;
                    gossipText << "Change my xp rate to: x" << i;
                    //AddGossipItemFor(player, GOSSIP_ICON_CHAT, gossipText.str(), GOSSIP_SENDER_MAIN, i);
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, gossipText.str(), GOSSIP_SENDER_MAIN, i);
                }
            }
            else
            {
                std::ostringstream gossipText;
                gossipText << "The Custom XP Rate NPC |cff4CFF00 is unavailable now. |r.";
                //AddGossipItemFor(player, GOSSIP_ICON_CHAT, gossipText.str(), GOSSIP_SENDER_MAIN, 0);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, gossipText.str(), GOSSIP_SENDER_MAIN, 0);
            }

            if (player->GetPersonnalXpRate())
            {
            /*   std::ostringstream gossipText;
                gossipText << "Alap szorzó: x" << sWorld->getRate(RATE_XP_KILL);
                AddGossipItemFor(player, GOSSIP_ICON_CHAT, gossipText.str(), GOSSIP_SENDER_MAIN, 0);
			*/
				std::ostringstream alapszorzo;
				alapszorzo << "Basic xp rate: x" << sWorld->getRate(RATE_XP_KILL);
				ChatHandler(player->GetSession()).PSendSysMessage(alapszorzo.str().c_str());	
				
				std::ostringstream aktualisxprate;
				aktualisxprate << "Your xp rate: x" << player->GetPersonnalXpRate();
				ChatHandler(player->GetSession()).PSendSysMessage(aktualisxprate.str().c_str());	
            }

            //SendGossipMenuFor(player, player->GetGossipTextId(creature), creature->GetGUID());
			player->PlayerTalkClass->SendGossipMenu(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            return true;
        }

        bool OnGossipSelect(Player* player, Creature* /*creature*/, uint32 /*uiSender*/, uint32 uiAction) override
        {
            //CloseGossipMenuFor(player);
			player->PlayerTalkClass->SendCloseGossip();
            player->SetPersonnalXpRate(float(std::min(XpNpcMaxRate, uiAction)));
			std::ostringstream aktualisxprate;
			aktualisxprate << "Xp rate changed to: x" << float(player->GetPersonnalXpRate());
			player->GetSession()->SendNotification(aktualisxprate.str().c_str());
            return true;
        }

        struct customnpc_passivesAI : public ScriptedAI
        {
            customnpc_passivesAI(Creature * creature) : ScriptedAI(creature) { }

            uint32 uiAdATimer;
            //uint32 uiAdBTimer;
            //uint32 uiAdCTimer;

            void Reset()
            {
                uiAdATimer = 1000;
                //uiAdBTimer = 23000;
                //uiAdCTimer = 11000;
            }

            // Speak
            void UpdateAI(uint32 diff)
            {
                if (Player *player = me->SelectNearestPlayer(10))
                {
                    if (uiAdATimer <= diff)
                    {
                        std::ostringstream messageTaunt;
                        messageTaunt << "|cff4CFF00Hello, " << player->GetName() << "!";
                        player->GetSession()->SendNotification(messageTaunt.str().c_str());
                        //me->HandleEmoteCommand(EMOTE_ONESHOT_EXCLAMATION);
                        me->CastSpell(me, 44940);
                        uiAdATimer = 61000;
                    }
                    else
                        uiAdATimer -= diff;

                    /*if (uiAdBTimer <= diff)
                    {
                        std::ostringstream messageTaunt;
                        messageTaunt << "|cff4CFF00I see you, " << player->GetName() << "!";
                        player->GetSession()->SendNotification(messageTaunt.str().c_str());
                        me->HandleEmoteCommand(EMOTE_ONESHOT_CHICKEN);
                        me->CastSpell(me, 74311);
                        uiAdBTimer = 61000;
                    }
                    else
                        uiAdBTimer -= diff;

                    if (uiAdCTimer <= diff)
                    {
                        std::ostringstream messageTaunt;
                        messageTaunt << "|cff4CFF00Hey " << player->GetName() << " Come over here!";
                        player->GetSession()->SendNotification(messageTaunt.str().c_str());
                        me->HandleEmoteCommand(EMOTE_ONESHOT_CRY);
                        me->CastSpell(me, 72339);
                        uiAdCTimer = 61000;
                    }
                    else
                    {
                        uiAdCTimer -= diff;
                    }*/
                }
            }
        };

        // Creature AI
        CreatureAI * GetAI(Creature * creature) const
        {
            return new customnpc_passivesAI(creature);
        }
		
	// on login print xp rate
	void OnLogin(Player *player)
		{
		if (sConfigMgr->GetBoolDefault("Custom.XP.Rate.NPC", true) && player->getLevel()<110)
			{
			std::ostringstream aktualisxprate;
			aktualisxprate << "Your xp rate: x" << float(player->GetPersonnalXpRate()) << ". If you want to change it, find Rha'jin in GM Island!";
			ChatHandler(player->GetSession()).PSendSysMessage(aktualisxprate.str().c_str());
			}
		}
};

void AddSC_XP_Modifier()
{
    new npc_rate_xp_modifier();
}
