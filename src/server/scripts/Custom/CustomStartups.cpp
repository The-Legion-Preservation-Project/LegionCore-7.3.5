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

class player_pandaria_quest_intro : public PlayerScript
{
public:
    player_pandaria_quest_intro() : PlayerScript("player_pandaria_quest_intro") {}

	void OnUpdateArea(Player* player, uint32 NewArea)
	{
		if (NewArea == 5853 && player->GetQuestStatus(29548) == QUEST_STATUS_INCOMPLETE && player->IsInAlliance())
		{
            Position pos;
            player->GetPosition(&pos);
            player->KilledMonsterCredit(66292);
            player->SendSpellScene(94, nullptr, true, &pos);
		}
	}
};

void AddSC_CustomStartups() {
   
    new OnMoPArrival();
    new player_pandaria_quest_intro();
}
