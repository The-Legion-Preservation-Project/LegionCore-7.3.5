/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
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

#include "ScriptLoader.h"

//bg
void AddSC_battleground_seething_shore();
void AddSC_battleground_warsong();
void AddSC_battleground_kotmogu();
void AddSC_battleground_shado_pan();

//spells
void AddSC_demonhunter_spell_scripts();
void AddSC_deathknight_spell_scripts();
void AddSC_druid_spell_scripts();
void AddSC_generic_spell_scripts();
void AddSC_hunter_spell_scripts();
void AddSC_mage_spell_scripts();
void AddSC_paladin_spell_scripts();
void AddSC_priest_spell_scripts();
void AddSC_rogue_spell_scripts();
void AddSC_shaman_spell_scripts();
void AddSC_warlock_spell_scripts();
void AddSC_warrior_spell_scripts();
void AddSC_monk_spell_scripts();
void AddSC_mastery_spell_scripts();
void AddSC_quest_spell_scripts();
void AddSC_item_spell_scripts();
void AddSC_holiday_spell_scripts();

#ifdef SCRIPTS

//garrison
void AddSC_garrison_general();
void AddSC_garrison_instance();

//world
void AddSC_areatrigger_scripts();
void AddSC_emerald_dragons();
void AddSC_generic_creature();
void AddSC_go_scripts();
void AddSC_guards();
void AddSC_item_scripts();
void AddSC_npc_professions();
void AddSC_npc_innkeeper();
void AddSC_npcs_special();
void AddSC_npc_taxi();
void AddSC_achievement_scripts();
void AddSC_challenge_scripts();
void AddSC_player_special_scripts();
void AddSC_fireworks_spectacular();
void AddSC_custom_events();
void AddSC_scene_scripts();

void AddSC_petbattle_abilities();
void AddSC_PetBattlePlayerScript();
void AddSC_npc_PetBattleTrainer();

// Pandaria
void AddSC_instance_temple_of_jade_serpent();       // Temple of Jade Serpent
void AddSC_boss_wise_mari();
void AddSC_boss_lorewalker_stonestep();
void AddSC_boss_liu_flameheat();
void AddSC_boss_sha_of_doubt();
void AddSC_instance_stormstout_brewery();           // The Stormstout Brewery
void AddSC_stormstout_brewery();
void AddSC_boss_ook_ook();
void AddSC_boss_hoptallus();
void AddSC_boss_yan_zhu();
void AddSC_instance_gate_setting_sun();             // Gate of the Setting Sun
void AddSC_gate_setting_sun();
void AddSC_boss_saboteur_kiptilak();
void AddSC_boss_striker_gadok();
void AddSC_boss_commander_rimok();
void AddSC_boss_raigonn();
void AddSC_boss_sha_of_anger();                     // Pandaria World Bosses
void AddSC_boss_galion();
void AddSC_boss_oondasta();
void AddSC_boss_nalak();
void AddSC_instance_mogu_shan_palace();             // Mogu'Shan Palace
void AddSC_mogu_shan_palace();
void AddSC_boss_trial_of_the_king();
void AddSC_boss_gekkan();
void AddSC_boss_xin_the_weaponmaster();
void AddSC_instance_shadopan_monastery();           // Shadopan Monastery
void AddSC_shadopan_monastery();
void AddSC_boss_gu_cloudstrike();
void AddSC_boss_master_snowdrift();
void AddSC_boss_sha_of_violence();
void AddSC_boss_taran_zhu();
void AddSC_instance_siege_of_the_niuzoa_temple();   // Siege of the Niuzoa Temple
void AddSC_siege_of_the_niuzoa_temple();
void AddSC_boss_jinbak();
void AddSC_boss_commander_vojak();
void AddSC_boss_general_pavalak();
void AddSC_boss_wing_leader_neronok();
void AddSC_instance_mogu_shan_vault();              // Mogu'Shan Vault
void AddSC_mogu_shan_vault();
void AddSC_boss_stone_guard();
void AddSC_boss_feng();
void AddSC_boss_garajal();
void AddSC_boss_spirit_kings();
void AddSC_boss_elegon();
void AddSC_boss_will_of_emperor();

void AddSC_instance_heart_of_fear();                // Heart of Fear
void AddSC_boss_vizier_zorlok();
void AddSC_boss_lord_tayak();
void AddSC_boss_garalon();
void AddSC_boss_lord_meljarak();
void AddSC_boss_unsok();
void AddSC_boss_empress_shekzeer();

