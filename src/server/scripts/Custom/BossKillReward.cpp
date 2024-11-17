/*

To the Worldserver.conf:

###################################################################################################
# Boss Kill Rewarding System
#
#	Rewards the player with InGameShop Credits for each boss kill in dungeons and raids.
#	These amounts are for dungeons but the MultiplierForRaid will multiply these values for raids.
#	Note: If you change a value you need to restart server. (".reload config" not work for this)
#
#       Default:     0  - (Disabled)
#                    1  - (Enabled)
#

BossKillReward.Enable = 0
BossKillReward.Amount.Classic = 5
BossKillReward.Amount.BC = 10
BossKillReward.Amount.Wotlk = 15
BossKillReward.Amount.Cata = 20
BossKillReward.Amount.Mop = 25
BossKillReward.Amount.Wod = 30
BossKillReward.Amount.Legion = 35
BossKillReward.MultiplierForRaid = 2

#
###################################################################################################

*/

#include "BattlePayMgr.h"
#include "Chat.h"
#include "Config.h"
#include "DB2Stores.h"
#include "Group.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "World.h"

uint32 amount_classic, amount_bc, amount_wotlk, amount_cata, amount_mop, amount_wod, amount_legion, raid_multiplier;

class BossKillReward : public PlayerScript
{
public:
    BossKillReward() : PlayerScript("BossKillReward") 
	{ 
		// In this way the config values loaded with the script loading .. if u change in config u need to restart server
		amount_classic = sConfigMgr->GetIntDefault("BossKillReward.Amount.Classic", 5);
		amount_bc = sConfigMgr->GetIntDefault("BossKillReward.Amount.BC", 10);
		amount_wotlk = sConfigMgr->GetIntDefault("BossKillReward.Amount.Wotlk", 15);
		amount_cata = sConfigMgr->GetIntDefault("BossKillReward.Amount.Cata", 20);
		amount_mop = sConfigMgr->GetIntDefault("BossKillReward.Amount.Mop", 25);
		amount_wod = sConfigMgr->GetIntDefault("BossKillReward.Amount.Wod", 30);
		amount_legion = sConfigMgr->GetIntDefault("BossKillReward.Amount.Legion", 35);
		raid_multiplier = sConfigMgr->GetIntDefault("BossKillReward.MultiplierForRaid", 2);
	}

	void SendReward(Player* player, MapEntry const* map, uint32 amount_expansion)
	{
		if (map->IsDungeon())
			player->AddDonateTokenCount(amount_expansion);

		if (map->IsRaid())
			player->AddDonateTokenCount(amount_expansion * raid_multiplier);
	}
	
    void OnCreatureKill(Player* player, Creature* killed)
    {
        // If enabled...
        if (sConfigMgr->GetBoolDefault("BossKillReward.Enable", true))
        {	
			// If the creature too low level skip reward
			if (killed->GetLevelForTarget(player) < player->getLevel() - 5)
				return;

			// If not boss
			if (!killed->isWorldBoss() && !killed->IsDungeonBoss())
				return;

			// Checking for expansion
			if (auto map = sDB2Manager.GetMapByID(killed->GetMapId()))
			{
				switch (map->ExpansionID)
				{
				case EXPANSION_CLASSIC:
					SendReward(player, map, amount_classic);
					break;
				case EXPANSION_THE_BURNING_CRUSADE:
					SendReward(player, map, amount_bc);
					break;
				case EXPANSION_WRATH_OF_THE_LICH_KING:
					SendReward(player, map, amount_wotlk);
					break;
				case EXPANSION_CATACLYSM:
					SendReward(player, map, amount_cata);
					break;
				case EXPANSION_MISTS_OF_PANDARIA:
					SendReward(player, map, amount_mop);
					break;
				case EXPANSION_WARLORDS_OF_DRAENOR:
					SendReward(player, map, amount_wod);
					break;
				case EXPANSION_LEGION:
					SendReward(player, map, amount_legion);
					break;
				}
			}
        }
	}
};

void AddSC_BossKillReward()
{
    new BossKillReward();
}