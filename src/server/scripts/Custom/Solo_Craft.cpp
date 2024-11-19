/*
SOLOCRAFT modified by Fluxurion (2022)
This SoloCraft is rewrited with more functions and more stat increase.
(These options can be enabled/disabled in the server config.)
- Player instantly gets resurrected after killed by a creature or committed suicide in dungeons or raids.
- All stat increase handled by an aura.
- Checking for SoloCraft Aura when a player enters a dungeon or raid.
- Checking for SoloCraft Aura when a player dies.
- Checking for SoloCraft Aura when a player logs in inside a dungeon or raid.
- Available in Normal Dungeon, Heroic Dungeon, Mythic Dungeon, Mythic+ Dungeon, Raid, 25Man Raid, Heroic Raid, Mythic Raid
- Increasing the following stats:
	basic stats: agi str int stam armor
	ratings: crit hit versatility leech etc..
- After apply the player gets full hp and full mana.
- It has option to remove cooldowns and cast times while it's applied.
- For mythic and mythic+ dungeons extra buffs will be applied.
- using an aura to check buffed state, preventing 1hp bug [[ Aura: 167349 ]]
- Config option for difficulty rate setting

To worldserver.conf:
###################################################################################################
#
#   ..::SOLOCRAFT modified by Fluxurion (2022)::..
#
#	Multiply by (stat/rating multipliers * difficulty rate) / groupsize
#

SoloCraft.Enable = 1
SoloCraft.InstantRess = 1
SoloCraft.CooldownCheat = 1
SoloCraft.CastTimeCheat = 1
SoloCraft.ExtraBuffs.OnMythic = 1

SoloCraft.StatMultiplier = 2
SoloCraft.RatingMultiplier = 10

SoloCraft.Normal.Dungeon.Rate = 2
SoloCraft.Heroic.TimeWalking.Rate = 3
SoloCraft.Scenario.Rate = 3
SoloCraft.Invasion.Point.Rate = 4
SoloCraft.Normal.Raid.Rate = 5
SoloCraft.Mythic.Dungeon.Rate = 6
SoloCraft.25Man.Raid.Rate = 7
SoloCraft.Heroic.Raid.Rate = 8
SoloCraft.Mythic.Raid.Rate = 9

#
###################################################################################################

To database:
DELETE FROM `spell_script_names` WHERE `spell_id`=167349 AND `ScriptName`='spell_solocraft_aura_167349';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (167349, 'spell_solocraft_aura_167349');
*/


#include <map>
#include "Config.h"
#include "ScriptMgr.h"
#include "Unit.h"
#include "Player.h"
#include "Pet.h"
#include "Map.h"
#include "Group.h"
#include "InstanceScript.h"
#include "Log.h"
#include "Chat.h"
#include "WorldSession.h"
#include "SpellMgr.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "GroupMgr.h"


namespace SoloCraft {

	std::vector<uint32> disabledMaps = { 1191 /*Ashran*/ }; // (garrisons already disabled by map flag)

	enum Data
	{
		SoloCraftAura = 167349,

		// not sure of these values
		STAT_CAP_CRIT = 40000,
		STAT_CAP_HASTE = 37500,
		STAT_CAP_MASTERY = 26666,
		STAT_CAP_VERSATILITY_DMGHEALDONE = 47497,
		STAT_CAP_VERSATILITY_DMGTAKEN = 970,
		STAT_CAP_LEECH = 22727,
		STAT_CAP_AVOIDANCE = 11111,
		STAT_CAP_SPEED = 8196,
		STAT_CAP_DODGE = 50000,
		STAT_CAP_PARRY = 50000,
		STAT_CAP_BLOCK = 208, // is it ?!
	};

