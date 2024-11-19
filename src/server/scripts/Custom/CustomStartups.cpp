#include "Config.h"
#include "Creature.h"
#include "Chat.h"
#include "Log.h"
#include "ScriptMgr.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "QuestData.h"
#include "ScriptedGossip.h"
#include "World.h"
#include "WorldSession.h"


class OnMoPArrival : public PlayerScript
{
public:
    OnMoPArrival() : PlayerScript("OnMoPArrival") { }

    enum
    {
        QUEST_TO_PANDARIA_H = 29611,
        QUEST_TO_PANDARIA_A = 29547
    };

    void OnLogin(Player* player, bool firstLogin) override
    {
        // Can happen in recovery cases
        if (player->getLevel() >= 85 && firstLogin)
            HandleMoPStart(player);
    }

    void OnLevelChanged(Player* player, uint8 oldLevel) override
    {
        if (oldLevel < 85 && player->getLevel() >= 85)
            HandleMoPStart(player);
    }

    void HandleMoPStart(Player* player)
    {

        if (player->GetQuestStatus(QUEST_TO_PANDARIA_A) == QUEST_STATUS_NONE && player->GetQuestStatus(QUEST_TO_PANDARIA_H) == QUEST_STATUS_NONE)
        {

            if (const Quest* quest = sQuestDataStore->GetQuestTemplate(player->IsInAlliance() ? QUEST_TO_PANDARIA_A : QUEST_TO_PANDARIA_H))
                player->AddQuest(quest, nullptr);
        }
    }
};

namespace {

    class OnNewCharFirstLogin : public PlayerScript
    {

    public:
        OnNewCharFirstLogin() : PlayerScript("OnNewCharFirstLogin")
        {
        }

        // run at first login of a new character
        void OnLogin(Player* player, bool firstLogin) override
        {
            if (firstLogin)
            {
                ChatHandler(player->GetSession()).SendSysMessage("|cffFF0000HeronCore |r");
            }
        }
    };
}

class OnWorldserverLoaded : public WorldScript
{
public:
    OnWorldserverLoaded() : WorldScript("OnWorldserverLoaded") {}

    // run always when worldserver has loaded
    void OnStartup() override
    {
        //TC_LOG_INFO(LOG_FILTER_WORLDSERVER, "LegionCore loaded...");
    }
};

void AddSC_CustomStartups() {
    new OnNewCharFirstLogin();
    new OnWorldserverLoaded();
    new OnMoPArrival();
}