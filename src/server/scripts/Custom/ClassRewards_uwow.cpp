/*
Made by Fluxurion, rewards the class with some cool stuff.


To the Worldserver.conf:

###################################################################################################
#

Class.Rewards.Enable = 1
Class.Rewards.FirstLogin.ClassMount.Enable = 1
Class.Rewards.OnLevel100.ArtifactWep.Enable = 1
Class.Rewards.FirstLogin.ApprenticeRiding.Enable = 1
Class.Rewards.FirstLogin.JourneymanRiding.Enable = 1
Class.Rewards.FirstLogin.ExpertRiding.Enable = 1
Class.Rewards.FirstLogin.ArtisanRiding.Enable = 1
Class.Rewards.FirstLogin.MasterRiding.Enable = 1
Class.Rewards.FirstLogin.AzerothFlying.Enable = 1
Class.Rewards.FirstLogin.NorthrendFlying.Enable = 1
Class.Rewards.FirstLogin.PandariaFlying.Enable = 1
Class.Rewards.FirstLogin.DraenorFlying.Enable = 0
Class.Rewards.FirstLogin.BrokenIsleFlying.Enable = 0

#
###################################################################################################

 */

#include "Chat.h"
#include "Player.h"
#include "SpellScript.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "Spell.h"
#include "Challenge.h"
#include "ChallengeMgr.h"
#include <Config.h>

class ClassRewards : public PlayerScript
{
public:
    ClassRewards() : PlayerScript("ClassRewards") { }
    


		
		void OnLogin(Player* player, bool firstLogin) override	
		{
			if (firstLogin)
            {
				if ((sConfigMgr->GetBoolDefault("Class.Rewards.Enable", true)) && (sConfigMgr->GetBoolDefault("Class.Rewards.FirstLogin.ClassMount.Enable", true)))
				{
					//  LOGIN	ClassMount
					if (player->getClass() == CLASS_WARRIOR) { player->learnSpell(229388, true); }
					if (player->getClass() == CLASS_PALADIN) { player->learnSpell(231435, true); }
					if (player->getClass() == CLASS_PALADIN) { player->learnSpell(231589, true); }
					if (player->getClass() == CLASS_PALADIN) { player->learnSpell(231587, true); }
					if (player->getClass() == CLASS_PALADIN) { player->learnSpell(231588, true); }
					if (player->getClass() == CLASS_HUNTER) {  player->learnSpell(229439, true); }
					if (player->getClass() == CLASS_HUNTER) {  player->learnSpell(229438, true); }
					if (player->getClass() == CLASS_ROGUE) {   player->learnSpell(231524, true); }
					if (player->getClass() == CLASS_ROGUE) {   player->learnSpell(231525, true); }
					if (player->getClass() == CLASS_ROGUE){    player->learnSpell(231523, true); }
					if (player->getClass() == CLASS_PRIEST) {  player->learnSpell(229377, true); }
					if (player->getClass() == CLASS_DEATH_KNIGHT) {  player->learnSpell(229387, true); }
					if (player->getClass() == CLASS_SHAMAN) {  player->learnSpell(231442, true); }
					if (player->getClass() == CLASS_MAGE) {    player->learnSpell(229376, true); }
					if (player->getClass() == CLASS_WARLOCK) { player->learnSpell(238452, true); }
					if (player->getClass() == CLASS_WARLOCK) { player->learnSpell(238454, true); }
					if (player->getClass() == CLASS_WARLOCK) { player->learnSpell(232412, true); }
					if (player->getClass() == CLASS_MONK) {    player->learnSpell(229385, true); }
					if (player->getClass() == CLASS_DRUID) {   player->learnSpell(231437, true); }
					if (player->getClass() == CLASS_DEMON_HUNTER) { player->learnSpell(229417,true); }
					//  LOGIN	Riding Skill
					if ((sConfigMgr->GetBoolDefault("Class.Rewards.Enable", true)) && (sConfigMgr->GetBoolDefault("Class.Rewards.FirstLogin.ApprenticeRiding.Enable", true)))
					{	player->CastSpell(player,33389,true);	}
					if ((sConfigMgr->GetBoolDefault("Class.Rewards.Enable", true)) && (sConfigMgr->GetBoolDefault("Class.Rewards.FirstLogin.JourneymanRiding.Enable", true)))
					{	player->CastSpell(player,33392,true);	}
					if ((sConfigMgr->GetBoolDefault("Class.Rewards.Enable", true)) && (sConfigMgr->GetBoolDefault("Class.Rewards.FirstLogin.ExpertRiding.Enable", true)))
					{	player->CastSpell(player,34092,true);	}
					if ((sConfigMgr->GetBoolDefault("Class.Rewards.Enable", true)) && (sConfigMgr->GetBoolDefault("Class.Rewards.FirstLogin.ArtisanRiding.Enable", true)))
					{	player->CastSpell(player,34093,true);	}
					if ((sConfigMgr->GetBoolDefault("Class.Rewards.Enable", true)) && (sConfigMgr->GetBoolDefault("Class.Rewards.FirstLogin.MasterRiding.Enable", true)))
					{	player->CastSpell(player,90266,true);	}
					if ((sConfigMgr->GetBoolDefault("Class.Rewards.Enable", true)) && (sConfigMgr->GetBoolDefault("Class.Rewards.FirstLogin.AzerothFlying.Enable", true)))
					{	player->CastSpell(player,90269,true);	}
					if ((sConfigMgr->GetBoolDefault("Class.Rewards.Enable", true)) && (sConfigMgr->GetBoolDefault("Class.Rewards.FirstLogin.NorthrendFlying.Enable", true)))
					{	player->CastSpell(player,54198,true);	}
					if ((sConfigMgr->GetBoolDefault("Class.Rewards.Enable", true)) && (sConfigMgr->GetBoolDefault("Class.Rewards.FirstLogin.PandariaFlying.Enable", true)))
					{	player->CastSpell(player,115916,true);	}
					if ((sConfigMgr->GetBoolDefault("Class.Rewards.Enable", true)) && (sConfigMgr->GetBoolDefault("Class.Rewards.FirstLogin.DraenorFlying.Enable", true)))
					{	player->CastSpell(player,191646,true);	}
					if ((sConfigMgr->GetBoolDefault("Class.Rewards.Enable", true)) && (sConfigMgr->GetBoolDefault("Class.Rewards.FirstLogin.BrokenIsleFlying.Enable", true)))
					{	player->CastSpell(player,228311,true); player->CastSpell(player,233367,true);  }
				}
			}
		}
			


		void OnLevelChanged(Player * player, uint8 oldLevel) override
		{	
			// ON LEVEL 100 ArtifactWep
			if ((sConfigMgr->GetBoolDefault("Class.Rewards.Enable", true)) && (sConfigMgr->GetBoolDefault("Class.Rewards.OnLevel100.ArtifactWep.Enable", true)))
			{
				if (player->getLevel() >= 100) 
				{
					if (player->getClass() == CLASS_WARRIOR){	  
					player->AddItem(128289, 1); 
					player->AddItem(128908, 1); 
					player->AddItem(128910, 1); 
					}
					if (player->getClass() == CLASS_PALADIN){
					player->AddItem(120978, 1); 
					player->AddItem(128823, 1); 
					player->AddItem(128866, 1); 
					}
					if (player->getClass() == CLASS_HUNTER) {
					player->AddItem(128808, 1); 
					player->AddItem(128826, 1); 
					player->AddItem(128861, 1); 
					}
					if (player->getClass() == CLASS_ROGUE) 	{
					player->AddItem(128870, 1); 
					player->AddItem(128872, 1); 
					player->AddItem(128476, 1); 
					}
					if (player->getClass() == CLASS_PRIEST) {	  
					player->AddItem(128827, 1); 
					player->AddItem(128868, 1); 
					player->AddItem(128825, 1); 
					}
					if (player->getClass() == CLASS_DEATH_KNIGHT){
					player->AddItem(128292, 1); 
					player->AddItem(128402, 1); 
					player->AddItem(128403, 1); 
					}
					if (player->getClass() == CLASS_SHAMAN) {	  
					player->AddItem(128935, 1); 
					player->AddItem(128819, 1); 
					player->AddItem(128911, 1); 
					}
					if (player->getClass() == CLASS_MAGE)	{	  
					player->AddItem(127857, 1); 
					player->AddItem(128862, 1); 
					player->AddItem(128820, 1); 
					}
					if (player->getClass() == CLASS_WARLOCK) {	  
					player->AddItem(128943, 1); 
					player->AddItem(128941, 1); 
					player->AddItem(128942, 1); 
					}
					if (player->getClass() == CLASS_MONK) 	{	  
					player->AddItem(128940, 1); 
					player->AddItem(128937, 1); 
					player->AddItem(128938, 1); 
					}
					if (player->getClass() == CLASS_DRUID)	{	  
					player->AddItem(128306, 1); 
					player->AddItem(128821, 1); 
					player->AddItem(128860, 1); 
					player->AddItem(128858, 1); 
					}
					if (player->getClass() == CLASS_DEMON_HUNTER){
					player->AddItem(127829, 1); 
					player->AddItem(127832, 1); 
					}
				}
			}
		}
	
	
};

/*
HotFixes DB
INSERT INTO `item_effect` (`ID`, `SpellID`, `CoolDownMSec`, `CategoryCoolDownMSec`, `Charges`, `SpellCategoryID`, `ChrSpecializationID`, `LegacySlotIndex`, `TriggerType`, `ItemID`, `VerifiedBuild`) VALUES ('3000201', '263152', '-2', '-2', '-1', '4', '0', '0', '0', '3000201', '0');
INSERT INTO `item_sparse` (`ID`, `AllowableRace`, `Display`, `Display1`, `Display2`, `Display3`, `Description`, `Flags1`, `Flags2`, `Flags3`, `Flags4`, `PriceRandomValue`, `PriceVariance`, `VendorStackCount`, `BuyPrice`, `SellPrice`, `RequiredAbility`, `MaxCount`, `Stackable`, `StatPercentEditor1`, `StatPercentEditor2`, `StatPercentEditor3`, `StatPercentEditor4`, `StatPercentEditor5`, `StatPercentEditor6`, `StatPercentEditor7`, `StatPercentEditor8`, `StatPercentEditor9`, `StatPercentEditor10`, `StatPercentageOfSocket1`, `StatPercentageOfSocket2`, `StatPercentageOfSocket3`, `StatPercentageOfSocket4`, `StatPercentageOfSocket5`, `StatPercentageOfSocket6`, `StatPercentageOfSocket7`, `StatPercentageOfSocket8`, `StatPercentageOfSocket9`, `StatPercentageOfSocket10`, `ItemRange`, `BagFamily`, `QualityModifier`, `DurationInInventory`, `DmgVariance`, `AllowableClass`, `ItemLevel`, `RequiredSkill`, `RequiredSkillRank`, `MinFactionID`, `ItemStatValue1`, `ItemStatValue2`, `ItemStatValue3`, `ItemStatValue4`, `ItemStatValue5`, `ItemStatValue6`, `ItemStatValue7`, `ItemStatValue8`, `ItemStatValue9`, `ItemStatValue10`, `ScalingStatDistributionID`, `ItemDelay`, `PageID`, `StartQuestID`, `LockID`, `RandomSelect`, `ItemRandomSuffixGroupID`, `ItemSet`, `ZoneBound`, `InstanceBound`, `TotemCategoryID`, `SocketMatch_enchantment_id`, `GemProperties`, `LimitCategory`, `RequiredHoliday`, `RequiredTransmogHoliday`, `ItemNameDescriptionID`, `OverallQualityID`, `InventoryType`, `RequiredLevel`, `RequiredPVPRank`, `RequiredPVPMedal`, `MinReputation`, `ContainerSlots`, `StatModifierBonusStat1`, `StatModifierBonusStat2`, `StatModifierBonusStat3`, `StatModifierBonusStat4`, `StatModifierBonusStat5`, `StatModifierBonusStat6`, `StatModifierBonusStat7`, `StatModifierBonusStat8`, `StatModifierBonusStat9`, `StatModifierBonusStat10`, `DamageDamageType`, `Bonding`, `LanguageID`, `PageMaterialID`, `Material`, `SheatheType`, `SocketType1`, `SocketType2`, `SocketType3`, `SpellWeightCategory`, `SpellWeight`, `ArtifactID`, `ExpansionID`, `VerifiedBuild`) VALUES ('3000201', '-1', 'Personal Loot & Instances Reset Scroll', '', '', NULL, '|cFFFF6347Heroncore Personal Loot & Instances Reset Scroll', '0', '268435456', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '8192', '4', '0', '0', '-1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '4', '0', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '0', '-1', '0', '0', '0', '0', '0', '0', '0', '0', '0');
INSERT INTO `item` (`ID`, `IconFileDataID`, `ClassID`, `SubclassID`, `SoundOverrideSubclass`, `Material`, `InventoryType`, `SheatheType`, `ItemGroupSoundsID`, `VerifiedBuild`) VALUES ('3000201', '1500889', '0', '0', '0', '-1', '0', '0', '0', '0');
INSERT INTO `hotfix_data` (`Id`, `TableHash`, `RecordID`, `Timestamp`, `Deleted`) VALUES ('3000201', '2442913102', '3000201', '0', '0');
INSERT INTO `hotfix_data` (`Id`, `TableHash`, `RecordID`, `Timestamp`, `Deleted`) VALUES ('3000201', '1344507586', '3000201', '0', '0');
INSERT INTO `hotfix_data` (`Id`, `TableHash`, `RecordID`, `Timestamp`, `Deleted`) VALUES ('3000201', '1073915313', '3000201', '0', '0');
UPDATE `item_sparse` SET `Display`='Instances Reset Scroll', `Description`='|cFFFF6347Heroncore Personal Loot, Hearthstone, and Instances Reset Scroll' WHERE  `ID`=3000201;
World DB
INSERT INTO `item_script_names` (`Id`, `ScriptName`) VALUES ('3000201', 'Scroll_Instance');
*/
//item 3000201 Scroll Instances Reset
class Heroncore_Instance : public ItemScript
{
public:
	Heroncore_Instance() : ItemScript("Scroll_Instance") { }

	bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/) override
	{
		if (!player || !item)
			return false;

		for (uint8 i = 0; i < MAX_DIFFICULTY; ++i)
		{
			Player::BoundInstancesMap &binds = player->GetBoundInstances(Difficulty(i));
			for (Player::BoundInstancesMap::iterator itr = binds.begin(); itr != binds.end();)
				player->UnbindInstance(itr, Difficulty(i));
		}

		player->RemoveSpellCooldown(8690, true);
		player->ResetLootCooldown();
		player->ResetLootCooldown();
		CharacterDatabase.PExecute("DELETE FROM character_loot_cooldown WHERE guid = %u", uint32(player->GetGUID().GetCounter()));
		CharacterDatabase.PExecute("DELETE FROM character_lfg_cooldown WHERE guid = %u", uint32(player->GetGUID().GetCounter()));
		ChatHandler(player->GetSession()).PSendSysMessage("Reset personal loot cooldown & Instances");
		player->DestroyItemCount(item->GetEntry(), 1, true);

		return true;

	}
};