	int CalculateDifficulty(Map* map, Player* player)
	{
		int difficulty = 1;
		if (map)
		{
			if (map->IsDungeon())
				difficulty = sConfigMgr->GetIntDefault("SoloCraft.Normal.Dungeon.Rate", 2);

			if (map->IsHeroic())
				difficulty = sConfigMgr->GetIntDefault("SoloCraft.Heroic.TimeWalking.Rate", 3); // timewalking dungeon, 

			if (map->IsScenario())
				difficulty = sConfigMgr->GetIntDefault("SoloCraft.Scenario.Rate", 3);

			if (map->GetId() == 1779)
				difficulty = sConfigMgr->GetIntDefault("SoloCraft.Invasion.Point.Rate", 4); // invasion point, 

			if (map->IsRaid())
				difficulty = sConfigMgr->GetIntDefault("SoloCraft.Normal.Raid.Rate", 5);

			if (map->isChallenge())
				difficulty = sConfigMgr->GetIntDefault("SoloCraft.Mythic.Dungeon.Rate", 6);

			if (map->Is25ManRaid())
				difficulty = sConfigMgr->GetIntDefault("SoloCraft.25Man.Raid.Rate", 7);

			if (map->IsHeroicPlusRaid())
				difficulty = sConfigMgr->GetIntDefault("SoloCraft.Heroic.Raid.Rate", 8);

			if (map->IsMythicRaid())
				difficulty = sConfigMgr->GetIntDefault("SoloCraft.Mythic.Raid.Rate", 9);

			// Disable in specific maps
			for (uint32 mapID : disabledMaps)
				if (map->GetId() == mapID)
					difficulty = 1;

			// Disable in garrisons
			if (map->IsGarrison())
				difficulty = 1;
		}
		return difficulty;
	};
};

class solocraft_player_instance_handler : public PlayerScript {
public:
	solocraft_player_instance_handler() : PlayerScript("solocraft_player_instance_handler")
	{
		TC_LOG_INFO(LOG_FILTER_CHARACTER, "[SoloCraft] solocraft_player_instance_handler Loaded");
	}

	void OnLogin(Player* player, bool /*firstLogin*/) override
	{
		if (sConfigMgr->GetBoolDefault("SoloCraft.Enable", true))
			ChatHandler(player->GetSession()).SendSysMessage("Power Increase System is enabled in Dungeons & Raids!");

		CheckForSolocraft(player);
	}

	void OnLogout(Player* player) override
	{
		if (player->HasAura(SoloCraft::SoloCraftAura))
			player->RemoveAurasDueToSpell(SoloCraft::SoloCraftAura);
	}

	// When a player changes map (e.g.: entering a dungeon) we check if the player is eligible for SoloCraft
	void OnMapChanged(Player* player) override
	{
		CheckForSolocraft(player);
	}

	// If the player dies by a creature in an instance we check if the player is eligible for InstantRess and SoloCraft
	void OnPlayerKilledByCreature(Creature* /*killer*/, Player* player) override
	{
		Map* map = player->GetMap();
		int difficulty = SoloCraft::CalculateDifficulty(map, player);

		// if InstantRess is enabled in config and the player is in instance
		if (sConfigMgr->GetBoolDefault("SoloCraft.InstantRess", false) && difficulty > 1)
			player->ResurrectPlayer(100);

		// + check for SoloCraft
		CheckForSolocraft(player);
	}

	// When a player committed suicide (xD yeah it happens)
	void OnPVPKill(Player* killer, Player* killed) override
	{
		Map* map = killed->GetMap();
		int difficulty = SoloCraft::CalculateDifficulty(map, killed);

		if (killer == killed)
		{
			// if InstantRess is enabled in config and the player is in instance
			if (sConfigMgr->GetBoolDefault("SoloCraft.InstantRess", false) && difficulty > 1)
				killed->ResurrectPlayer(100);

			// + check for SoloCraft
			CheckForSolocraft(killed);
		}
	}

	// Check if the player is eligible for SoloCraft
	void CheckForSolocraft(Player* player)
	{
		Map* map = player->GetMap();
		int difficulty = SoloCraft::CalculateDifficulty(map, player);

		if (sConfigMgr->GetBoolDefault("SoloCraft.Enable", true))
		{
			// If the map is NOT instance and the player HAS the SoloCraftAura we remove
			if (difficulty > 1)
				player->CastSpell(player, SoloCraft::SoloCraftAura, true);
			else
				player->RemoveAurasDueToSpell(SoloCraft::SoloCraftAura);
		}
	}

};

// SoloCraftAura AuraScript - to handle solocraft switching with aura
class spell_solocraft_aura_167349 : public AuraScript
{
	PrepareAuraScript(spell_solocraft_aura_167349);

	void OnApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
	{
		if (Unit* caster = GetCaster())
			if (caster->IsPlayer())
			{
				Player* player = caster->ToPlayer();
				Map* map = player->GetMap();
				int difficulty = SoloCraft::CalculateDifficulty(map, player);
				int numInGroup = GetNumInGroup(player);

				ApplySoloCraft(player, map, difficulty, numInGroup);
			}
	}

	void OnRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
	{
		if (Unit* caster = GetCaster())
			if (caster->IsPlayer())
			{
				Player* player = caster->ToPlayer();
				Map* map = player->GetMap();
				int difficulty = SoloCraft::CalculateDifficulty(map, player);
				int numInGroup = GetNumInGroup(player);

				ClearSoloCraft(player, map, difficulty, numInGroup);
			}
	}

