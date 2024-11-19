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

 // This is where scripts' loading functions should be declared:
void AddSC_DungeonBalance();
void AddSC_event_april();
void AddSC_Loskutik();
void AddSC_midsummer_fire_festival();
void AddSC_multi_vendor();
void AddSC_custom_reward();
void AddSC_custum_trainer();
void AddSC_npc_profession();
void AddSC_npc_beastmaster();
void AddSC_npc_teleguy();
void AddSC_Duel();
void AddSC_custom_arena_1v1();
void AddSC_GOMove_commandscript();
void AddSC_npc_quest_giver();
void AddSC_darkmoon();
void AddSC_BossKillReward();
void AddSC_KeyStoneGenerator_npc();
void AddSC_solocraft();
void AddLfgSoloScripts();
void AddSC_ClassRewards();
void AddSC_cs_world_chat();
void AddSC_XP_Modifier();
void AddSC_WhoLoggedScripts();
void AddSC_WhoLoggedScripts();
void AddSC_CustomStartups();


// The name of this function should match:
// void Add${NameOfDirectory}Scripts()
void AddCustomScripts()
{
	AddSC_DungeonBalance();
	AddSC_event_april();
    AddSC_Loskutik();
    AddSC_midsummer_fire_festival();
    AddSC_multi_vendor();
    AddSC_custom_reward();
    AddSC_custum_trainer();
    AddSC_npc_profession();
    AddSC_npc_beastmaster();
    AddSC_npc_teleguy();
    AddSC_Duel();
    AddSC_custom_arena_1v1();
    AddSC_GOMove_commandscript();
    AddSC_npc_quest_giver();
    AddSC_darkmoon();
    AddSC_KeyStoneGenerator_npc();
	AddSC_BossKillReward();
	AddSC_solocraft();
	AddLfgSoloScripts();
	AddSC_ClassRewards();
	AddSC_cs_world_chat();
	AddSC_XP_Modifier();
    AddSC_CustomStartups();
	
}