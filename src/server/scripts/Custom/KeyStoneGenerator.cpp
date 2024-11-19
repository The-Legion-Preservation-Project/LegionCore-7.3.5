#include "Define.h"
#include "Config.h"
#include "ScriptMgr.h"
#include "ScriptedGossip.h"
#include "Player.h"
#include "PlayerDefines.h"
#include "SharedDefines.h"
#include "Item.h"

class KeyStoneGenerator_npc : public CreatureScript
{
public:
	KeyStoneGenerator_npc() : CreatureScript("KeyStoneGenerator_npc") { }

	bool OnGossipHello(Player* player, Creature* creature) override
	{
		player->PlayerTalkClass->ClearMenus();
		player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_BATTLE, "Generate a Mythic Keystone", GOSSIP_SENDER_MAIN, 0, "Generating a random Mythic Keystone. Are you sure?", 0, false);
		player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_BATTLE, "Destroy my Mythic Kestone", GOSSIP_SENDER_MAIN, 1, "Destroying the Mythic Keystone from your bag. Are you sure?", 0, false);
		player->PlayerTalkClass->SendGossipMenu(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());

		return true;
	}

	bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action)  override
	{

		switch (action)
		{
		case 0:
			if (!player->HasItemCount(138019, 1, true))
				player->AddItem(138019, 1);

			if (Item* keystone = player->GetItemByEntry(138019))
			{
				player->CreateChallengeKey(keystone);
				player->InitChallengeKey(keystone);
			}

			player->PlaySpellVisual(player->GetPosition(), 74691, 0.75f, ObjectGuid::Empty, false);

			player->PlayerTalkClass->SendCloseGossip();
		break;
		case 1:
			if (player->HasItemCount(138019, 1, true))
				player->DestroyItemCount(138019, 1, true, false);

			player->PlaySpellVisual(player->GetPosition(), 61300, 0.75f, ObjectGuid::Empty, false);

			player->PlayerTalkClass->SendCloseGossip();
			break;
		}
		return true;
	}

};



void AddSC_KeyStoneGenerator_npc()
{
	new KeyStoneGenerator_npc();
}