	void Register() override
	{
		OnEffectApply += AuraEffectApplyFn(spell_solocraft_aura_167349::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
		AfterEffectRemove += AuraEffectRemoveFn(spell_solocraft_aura_167349::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
	}

	int GetNumInGroup(Player* player)
	{
		int numInGroup = 1;
		Group* group = player->GetGroup();
		if (group) {
			Group::MemberSlotList const& groupMembers = group->GetMemberSlots();
			numInGroup = groupMembers.size();
		}
		return numInGroup;
	}

	void ApplySoloCraft(Player* player, Map* map, int difficulty, int numInGroup)
	{
		base_agi = player->GetStat(Stats::STAT_AGILITY);
		base_int = player->GetStat(Stats::STAT_INTELLECT);
		base_stam = player->GetStat(Stats::STAT_STAMINA);
		base_str = player->GetStat(Stats::STAT_STRENGTH);
		base_armor = player->GetArmor();
		base_spellpower = (player->GetSpellPowerDamage() + player->GetSpellPowerHealing()) / 2;
		base_mastery = player->GetRatingMultiplier(CR_MASTERY);
		base_multistrike = player->GetRatingMultiplier(CR_MULTISTRIKE);
		base_lifesteal = player->GetRatingMultiplier(CR_LIFESTEAL);
		base_avoidance = player->GetRatingMultiplier(CR_AVOIDANCE);
		base_dodge = player->GetRatingMultiplier(CR_DODGE);
		base_parry = player->GetRatingMultiplier(CR_PARRY);
		base_block = player->GetRatingMultiplier(CR_BLOCK);
		base_defense = player->GetRatingMultiplier(CR_DEFENSE_SKILL);
		base_hitm = player->GetRatingMultiplier(CR_HIT_MELEE);
		base_hitr = player->GetRatingMultiplier(CR_HIT_RANGED);
		base_hits = player->GetRatingMultiplier(CR_HIT_SPELL);
		base_critm = player->GetRatingMultiplier(CR_CRIT_MELEE);
		base_critr = player->GetRatingMultiplier(CR_CRIT_RANGED);
		base_crits = player->GetRatingMultiplier(CR_CRIT_SPELL);
		base_hastem = player->GetRatingMultiplier(CR_HASTE_MELEE);
		base_haster = player->GetRatingMultiplier(CR_HASTE_RANGED);
		base_hastes = player->GetRatingMultiplier(CR_HASTE_SPELL);
		base_speed = player->GetRatingMultiplier(CR_SPEED);
		base_versa_dmgdone = player->GetRatingMultiplier(CR_VERSATILITY_DAMAGE_DONE);
		base_versa_healdone = player->GetRatingMultiplier(CR_VERSATILITY_HEALING_DONE);
		base_versa_dmgtaken = player->GetRatingMultiplier(CR_VERSATILITY_DAMAGE_TAKEN);

		base_agi = (base_agi < 100) ? base_agi = 100 : base_agi;
		base_int = (base_int < 100) ? base_int = 100 : base_int;
		base_stam = (base_stam < 100) ? base_stam = 100 : base_stam;
		base_str = (base_str < 100) ? base_str = 100 : base_str;
		base_spellpower = (base_spellpower < 100) ? base_spellpower = 100 : base_spellpower;
		base_lifesteal = (base_lifesteal < 1000) ? base_lifesteal = 1000 : base_lifesteal;

		final_multiplier_stat = (config_statmultiplier * difficulty) / numInGroup;
		final_multiplier_rat = (config_ratingmultiplier * difficulty) / numInGroup;

		SwitchIncreasedStats(player, true, difficulty, numInGroup);

		// Healing up
		player->SetFullHealth();

		// Filling up the mana if the player has
		if (player->getPowerType() == POWER_MANA)
			player->SetPower(POWER_MANA, player->GetMaxPower(POWER_MANA));

		// Cooldown Cheat (be careful)
		if (sConfigMgr->GetBoolDefault("SoloCraft.CooldownCheat", false))
		{
			player->SetCommandStatusOn(CHEAT_COOLDOWN);
			ChatHandler(player->GetSession()).PSendSysMessage("Cooldown turned off.");
		}

		// Casttime Cheat (be careful)
		if (sConfigMgr->GetBoolDefault("SoloCraft.CastTimeCheat", false))
		{
			player->SetCommandStatusOn(CHEAT_CASTTIME);
			ChatHandler(player->GetSession()).PSendSysMessage("Instant cast turned on.");
		}

		// Mythic and Challenge dungeons has the option for extra buffs
		if (sConfigMgr->GetBoolDefault("SoloCraft.ExtraBuffs.OnMythic", false) && (map->isChallenge() || map->IsMythicRaid()))
		{
			player->CastSpell(player, 224001, true);
			player->CastSpell(player, 215607, true);
			player->CastSpell(player, 215598, true);
			player->CastSpell(player, 215387, true);
			player->CastSpell(player, 214336, true);
			player->CastSpell(player, 214337, true);
			player->CastSpell(player, 214338, true);
			player->AddAura(199368, player);
			player->AddAura(235727, player);
			ChatHandler(player->GetSession()).PSendSysMessage("|cffB400B4 Extra buffs for Mythic difficulty applied!");
		}

		ChatHandler(player->GetSession()).PSendSysMessage("Welcome to the %s! Power Increase System activated. Your base stat multiplier x%u, rating multiplier x%u.", map->GetMapName(), uint32(final_multiplier_stat), uint32(final_multiplier_rat));

		TC_LOG_INFO(LOG_FILTER_PLAYER, "[SoloCraft] Player %s entered map: %s (mapid: %u), stat multiplier is %u, rating multiplier is %u.", player->GetName(), map->GetMapName(), map->GetId(), uint32(final_multiplier_stat), uint32(final_multiplier_rat));
	}

	void ClearSoloCraft(Player* player, Map* map, int difficulty, int numInGroup)
	{
		ChatHandler(player->GetSession()).PSendSysMessage("Power Increase System deactivated.");

		SwitchIncreasedStats(player, false, difficulty, numInGroup);

		if (sConfigMgr->GetBoolDefault("SoloCraft.CooldownCheat", false))
		{
			player->SetCommandStatusOff(CHEAT_COOLDOWN);
			ChatHandler(player->GetSession()).PSendSysMessage("Cooldown turned back on.");
		}

		if (sConfigMgr->GetBoolDefault("SoloCraft.CastTimeCheat", false))
		{
			player->SetCommandStatusOff(CHEAT_CASTTIME);
			ChatHandler(player->GetSession()).PSendSysMessage("Instant cast turned off.");
		}

		if (sConfigMgr->GetBoolDefault("SoloCraft.ExtraBuffs.OnMythic", false))
		{
			player->RemoveAurasDueToSpell(224001);
			player->RemoveAurasDueToSpell(215607);
			player->RemoveAurasDueToSpell(215598);
			player->RemoveAurasDueToSpell(215387);
			player->RemoveAurasDueToSpell(214336);
			player->RemoveAurasDueToSpell(214337);
			player->RemoveAurasDueToSpell(214338);
			player->RemoveAurasDueToSpell(199368);
			player->RemoveAurasDueToSpell(235727);
		}

		// Refresh stats
		player->InitStatsForLevel();
		player->SetCanModifyStats(true);
		player->UpdateAllStats();
		player->UpdateItemLevels();
	}

	void SwitchIncreasedStats(Player* player, bool onoff, int difficulty, int numInGroup)
	{
		// Increasing the following stats: 
		//	  agility, strength, intellect, stamina, avoidance, dodge, parry, block, defense,
		//	  hit rating, crit rating, haste rating, multistrike, mastery, life steal, run speed, versatility

		// Increase basic stats
		player->HandleStatModifier(UnitMods::UNIT_MOD_STAT_AGILITY, UnitModifierType::TOTAL_VALUE, final_multiplier_stat * base_agi, onoff);
		player->HandleStatModifier(UnitMods::UNIT_MOD_STAT_STRENGTH, UnitModifierType::TOTAL_VALUE, final_multiplier_stat * base_str, onoff);
		player->HandleStatModifier(UnitMods::UNIT_MOD_STAT_INTELLECT, UnitModifierType::TOTAL_VALUE, final_multiplier_stat * base_int, onoff);
		player->HandleStatModifier(UnitMods::UNIT_MOD_STAT_STAMINA, UnitModifierType::TOTAL_VALUE, final_multiplier_stat * base_stam, onoff);
		player->HandleStatModifier(UnitMods::UNIT_MOD_ARMOR, UnitModifierType::TOTAL_VALUE, final_multiplier_stat * base_armor, onoff);
		// Increase spellpower + manaregen
		player->ApplySpellPowerBonus(final_multiplier_stat * base_spellpower, onoff);
		player->ApplyManaRegenBonus(final_multiplier_stat, onoff);
		// Increase defense
		player->ApplyRatingMod(CombatRating::CR_AVOIDANCE, final_multiplier_rat * base_avoidance, onoff);
		player->ApplyRatingMod(CombatRating::CR_DODGE, final_multiplier_rat * base_dodge, onoff);
		player->ApplyRatingMod(CombatRating::CR_PARRY, final_multiplier_rat * base_parry, onoff);
		player->ApplyRatingMod(CombatRating::CR_BLOCK, final_multiplier_rat * base_block, onoff);
		player->ApplyRatingMod(CombatRating::CR_DEFENSE_SKILL, final_multiplier_rat * base_defense, onoff);
		// Increase hit
		player->ApplyRatingMod(CombatRating::CR_HIT_MELEE, final_multiplier_rat * base_hitm, onoff);
		player->ApplyRatingMod(CombatRating::CR_HIT_RANGED, final_multiplier_rat * base_hitr, onoff);
		player->ApplyRatingMod(CombatRating::CR_HIT_SPELL, final_multiplier_rat * base_hits, onoff);
		// Increase crit
		player->ApplyRatingMod(CombatRating::CR_CRIT_MELEE, final_multiplier_rat * base_critm, onoff);
		player->ApplyRatingMod(CombatRating::CR_CRIT_RANGED, final_multiplier_rat * base_critr, onoff);
		player->ApplyRatingMod(CombatRating::CR_CRIT_SPELL, final_multiplier_rat * base_crits, onoff);
		// Increase haste
		player->ApplyRatingMod(CombatRating::CR_HASTE_MELEE, final_multiplier_rat * base_hastem, onoff);
		player->ApplyRatingMod(CombatRating::CR_HASTE_RANGED, final_multiplier_rat * base_haster, onoff);
		player->ApplyRatingMod(CombatRating::CR_HASTE_SPELL, final_multiplier_rat * base_hastes, onoff);
		// Increase versatility
		player->ApplyRatingMod(CombatRating::CR_VERSATILITY_DAMAGE_DONE, final_multiplier_rat * base_versa_dmgdone, onoff);
		player->ApplyRatingMod(CombatRating::CR_VERSATILITY_HEALING_DONE, final_multiplier_rat * base_versa_healdone, onoff);
		player->ApplyRatingMod(CombatRating::CR_VERSATILITY_DAMAGE_TAKEN, final_multiplier_rat * base_versa_dmgtaken, onoff);
		// Increase multistrike
		player->ApplyRatingMod(CombatRating::CR_MULTISTRIKE, base_multistrike * final_multiplier_rat, onoff);
		// Increase mastery
		player->ApplyRatingMod(CombatRating::CR_MASTERY, base_mastery * final_multiplier_rat, onoff);
		// Increase lifesteal
		player->ApplyRatingMod(CombatRating::CR_LIFESTEAL, base_lifesteal * final_multiplier_rat, onoff);
		// Increase speed
		player->ApplyRatingMod(CombatRating::CR_SPEED, base_speed * final_multiplier_rat, onoff);
	}

private:
	int config_statmultiplier = sConfigMgr->GetIntDefault("SoloCraft.StatMultiplier", 8);
	int config_ratingmultiplier = sConfigMgr->GetIntDefault("SoloCraft.RatingMultiplier", 15);
	float final_multiplier_stat, final_multiplier_rat, base_agi, base_int, base_stam, base_str,
		base_spellpower, base_manaregen, base_mastery, base_multistrike, base_lifesteal,
		base_avoidance, base_dodge, base_parry, base_block, base_defense, base_armor,
		base_hitm, base_hitr, base_hits, base_critm, base_critr, base_crits, base_speed,
		base_hastem, base_haster, base_hastes, base_versa_dmgdone, base_versa_healdone, base_versa_dmgtaken;
};

class create_group_on_dungeon_enter : public PlayerScript {
public:
	create_group_on_dungeon_enter() : PlayerScript("create_group_on_dungeon_enter") {}

	void OnMapChanged(Player* player) override
	{
		if (player->GetMap()->isChallenge())
		{
			auto group = player->GetGroup();
			if (group && group->isBGGroup())
				group = player->GetOriginalGroup();

			if (!group)
			{
				group = new Group;
				if (!group->AddLeaderInvite(player))
				{
					delete group;
					return;
				}

				group->Create(player, 4);
				sGroupMgr->AddGroup(group);
			}
		}
	}
};


void AddSC_solocraft()
{
	new solocraft_player_instance_handler();
	new create_group_on_dungeon_enter();
	RegisterAuraScript(spell_solocraft_aura_167349);
}