/*
Hotfixes DB
INSERT INTO `item_effect` (`ID`, `SpellID`, `CoolDownMSec`, `CategoryCoolDownMSec`, `Charges`, `SpellCategoryID`, `ChrSpecializationID`, `LegacySlotIndex`, `TriggerType`, `ItemID`, `VerifiedBuild`) VALUES ('3000202', '263152', '-2', '-2', '-1', '4', '0', '0', '0', '3000202', '0');
INSERT INTO `item_sparse` (`ID`, `AllowableRace`, `Display`, `Display1`, `Display2`, `Display3`, `Description`, `Flags1`, `Flags2`, `Flags3`, `Flags4`, `PriceRandomValue`, `PriceVariance`, `VendorStackCount`, `BuyPrice`, `SellPrice`, `RequiredAbility`, `MaxCount`, `Stackable`, `StatPercentEditor1`, `StatPercentEditor2`, `StatPercentEditor3`, `StatPercentEditor4`, `StatPercentEditor5`, `StatPercentEditor6`, `StatPercentEditor7`, `StatPercentEditor8`, `StatPercentEditor9`, `StatPercentEditor10`, `StatPercentageOfSocket1`, `StatPercentageOfSocket2`, `StatPercentageOfSocket3`, `StatPercentageOfSocket4`, `StatPercentageOfSocket5`, `StatPercentageOfSocket6`, `StatPercentageOfSocket7`, `StatPercentageOfSocket8`, `StatPercentageOfSocket9`, `StatPercentageOfSocket10`, `ItemRange`, `BagFamily`, `QualityModifier`, `DurationInInventory`, `DmgVariance`, `AllowableClass`, `ItemLevel`, `RequiredSkill`, `RequiredSkillRank`, `MinFactionID`, `ItemStatValue1`, `ItemStatValue2`, `ItemStatValue3`, `ItemStatValue4`, `ItemStatValue5`, `ItemStatValue6`, `ItemStatValue7`, `ItemStatValue8`, `ItemStatValue9`, `ItemStatValue10`, `ScalingStatDistributionID`, `ItemDelay`, `PageID`, `StartQuestID`, `LockID`, `RandomSelect`, `ItemRandomSuffixGroupID`, `ItemSet`, `ZoneBound`, `InstanceBound`, `TotemCategoryID`, `SocketMatch_enchantment_id`, `GemProperties`, `LimitCategory`, `RequiredHoliday`, `RequiredTransmogHoliday`, `ItemNameDescriptionID`, `OverallQualityID`, `InventoryType`, `RequiredLevel`, `RequiredPVPRank`, `RequiredPVPMedal`, `MinReputation`, `ContainerSlots`, `StatModifierBonusStat1`, `StatModifierBonusStat2`, `StatModifierBonusStat3`, `StatModifierBonusStat4`, `StatModifierBonusStat5`, `StatModifierBonusStat6`, `StatModifierBonusStat7`, `StatModifierBonusStat8`, `StatModifierBonusStat9`, `StatModifierBonusStat10`, `DamageDamageType`, `Bonding`, `LanguageID`, `PageMaterialID`, `Material`, `SheatheType`, `SocketType1`, `SocketType2`, `SocketType3`, `SpellWeightCategory`, `SpellWeight`, `ArtifactID`, `ExpansionID`, `VerifiedBuild`) VALUES ('3000202', '-1', 'Goblin Warrior Mercenary Contract', '', '', NULL, '|cFFFF6347SPP Goblin Warrior Mercenary Scroll', '0', '268435456', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '8192', '4', '0', '0', '-1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '4', '0', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '0', '-1', '0', '0', '0', '0', '0', '0', '0', '0', '0');
INSERT INTO `item` (`ID`, `IconFileDataID`, `ClassID`, `SubclassID`, `SoundOverrideSubclass`, `Material`, `InventoryType`, `SheatheType`, `ItemGroupSoundsID`, `VerifiedBuild`) VALUES ('3000202', '1500889', '0', '0', '0', '-1', '0', '0', '0', '0');
INSERT INTO `hotfix_data` (`Id`, `TableHash`, `RecordID`, `Timestamp`, `Deleted`) VALUES ('3000202', '2442913102', '3000202', '0', '0');
INSERT INTO `hotfix_data` (`Id`, `TableHash`, `RecordID`, `Timestamp`, `Deleted`) VALUES ('3000202', '1344507586', '3000202', '0', '0');
INSERT INTO `hotfix_data` (`Id`, `TableHash`, `RecordID`, `Timestamp`, `Deleted`) VALUES ('3000202', '1073915313', '3000202', '0', '0');

WORLD DB
INSERT INTO `item_script_names` (`Id`, `ScriptName`) VALUES ('3000202', 'goblin_warrior');
*/
//item 3000202 warrior
class Heroncore_MERC1 : public ItemScript
{
public:
	Heroncore_MERC1() : ItemScript("goblin_warrior") { }

	bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/) override
	{
		if (!player || !item)
			return false;
		player->CastSpell(player, uint32(436973), true);
		player->DestroyItemCount(item->GetEntry(), 1, true);
		return true;
	}
};
/*

INSERT INTO `item_effect` (`ID`, `SpellID`, `CoolDownMSec`, `CategoryCoolDownMSec`, `Charges`, `SpellCategoryID`, `ChrSpecializationID`, `LegacySlotIndex`, `TriggerType`, `ItemID`, `VerifiedBuild`) VALUES ('3000203', '263152', '-2', '-2', '-1', '4', '0', '0', '0', '3000203', '0');
INSERT INTO `item_sparse` (`ID`, `AllowableRace`, `Display`, `Display1`, `Display2`, `Display3`, `Description`, `Flags1`, `Flags2`, `Flags3`, `Flags4`, `PriceRandomValue`, `PriceVariance`, `VendorStackCount`, `BuyPrice`, `SellPrice`, `RequiredAbility`, `MaxCount`, `Stackable`, `StatPercentEditor1`, `StatPercentEditor2`, `StatPercentEditor3`, `StatPercentEditor4`, `StatPercentEditor5`, `StatPercentEditor6`, `StatPercentEditor7`, `StatPercentEditor8`, `StatPercentEditor9`, `StatPercentEditor10`, `StatPercentageOfSocket1`, `StatPercentageOfSocket2`, `StatPercentageOfSocket3`, `StatPercentageOfSocket4`, `StatPercentageOfSocket5`, `StatPercentageOfSocket6`, `StatPercentageOfSocket7`, `StatPercentageOfSocket8`, `StatPercentageOfSocket9`, `StatPercentageOfSocket10`, `ItemRange`, `BagFamily`, `QualityModifier`, `DurationInInventory`, `DmgVariance`, `AllowableClass`, `ItemLevel`, `RequiredSkill`, `RequiredSkillRank`, `MinFactionID`, `ItemStatValue1`, `ItemStatValue2`, `ItemStatValue3`, `ItemStatValue4`, `ItemStatValue5`, `ItemStatValue6`, `ItemStatValue7`, `ItemStatValue8`, `ItemStatValue9`, `ItemStatValue10`, `ScalingStatDistributionID`, `ItemDelay`, `PageID`, `StartQuestID`, `LockID`, `RandomSelect`, `ItemRandomSuffixGroupID`, `ItemSet`, `ZoneBound`, `InstanceBound`, `TotemCategoryID`, `SocketMatch_enchantment_id`, `GemProperties`, `LimitCategory`, `RequiredHoliday`, `RequiredTransmogHoliday`, `ItemNameDescriptionID`, `OverallQualityID`, `InventoryType`, `RequiredLevel`, `RequiredPVPRank`, `RequiredPVPMedal`, `MinReputation`, `ContainerSlots`, `StatModifierBonusStat1`, `StatModifierBonusStat2`, `StatModifierBonusStat3`, `StatModifierBonusStat4`, `StatModifierBonusStat5`, `StatModifierBonusStat6`, `StatModifierBonusStat7`, `StatModifierBonusStat8`, `StatModifierBonusStat9`, `StatModifierBonusStat10`, `DamageDamageType`, `Bonding`, `LanguageID`, `PageMaterialID`, `Material`, `SheatheType`, `SocketType1`, `SocketType2`, `SocketType3`, `SpellWeightCategory`, `SpellWeight`, `ArtifactID`, `ExpansionID`, `VerifiedBuild`) VALUES ('3000203', '-1', 'Goblin Hunter Mercenary Contract', '', '', NULL, '|cFFFF6347SPP Goblin Hunter Mercenary Scroll', '0', '268435456', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '8192', '4', '0', '0', '-1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '4', '0', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '0', '-1', '0', '0', '0', '0', '0', '0', '0', '0', '0');
INSERT INTO `item` (`ID`, `IconFileDataID`, `ClassID`, `SubclassID`, `SoundOverrideSubclass`, `Material`, `InventoryType`, `SheatheType`, `ItemGroupSoundsID`, `VerifiedBuild`) VALUES ('3000203', '1500889', '0', '0', '0', '-1', '0', '0', '0', '0');


INSERT INTO `hotfix_data` (`Id`, `TableHash`, `RecordID`, `Timestamp`, `Deleted`) VALUES ('3000203', '2442913102', '3000203', '0', '0');
INSERT INTO `hotfix_data` (`Id`, `TableHash`, `RecordID`, `Timestamp`, `Deleted`) VALUES ('3000203', '1344507586', '3000203', '0', '0');
INSERT INTO `hotfix_data` (`Id`, `TableHash`, `RecordID`, `Timestamp`, `Deleted`) VALUES ('3000203', '1073915313', '3000203', '0', '0');

WORLD DB
INSERT INTO `item_script_names` (`Id`, `ScriptName`) VALUES ('3000203', 'goblin_hunter');
*/
//hunter
class Heroncore_MERC2 : public ItemScript
{
public:
	Heroncore_MERC2() : ItemScript("goblin_hunter") { }

	bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/) override
	{
		if (!player || !item)
			return false;
		player->CastSpell(player, uint32(436975), true);
		player->DestroyItemCount(item->GetEntry(), 1, true);
		return true;
	}
};

/*
HotFixes DB
INSERT INTO `item_effect` (`ID`, `SpellID`, `CoolDownMSec`, `CategoryCoolDownMSec`, `Charges`, `SpellCategoryID`, `ChrSpecializationID`, `LegacySlotIndex`, `TriggerType`, `ItemID`, `VerifiedBuild`) VALUES ('3000204', '263152', '-2', '-2', '-1', '4', '0', '0', '0', '3000204', '0');
INSERT INTO `item_sparse` (`ID`, `AllowableRace`, `Display`, `Display1`, `Display2`, `Display3`, `Description`, `Flags1`, `Flags2`, `Flags3`, `Flags4`, `PriceRandomValue`, `PriceVariance`, `VendorStackCount`, `BuyPrice`, `SellPrice`, `RequiredAbility`, `MaxCount`, `Stackable`, `StatPercentEditor1`, `StatPercentEditor2`, `StatPercentEditor3`, `StatPercentEditor4`, `StatPercentEditor5`, `StatPercentEditor6`, `StatPercentEditor7`, `StatPercentEditor8`, `StatPercentEditor9`, `StatPercentEditor10`, `StatPercentageOfSocket1`, `StatPercentageOfSocket2`, `StatPercentageOfSocket3`, `StatPercentageOfSocket4`, `StatPercentageOfSocket5`, `StatPercentageOfSocket6`, `StatPercentageOfSocket7`, `StatPercentageOfSocket8`, `StatPercentageOfSocket9`, `StatPercentageOfSocket10`, `ItemRange`, `BagFamily`, `QualityModifier`, `DurationInInventory`, `DmgVariance`, `AllowableClass`, `ItemLevel`, `RequiredSkill`, `RequiredSkillRank`, `MinFactionID`, `ItemStatValue1`, `ItemStatValue2`, `ItemStatValue3`, `ItemStatValue4`, `ItemStatValue5`, `ItemStatValue6`, `ItemStatValue7`, `ItemStatValue8`, `ItemStatValue9`, `ItemStatValue10`, `ScalingStatDistributionID`, `ItemDelay`, `PageID`, `StartQuestID`, `LockID`, `RandomSelect`, `ItemRandomSuffixGroupID`, `ItemSet`, `ZoneBound`, `InstanceBound`, `TotemCategoryID`, `SocketMatch_enchantment_id`, `GemProperties`, `LimitCategory`, `RequiredHoliday`, `RequiredTransmogHoliday`, `ItemNameDescriptionID`, `OverallQualityID`, `InventoryType`, `RequiredLevel`, `RequiredPVPRank`, `RequiredPVPMedal`, `MinReputation`, `ContainerSlots`, `StatModifierBonusStat1`, `StatModifierBonusStat2`, `StatModifierBonusStat3`, `StatModifierBonusStat4`, `StatModifierBonusStat5`, `StatModifierBonusStat6`, `StatModifierBonusStat7`, `StatModifierBonusStat8`, `StatModifierBonusStat9`, `StatModifierBonusStat10`, `DamageDamageType`, `Bonding`, `LanguageID`, `PageMaterialID`, `Material`, `SheatheType`, `SocketType1`, `SocketType2`, `SocketType3`, `SpellWeightCategory`, `SpellWeight`, `ArtifactID`, `ExpansionID`, `VerifiedBuild`) VALUES ('3000204', '-1', 'Goblin Brawler Mercenary Contract', '', '', NULL, '|cFFFF6347SPP Goblin Brawler Mercenary Scroll', '0', '268435456', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '8192', '4', '0', '0', '-1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '4', '0', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '0', '-1', '0', '0', '0', '0', '0', '0', '0', '0', '0');
INSERT INTO `item` (`ID`, `IconFileDataID`, `ClassID`, `SubclassID`, `SoundOverrideSubclass`, `Material`, `InventoryType`, `SheatheType`, `ItemGroupSoundsID`, `VerifiedBuild`) VALUES ('3000204', '1500889', '0', '0', '0', '-1', '0', '0', '0', '0');
INSERT INTO `hotfix_data` (`Id`, `TableHash`, `RecordID`, `Timestamp`, `Deleted`) VALUES ('3000204', '2442913102', '3000204', '0', '0');
INSERT INTO `hotfix_data` (`Id`, `TableHash`, `RecordID`, `Timestamp`, `Deleted`) VALUES ('3000204', '1344507586', '3000204', '0', '0');
INSERT INTO `hotfix_data` (`Id`, `TableHash`, `RecordID`, `Timestamp`, `Deleted`) VALUES ('3000204', '1073915313', '3000204', '0', '0');

WORLD DB:
INSERT INTO `item_script_names` (`Id`, `ScriptName`) VALUES ('3000204', 'goblin_fist');

*/
//brawler
class Heroncore_MERC3 : public ItemScript
{
public:
	Heroncore_MERC3() : ItemScript("goblin_fist") { }

	bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/) override
	{
		if (!player || !item)
			return false;
		player->CastSpell(player, uint32(436974), true);
		player->DestroyItemCount(item->GetEntry(), 1, true);
		return true;
	}
};
/*
Hotfixes DB
INSERT INTO `item_effect` (`ID`, `SpellID`, `CoolDownMSec`, `CategoryCoolDownMSec`, `Charges`, `SpellCategoryID`, `ChrSpecializationID`, `LegacySlotIndex`, `TriggerType`, `ItemID`, `VerifiedBuild`) VALUES ('3000205', '263152', '-2', '-2', '-1', '4', '0', '0', '0', '3000205', '0');
INSERT INTO `item_sparse` (`ID`, `AllowableRace`, `Display`, `Display1`, `Display2`, `Display3`, `Description`, `Flags1`, `Flags2`, `Flags3`, `Flags4`, `PriceRandomValue`, `PriceVariance`, `VendorStackCount`, `BuyPrice`, `SellPrice`, `RequiredAbility`, `MaxCount`, `Stackable`, `StatPercentEditor1`, `StatPercentEditor2`, `StatPercentEditor3`, `StatPercentEditor4`, `StatPercentEditor5`, `StatPercentEditor6`, `StatPercentEditor7`, `StatPercentEditor8`, `StatPercentEditor9`, `StatPercentEditor10`, `StatPercentageOfSocket1`, `StatPercentageOfSocket2`, `StatPercentageOfSocket3`, `StatPercentageOfSocket4`, `StatPercentageOfSocket5`, `StatPercentageOfSocket6`, `StatPercentageOfSocket7`, `StatPercentageOfSocket8`, `StatPercentageOfSocket9`, `StatPercentageOfSocket10`, `ItemRange`, `BagFamily`, `QualityModifier`, `DurationInInventory`, `DmgVariance`, `AllowableClass`, `ItemLevel`, `RequiredSkill`, `RequiredSkillRank`, `MinFactionID`, `ItemStatValue1`, `ItemStatValue2`, `ItemStatValue3`, `ItemStatValue4`, `ItemStatValue5`, `ItemStatValue6`, `ItemStatValue7`, `ItemStatValue8`, `ItemStatValue9`, `ItemStatValue10`, `ScalingStatDistributionID`, `ItemDelay`, `PageID`, `StartQuestID`, `LockID`, `RandomSelect`, `ItemRandomSuffixGroupID`, `ItemSet`, `ZoneBound`, `InstanceBound`, `TotemCategoryID`, `SocketMatch_enchantment_id`, `GemProperties`, `LimitCategory`, `RequiredHoliday`, `RequiredTransmogHoliday`, `ItemNameDescriptionID`, `OverallQualityID`, `InventoryType`, `RequiredLevel`, `RequiredPVPRank`, `RequiredPVPMedal`, `MinReputation`, `ContainerSlots`, `StatModifierBonusStat1`, `StatModifierBonusStat2`, `StatModifierBonusStat3`, `StatModifierBonusStat4`, `StatModifierBonusStat5`, `StatModifierBonusStat6`, `StatModifierBonusStat7`, `StatModifierBonusStat8`, `StatModifierBonusStat9`, `StatModifierBonusStat10`, `DamageDamageType`, `Bonding`, `LanguageID`, `PageMaterialID`, `Material`, `SheatheType`, `SocketType1`, `SocketType2`, `SocketType3`, `SpellWeightCategory`, `SpellWeight`, `ArtifactID`, `ExpansionID`, `VerifiedBuild`) VALUES ('3000205', '-1', 'Goblin Arsonist Mercenary Contract', '', '', NULL, '|cFFFF6347SPP Goblin Arsonist Mercenary Scroll', '0', '268435456', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '8192', '4', '0', '0', '-1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '4', '0', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '0', '-1', '0', '0', '0', '0', '0', '0', '0', '0', '0');
INSERT INTO `item` (`ID`, `IconFileDataID`, `ClassID`, `SubclassID`, `SoundOverrideSubclass`, `Material`, `InventoryType`, `SheatheType`, `ItemGroupSoundsID`, `VerifiedBuild`) VALUES ('3000205', '1500889', '0', '0', '0', '-1', '0', '0', '0', '0');
INSERT INTO `hotfix_data` (`Id`, `TableHash`, `RecordID`, `Timestamp`, `Deleted`) VALUES ('3000205', '2442913102', '3000205', '0', '0');
INSERT INTO `hotfix_data` (`Id`, `TableHash`, `RecordID`, `Timestamp`, `Deleted`) VALUES ('3000205', '1344507586', '3000205', '0', '0');
INSERT INTO `hotfix_data` (`Id`, `TableHash`, `RecordID`, `Timestamp`, `Deleted`) VALUES ('3000205', '1073915313', '3000205', '0', '0');

WORLD DB:
INSERT INTO `item_script_names` (`Id`, `ScriptName`) VALUES ('3000205', 'goblin_grit');

*/
//arsonist
class Heroncore_MERC4 : public ItemScript
{
public:
	Heroncore_MERC4() : ItemScript("goblin_grit") { }

	bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/) override
	{
		if (!player || !item)
			return false;
		player->CastSpell(player, uint32(436972), true);
		player->DestroyItemCount(item->GetEntry(), 1, true);
		return true;
	}
};

//INSERT INTO `item_script_names` (`Id`, `ScriptName`) VALUES('127844', 'OLDWAR');
//Item 127844
class OLDWAR : public ItemScript
{
public:
	OLDWAR() : ItemScript("OLDWAR") { }

	bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/) override
	{
		if (!player || !item)
			return false;
		player->CastSpell(player, uint32(137639), true);
		player->DestroyItemCount(item->GetEntry(), 1, true);
		return true;
	}
};

/*
hotfixes db
INSERT INTO `item_effect` (`ID`, `SpellID`, `CoolDownMSec`, `CategoryCoolDownMSec`, `Charges`, `SpellCategoryID`, `ChrSpecializationID`, `LegacySlotIndex`, `TriggerType`, `ItemID`, `VerifiedBuild`) VALUES ('3000206', '263152', '-2', '-2', '-1', '4', '0', '0', '0', '3000206', '0');
INSERT INTO `item_sparse` (`ID`, `AllowableRace`, `Display`, `Display1`, `Display2`, `Display3`, `Description`, `Flags1`, `Flags2`, `Flags3`, `Flags4`, `PriceRandomValue`, `PriceVariance`, `VendorStackCount`, `BuyPrice`, `SellPrice`, `RequiredAbility`, `MaxCount`, `Stackable`, `StatPercentEditor1`, `StatPercentEditor2`, `StatPercentEditor3`, `StatPercentEditor4`, `StatPercentEditor5`, `StatPercentEditor6`, `StatPercentEditor7`, `StatPercentEditor8`, `StatPercentEditor9`, `StatPercentEditor10`, `StatPercentageOfSocket1`, `StatPercentageOfSocket2`, `StatPercentageOfSocket3`, `StatPercentageOfSocket4`, `StatPercentageOfSocket5`, `StatPercentageOfSocket6`, `StatPercentageOfSocket7`, `StatPercentageOfSocket8`, `StatPercentageOfSocket9`, `StatPercentageOfSocket10`, `ItemRange`, `BagFamily`, `QualityModifier`, `DurationInInventory`, `DmgVariance`, `AllowableClass`, `ItemLevel`, `RequiredSkill`, `RequiredSkillRank`, `MinFactionID`, `ItemStatValue1`, `ItemStatValue2`, `ItemStatValue3`, `ItemStatValue4`, `ItemStatValue5`, `ItemStatValue6`, `ItemStatValue7`, `ItemStatValue8`, `ItemStatValue9`, `ItemStatValue10`, `ScalingStatDistributionID`, `ItemDelay`, `PageID`, `StartQuestID`, `LockID`, `RandomSelect`, `ItemRandomSuffixGroupID`, `ItemSet`, `ZoneBound`, `InstanceBound`, `TotemCategoryID`, `SocketMatch_enchantment_id`, `GemProperties`, `LimitCategory`, `RequiredHoliday`, `RequiredTransmogHoliday`, `ItemNameDescriptionID`, `OverallQualityID`, `InventoryType`, `RequiredLevel`, `RequiredPVPRank`, `RequiredPVPMedal`, `MinReputation`, `ContainerSlots`, `StatModifierBonusStat1`, `StatModifierBonusStat2`, `StatModifierBonusStat3`, `StatModifierBonusStat4`, `StatModifierBonusStat5`, `StatModifierBonusStat6`, `StatModifierBonusStat7`, `StatModifierBonusStat8`, `StatModifierBonusStat9`, `StatModifierBonusStat10`, `DamageDamageType`, `Bonding`, `LanguageID`, `PageMaterialID`, `Material`, `SheatheType`, `SocketType1`, `SocketType2`, `SocketType3`, `SpellWeightCategory`, `SpellWeight`, `ArtifactID`, `ExpansionID`, `VerifiedBuild`) VALUES ('3000206', '-1', 'Level 100 Boost Scroll', '', '', NULL, '|cFFFF6347Level 100 Character Boost Scroll, ensure character has everything equipped before using. Plus one bonus Mercenary Contract.', '0', '268435456', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '8192', '4', '0', '0', '-1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '4', '0', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '0', '-1', '0', '0', '0', '0', '0', '0', '0', '0', '0');
INSERT INTO `item` (`ID`, `IconFileDataID`, `ClassID`, `SubclassID`, `SoundOverrideSubclass`, `Material`, `InventoryType`, `SheatheType`, `ItemGroupSoundsID`, `VerifiedBuild`) VALUES ('3000206', '1500889', '0', '0', '0', '-1', '0', '0', '0', '0');
INSERT INTO `hotfix_data` (`Id`, `TableHash`, `RecordID`, `Timestamp`, `Deleted`) VALUES ('3000206', '2442913102', '3000206', '0', '0');
INSERT INTO `hotfix_data` (`Id`, `TableHash`, `RecordID`, `Timestamp`, `Deleted`) VALUES ('3000206', '1344507586', '3000206', '0', '0');
INSERT INTO `hotfix_data` (`Id`, `TableHash`, `RecordID`, `Timestamp`, `Deleted`) VALUES ('3000206', '1073915313', '3000206', '0', '0');

WorldDB:
INSERT INTO `item_script_names` (`Id`, `ScriptName`) VALUES('3000206', 'hundredboost');
*/
//item 3000206
class hundredboost : public ItemScript
{
public:
	hundredboost() : ItemScript("hundredboost") { }

	bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/) override
	{
		if (!player || !item)
			return false;
		if (player->GetTeamId() == TEAM_ALLIANCE)
		{
			player->CastSpell(player, 136144, true);
		}
		else
		{
			player->CastSpell(player, 136143, true);
		}
		int32 tabMCC[4] = { 3000202,3000203,3000204,3000205 };
		uint8 idINTMCC = urand(1, 4);
		player->AddItem(tabMCC[idINTMCC], 1);
		player->CastSpell(player, 104855, true);
		player->SetLevel(100);
		player->DestroyItemCount(item->GetEntry(), 1, true);
		ObjectAccessor::SaveAllPlayers();
		return true;
	}
};

/*
hotfixes db
INSERT INTO `item_effect` (`ID`, `SpellID`, `CoolDownMSec`, `CategoryCoolDownMSec`, `Charges`, `SpellCategoryID`, `ChrSpecializationID`, `LegacySlotIndex`, `TriggerType`, `ItemID`, `VerifiedBuild`) VALUES ('3000050', '263152', '-2', '-2', '-1', '4', '0', '0', '0', '3000050', '0');
INSERT INTO `item_sparse` (`ID`, `AllowableRace`, `Display`, `Display1`, `Display2`, `Display3`, `Description`, `Flags1`, `Flags2`, `Flags3`, `Flags4`, `PriceRandomValue`, `PriceVariance`, `VendorStackCount`, `BuyPrice`, `SellPrice`, `RequiredAbility`, `MaxCount`, `Stackable`, `StatPercentEditor1`, `StatPercentEditor2`, `StatPercentEditor3`, `StatPercentEditor4`, `StatPercentEditor5`, `StatPercentEditor6`, `StatPercentEditor7`, `StatPercentEditor8`, `StatPercentEditor9`, `StatPercentEditor10`, `StatPercentageOfSocket1`, `StatPercentageOfSocket2`, `StatPercentageOfSocket3`, `StatPercentageOfSocket4`, `StatPercentageOfSocket5`, `StatPercentageOfSocket6`, `StatPercentageOfSocket7`, `StatPercentageOfSocket8`, `StatPercentageOfSocket9`, `StatPercentageOfSocket10`, `ItemRange`, `BagFamily`, `QualityModifier`, `DurationInInventory`, `DmgVariance`, `AllowableClass`, `ItemLevel`, `RequiredSkill`, `RequiredSkillRank`, `MinFactionID`, `ItemStatValue1`, `ItemStatValue2`, `ItemStatValue3`, `ItemStatValue4`, `ItemStatValue5`, `ItemStatValue6`, `ItemStatValue7`, `ItemStatValue8`, `ItemStatValue9`, `ItemStatValue10`, `ScalingStatDistributionID`, `ItemDelay`, `PageID`, `StartQuestID`, `LockID`, `RandomSelect`, `ItemRandomSuffixGroupID`, `ItemSet`, `ZoneBound`, `InstanceBound`, `TotemCategoryID`, `SocketMatch_enchantment_id`, `GemProperties`, `LimitCategory`, `RequiredHoliday`, `RequiredTransmogHoliday`, `ItemNameDescriptionID`, `OverallQualityID`, `InventoryType`, `RequiredLevel`, `RequiredPVPRank`, `RequiredPVPMedal`, `MinReputation`, `ContainerSlots`, `StatModifierBonusStat1`, `StatModifierBonusStat2`, `StatModifierBonusStat3`, `StatModifierBonusStat4`, `StatModifierBonusStat5`, `StatModifierBonusStat6`, `StatModifierBonusStat7`, `StatModifierBonusStat8`, `StatModifierBonusStat9`, `StatModifierBonusStat10`, `DamageDamageType`, `Bonding`, `LanguageID`, `PageMaterialID`, `Material`, `SheatheType`, `SocketType1`, `SocketType2`, `SocketType3`, `SpellWeightCategory`, `SpellWeight`, `ArtifactID`, `ExpansionID`, `VerifiedBuild`) VALUES ('3000050', '-1', '|cFFD2691ESPP Pandaren Starter Zone Skip Scroll', '', '', NULL, '|cFFD2691ESPP Pandaren Starter Zone Skip Scroll', '0', '268435456', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '8192', '4', '0', '0', '-1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '8', '0', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '0', '-1', '0', '0', '0', '0', '0', '0', '0', '0', '0');
INSERT INTO `item` (`ID`, `IconFileDataID`, `ClassID`, `SubclassID`, `SoundOverrideSubclass`, `Material`, `InventoryType`, `SheatheType`, `ItemGroupSoundsID`, `VerifiedBuild`) VALUES ('3000050', '1500869', '0', '0', '0', '-1', '0', '0', '0', '0');
INSERT INTO `hotfix_data` (`Id`, `TableHash`, `RecordID`, `Timestamp`, `Deleted`) VALUES ('3000050', '2442913102', '3000050', '0', '0');
INSERT INTO `hotfix_data` (`Id`, `TableHash`, `RecordID`, `Timestamp`, `Deleted`) VALUES ('3000050', '1344507586', '3000050', '0', '0');
INSERT INTO `hotfix_data` (`Id`, `TableHash`, `RecordID`, `Timestamp`, `Deleted`) VALUES ('3000050', '1073915313', '3000050', '0', '0');

world db
INSERT INTO `item_script_names` (`Id`, `ScriptName`) VALUES('3000050', 'PandaSkip');
*/
class PandaSkip : public ItemScript
{
public:
	PandaSkip() : ItemScript("PandaSkip") { }

	bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/) override
	{
		if (!player || !item)
			return false;
		if (player->getRace() == RACE_PANDAREN_NEUTRAL)
		{
			player->ShowNeutralPlayerFactionSelectUI();
			player->SetLevel(10);
			player->DestroyItemCount(item->GetEntry(), 1, true);
			ObjectAccessor::SaveAllPlayers();
		}
		return true;
	}
};

/*
hotfixes db
INSERT INTO `item_effect` (`ID`, `SpellID`, `CoolDownMSec`, `CategoryCoolDownMSec`, `Charges`, `SpellCategoryID`, `ChrSpecializationID`, `LegacySlotIndex`, `TriggerType`, `ItemID`, `VerifiedBuild`) VALUES ('3000051', '263152', '-2', '-2', '-1', '4', '0', '0', '0', '3000051', '0');
INSERT INTO `item_sparse` (`ID`, `AllowableRace`, `Display`, `Display1`, `Display2`, `Display3`, `Description`, `Flags1`, `Flags2`, `Flags3`, `Flags4`, `PriceRandomValue`, `PriceVariance`, `VendorStackCount`, `BuyPrice`, `SellPrice`, `RequiredAbility`, `MaxCount`, `Stackable`, `StatPercentEditor1`, `StatPercentEditor2`, `StatPercentEditor3`, `StatPercentEditor4`, `StatPercentEditor5`, `StatPercentEditor6`, `StatPercentEditor7`, `StatPercentEditor8`, `StatPercentEditor9`, `StatPercentEditor10`, `StatPercentageOfSocket1`, `StatPercentageOfSocket2`, `StatPercentageOfSocket3`, `StatPercentageOfSocket4`, `StatPercentageOfSocket5`, `StatPercentageOfSocket6`, `StatPercentageOfSocket7`, `StatPercentageOfSocket8`, `StatPercentageOfSocket9`, `StatPercentageOfSocket10`, `ItemRange`, `BagFamily`, `QualityModifier`, `DurationInInventory`, `DmgVariance`, `AllowableClass`, `ItemLevel`, `RequiredSkill`, `RequiredSkillRank`, `MinFactionID`, `ItemStatValue1`, `ItemStatValue2`, `ItemStatValue3`, `ItemStatValue4`, `ItemStatValue5`, `ItemStatValue6`, `ItemStatValue7`, `ItemStatValue8`, `ItemStatValue9`, `ItemStatValue10`, `ScalingStatDistributionID`, `ItemDelay`, `PageID`, `StartQuestID`, `LockID`, `RandomSelect`, `ItemRandomSuffixGroupID`, `ItemSet`, `ZoneBound`, `InstanceBound`, `TotemCategoryID`, `SocketMatch_enchantment_id`, `GemProperties`, `LimitCategory`, `RequiredHoliday`, `RequiredTransmogHoliday`, `ItemNameDescriptionID`, `OverallQualityID`, `InventoryType`, `RequiredLevel`, `RequiredPVPRank`, `RequiredPVPMedal`, `MinReputation`, `ContainerSlots`, `StatModifierBonusStat1`, `StatModifierBonusStat2`, `StatModifierBonusStat3`, `StatModifierBonusStat4`, `StatModifierBonusStat5`, `StatModifierBonusStat6`, `StatModifierBonusStat7`, `StatModifierBonusStat8`, `StatModifierBonusStat9`, `StatModifierBonusStat10`, `DamageDamageType`, `Bonding`, `LanguageID`, `PageMaterialID`, `Material`, `SheatheType`, `SocketType1`, `SocketType2`, `SocketType3`, `SpellWeightCategory`, `SpellWeight`, `ArtifactID`, `ExpansionID`, `VerifiedBuild`) VALUES ('3000051', '-1', '|cFF1E90FFSPP Death Knight Starter Zone Skip Scroll', '', '', NULL, '|cFF1E90FFSPP Death Knight Starter Zone Skip Scroll', '0', '268435456', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '8192', '4', '0', '0', '-1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '8', '0', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '0', '-1', '0', '0', '0', '0', '0', '0', '0', '0', '0');
INSERT INTO `item` (`ID`, `IconFileDataID`, `ClassID`, `SubclassID`, `SoundOverrideSubclass`, `Material`, `InventoryType`, `SheatheType`, `ItemGroupSoundsID`, `VerifiedBuild`) VALUES ('3000051', '1500869', '0', '0', '0', '-1', '0', '0', '0', '0');
INSERT INTO `hotfix_data` (`Id`, `TableHash`, `RecordID`, `Timestamp`, `Deleted`) VALUES ('3000051', '2442913102', '3000051', '0', '0');
INSERT INTO `hotfix_data` (`Id`, `TableHash`, `RecordID`, `Timestamp`, `Deleted`) VALUES ('3000051', '1344507586', '3000051', '0', '0');
INSERT INTO `hotfix_data` (`Id`, `TableHash`, `RecordID`, `Timestamp`, `Deleted`) VALUES ('3000051', '1073915313', '3000051', '0', '0');

world db
INSERT INTO `item_script_names` (`Id`, `ScriptName`) VALUES('3000051', 'DeathSkip');
*/
class DeathSkip : public ItemScript
{
public:
	DeathSkip() : ItemScript("DeathSkip") { }

	bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/) override
	{
		if (!player || !item)
			return false;
		if (player->getClass() == CLASS_DEATH_KNIGHT && player->getRace() == RACE_PANDAREN_NEUTRAL)
		{
			player->SetLevel(58);
			player->learnSpell(53428, true); // runeforging
			player->learnSpell(53431, true); // runeforging
			player->learnSpell(53441, true); // runeforging
			player->learnSpell(54586, true); // runeforging credit
			player->learnSpell(62622, true); // runeforging credit
			player->learnSpell(48778, true);//acherus deathcharger
			player->learnSkillRewardedSpells(776, 375);
			player->learnSkillRewardedSpells(960, 375);
			player->AddItem(39320, true);
			player->AddItem(38664, true);
			player->AddItem(39322, true);
			player->AddItem(38633, true);
			player->AddItem(38632, true);
			player->AddItem(6948, true);
			player->ShowNeutralPlayerFactionSelectUI();
			player->DestroyItemCount(item->GetEntry(), 1, true);
		}
		if (player->getClass() == CLASS_DEATH_KNIGHT)
		{
			player->SetLevel(58);
			player->learnSpell(53428, true); // runeforging
			player->learnSpell(53431, true); // runeforging
			player->learnSpell(53441, true); // runeforging
			player->learnSpell(54586, true); // runeforging credit
			player->learnSpell(62622, true); // runeforging credit
			player->learnSpell(48778, true);//acherus deathcharger
			player->learnSkillRewardedSpells(776, 375);
			player->learnSkillRewardedSpells(960, 375);
			player->AddItem(39320, true);
			player->AddItem(38664, true);
			player->AddItem(39322, true);
			player->AddItem(38633, true);
			player->AddItem(38632, true);
			player->AddItem(6948, true);

			if (player->GetTeam() == ALLIANCE)
				player->TeleportTo(0, -8833.37f, 628.62f, 94.00f, 1.06f);
			else
				player->TeleportTo(1, 1569.59f, -4397.63f, 16.06f, 0.54f);

			player->DestroyItemCount(item->GetEntry(), 1, true);
			ObjectAccessor::SaveAllPlayers();
		}
		return true;
	}
};

/*
hotfixes db
INSERT INTO `item_effect` (`ID`, `SpellID`, `CoolDownMSec`, `CategoryCoolDownMSec`, `Charges`, `SpellCategoryID`, `ChrSpecializationID`, `LegacySlotIndex`, `TriggerType`, `ItemID`, `VerifiedBuild`) VALUES ('3000052', '263152', '-2', '-2', '-1', '4', '0', '0', '0', '3000052', '0');
INSERT INTO `item_sparse` (`ID`, `AllowableRace`, `Display`, `Display1`, `Display2`, `Display3`, `Description`, `Flags1`, `Flags2`, `Flags3`, `Flags4`, `PriceRandomValue`, `PriceVariance`, `VendorStackCount`, `BuyPrice`, `SellPrice`, `RequiredAbility`, `MaxCount`, `Stackable`, `StatPercentEditor1`, `StatPercentEditor2`, `StatPercentEditor3`, `StatPercentEditor4`, `StatPercentEditor5`, `StatPercentEditor6`, `StatPercentEditor7`, `StatPercentEditor8`, `StatPercentEditor9`, `StatPercentEditor10`, `StatPercentageOfSocket1`, `StatPercentageOfSocket2`, `StatPercentageOfSocket3`, `StatPercentageOfSocket4`, `StatPercentageOfSocket5`, `StatPercentageOfSocket6`, `StatPercentageOfSocket7`, `StatPercentageOfSocket8`, `StatPercentageOfSocket9`, `StatPercentageOfSocket10`, `ItemRange`, `BagFamily`, `QualityModifier`, `DurationInInventory`, `DmgVariance`, `AllowableClass`, `ItemLevel`, `RequiredSkill`, `RequiredSkillRank`, `MinFactionID`, `ItemStatValue1`, `ItemStatValue2`, `ItemStatValue3`, `ItemStatValue4`, `ItemStatValue5`, `ItemStatValue6`, `ItemStatValue7`, `ItemStatValue8`, `ItemStatValue9`, `ItemStatValue10`, `ScalingStatDistributionID`, `ItemDelay`, `PageID`, `StartQuestID`, `LockID`, `RandomSelect`, `ItemRandomSuffixGroupID`, `ItemSet`, `ZoneBound`, `InstanceBound`, `TotemCategoryID`, `SocketMatch_enchantment_id`, `GemProperties`, `LimitCategory`, `RequiredHoliday`, `RequiredTransmogHoliday`, `ItemNameDescriptionID`, `OverallQualityID`, `InventoryType`, `RequiredLevel`, `RequiredPVPRank`, `RequiredPVPMedal`, `MinReputation`, `ContainerSlots`, `StatModifierBonusStat1`, `StatModifierBonusStat2`, `StatModifierBonusStat3`, `StatModifierBonusStat4`, `StatModifierBonusStat5`, `StatModifierBonusStat6`, `StatModifierBonusStat7`, `StatModifierBonusStat8`, `StatModifierBonusStat9`, `StatModifierBonusStat10`, `DamageDamageType`, `Bonding`, `LanguageID`, `PageMaterialID`, `Material`, `SheatheType`, `SocketType1`, `SocketType2`, `SocketType3`, `SpellWeightCategory`, `SpellWeight`, `ArtifactID`, `ExpansionID`, `VerifiedBuild`) VALUES ('3000052', '-1', '|cFF90EE90SPP Goblin Starter Zone Skip Scroll', '', '', NULL, '|cFF90EE90SPP Goblin Starter Zone Skip Scroll', '0', '268435456', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '8192', '4', '0', '0', '-1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '8', '0', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '0', '-1', '0', '0', '0', '0', '0', '0', '0', '0', '0');
INSERT INTO `item` (`ID`, `IconFileDataID`, `ClassID`, `SubclassID`, `SoundOverrideSubclass`, `Material`, `InventoryType`, `SheatheType`, `ItemGroupSoundsID`, `VerifiedBuild`) VALUES ('3000052', '1500869', '0', '0', '0', '-1', '0', '0', '0', '0');
INSERT INTO `hotfix_data` (`Id`, `TableHash`, `RecordID`, `Timestamp`, `Deleted`) VALUES ('3000052', '2442913102', '3000052', '0', '0');
INSERT INTO `hotfix_data` (`Id`, `TableHash`, `RecordID`, `Timestamp`, `Deleted`) VALUES ('3000052', '1344507586', '3000052', '0', '0');
INSERT INTO `hotfix_data` (`Id`, `TableHash`, `RecordID`, `Timestamp`, `Deleted`) VALUES ('3000052', '1073915313', '3000052', '0', '0');

world db
INSERT INTO `item_script_names` (`Id`, `ScriptName`) VALUES('3000052', 'GobSkip');
*/
class GobSkip : public ItemScript
{
public:
	GobSkip() : ItemScript("GobSkip") { }

	bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/) override
	{
		if (!player || !item)
			return false;
		if (player->getRace() == RACE_GOBLIN)
		{
			player->learnSpell(77534, true); // Pack Hobgoblin
			player->SetLevel(16);
			player->TeleportTo(1, 1569.59f, -4397.63f, 16.06f, 0.54f);
			player->DestroyItemCount(item->GetEntry(), 1, true);
			ObjectAccessor::SaveAllPlayers();
		}
		return true;
	}
};