// MoP raids - Terrace of Endless Spring
void AddSC_instance_terrace_of_endless_spring();
void AddSC_terrace_of_endless_spring();
void AddSC_boss_protectors_of_the_endless();
void AddSC_boss_tsulong();
void AddSC_boss_lei_shi();
void AddSC_boss_sha_of_fear();
void AddSC_instance_throne_of_thunder();            // Throne of Thunder
void AddSC_boss_jinrokh();
void AddSC_boss_horridon();
void AddSC_boss_council_of_elders();
void AddSC_boss_tortos();
void AddSC_boss_megaera();
void AddSC_boss_jikun();
void AddSC_boss_durumu();
void AddSC_boss_primordius();
void AddSC_boss_dark_animus();
void AddSC_boss_iron_qon();
void AddSC_boss_twin_consorts();
void AddSC_boss_lei_shen();
void AddSC_boss_ra_den();
void AddSC_instance_siege_of_orgrimmar();           // Siege of Orgrimmar 
void AddSC_siege_of_orgrimmar();
void AddSC_boss_immerseus();
void AddSC_boss_fallen_protectors();
void AddSC_boss_norushen();
void AddSC_boss_sha_of_pride();
void AddSC_boss_galakras();
void AddSC_boss_iron_juggernaut();
void AddSC_boss_korkron_dark_shaman();
void AddSC_boss_general_nazgrim();
void AddSC_boss_malkorok();
void AddSC_boss_spoils_of_pandaria();
void AddSC_boss_thok_the_bloodthirsty();
void AddSC_boss_siegecrafter_blackfuse();
void AddSC_boss_paragons_of_the_klaxxi();
void AddSC_boss_garrosh_hellscream();

void AddSC_valley_of_the_four_winds();
void AddSC_krasarang_wilds();
void AddSC_kun_lai_summit();

void AddSC_WanderingIsland();
void AddSC_timeless_isle();

// Scenarios

//< Arena of Annihilation
void AddSC_instance_arena_of_annihilation();
void AddSC_boss_scar_shell();
void AddSC_boss_jolgrum();
void AddSC_boss_little_liuyang();
void AddSC_boss_chagan_firehoof();
void AddSC_arena_of_annihilation();

//< Fall of Shan'bu
void AddSC_fall_of_shan_bu();
void AddSC_instance_fall_of_shan_bu();

//< Pursuing the Black Harvest
void AddSC_pursing_the_black_harvest();
void AddSC_instance_pursuing_the_black_harvest();

//< Thunder Forge
void AddSC_thunder_forge();
void AddSC_instance_thunder_forge();

//< Troves of the Thunder King
void AddSC_troves_of_the_thunder_king();
void AddSC_instance_troves_of_the_thunder_king();

//< A Brewing Storm
void AddSC_a_brewing_storm();
void AddSC_instance_a_brewing_storm();

//< A Little Patience
void AddSC_a_little_patience();
void AddSC_instance_a_little_patience();

//< Assault on Zan'vess
void AddSC_assault_on_zanvess();
void AddSC_instance_assault_on_zanvess();

//< Battle on the High Seas
void AddSC_battle_on_the_high_seas();
void AddSC_instance_battle_on_the_high_seas();

//< Blood in the Snow
void AddSC_blood_in_the_snow();
void AddSC_instance_blood_in_the_snow();

//< Brewmoon Festival
void AddSC_brewmoon_festival();
void AddSC_instance_brewmoon_festival();

//< Celestial Tournament
void AddSC_celestial_tournament();
void AddSC_instance_celestial_tournament();

//< Crypt of Forgotten Kings
void AddSC_crypt_of_forgotten_kings();
void AddSC_instance_crypt_of_forgotten_kings();

//< Dagger in the Dark
void AddSC_dagger_in_the_dark();
void AddSC_instance_dagger_in_the_dark();

//< Dark Heart of Pandaria
void AddSC_dark_heart_of_pandaria();
void AddSC_instance_dark_heart_of_pandaria();

//< Domination Point
void AddSC_domination_point();
void AddSC_instance_domination_point();

//< Greenstone Village
void AddSC_greenstone_village();
void AddSC_instance_greenstone_village();

//< Lion's Landing
void AddSC_lions_landing();
void AddSC_instance_lions_landing();

//< The Secrets of Ragefire
void AddSC_the_secrets_of_ragefire();
void AddSC_instance_the_secrets_of_ragefire();

//< Unga Ingoo
void AddSC_unga_ingoo();
void AddSC_instance_unga_ingoo();