/*
hotfixesdb
INSERT INTO `item_effect` (`ID`, `SpellID`, `CoolDownMSec`, `CategoryCoolDownMSec`, `Charges`, `SpellCategoryID`, `ChrSpecializationID`, `LegacySlotIndex`, `TriggerType`, `ItemID`, `VerifiedBuild`) VALUES ('3000053', '263152', '-2', '-2', '-1', '4', '0', '0', '0', '3000053', '0');
INSERT INTO `item_sparse` (`ID`, `AllowableRace`, `Display`, `Display1`, `Display2`, `Display3`, `Description`, `Flags1`, `Flags2`, `Flags3`, `Flags4`, `PriceRandomValue`, `PriceVariance`, `VendorStackCount`, `BuyPrice`, `SellPrice`, `RequiredAbility`, `MaxCount`, `Stackable`, `StatPercentEditor1`, `StatPercentEditor2`, `StatPercentEditor3`, `StatPercentEditor4`, `StatPercentEditor5`, `StatPercentEditor6`, `StatPercentEditor7`, `StatPercentEditor8`, `StatPercentEditor9`, `StatPercentEditor10`, `StatPercentageOfSocket1`, `StatPercentageOfSocket2`, `StatPercentageOfSocket3`, `StatPercentageOfSocket4`, `StatPercentageOfSocket5`, `StatPercentageOfSocket6`, `StatPercentageOfSocket7`, `StatPercentageOfSocket8`, `StatPercentageOfSocket9`, `StatPercentageOfSocket10`, `ItemRange`, `BagFamily`, `QualityModifier`, `DurationInInventory`, `DmgVariance`, `AllowableClass`, `ItemLevel`, `RequiredSkill`, `RequiredSkillRank`, `MinFactionID`, `ItemStatValue1`, `ItemStatValue2`, `ItemStatValue3`, `ItemStatValue4`, `ItemStatValue5`, `ItemStatValue6`, `ItemStatValue7`, `ItemStatValue8`, `ItemStatValue9`, `ItemStatValue10`, `ScalingStatDistributionID`, `ItemDelay`, `PageID`, `StartQuestID`, `LockID`, `RandomSelect`, `ItemRandomSuffixGroupID`, `ItemSet`, `ZoneBound`, `InstanceBound`, `TotemCategoryID`, `SocketMatch_enchantment_id`, `GemProperties`, `LimitCategory`, `RequiredHoliday`, `RequiredTransmogHoliday`, `ItemNameDescriptionID`, `OverallQualityID`, `InventoryType`, `RequiredLevel`, `RequiredPVPRank`, `RequiredPVPMedal`, `MinReputation`, `ContainerSlots`, `StatModifierBonusStat1`, `StatModifierBonusStat2`, `StatModifierBonusStat3`, `StatModifierBonusStat4`, `StatModifierBonusStat5`, `StatModifierBonusStat6`, `StatModifierBonusStat7`, `StatModifierBonusStat8`, `StatModifierBonusStat9`, `StatModifierBonusStat10`, `DamageDamageType`, `Bonding`, `LanguageID`, `PageMaterialID`, `Material`, `SheatheType`, `SocketType1`, `SocketType2`, `SocketType3`, `SpellWeightCategory`, `SpellWeight`, `ArtifactID`, `ExpansionID`, `VerifiedBuild`) VALUES ('3000053', '-1', '|cFFFFD700SPP Worgen Starter Zone Skip Scroll', '', '', NULL, '|cFFFFD700SPP Worgen Starter Zone Skip Scroll', '0', '268435456', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '8192', '4', '0', '0', '-1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '8', '0', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '0', '-1', '0', '0', '0', '0', '0', '0', '0', '0', '0');
INSERT INTO `item` (`ID`, `IconFileDataID`, `ClassID`, `SubclassID`, `SoundOverrideSubclass`, `Material`, `InventoryType`, `SheatheType`, `ItemGroupSoundsID`, `VerifiedBuild`) VALUES ('3000053', '1500869', '0', '0', '0', '-1', '0', '0', '0', '0');
INSERT INTO `hotfix_data` (`Id`, `TableHash`, `RecordID`, `Timestamp`, `Deleted`) VALUES ('3000053', '2442913102', '3000053', '0', '0');
INSERT INTO `hotfix_data` (`Id`, `TableHash`, `RecordID`, `Timestamp`, `Deleted`) VALUES ('3000053', '1344507586', '3000053', '0', '0');
INSERT INTO `hotfix_data` (`Id`, `TableHash`, `RecordID`, `Timestamp`, `Deleted`) VALUES ('3000053', '1073915313', '3000053', '0', '0');

world db
INSERT INTO `item_script_names` (`Id`, `ScriptName`) VALUES('3000053', 'FurrySkip');
*/
class FurrySkip : public ItemScript
{
public:
	FurrySkip() : ItemScript("FurrySkip") { }

	bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/) override
	{
		if (!player || !item)
			return false;
		if (player->getRace() == RACE_WORGEN)
		{
			player->learnSpell(72792, true); // Learn Racials
			player->learnSpell(72857, true); // Learn Two Forms
			player->learnSpell(95759, true); // Learn Darkflight
			player->TeleportTo(1, 8181.060059f, 999.103027f, 7.253240f, 6.174160f);
			player->SetLevel(18);
			player->DestroyItemCount(item->GetEntry(), 1, true);
			ObjectAccessor::SaveAllPlayers();
		}
		return true;
	}
};

/*
Hotfixes DB:
INSERT INTO `item_effect` (`ID`, `SpellID`, `CoolDownMSec`, `CategoryCoolDownMSec`, `Charges`, `SpellCategoryID`, `ChrSpecializationID`, `LegacySlotIndex`, `TriggerType`, `ItemID`, `VerifiedBuild`) VALUES ('3000207', '263152', '-2', '-2', '-1', '4', '0', '0', '0', '3000207', '0');
INSERT INTO `item_sparse` (`ID`, `AllowableRace`, `Display`, `Display1`, `Display2`, `Display3`, `Description`, `Flags1`, `Flags2`, `Flags3`, `Flags4`, `PriceRandomValue`, `PriceVariance`, `VendorStackCount`, `BuyPrice`, `SellPrice`, `RequiredAbility`, `MaxCount`, `Stackable`, `StatPercentEditor1`, `StatPercentEditor2`, `StatPercentEditor3`, `StatPercentEditor4`, `StatPercentEditor5`, `StatPercentEditor6`, `StatPercentEditor7`, `StatPercentEditor8`, `StatPercentEditor9`, `StatPercentEditor10`, `StatPercentageOfSocket1`, `StatPercentageOfSocket2`, `StatPercentageOfSocket3`, `StatPercentageOfSocket4`, `StatPercentageOfSocket5`, `StatPercentageOfSocket6`, `StatPercentageOfSocket7`, `StatPercentageOfSocket8`, `StatPercentageOfSocket9`, `StatPercentageOfSocket10`, `ItemRange`, `BagFamily`, `QualityModifier`, `DurationInInventory`, `DmgVariance`, `AllowableClass`, `ItemLevel`, `RequiredSkill`, `RequiredSkillRank`, `MinFactionID`, `ItemStatValue1`, `ItemStatValue2`, `ItemStatValue3`, `ItemStatValue4`, `ItemStatValue5`, `ItemStatValue6`, `ItemStatValue7`, `ItemStatValue8`, `ItemStatValue9`, `ItemStatValue10`, `ScalingStatDistributionID`, `ItemDelay`, `PageID`, `StartQuestID`, `LockID`, `RandomSelect`, `ItemRandomSuffixGroupID`, `ItemSet`, `ZoneBound`, `InstanceBound`, `TotemCategoryID`, `SocketMatch_enchantment_id`, `GemProperties`, `LimitCategory`, `RequiredHoliday`, `RequiredTransmogHoliday`, `ItemNameDescriptionID`, `OverallQualityID`, `InventoryType`, `RequiredLevel`, `RequiredPVPRank`, `RequiredPVPMedal`, `MinReputation`, `ContainerSlots`, `StatModifierBonusStat1`, `StatModifierBonusStat2`, `StatModifierBonusStat3`, `StatModifierBonusStat4`, `StatModifierBonusStat5`, `StatModifierBonusStat6`, `StatModifierBonusStat7`, `StatModifierBonusStat8`, `StatModifierBonusStat9`, `StatModifierBonusStat10`, `DamageDamageType`, `Bonding`, `LanguageID`, `PageMaterialID`, `Material`, `SheatheType`, `SocketType1`, `SocketType2`, `SocketType3`, `SpellWeightCategory`, `SpellWeight`, `ArtifactID`, `ExpansionID`, `VerifiedBuild`) VALUES ('3000207', '-1', 'SPP Skip Demon Hunter Starter Zone Scroll', '', '', NULL, '|cFFFF6347SPP Skip Demon Hunter Starter Zone Scroll', '0', '268435456', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '8192', '4', '0', '0', '-1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '4', '0', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '0', '-1', '0', '0', '0', '0', '0', '0', '0', '0', '0');
INSERT INTO `item` (`ID`, `IconFileDataID`, `ClassID`, `SubclassID`, `SoundOverrideSubclass`, `Material`, `InventoryType`, `SheatheType`, `ItemGroupSoundsID`, `VerifiedBuild`) VALUES ('3000207', '1500889', '0', '0', '0', '-1', '0', '0', '0', '0');
INSERT INTO `hotfix_data` (`Id`, `TableHash`, `RecordID`, `Timestamp`, `Deleted`) VALUES ('3000207', '2442913102', '3000207', '0', '0');
INSERT INTO `hotfix_data` (`Id`, `TableHash`, `RecordID`, `Timestamp`, `Deleted`) VALUES ('3000207', '1344507586', '3000207', '0', '0');
INSERT INTO `hotfix_data` (`Id`, `TableHash`, `RecordID`, `Timestamp`, `Deleted`) VALUES ('3000207', '1073915313', '3000207', '0', '0');

World db:
INSERT INTO `item_script_names` (`Id`, `ScriptName`) VALUES('3000207', 'DemonSkip');
*/

class DemonSkip : public ItemScript
{
public:
	DemonSkip() : ItemScript("DemonSkip") { }

	bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/) override
	{
		if (!player || !item)
			return false;
		if (player->getClass() == CLASS_DEMON_HUNTER && player->getRace() == RACE_PANDAREN_NEUTRAL)
		{
			player->SetLevel(100);
			player->learnSpell(200175, true); //Fel Saber Mount
			player->AddItem(129192, 1); // inquisitors - menacing - eye
			player->AddItem(128946, 1); // mardum - lord - signet
			player->AddItem(128948, 1); // nefarious - ring
			player->AddItem(128947, 1); // pit - lords - cuffs
			player->AddItem(133580, 1); // brutargs - sword - tip
			player->AddItem(129195, 1); // vile - stalkerskin - pouch
			player->AddItem(128944, 1); // voras - silk - drape
			player->AddItem(128958, 1); // lekos - leash
			player->AddItem(128945, 1); // inquisitors - glowering - eye
			player->AddItem(128953, 1); // treads - of - illidari - supremacy
			player->AddItem(128952, 1); // torment - enders - chestguard
			player->AddItem(128951, 1); // leggings - of - sacrifice
			player->AddItem(128954, 1); // power - handlers - gloves
			player->AddItem(133311, 1); // demon - rend - shoulderblades
			player->AddItem(133310, 1); // the - brood - queens - veil
			player->AddItem(128949, 1); // infernal - firecord - sash
			player->AddItem(141650, 1); // helm - of - reaffirmed - purpose
			player->learnSpell(198013, true); // eye - beam
			player->learnSpell(183752, true); // disrupt
			player->learnSpell(185123, true); // throw - glaive
			player->learnSpell(191427, true); // metamorphosis
			player->learnSpell(198793, true); // vengeful - retreat)
			player->learnSpell(204021, true); // fiery - brand)
			player->learnSpell(179057, true); // chaos - nova
			player->learnSpell(178740, true); // immolation - aura
			player->learnSpell(198589, true); // blur
			player->learnSpell(185245, true); // torment
			player->ShowNeutralPlayerFactionSelectUI();
			ObjectAccessor::SaveAllPlayers();
			player->DestroyItemCount(item->GetEntry(), 1, true);
		}
		if (player->getClass() == CLASS_DEMON_HUNTER)
		{
			player->SetLevel(100);
			player->learnSpell(200175, true); //Fel Saber Mount
			player->AddItem(129192, 1); // inquisitors - menacing - eye
			player->AddItem(128946, 1); // mardum - lord - signet
			player->AddItem(128948, 1); // nefarious - ring
			player->AddItem(128947, 1); // pit - lords - cuffs
			player->AddItem(133580, 1); // brutargs - sword - tip
			player->AddItem(129195, 1); // vile - stalkerskin - pouch
			player->AddItem(128944, 1); // voras - silk - drape
			player->AddItem(128958, 1); // lekos - leash
			player->AddItem(128945, 1); // inquisitors - glowering - eye
			player->AddItem(128953, 1); // treads - of - illidari - supremacy
			player->AddItem(128952, 1); // torment - enders - chestguard
			player->AddItem(128951, 1); // leggings - of - sacrifice
			player->AddItem(128954, 1); // power - handlers - gloves
			player->AddItem(133311, 1); // demon - rend - shoulderblades
			player->AddItem(133310, 1); // the - brood - queens - veil
			player->AddItem(128949, 1); // infernal - firecord - sash
			player->AddItem(141650, 1); // helm - of - reaffirmed - purpose
			player->learnSpell(198013, true); // eye - beam
			player->learnSpell(183752, true); // disrupt
			player->learnSpell(185123, true); // throw - glaive
			player->learnSpell(191427, true); // metamorphosis
			player->learnSpell(198793, true); // vengeful - retreat)
			player->learnSpell(204021, true); // fiery - brand)
			player->learnSpell(179057, true); // chaos - nova
			player->learnSpell(178740, true); // immolation - aura
			player->learnSpell(198589, true); // blur
			player->learnSpell(185245, true); // torment
			ObjectAccessor::SaveAllPlayers();

			if (player->GetTeam() == ALLIANCE)
				player->TeleportTo(0, -8833.37f, 628.62f, 94.00f, 1.06f);
			else
				player->TeleportTo(1, 1569.59f, -4397.63f, 16.06f, 0.54f);

			player->DestroyItemCount(item->GetEntry(), 1, true);
		}
		return true;
	}
};

/*
hotfixes db:
INSERT INTO `item_effect` (`ID`, `SpellID`, `CoolDownMSec`, `CategoryCoolDownMSec`, `Charges`, `SpellCategoryID`, `ChrSpecializationID`, `LegacySlotIndex`, `TriggerType`, `ItemID`, `VerifiedBuild`) VALUES ('3000208', '263152', '-2', '-2', '-1', '4', '0', '0', '0', '3000208', '0');
INSERT INTO `item_sparse` (`ID`, `AllowableRace`, `Display`, `Display1`, `Display2`, `Display3`, `Description`, `Flags1`, `Flags2`, `Flags3`, `Flags4`, `PriceRandomValue`, `PriceVariance`, `VendorStackCount`, `BuyPrice`, `SellPrice`, `RequiredAbility`, `MaxCount`, `Stackable`, `StatPercentEditor1`, `StatPercentEditor2`, `StatPercentEditor3`, `StatPercentEditor4`, `StatPercentEditor5`, `StatPercentEditor6`, `StatPercentEditor7`, `StatPercentEditor8`, `StatPercentEditor9`, `StatPercentEditor10`, `StatPercentageOfSocket1`, `StatPercentageOfSocket2`, `StatPercentageOfSocket3`, `StatPercentageOfSocket4`, `StatPercentageOfSocket5`, `StatPercentageOfSocket6`, `StatPercentageOfSocket7`, `StatPercentageOfSocket8`, `StatPercentageOfSocket9`, `StatPercentageOfSocket10`, `ItemRange`, `BagFamily`, `QualityModifier`, `DurationInInventory`, `DmgVariance`, `AllowableClass`, `ItemLevel`, `RequiredSkill`, `RequiredSkillRank`, `MinFactionID`, `ItemStatValue1`, `ItemStatValue2`, `ItemStatValue3`, `ItemStatValue4`, `ItemStatValue5`, `ItemStatValue6`, `ItemStatValue7`, `ItemStatValue8`, `ItemStatValue9`, `ItemStatValue10`, `ScalingStatDistributionID`, `ItemDelay`, `PageID`, `StartQuestID`, `LockID`, `RandomSelect`, `ItemRandomSuffixGroupID`, `ItemSet`, `ZoneBound`, `InstanceBound`, `TotemCategoryID`, `SocketMatch_enchantment_id`, `GemProperties`, `LimitCategory`, `RequiredHoliday`, `RequiredTransmogHoliday`, `ItemNameDescriptionID`, `OverallQualityID`, `InventoryType`, `RequiredLevel`, `RequiredPVPRank`, `RequiredPVPMedal`, `MinReputation`, `ContainerSlots`, `StatModifierBonusStat1`, `StatModifierBonusStat2`, `StatModifierBonusStat3`, `StatModifierBonusStat4`, `StatModifierBonusStat5`, `StatModifierBonusStat6`, `StatModifierBonusStat7`, `StatModifierBonusStat8`, `StatModifierBonusStat9`, `StatModifierBonusStat10`, `DamageDamageType`, `Bonding`, `LanguageID`, `PageMaterialID`, `Material`, `SheatheType`, `SocketType1`, `SocketType2`, `SocketType3`, `SpellWeightCategory`, `SpellWeight`, `ArtifactID`, `ExpansionID`, `VerifiedBuild`) VALUES ('3000208', '-1', 'SPP MOUNTS SCROLL', '', '', NULL, '|cFFFF6347SPP Mounts Scroll', '0', '268435456', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '8192', '4', '0', '0', '-1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '4', '0', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '0', '-1', '0', '0', '0', '0', '0', '0', '0', '0', '0');
INSERT INTO `item` (`ID`, `IconFileDataID`, `ClassID`, `SubclassID`, `SoundOverrideSubclass`, `Material`, `InventoryType`, `SheatheType`, `ItemGroupSoundsID`, `VerifiedBuild`) VALUES ('3000208', '1500889', '0', '0', '0', '-1', '0', '0', '0', '0');
INSERT INTO `hotfix_data` (`Id`, `TableHash`, `RecordID`, `Timestamp`, `Deleted`) VALUES ('3000208', '2442913102', '3000208', '0', '0');
INSERT INTO `hotfix_data` (`Id`, `TableHash`, `RecordID`, `Timestamp`, `Deleted`) VALUES ('3000208', '1344507586', '3000208', '0', '0');
INSERT INTO `hotfix_data` (`Id`, `TableHash`, `RecordID`, `Timestamp`, `Deleted`) VALUES ('3000208', '1073915313', '3000208', '0', '0');

world db:
INSERT INTO `item_script_names` (`Id`, `ScriptName`) VALUES('3000208', 'SPP_Mounts_Scroll');
*/
class Heroncore_Mounts_Scroll : public ItemScript
{
public:
	Heroncore_Mounts_Scroll() : ItemScript("Heroncore_Mounts_Scroll") { }

	bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/) override
	{
		if (!player || !item)
			return false;

		player->learnSpell(48778, false);     //Acherus Deathcharger
		player->learnSpell(60025, false);     //Albino Drake
		player->learnSpell(127180, false);     //Albino Riding Crane
		player->learnSpell(98204, false);     //Amani Battle Bear
		player->learnSpell(96503, false);     //Amani Dragonhawk
		player->learnSpell(43688, false);     //Amani War Bear
		player->learnSpell(138424, false);     //Amber Primordial Direhorn
		player->learnSpell(123886, false);     //Amber Scorpion
		player->learnSpell(16056, false);     //Ancient Frostsaber
		player->learnSpell(171618, false);     //Ancient Leatherhide
		player->learnSpell(16081, false);     //Arctic Wolf
		player->learnSpell(66906, false);     //Argent Charger
		player->learnSpell(63844, false);     //Argent Hippogryph
		player->learnSpell(66907, false);     //Argent Warhorse
		player->learnSpell(67466, false);     //Argent Warhorse
		player->learnSpell(139595, false);     //Armored Bloodwing
		player->learnSpell(142478, false);     //Armored Blue Dragonhawk
		player->learnSpell(61230, false);     //Armored Blue Wind Rider
		player->learnSpell(60114, false);     //Armored Brown Bear
		player->learnSpell(60116, false);     //Armored Brown Bear
		player->learnSpell(171629, false);     //Armored Frostboar
		player->learnSpell(171838, false);     //Armored Frostwolf
		player->learnSpell(171626, false);     //Armored Irontusk
		player->learnSpell(171630, false);     //Armored Razorback
		player->learnSpell(96491, false);     //Armored Razzashi Raptor
		player->learnSpell(142266, false);     //Armored Red Dragonhawk
		player->learnSpell(136400, false);     //Armored Skyscreamer
		player->learnSpell(61229, false);     //Armored Snowy Gryphon
		player->learnSpell(132117, false);     //Ashen Pandaren Phoenix
		player->learnSpell(40192, false);     //Ashes of Al'ar
		player->learnSpell(148428, false);     //Ashhide Mushan Beast
		player->learnSpell(127170, false);     //Astral Cloud Serpent
		player->learnSpell(123992, false);     //Azure Cloud Serpent
		player->learnSpell(59567, false);     //Azure Drake
		player->learnSpell(41514, false);     //Azure Netherwing Drake
		player->learnSpell(127174, false);     //Azure Riding Crane
		player->learnSpell(118089, false);     //Azure Water Strider
		player->learnSpell(51412, false);     //Big Battle Bear
		player->learnSpell(58983, false);     //Big Blizzard Bear
		player->learnSpell(71342, false);     //Big Love Rocket
		player->learnSpell(22719, false);     //Black Battlestrider
		player->learnSpell(127286, false);     //Black Dragon Turtle
		player->learnSpell(59650, false);     //Black Drake
		player->learnSpell(35022, false);     //Black Hawkstrider
		player->learnSpell(16055, false);     //Black Nightsaber
		player->learnSpell(138642, false);     //Black Primal Raptor
		player->learnSpell(59976, false);     //Black Proto-Drake
		player->learnSpell(25863, false);     //Black Qiraji Battle Tank
		player->learnSpell(26655, false);     //Black Qiraji Battle Tank
		player->learnSpell(26656, false);     //Black Qiraji Battle Tank
		player->learnSpell(17461, false);     //Black Ram
		player->learnSpell(130138, false);     //Black Riding Goat
		player->learnSpell(127209, false);     //Black Riding Yak
		player->learnSpell(64977, false);     //Black Skeletal Horse
		player->learnSpell(470, false);     //Black Stallion
		player->learnSpell(60118, false);     //Black War Bear
		player->learnSpell(60119, false);     //Black War Bear
		player->learnSpell(48027, false);     //Black War Elekk
		player->learnSpell(22718, false);     //Black War Kodo
		player->learnSpell(59785, false);     //Black War Mammoth
		player->learnSpell(59788, false);     //Black War Mammoth
		player->learnSpell(22720, false);     //Black War Ram
		player->learnSpell(22721, false);     //Black War Raptor
		player->learnSpell(22717, false);     //Black War Steed
		player->learnSpell(22723, false);     //Black War Tiger
		player->learnSpell(22724, false);     //Black War Wolf
		player->learnSpell(64658, false);     //Black Wolf
		player->learnSpell(171627, false);     //Blacksteel Battleboar
		player->learnSpell(107842, false);     //Blazing Drake
		player->learnSpell(74856, false);     //Blazing Hippogryph
		player->learnSpell(127220, false);     //Blonde Riding Yak
		player->learnSpell(72808, false);     //Bloodbathed Frostbrood Vanquisher
		player->learnSpell(171620, false);     //Bloodhoof Bull
		player->learnSpell(127287, false);     //Blue Dragon Turtle
		player->learnSpell(61996, false);     //Blue Dragonhawk
		player->learnSpell(59568, false);     //Blue Drake
		player->learnSpell(35020, false);     //Blue Hawkstrider
		player->learnSpell(10969, false);     //Blue Mechanostrider
		player->learnSpell(59996, false);     //Blue Proto-Drake
		player->learnSpell(25953, false);     //Blue Qiraji Battle Tank
		player->learnSpell(39803, false);     //Blue Riding Nether Ray
		player->learnSpell(129934, false);     //Blue Shado-Pan Riding Tiger
		player->learnSpell(17463, false);     //Blue Skeletal Horse
		player->learnSpell(64656, false);     //Blue Skeletal Warhorse
		player->learnSpell(32244, false);     //Blue Wind Rider
		player->learnSpell(138640, false);     //Bone-White Primal Raptor
		player->learnSpell(142641, false);     //Brawler's Burly Mushan Beast
		player->learnSpell(171832, false);     //Breezestrider Stallion
		player->learnSpell(50869, false);     //Brewfest Kodo
		player->learnSpell(43899, false);     //Brewfest Ram
		player->learnSpell(190690, false);     //Bristling Hellboar
		player->learnSpell(59569, false);     //Bronze Drake
		player->learnSpell(127288, false);     //Brown Dragon Turtle
		player->learnSpell(34406, false);     //Brown Elekk
		player->learnSpell(458, false);     //Brown Horse
		player->learnSpell(18990, false);     //Brown Kodo
		player->learnSpell(6899, false);     //Brown Ram
		player->learnSpell(88748, false);     //Brown Riding Camel
		player->learnSpell(130086, false);     //Brown Riding Goat
		player->learnSpell(127213, false);     //Brown Riding Yak
		player->learnSpell(17464, false);     //Brown Skeletal Horse
		player->learnSpell(6654, false);     //Brown Wolf
		player->learnSpell(58615, false);     //Brutal Nether Drake
		player->learnSpell(124550, false);     //Cataclysmic Gladiator's Twilight Drake
		player->learnSpell(75614, false);     //Celestial Steed
		player->learnSpell(43927, false);     //Cenarion War Hippogryph
		player->learnSpell(171848, false);     //Challenger's War Yeti
		player->learnSpell(171846, false);     //Champion's Treadblade
		player->learnSpell(6648, false);     //Chestnut Mare
		player->learnSpell(171847, false);     //Cindermane Charger
		player->learnSpell(139448, false);     //Clutch of Ji-Kun
		player->learnSpell(189364, false);     //Coalfist Gronnling
		player->learnSpell(41515, false);     //Cobalt Netherwing Drake
		player->learnSpell(138423, false);     //Cobalt Primordial Direhorn
		player->learnSpell(39315, false);     //Cobalt Riding Talbuk
		player->learnSpell(34896, false);     //Cobalt War Talbuk
		player->learnSpell(170347, false);     //Core Hound
		player->learnSpell(183117, false);     //Corrupted Dreadwing
		player->learnSpell(97560, false);     //Corrupted Fire Hawk
		player->learnSpell(102514, false);     //Corrupted Hippogryph
		player->learnSpell(169952, false);     //Creeping Carpet
		player->learnSpell(127156, false);     //Crimson Cloud Serpent
		player->learnSpell(73313, false);     //Crimson Deathcharger
		player->learnSpell(129552, false);     //Crimson Pandaren Phoenix
		player->learnSpell(140250, false);     //Crimson Primal Direhorn
		player->learnSpell(123160, false);     //Crimson Riding Crane
		player->learnSpell(127271, false);     //Crimson Water Strider
		player->learnSpell(68188, false);     //Crusader's Black Warhorse
		player->learnSpell(68187, false);     //Crusader's White Warhorse
		player->learnSpell(88990, false);     //Dark Phoenix
		player->learnSpell(39316, false);     //Dark Riding Talbuk
		player->learnSpell(34790, false);     //Dark War Talbuk
		player->learnSpell(103081, false);     //Darkmoon Dancing Bear
		player->learnSpell(63635, false);     //Darkspear Raptor
		player->learnSpell(63637, false);     //Darnassian Nightsaber
		player->learnSpell(64927, false);     //Deadly Gladiator's Frost Wyrm
		player->learnSpell(190977, false);     //Deathtusk Felboar
		player->learnSpell(193007, false);     //Demonsaber
		player->learnSpell(126507, false);     //Depleted-Kyparium Rocket
		player->learnSpell(6653, false);     //Dire Wolf
		player->learnSpell(171634, false);     //Domesticated Razorback
		player->learnSpell(88335, false);     //Drake of the East Wind
		player->learnSpell(88742, false);     //Drake of the North Wind
		player->learnSpell(88744, false);     //Drake of the South Wind
		player->learnSpell(88741, false);     //Drake of the West Wind
		player->learnSpell(155741, false);     //Dread Raven
		player->learnSpell(171844, false);     //Dustmane Direwolf
		player->learnSpell(171625, false);     //Dusty Rockhide
		player->learnSpell(32239, false);     //Ebon Gryphon
		player->learnSpell(194464, false);     //Eclipse Dragonhawk
		player->learnSpell(175700, false);     //Emerald Drake
		player->learnSpell(149801, false);     //Emerald Hippogryph
		player->learnSpell(132118, false);     //Emerald Pandaren Phoenix
		player->learnSpell(8395, false);     //Emerald Raptor
		player->learnSpell(142878, false);     //Enchanted Fey Dragon
		player->learnSpell(63639, false);     //Exodar Elekk
		player->learnSpell(110039, false);     //Experiment 12-B
		player->learnSpell(113120, false);     //Feldrake
		player->learnSpell(97501, false);     //Felfire Hawk
		player->learnSpell(200175, false);     //Felsaber
		player->learnSpell(182912, false);     //Felsteel Annihilator
		player->learnSpell(36702, false);     //Fiery Warhorse
		player->learnSpell(101542, false);     //Flametalon of Alysrazor
		player->learnSpell(97359, false);     //Flameward Hippogryph
		player->learnSpell(61451, false);     //Flying Carpet
		player->learnSpell(44153, false);     //Flying Machine
		player->learnSpell(63643, false);     //Forsaken Warhorse
		player->learnSpell(84751, false);     //Fossilized Raptor
		player->learnSpell(17460, false);     //Frost Ram
		player->learnSpell(171632, false);     //Frostplains Battleboar
		player->learnSpell(23509, false);     //Frostwolf Howler
		player->learnSpell(75596, false);     //Frosty Flying Carpet
		player->learnSpell(65439, false);     //Furious Gladiator's Frost Wyrm
		player->learnSpell(171851, false);     //Garn Nighthowl
		player->learnSpell(171836, false);     //Garn Steelmaw
		player->learnSpell(126508, false);     //Geosynchronous World Spinner
		player->learnSpell(136505, false);     //Ghastly Charger
		player->learnSpell(171635, false);     //Giant Coldsnout
		player->learnSpell(63638, false);     //Gnomeregan Mechanostrider
		player->learnSpell(89520, false);     //Goblin Mini Hotrod
		player->learnSpell(87090, false);     //Goblin Trike
		player->learnSpell(87091, false);     //Goblin Turbo-Trike
		player->learnSpell(123993, false);     //Golden Cloud Serpent
		player->learnSpell(32235, false);     //Golden Gryphon
		player->learnSpell(90621, false);     //Golden King
		player->learnSpell(140249, false);     //Golden Primal Direhorn
		player->learnSpell(127176, false);     //Golden Riding Crane
		player->learnSpell(127278, false);     //Golden Water Strider
		player->learnSpell(171436, false);     //Gorestrider Gronnling
		player->learnSpell(135416, false);     //Grand Armored Gryphon
		player->learnSpell(135418, false);     //Grand Armored Wyvern
		player->learnSpell(61465, false);     //Grand Black War Mammoth
		player->learnSpell(61467, false);     //Grand Black War Mammoth
		player->learnSpell(122708, false);     //Grand Expedition Yak
		player->learnSpell(136163, false);     //Grand Gryphon
		player->learnSpell(61470, false);     //Grand Ice Mammoth
		player->learnSpell(61469, false);     //Grand Ice Mammoth
		player->learnSpell(136164, false);     //Grand Wyvern
		player->learnSpell(35710, false);     //Gray Elekk
		player->learnSpell(18989, false);     //Gray Kodo
		player->learnSpell(6777, false);     //Gray Ram
		player->learnSpell(127295, false);     //Great Black Dragon Turtle
		player->learnSpell(127302, false);     //Great Blue Dragon Turtle
		player->learnSpell(35713, false);     //Great Blue Elekk
		player->learnSpell(49379, false);     //Great Brewfest Kodo
		player->learnSpell(127308, false);     //Great Brown Dragon Turtle
		player->learnSpell(23249, false);     //Great Brown Kodo
		player->learnSpell(65641, false);     //Great Golden Kodo
		player->learnSpell(23248, false);     //Great Gray Kodo
		player->learnSpell(127293, false);     //Great Green Dragon Turtle
		player->learnSpell(35712, false);     //Great Green Elekk
		player->learnSpell(171636, false);     //Great Greytusk
		player->learnSpell(127310, false);     //Great Purple Dragon Turtle
		player->learnSpell(35714, false);     //Great Purple Elekk
		player->learnSpell(120822, false);     //Great Red Dragon Turtle
		player->learnSpell(65637, false);     //Great Red Elekk
		player->learnSpell(23247, false);     //Great White Kodo
		player->learnSpell(120395, false);     //Green Dragon Turtle
		player->learnSpell(18991, false);     //Green Kodo
		player->learnSpell(17453, false);     //Green Mechanostrider
		player->learnSpell(138643, false);     //Green Primal Raptor
		player->learnSpell(61294, false);     //Green Proto-Drake
		player->learnSpell(26056, false);     //Green Qiraji Battle Tank
		player->learnSpell(39798, false);     //Green Riding Nether Ray
		player->learnSpell(129932, false);     //Green Shado-Pan Riding Tiger
		player->learnSpell(17465, false);     //Green Skeletal Warhorse
		player->learnSpell(32245, false);     //Green Wind Rider
		player->learnSpell(88750, false);     //Grey Riding Camel
		player->learnSpell(127216, false);     //Grey Riding Yak
		player->learnSpell(148619, false);     //Grievous Gladiator's Cloud Serpent
		player->learnSpell(163025, false);     //Grinning Reaver
		player->learnSpell(189999, false);     //Grove Warden
		player->learnSpell(48025, false);     //Headless Horseman's Mount
		player->learnSpell(110051, false);     //Heart of the Aspects
		player->learnSpell(142073, false);     //Hearthsteed
		player->learnSpell(127169, false);     //Heavenly Azure Cloud Serpent
		player->learnSpell(127161, false);     //Heavenly Crimson Cloud Serpent
		player->learnSpell(127164, false);     //Heavenly Golden Cloud Serpent
		player->learnSpell(127165, false);     //Heavenly Jade Cloud Serpent
		player->learnSpell(127158, false);     //Heavenly Onyx Cloud Serpent
		player->learnSpell(59799, false);     //Ice Mammoth
		player->learnSpell(59797, false);     //Ice Mammoth
		player->learnSpell(72807, false);     //Icebound Frostbrood Vanquisher
		player->learnSpell(17459, false);     //Icy Blue Mechanostrider Mod A
		player->learnSpell(189998, false);     //Illidari Felstalker
		player->learnSpell(124659, false);     //Imperial Quilen
		player->learnSpell(186305, false);     //Infernal Direwolf
		player->learnSpell(201098, false);     //Infinite Timereaver
		player->learnSpell(72286, false);     //Invincible
		player->learnSpell(153489, false);     //Iron Skyreaver
		player->learnSpell(63956, false);     //Ironbound Proto-Drake
		player->learnSpell(142910, false);     //Ironbound Wraithcharger
		player->learnSpell(63636, false);     //Ironforge Ram
		player->learnSpell(171621, false);     //Ironhoof Destroyer
		player->learnSpell(171839, false);     //Ironside Warwolf
		player->learnSpell(17450, false);     //Ivory Raptor
		player->learnSpell(113199, false);     //Jade Cloud Serpent
		player->learnSpell(133023, false);     //Jade Pandaren Kite
		player->learnSpell(121837, false);     //Jade Panther
		player->learnSpell(138426, false);     //Jade Primordial Direhorn
		player->learnSpell(127274, false);     //Jade Water Strider
		player->learnSpell(120043, false);     //Jeweled Onyx Panther
		player->learnSpell(127178, false);     //Jungle Riding Crane
		player->learnSpell(93644, false);     //Kor'kron Annihilator
		player->learnSpell(148417, false);     //Kor'kron Juggernaut
		player->learnSpell(148396, false);     //Kor'kron War Wolf
		player->learnSpell(107845, false);     //Life-Binder's Handmaiden
		player->learnSpell(65917, false);     //Magic Rooster
		player->learnSpell(61309, false);     //Magnificent Flying Carpet
		player->learnSpell(139407, false);     //Malevolent Gladiator's Cloud Serpent
		player->learnSpell(55531, false);     //Mechano-Hog
		player->learnSpell(60424, false);     //Mekgineer's Chopper
		player->learnSpell(44744, false);     //Merciless Nether Drake
		player->learnSpell(63796, false);     //Mimiron's Head
		player->learnSpell(191314, false);     //Minion of Grumpus
		player->learnSpell(171825, false);     //Mosshide Riverwallow
		player->learnSpell(93623, false);     //Mottled Drake
		player->learnSpell(171622, false);     //Mottled Meadowstomper
		player->learnSpell(16084, false);     //Mottled Red Raptor
		player->learnSpell(171850, false);     //Mount Template 49
		player->learnSpell(171827, false);     //Mount Template 50
		player->learnSpell(171840, false);     //Mount Template 51
		player->learnSpell(103195, false);     //Mountain Horse
		player->learnSpell(171826, false);     //Mudback Riverbeast
		player->learnSpell(180545, false);     //Mystic Runesaber
		player->learnSpell(121820, false);     //Obsidian Nightwing
		player->learnSpell(66846, false);     //Ochre Skeletal Warhorse
		player->learnSpell(127154, false);     //Onyx Cloud Serpent
		player->learnSpell(41513, false);     //Onyx Netherwing Drake
		player->learnSpell(69395, false);     //Onyxian Drake
		player->learnSpell(127272, false);     //Orange Water Strider
		player->learnSpell(63640, false);     //Orgrimmar Wolf
		player->learnSpell(171833, false);     //Pale Thorngrazer
		player->learnSpell(16082, false);     //Palomino
		player->learnSpell(118737, false);     //Pandaren Kite
		player->learnSpell(130985, false);     //Pandaren Kite
		player->learnSpell(32345, false);     //Peep the Phoenix Mount
		player->learnSpell(88718, false);     //Phosphorescent Stone Drake
		player->learnSpell(472, false);     //Pinto
		player->learnSpell(60021, false);     //Plagued Proto-Drake
		player->learnSpell(193695, false);     //Prestigious War Steed
		player->learnSpell(204166, false);     //Prestigious War Wolf
		player->learnSpell(148620, false);     //Prideful Gladiator's Cloud Serpent
		player->learnSpell(186828, false);     //Primal Gladiator's Felblood Gronnling
		player->learnSpell(97493, false);     //Pureblood Fire Hawk
		player->learnSpell(127289, false);     //Purple Dragon Turtle
		player->learnSpell(35711, false);     //Purple Elekk
		player->learnSpell(35018, false);     //Purple Hawkstrider
		player->learnSpell(41516, false);     //Purple Netherwing Drake
		player->learnSpell(39801, false);     //Purple Riding Nether Ray
		player->learnSpell(23246, false);     //Purple Skeletal Warhorse
		player->learnSpell(66090, false);     //Quel'dorei Steed
		player->learnSpell(41252, false);     //Raven Lord
		player->learnSpell(127290, false);     //Red Dragon Turtle
		player->learnSpell(61997, false);     //Red Dragonhawk
		player->learnSpell(59570, false);     //Red Drake
		player->learnSpell(130092, false);     //Red Flying Cloud
		player->learnSpell(34795, false);     //Red Hawkstrider
		player->learnSpell(10873, false);     //Red Mechanostrider
		player->learnSpell(138641, false);     //Red Primal Raptor
		player->learnSpell(59961, false);     //Red Proto-Drake
		player->learnSpell(26054, false);     //Red Qiraji Battle Tank
		player->learnSpell(39800, false);     //Red Riding Nether Ray
		player->learnSpell(129935, false);     //Red Shado-Pan Riding Tiger
		player->learnSpell(17462, false);     //Red Skeletal Horse
		player->learnSpell(22722, false);     //Red Skeletal Warhorse
		player->learnSpell(16080, false);     //Red Wolf
		player->learnSpell(127177, false);     //Regal Riding Crane
		player->learnSpell(67336, false);     //Relentless Gladiator's Frost Wyrm
		player->learnSpell(30174, false);     //Riding Turtle
		player->learnSpell(17481, false);     //Rivendare's Deathcharger
		player->learnSpell(171628, false);     //Rocktusk Battleboar
		player->learnSpell(121838, false);     //Ruby Panther
		player->learnSpell(63963, false);     //Rusted Proto-Drake
		player->learnSpell(101821, false);     //Ruthless Gladiator's Twilight Drake
		player->learnSpell(93326, false);     //Sandstone Drake
		player->learnSpell(121836, false);     //Sapphire Panther
		player->learnSpell(171824, false);     //Sapphire Riverbeast
		player->learnSpell(97581, false);     //Savage Raptor
		player->learnSpell(64731, false);     //Sea Turtle
		player->learnSpell(171624, false);     //Shadowhide Pearltusk
		player->learnSpell(171829, false);     //Shadowmane Charger
		player->learnSpell(66087, false);     //Silver Covenant Hippogryph
		player->learnSpell(39802, false);     //Silver Riding Nether Ray
		player->learnSpell(39317, false);     //Silver Riding Talbuk
		player->learnSpell(34898, false);     //Silver War Talbuk
		player->learnSpell(63642, false);     //Silvermoon Hawkstrider
		player->learnSpell(134359, false);     //Sky Golem
		player->learnSpell(138425, false);     //Slate Primordial Direhorn
		player->learnSpell(171843, false);     //Smoky Direwolf
		player->learnSpell(32240, false);     //Snowy Gryphon
		player->learnSpell(191633, false);     //Soaring Skyterror
		player->learnSpell(171828, false);     //Solar Spirehawk
		player->learnSpell(130965, false);     //Son of Galleon
		player->learnSpell(148392, false);     //Spawn of Galakras
		player->learnSpell(136471, false);     //Spawn of Horridon
		player->learnSpell(107516, false);     //Spectral Gryphon
		player->learnSpell(92231, false);     //Spectral Steed
		player->learnSpell(42776, false);     //Spectral Tiger
		player->learnSpell(107517, false);     //Spectral Wind Rider
		player->learnSpell(92232, false);     //Spectral Wolf
		player->learnSpell(196681, false);     //Spirit of Eche'ro
		player->learnSpell(10789, false);     //Spotted Frostsaber
		player->learnSpell(147595, false);     //Stormcrow
		player->learnSpell(23510, false);     //Stormpike Battle Charger
		player->learnSpell(63232, false);     //Stormwind Steed
		player->learnSpell(66847, false);     //Striped Dawnsaber
		player->learnSpell(8394, false);     //Striped Frostsaber
		player->learnSpell(10793, false);     //Striped Nightsaber
		player->learnSpell(98718, false);     //Subdued Seahorse
		player->learnSpell(179245, false);     //Summon Chauffeur
		player->learnSpell(179244, false);     //Summon Chauffeur
		player->learnSpell(171849, false);     //Sunhide Gronnling
		player->learnSpell(66088, false);     //Sunreaver Dragonhawk
		player->learnSpell(66091, false);     //Sunreaver Hawkstrider
		player->learnSpell(121839, false);     //Sunstone Panther
		player->learnSpell(68057, false);     //Swift Alliance Steed
		player->learnSpell(32242, false);     //Swift Blue Gryphon
		player->learnSpell(23241, false);     //Swift Blue Raptor
		player->learnSpell(171830, false);     //Swift Breezestrider
		player->learnSpell(43900, false);     //Swift Brewfest Ram
		player->learnSpell(23238, false);     //Swift Brown Ram
		player->learnSpell(23229, false);     //Swift Brown Steed
		player->learnSpell(23250, false);     //Swift Brown Wolf
		player->learnSpell(65646, false);     //Swift Burgundy Wolf
		player->learnSpell(102346, false);     //Swift Forest Strider
		player->learnSpell(23221, false);     //Swift Frostsaber
		player->learnSpell(171842, false);     //Swift Frostwolf
		player->learnSpell(23239, false);     //Swift Gray Ram
		player->learnSpell(65640, false);     //Swift Gray Steed
		player->learnSpell(23252, false);     //Swift Gray Wolf
		player->learnSpell(32290, false);     //Swift Green Gryphon
		player->learnSpell(35025, false);     //Swift Green Hawkstrider
		player->learnSpell(23225, false);     //Swift Green Mechanostrider
		player->learnSpell(32295, false);     //Swift Green Wind Rider
		player->learnSpell(68056, false);     //Swift Horde Wolf
		player->learnSpell(102350, false);     //Swift Lovebird
		player->learnSpell(23219, false);     //Swift Mistsaber
		player->learnSpell(65638, false);     //Swift Moonsaber
		player->learnSpell(103196, false);     //Swift Mountain Horse
		player->learnSpell(37015, false);     //Swift Nether Drake
		player->learnSpell(23242, false);     //Swift Olive Raptor
		player->learnSpell(23243, false);     //Swift Orange Raptor
		player->learnSpell(23227, false);     //Swift Palomino
		player->learnSpell(33660, false);     //Swift Pink Hawkstrider
		player->learnSpell(32292, false);     //Swift Purple Gryphon
		player->learnSpell(35027, false);     //Swift Purple Hawkstrider
		player->learnSpell(65644, false);     //Swift Purple Raptor
		player->learnSpell(32297, false);     //Swift Purple Wind Rider
		player->learnSpell(24242, false);     //Swift Razzashi Raptor
		player->learnSpell(32289, false);     //Swift Red Gryphon
		player->learnSpell(65639, false);     //Swift Red Hawkstrider
		player->learnSpell(32246, false);     //Swift Red Wind Rider
		player->learnSpell(101573, false);     //Swift Shorestrider
		player->learnSpell(55164, false);     //Swift Spectral Gryphon
		player->learnSpell(194046, false);     //Swift Spectral Rylak
		player->learnSpell(42777, false);     //Swift Spectral Tiger
		player->learnSpell(102349, false);     //Swift Springstrider
		player->learnSpell(23338, false);     //Swift Stormsaber
		player->learnSpell(23251, false);     //Swift Timber Wolf
		player->learnSpell(65643, false);     //Swift Violet Ram
		player->learnSpell(35028, false);     //Swift Warstrider
		player->learnSpell(46628, false);     //Swift White Hawkstrider
		player->learnSpell(23223, false);     //Swift White Mechanostrider
		player->learnSpell(23240, false);     //Swift White Ram
		player->learnSpell(23228, false);     //Swift White Steed
		player->learnSpell(134573, false);     //Swift Windsteed
		player->learnSpell(23222, false);     //Swift Yellow Mechanostrider
		player->learnSpell(32296, false);     //Swift Yellow Wind Rider
		player->learnSpell(49322, false);     //Swift Zhevra
		player->learnSpell(96499, false);     //Swift Zulian Panther
		player->learnSpell(24252, false);     //Swift Zulian Tiger
		player->learnSpell(88749, false);     //Tan Riding Camel
		player->learnSpell(39318, false);     //Tan Riding Talbuk
		player->learnSpell(34899, false);     //Tan War Talbuk
		player->learnSpell(32243, false);     //Tawny Wind Rider
		player->learnSpell(18992, false);     //Teal Kodo
		player->learnSpell(63641, false);     //Thunder Bluff Kodo
		player->learnSpell(129918, false);     //Thundering August Cloud Serpent
		player->learnSpell(139442, false);     //Thundering Cobalt Cloud Serpent
		player->learnSpell(124408, false);     //Thundering Jade Cloud Serpent
		player->learnSpell(148476, false);     //Thundering Onyx Cloud Serpent
		player->learnSpell(132036, false);     //Thundering Ruby Cloud Serpent
		player->learnSpell(580, false);     //Timber Wolf
		player->learnSpell(60002, false);     //Time-Lost Proto-Drake
		player->learnSpell(171617, false);     //Trained Icehoof
		player->learnSpell(171623, false);     //Trained Meadowstomper
		player->learnSpell(171638, false);     //Trained Riverwallow
		player->learnSpell(171637, false);     //Trained Rocktusk
		player->learnSpell(171831, false);     //Trained Silverpelt
		player->learnSpell(171841, false);     //Trained Snarler
		player->learnSpell(61425, false);     //Traveler's Tundra Mammoth
		player->learnSpell(61447, false);     //Traveler's Tundra Mammoth
		player->learnSpell(171619, false);     //Tundra Icehoof
		player->learnSpell(44151, false);     //Turbo-Charged Flying Machine
		player->learnSpell(65642, false);     //Turbostrider
		player->learnSpell(10796, false);     //Turquoise Raptor
		player->learnSpell(59571, false);     //Twilight Drake
		player->learnSpell(107844, false);     //Twilight Harbinger
		player->learnSpell(107203, false);     //Tyrael's Charger
		player->learnSpell(148618, false);     //Tyrannical Gladiator's Cloud Serpent
		player->learnSpell(92155, false);     //Ultramarine Qiraji Battle Tank
		player->learnSpell(17454, false);     //Unpainted Mechanostrider
		player->learnSpell(75207, false);     //Vashj'ir Seahorse
		player->learnSpell(49193, false);     //Vengeful Nether Drake
		player->learnSpell(64659, false);     //Venomhide Ravasaur
		player->learnSpell(41517, false);     //Veridian Netherwing Drake
		player->learnSpell(101282, false);     //Vicious Gladiator's Twilight Drake
		player->learnSpell(146622, false);     //Vicious Skeletal Warhorse
		player->learnSpell(185052, false);     //Vicious War Kodo
		player->learnSpell(183889, false);     //Vicious War Mechanostrider
		player->learnSpell(171834, false);     //Vicious War Ram
		player->learnSpell(171835, false);     //Vicious War Raptor
		player->learnSpell(100332, false);     //Vicious War Steed
		player->learnSpell(100333, false);     //Vicious War Wolf
		player->learnSpell(146615, false);     //Vicious Warsaber
		player->learnSpell(41518, false);     //Violet Netherwing Drake
		player->learnSpell(132119, false);     //Violet Pandaren Phoenix
		player->learnSpell(60024, false);     //Violet Proto-Drake
		player->learnSpell(10799, false);     //Violet Raptor
		player->learnSpell(88746, false);     //Vitreous Stone Drake
		player->learnSpell(179478, false);     //Voidtalon of the Dark Star
		player->learnSpell(88331, false);     //Volcanic Stone Drake
		player->learnSpell(163024, false);     //Warforged Nightmare
		player->learnSpell(171845, false);     //Warlord's Deathwheel
		player->learnSpell(189044, false);     //Warmongering Gladiator's Felblood Gronnling
		player->learnSpell(171837, false);     //Warsong Direfang
		player->learnSpell(64657, false);     //White Kodo
		player->learnSpell(15779, false);     //White Mechanostrider Mod B
		player->learnSpell(54753, false);     //White Polar Bear
		player->learnSpell(6898, false);     //White Ram
		player->learnSpell(102488, false);     //White Riding Camel
		player->learnSpell(130137, false);     //White Riding Goat
		player->learnSpell(39319, false);     //White Riding Talbuk
		player->learnSpell(123182, false);     //White Riding Yak
		player->learnSpell(65645, false);     //White Skeletal Warhorse
		player->learnSpell(16083, false);     //White Stallion
		player->learnSpell(34897, false);     //White War Talbuk
		player->learnSpell(189043, false);     //Wild Gladiator's Felblood Gronnling
		player->learnSpell(171633, false);     //Wild Goretusk
		player->learnSpell(98727, false);     //Winged Guardian
		player->learnSpell(54729, false);     //Winged Steed of the Ebon Blade
		player->learnSpell(17229, false);     //Winterspring Frostsaber
		player->learnSpell(171616, false);     //Witherhide Cliffstomper
		player->learnSpell(59791, false);     //Wooly Mammoth
		player->learnSpell(59793, false);     //Wooly Mammoth
		player->learnSpell(74918, false);     //Wooly White Rhino
		player->learnSpell(71810, false);     //Wrathful Gladiator's Frost Wyrm
		player->learnSpell(46197, false);     //X-51 Nether-Rocket
		player->learnSpell(46199, false);     //X-51 Nether-Rocket X-TREME
		player->learnSpell(75973, false);     //X-53 Touring Rocket
		player->learnSpell(26055, false);     //Yellow Qiraji Battle Tank
		player->learnSpell(305002, false);     //Magmatic hound
		player->learnSpell(47977, false);     // Magic Broom
		player->learnSpell(305184, false);     //Affected Abyss Bird of Fire
		player->learnSpell(305185, false);     //Blue Phoenix
		player->learnSpell(305186, false);     // Lilac Phoenix
		player->learnSpell(305187, false);     // Blzaing phoenix
		player->learnSpell(305188, false);     // Provinces of Bad Claws
		player->learnSpell(305189, false);     // Xuen Child
		player->learnSpell(305190, false);     // The Lokenahak Provinces
		player->learnSpell(305191, false);     //Illidari Badger
		player->learnSpell(305192, false);     // Phantom Hippogriff
		player->learnSpell(305193, false);     // Kul-Tiras Horse
		player->learnSpell(305194, false);     // Zuldazar's Lizard
		player->learnSpell(305195, false);     // Shu-tsen Divine Guardian
		player->learnSpell(305286, false);     // Somber Dawn
		player->learnSpell(305288, false);     // Red Illidari Felbat
		player->learnSpell(232519, false);     // abyss worm
		player->learnSpell(253662, false);     // acid belcher
		player->learnSpell(253004, false);     // Amethyst Ruinstrider
		player->learnSpell(253088, false);     // Antoran Charhound
		player->learnSpell(253087, false);     // antoran gloomhound
		player->learnSpell(227956, false);     //arcadian war turtle
		player->learnSpell(230987, false);     // Arcanist's Manasaber
		player->learnSpell(254259, false);     // avenging felcrusher
		player->learnSpell(253005, false);     //beryl ruinstrider
		player->learnSpell(253660, false);     //biletooth gnasher
		player->learnSpell(254260, false);     //bleakhoof ruinstrider
		player->learnSpell(254258, false);     // blessed felcrusher
		player->learnSpell(213115, false);     //bloodfang widow
		player->learnSpell(213164, false);     // Brilliant Direbeak
		player->learnSpell(214791, false);     //brinedeep bottom-feeder
		player->learnSpell(253007, false);     //Cerulean Ruinstrider
		player->learnSpell(242881, false);     // cloudwing hippogryph
		player->learnSpell(253661, false);     //crimson slavermaw
		player->learnSpell(235764, false);     // darkspore mana ray
		player->learnSpell(228919, false);     // darkwater skate
		player->learnSpell(223018, false);     // fantom dweller
		player->learnSpell(213134, false);     // felblaze infernal
		player->learnSpell(253108, false);     //felglow mana ray
		player->learnSpell(254069, false);     // glorious felcrusher
		player->learnSpell(213339, false);     //Great Northern Elderhorn
		player->learnSpell(242874, false);     // highmountain elderhorn
		player->learnSpell(229439, false);     // huntmaster dire wolfhawk
		player->learnSpell(229438, false);     // huntmaster fierce wolfhawk
		player->learnSpell(229386, false);     // huntmaster loyal wolfhawk
		player->learnSpell(229829, false);     // huntmaster loyal wolfhawk
		player->learnSpell(305193, false);     //
		player->learnSpell(253107, false);     //lambent mana ray
		player->learnSpell(225765, false);     //leyfeather hippogrphy
		player->learnSpell(233364, false);     //leywoven flying carpet
		player->learnSpell(239013, false);     //lightforged warframe
		player->learnSpell(215159, false);     //long forgotten hippogrphy
		player->learnSpell(247402, false);     // lucid nightmare
		player->learnSpell(253058, false);     //maddened chaosrunner
		player->learnSpell(223814, false);     // mechanized lumber extractor
		player->learnSpell(229499, false);     // Midnight
		player->learnSpell(253711, false);     // pond nettle
		player->learnSpell(213158, false);     // Predatory Bloodgazer
		player->learnSpell(232405, false);     // Primal Flamesaber
		player->learnSpell(215558, false);     // Ratstallion
		player->learnSpell(243025, false);     //riddler's mind-worm
		player->learnSpell(253006, false);     // Russet Ruinstrider
		player->learnSpell(242305, false);     //Sable Ruinstrider
		player->learnSpell(253109, false);     //scintillating mana ray
		player->learnSpell(243651, false);     // shackled ur'zul
		player->learnSpell(231428, false);     //smoldering ember wyrm
		player->learnSpell(213163, false);     // snowfeather hunter
		player->learnSpell(213209, false);     // steelbound devourer
		player->learnSpell(101641, false);     //tarecgox's visage
		player->learnSpell(253008, false);     // umber ruinstrider
		player->learnSpell(242822, false);     // valarjar stormwing
		player->learnSpell(253106, false);     //vibrant mana ray
		player->learnSpell(243652, false);     // vile fiend
		player->learnSpell(253639, false);     //violet spellwing
		player->learnSpell(213165, false);     //viridian sharptalon
		player->learnSpell(242875, false);     // wild dreamrunner
		player->learnSpell(230401, false);     //ivory hawkstrider
		player->learnSpell(242882, false);     //valarjar stormwing
		player->learnSpell(229376, false); //archmage's prismatic disc
		player->learnSpell(229385, false); //ban-lu Grandmaster Companion
		player->learnSpell(229388, false); //battlelord's bloodthristy war wyrm
		player->learnSpell(229387, false); //deathlord's vilebrood vanquisher
		player->learnSpell(231442, false); //farseer's raging tempest
		player->learnSpell(69826, false); //great sunwalker kodo
		player->learnSpell(229377, false); //high priest's lightsworn seeker
		player->learnSpell(231435, false); //highlord's golden charger
		player->learnSpell(231587, false); //highlord's vengeful Charger
		player->learnSpell(231588, false); //highlord's vigilant charger
		player->learnSpell(238454, false); //netherlord's accursed wrathsteed
		player->learnSpell(238452, false); //netherlord's brimstone wrathsteed
		player->learnSpell(232412, false); //netherlord's chaotic wrathsteed
		player->learnSpell(231524, false); //shadowblades baneful omen
		player->learnSpell(231525, false); //shadowblades crimson omen
		player->learnSpell(231523, false); //shadowblades lethal omen
		player->learnSpell(231434, false); //shadowblades murderous omen
		player->learnSpell(229417, false); //slayer's felbroken shrieker
		player->learnSpell(231434, false); //shadowblades murderous omen
		player->learnSpell(69820, false); //sunwalker kodo
		player->learnSpell(34767, false); //Thalassian Charger
		player->learnSpell(34769, false); //thalassian warhorse
		player->learnSpell(231589, false); //highlord's valarous Charger
		player->learnSpell(258060, false); //Highmountain Thunderhoof
		player->learnSpell(258845, false); //nightborne manasaber
		player->learnSpell(258022, false); //lightforged felcrusher
		player->learnSpell(259202, false); //starcursed voidstrider
		player->learnSpell(142641, false); //brawler's burly
		player->learnSpell(247448, false); //darkmoon dirigible
		player->learnSpell(23161, false); //dreadsteed
		player->learnSpell(5784, false); //felsteed
		player->learnSpell(243512, false); //luminous starseeker
		player->learnSpell(229486, false); //vicious warbear
		player->learnSpell(242896, false); //vicious fox
		player->learnSpell(242897, false); //vicious fox
		player->learnSpell(230988, false); //vicious scorpion
		player->learnSpell(223354, false); //vicious trike
		player->learnSpell(230988, false); //vicious scorpion
		player->learnSpell(232523, false); //vicious turtle
		player->learnSpell(232525, false); //vicious turtle
		player->learnSpell(223363, false); //vicious warstrider
		player->learnSpell(230844, false); //brawler's burly
		player->learnSpell(73629, false); //exarch elekk
		player->learnSpell(73630, false); //exarch elekk
		player->learnSpell(305297, false); //
		player->learnSpell(223341, false); //
		player->learnSpell(229487, false); //
		player->learnSpell(223578, false); //
		player->learnSpell(229512, false); //

		ChatHandler(player->GetSession()).PSendSysMessage("ALL MOUNTS SCROLL CONSUMED");
		player->DestroyItemCount(item->GetEntry(), 1, true);

		return true;

	}
};

void AddSC_ClassRewards()
{
    new ClassRewards;
	new Heroncore_Instance();
	new Heroncore_MERC1();
	new Heroncore_MERC2();
	new Heroncore_MERC3();
	new Heroncore_MERC4();
	new OLDWAR();
	new hundredboost();
	new PandaSkip();
	new DeathSkip();
	new GobSkip();
	new FurrySkip();
	new DemonSkip();
	new Heroncore_Mounts_Scroll();
}