//< Proving Grounds
void AddSC_instance_proving_grounds();
void AddSC_proving_grounds();

//< Broken Shore
void AddSC_instance_broken_shore();
void AddSC_broken_shore();

//< Shield's Reset
void AddSC_instance_shields_rest();
void AddSC_shields_rest();

//< Vengeance Art
void AddSC_DH_vengeance_art_scenario();

// Rogue Arts
void AddSC_Dreadblades();

void AddSC_Kingslayers();
void AddSC_instance_kingslayers();

void AddSC_Devourer();
void AddSC_instance_devourer();

// Warrior scenarios
void AddSC_wars_intro_scenario();

void AddSC_Warswords();
void AddSC_instance_warswords();

void AddSC_EarthWarder();

//priest art
void AddSC_Tuure();
void AddSC_instance_tuure();

// Paladin Art
void AddSC_SilverHand();
void AddSC_instance_silver_hand();

// Shaman Art
void AddSC_Sharasdal();
void AddSC_instance_sharasdal();

// Monk Art
void AddSC_monk_intro_scenario();

void AddSC_Sheylun();

// Arts common
void AddSC_instance_class_art_scenarios();

void AddSC_AssaultBrokenShore();
void AddSC_instance_AssaultBrokenShore();

void AddSC_InvasionAzsuna();
void AddSC_instance_InvasionAzsuna();

void AddSC_InvasionHM();
void AddSC_instance_invasion_HM();

void AddSC_InvasionValshara();
void AddSC_instance_invasion_valshara();

void AddSC_InvasionStormheim();
void AddSC_instance_invasion_stormheim();

void AddSC_instance_walling_caverns_pet();
void AddSC_instance_dead_mines_per();

void AddSC_army_training();
void AddSC_instance_army_training();

void AddSC_instance_temple_of_the_jade_serpent();

// player
void AddSC_chat_log();
#endif

void AddScripts()
{
    AddBattlePayScripts();
    AddSpellScripts();
    AddCommandsScripts();
    AddBattlegroundScripts();
#ifdef SCRIPTS
    AddWorldScripts();
    AddKalimdorScripts();
    AddOutlandScripts();
    AddNorthrendScripts();
    AddLegionScripts();
    AddDraenorScripts();
    AddEasternKingdomsScripts();
    AddMaelstromScripts();
    AddPandarieScripts();
    AddOutdoorPvPScripts();
    AddBrawlersGuildScripts();
    AddScenarioScripts();
    AddCustomScripts();
#endif
}

void AddSpellScripts()
{
    AddSC_demonhunter_spell_scripts();
    AddSC_deathknight_spell_scripts();
    AddSC_druid_spell_scripts();
    AddSC_generic_spell_scripts();
    AddSC_hunter_spell_scripts();
    AddSC_mage_spell_scripts();
    AddSC_paladin_spell_scripts();
    AddSC_priest_spell_scripts();
    AddSC_rogue_spell_scripts();
    AddSC_shaman_spell_scripts();
    AddSC_warlock_spell_scripts();
    AddSC_warrior_spell_scripts();
    AddSC_monk_spell_scripts();
    AddSC_mastery_spell_scripts();
    AddSC_quest_spell_scripts();
    AddSC_item_spell_scripts();
    AddSC_holiday_spell_scripts();
}

void AddWorldScripts()
{
#ifdef SCRIPTS
    AddSC_garrison_general();
    AddSC_garrison_instance();

    AddSC_areatrigger_scripts();
    AddSC_emerald_dragons();
    AddSC_generic_creature();
    AddSC_go_scripts();
    AddSC_guards();
    AddSC_item_scripts();
    AddSC_npc_professions();
    AddSC_npc_innkeeper();
    AddSC_npcs_special();
    AddSC_npc_taxi();
    AddSC_achievement_scripts();
    AddSC_challenge_scripts();
    AddSC_player_special_scripts();
    AddSC_chat_log();
    AddSC_fireworks_spectacular();
    AddSC_custom_events();
    AddSC_scene_scripts();
    AddSC_petbattle_abilities();
    AddSC_PetBattlePlayerScript();
    AddSC_npc_PetBattleTrainer();
#endif
}

void AddPandarieScripts()
{
#ifdef SCRIPTS
    AddSC_instance_temple_of_jade_serpent();        // Temple of Jade Serpent
    AddSC_boss_wise_mari();
    AddSC_boss_lorewalker_stonestep();
    AddSC_boss_liu_flameheat();
    AddSC_boss_sha_of_doubt();
    AddSC_instance_stormstout_brewery();            // The Stormstout Brewery
    AddSC_stormstout_brewery();
    AddSC_boss_ook_ook();
    AddSC_boss_hoptallus();
    AddSC_boss_yan_zhu();
    AddSC_instance_mogu_shan_palace();              // The Mogu'Shan Palace
    AddSC_mogu_shan_palace();
    AddSC_boss_trial_of_the_king();
    AddSC_boss_gekkan();
    AddSC_boss_xin_the_weaponmaster();
    AddSC_instance_gate_setting_sun();              // Gate of the Setting Sun
    AddSC_gate_setting_sun();
    AddSC_boss_saboteur_kiptilak();
    AddSC_boss_striker_gadok();
    AddSC_boss_commander_rimok();
    AddSC_boss_raigonn();
    AddSC_boss_sha_of_anger();                      // Pandaria World Bosses
    AddSC_boss_galion();
    AddSC_boss_oondasta();
    AddSC_boss_nalak();
    AddSC_instance_shadopan_monastery();            // Shadopan Monastery
    AddSC_shadopan_monastery();
    AddSC_boss_gu_cloudstrike();
    AddSC_boss_master_snowdrift();
    AddSC_boss_sha_of_violence();
    AddSC_boss_taran_zhu();
    AddSC_instance_siege_of_the_niuzoa_temple();    // Siege of the Niuzoa Temple
    AddSC_siege_of_the_niuzoa_temple();
    AddSC_boss_jinbak();
    AddSC_boss_commander_vojak();
    AddSC_boss_general_pavalak();
    AddSC_boss_wing_leader_neronok();
    AddSC_instance_mogu_shan_vault();               // Mogu'Shan Vault
    AddSC_mogu_shan_vault();
    AddSC_boss_stone_guard();
    AddSC_boss_feng();
    AddSC_boss_garajal();
    AddSC_boss_spirit_kings();
    AddSC_boss_elegon();
    AddSC_boss_will_of_emperor();

    AddSC_instance_heart_of_fear();                //Heart of Fear
    AddSC_boss_vizier_zorlok();
    AddSC_boss_lord_tayak();
    AddSC_boss_garalon();
    AddSC_boss_lord_meljarak();
    AddSC_boss_unsok();
    AddSC_boss_empress_shekzeer();

    // Pandaria Raids - Terrace of Endless Spring
    AddSC_instance_terrace_of_endless_spring();
    AddSC_terrace_of_endless_spring();
    AddSC_boss_protectors_of_the_endless();
    AddSC_boss_tsulong();
    AddSC_boss_lei_shi();
    AddSC_boss_sha_of_fear();

    AddSC_instance_throne_of_thunder();            // Throne of Thunder
    AddSC_boss_jinrokh();
    AddSC_boss_horridon();
    AddSC_boss_council_of_elders();
    AddSC_boss_tortos();
    AddSC_boss_megaera();
    AddSC_boss_jikun();
    AddSC_boss_durumu();
    AddSC_boss_primordius();
    AddSC_boss_dark_animus();
    AddSC_boss_iron_qon();
    AddSC_boss_twin_consorts();
    AddSC_boss_lei_shen();
    AddSC_boss_ra_den();
    AddSC_instance_siege_of_orgrimmar();           // Siege of Orgrimmar 
    AddSC_siege_of_orgrimmar();
    AddSC_boss_immerseus();
    AddSC_boss_fallen_protectors();
    AddSC_boss_norushen();
    AddSC_boss_sha_of_pride();
    AddSC_boss_galakras();
    AddSC_boss_iron_juggernaut();
    AddSC_boss_korkron_dark_shaman();
    AddSC_boss_general_nazgrim();
    AddSC_boss_malkorok();
    AddSC_boss_spoils_of_pandaria();
    AddSC_boss_thok_the_bloodthirsty();
    AddSC_boss_siegecrafter_blackfuse();
    AddSC_boss_paragons_of_the_klaxxi();
    AddSC_boss_garrosh_hellscream();

    AddSC_valley_of_the_four_winds();
    AddSC_krasarang_wilds();
    AddSC_kun_lai_summit();

    AddSC_WanderingIsland();
    AddSC_timeless_isle();

    // Scenarios
    //< Arena of Annihilation
    AddSC_instance_arena_of_annihilation();
    AddSC_boss_scar_shell();
    AddSC_boss_jolgrum();
    AddSC_boss_little_liuyang();
    AddSC_boss_chagan_firehoof();
    AddSC_arena_of_annihilation();

    //< Fall of Shan'bu
    AddSC_fall_of_shan_bu();
    AddSC_instance_fall_of_shan_bu();

    //< Pursuing the Black Harvest
    AddSC_pursing_the_black_harvest();
    AddSC_instance_pursuing_the_black_harvest();

    //< Thunder Forge
    AddSC_thunder_forge();
    AddSC_instance_thunder_forge();

    //< Troves of the Thunder King
    AddSC_troves_of_the_thunder_king();
    AddSC_instance_troves_of_the_thunder_king();

    //< A Brewing Storm
    AddSC_a_brewing_storm();
    AddSC_instance_a_brewing_storm();

    //< A Little Patience
    AddSC_a_little_patience();
    AddSC_instance_a_little_patience();

    //< Assault on Zan'vess
    AddSC_assault_on_zanvess();
    AddSC_instance_assault_on_zanvess();

    //< Battle on the High Seas
    AddSC_battle_on_the_high_seas();
    AddSC_instance_battle_on_the_high_seas();

    //< Blood in the Snow
    AddSC_blood_in_the_snow();
    AddSC_instance_blood_in_the_snow();

    //< Brewmoon Festival
    AddSC_brewmoon_festival();
    AddSC_instance_brewmoon_festival();

    //< Celestial Tournament
    AddSC_celestial_tournament();
    AddSC_instance_celestial_tournament();

    //< Crypt of Forgotten Kings
    AddSC_crypt_of_forgotten_kings();
    AddSC_instance_crypt_of_forgotten_kings();

    //< Dagger in the Dark
    AddSC_dagger_in_the_dark();
    AddSC_instance_dagger_in_the_dark();

    //< Dark Heart of Pandaria
    AddSC_dark_heart_of_pandaria();
    AddSC_instance_dark_heart_of_pandaria();

    //< Domination Point
    AddSC_domination_point();
    AddSC_instance_domination_point();

    //< Greenstone Village
    AddSC_greenstone_village();
    AddSC_instance_greenstone_village();

    //< Lion's Landing
    AddSC_lions_landing();
    AddSC_instance_lions_landing();

    //< The Secrets of Ragefire
    AddSC_the_secrets_of_ragefire();
    AddSC_instance_the_secrets_of_ragefire();

    //< Unga Ingoo
    AddSC_unga_ingoo();
    AddSC_instance_unga_ingoo();

    //< Proving Grounds
    AddSC_instance_proving_grounds();
    AddSC_proving_grounds();
    
    //< Broken Shore
    AddSC_instance_broken_shore();
    AddSC_broken_shore();

    //< Shield's Reset
    AddSC_instance_shields_rest();
    AddSC_shields_rest();

    //< Vengeance Art
    AddSC_DH_vengeance_art_scenario();
    
    // Rogue Art
    AddSC_Dreadblades();
    
    AddSC_Kingslayers();
    AddSC_instance_kingslayers();
    
    AddSC_Devourer();
    AddSC_instance_devourer();
    
    // Warrior scenarios
    AddSC_wars_intro_scenario();
    
    AddSC_Warswords();
    AddSC_instance_warswords();
    
    AddSC_EarthWarder();
    
    // Priest Art
    AddSC_Tuure();
    AddSC_instance_tuure();
    
    // Pdladin Art
    AddSC_SilverHand();
    AddSC_instance_silver_hand();
    
    // Shaman Art
    AddSC_Sharasdal();
    AddSC_instance_sharasdal();
    
    // Monk Art
    AddSC_monk_intro_scenario();
    
    AddSC_Sheylun();

    // Art Common
    AddSC_instance_class_art_scenarios();
    
    AddSC_AssaultBrokenShore();
    AddSC_instance_AssaultBrokenShore();
    
    AddSC_InvasionAzsuna();
    AddSC_instance_InvasionAzsuna();
    
    AddSC_InvasionHM();
    AddSC_instance_invasion_HM();
    
    AddSC_InvasionValshara();
    AddSC_instance_invasion_valshara();
    
    AddSC_InvasionStormheim();
    AddSC_instance_invasion_stormheim();

    AddSC_instance_walling_caverns_pet();
    AddSC_instance_dead_mines_per();
    
    AddSC_army_training();
    AddSC_instance_army_training();
    
    AddSC_instance_temple_of_the_jade_serpent();
#endif
}

void AddBattlegroundScripts()
{
    AddSC_battleground_seething_shore();
    AddSC_battleground_warsong();
    AddSC_battleground_kotmogu();
    AddSC_battleground_shado_pan();
}

