/*
 * Copyright (C) 2017-2018 AshamaneProject <https://github.com/AshamaneProject>
 * Copyright (C) 2016 Firestorm Servers <https://firestorm-servers.com>
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
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "ScriptedEscortAI.h"
#include "CreatureTextMgr.h"
#include "GameObject.h"
#include "Player.h"
#include "Unit.h"
#include "Spell.h"
#include "ObjectMgr.h"
#include "jade_forest.h"
#include "GridNotifiers.h"
#include <Spell.cpp>
#include <SmartScriptMgr.h>

const Position MySerpentPath[2]
{
    { 1709.67f, -2667.93f, 165.56f, 2.96f },
    { 1647.79f, -2647.67f, 153.89f, 2.72f },
};

const Position MySerpentRidingPath[2]
{
    { 1601.57f, -2609.71f, 150.51f, 5.54f },
    { 1632.73f, -2638.27f, 149.90f, 5.54f },
};

enum Spells
{
    // Misc
    SPELL_SUMMON_INSTRUCTOR_WINDBLADE = 110976,
    SPELL_FLEET_WINDS = 110668,
    SPELL_CLOUD_RING_VISUAL = 110671,
    SPELL_CLOUD_RING_AREATRIGGER = 110669,

    SPELL_ROPE_BEAM = 104884,
    SPELL_SILKEN_ROPE = 110171,
    SPELL_KEG_OF_FELLOWSHIP = 134263,
    SPELL_FEEL_THE_FLAVOR = 134262,
    SPELL_FEEL_THE_FLAVOR_EFF = 134261,
    SPELL_BEHOLD_METAL_BREW = 134180,
    SPELL_DEFEAT_METAL_BREWMASTER = 135786,
    SPELL_SUMMON_METAL_BREWMASTER = 134254,
    SPELL_BOILING_CAULDRON = 102879,
    SPELL_BOILING_CAULDRON_VISUAL = 102878,
    SPELL_CAULDRON_ROPE = 102881,
    SPELL_STAY_AND_WHILE = 123312,
    SPELL_CHO_TEAPOT_MASTER = 123246,
    SPELL_MONOLITH_01_MASTER = 123589,
    SPELL_MONOLITH_03_MASTER = 123590,
    SPELL_MONOLITH_02_MASTER = 123544,
    SPELL_PATH_OF_BLOSSOM_EFF = 122036,
    SPELL_PROWL = 8152,
    SPELL_FEROCIOUS_CLAW = 115083,
    SPELL_POUNCE = 116273,
    SPELL_CALL_OF_THE_SPIRITSAGE = 104596,
    SPELL_LI_LIS_DAY_OFF_SUMMON_LI_LI = 106276,
};

enum Creatures
{
    NPC_GOLDEN_CLOUD_SERPENT = 58528,
    NPC_CLOUD_RING = 58423,
    NPC_WINDWARD_HATCHLING = 58248,
    NPC_MOGU_RUINS_CREDIT = 55471,
    NPC_CAULDRON_CREDIT = 55105,
    NPC_MOTHER_WU = 55092,
    NPC_GROUNDSKEEPER_WU = 54915,
    NPC_STAY_AND_WHILE_CRED = 63114,
    NPC_LOREWALKER_CHO_STONES = 63217,
    NPC_STONE_1_CREDIT = 63235,
    NPC_STONE_2_CREDIT = 63236,
    NPC_STONE_3_CREDIT = 63237,
    NPC_WAYWARD_ANCESTOR = 55290,
};

enum Quests
{
    QUEST_GROW_LIKE_WEEDS = 30143,
    QUEST_RING_ROUND_UP = 30144,
    QUEST_EMPTY_NESTS = 30136,
    QUEST_SPIRITES_PLIGHT = 29745,
    QUEST_A_STRONG_BACK = 29628,
    QUEST_STAY_AND_WHILE = 31121,
    QUEST_IF_STONES_COULD_SPEAK = 31134,
    QUEST_THE_WAYWARD_DEAD = 29752,
};

enum eTalks
{
    TALK_INTRO,
    TALK_SPECIAL_1,
    TALK_SPECIAL_2,
    TALK_SPECIAL_3,
    TALK_SPECIAL_4,
    TALK_SPECIAL_5,
};

#define REPUTATION_ORDER_OF_THE_CLOUD_SERPENT 1271
#define GOSSIP_TEXT_I 12585
#define GOSSIP_CHOICE_2 "Let's see which one of us is the better student. I challenge you to a duel !"
#define GOSSIP_CHOICE_1 "Challenge the Patriarch."

// Shadow of Doubt - 57389
class mob_shadow_of_doubt : public CreatureScript
{
    public:
        mob_shadow_of_doubt() : CreatureScript("mob_shadow_of_doubt") {}

        struct mob_shadow_of_doubtAI : public ScriptedAI
        {
            mob_shadow_of_doubtAI(Creature* creature) : ScriptedAI(creature) {}

            EventMap events;

            void Reset() override
            {
                events.Reset();

                events.ScheduleEvent(EVENT_DEAFENED, 30000);
                events.ScheduleEvent(EVENT_GROWING_DOUBT, 9000);
                events.ScheduleEvent(EVENT_SHA_BOLT, 15000);
            }

            void UpdateAI(uint32 diff) override
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_DEAFENED:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_DEAFENED, false);
                            events.ScheduleEvent(EVENT_DEAFENED, 30000);
                            break;
                        case EVENT_GROWING_DOUBT:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_GROWING_DOUBT, false);
                            events.ScheduleEvent(EVENT_GROWING_DOUBT, 9000);
                            break;
                        case EVENT_SHA_BOLT:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_SHA_BOLT, false);
                            events.ScheduleEvent(EVENT_SHA_BOLT, 15000);
                            break;

                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new mob_shadow_of_doubtAI(creature);
        }
};

// Kor'nas Nightsavage - 50338
class mob_kor_nas_nightsavage : public CreatureScript
{
    public:
        mob_kor_nas_nightsavage() : CreatureScript("mob_kor_nas_nightsavage") {}

        struct mob_kor_nas_nightsavageAI : public ScriptedAI
        {
            mob_kor_nas_nightsavageAI(Creature* creature) : ScriptedAI(creature) {}

            EventMap events;

            void Reset() override
            {
                events.Reset();

                events.ScheduleEvent(EVENT_SHADOWBOLT, 50000);
                events.ScheduleEvent(EVENT_VOIDCLOUD, 15000);
            }

            void UpdateAI(uint32 diff) override
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_SHADOWBOLT:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_SHADOWBOLT, false);
                            events.ScheduleEvent(EVENT_SHADOWBOLT, 70000);
                            break;
                        case EVENT_VOIDCLOUD:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_VOIDCLOUD, false);
                            events.ScheduleEvent(EVENT_VOIDCLOUD, 35000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new mob_kor_nas_nightsavageAI(creature);
        }
};

// Krax'ik - 50363
class mob_krax_ik : public CreatureScript
{
    public:
        mob_krax_ik() : CreatureScript("mob_krax_ik") {}

        struct mob_krax_ikAI : public ScriptedAI
        {
            mob_krax_ikAI(Creature* creature) : ScriptedAI(creature) {}

            EventMap events;

            void Reset() override
            {
                events.Reset();

                me->CastSpell(me, SPELL_STEALTH, true);

                events.ScheduleEvent(EVENT_TORNADO, 5000);
                events.ScheduleEvent(EVENT_BLADE_FURY, 25000);
                events.ScheduleEvent(EVENT_WINDSONG, 30000);
            }

            void EnterCombat(Unit* /*attacker*/) override
            {
                me->RemoveAurasDueToSpell(SPELL_STEALTH);
            }

            void JustSummoned(Creature* summon) override
            {
                if (summon->GetEntry() == 64267)
                {
                    summon->DespawnOrUnsummon(15000);
                    summon->AddAura(SPELL_TORNADO_DMG, summon);
                    summon->SetReactState(REACT_PASSIVE);
                    summon->GetMotionMaster()->MoveRandom(20.0f);
                }

            }

            void UpdateAI(uint32 diff) override
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_TORNADO:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_TORNADO, false);
                            events.ScheduleEvent(EVENT_TORNADO, 70000);
                            break;
                        case EVENT_BLADE_FURY:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_BLADE_FURY, false);
                            events.ScheduleEvent(EVENT_BLADE_FURY, 20000);
                            break;
                        case EVENT_WINDSONG:
                            me->CastSpell(me, SPELL_WINDSONG, false);
                            events.ScheduleEvent(EVENT_WINDSONG, 30000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new mob_krax_ikAI(creature);
        }
};

// Mister Ferocious - 50823
class mob_mister_ferocious : public CreatureScript
{
    public:
        mob_mister_ferocious() : CreatureScript("mob_mister_ferocious") {}

        struct mob_mister_ferociousAI : public ScriptedAI
        {
            mob_mister_ferociousAI(Creature* creature) : ScriptedAI(creature) {}

            EventMap events;

            void Reset() override
            {
                events.Reset();

                events.ScheduleEvent(EVENT_GOING_BANANAS, 12000);
                events.ScheduleEvent(EVENT_BANANARANG, 8000);
                events.ScheduleEvent(EVENT_TOSS_FILTH, 15000);
            }

            void JustSummoned(Creature* summon) override
            {
                summon->DespawnOrUnsummon(12000);
            }

            void UpdateAI(uint32 diff) override
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_GOING_BANANAS:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_GOING_BANANAS, false);
                            events.ScheduleEvent(EVENT_GOING_BANANAS, 10000);
                            break;
                        case EVENT_BANANARANG:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_BANANARANG, false);
                            events.ScheduleEvent(EVENT_BANANARANG, 20000);
                            break;
                        case EVENT_TOSS_FILTH:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_TOSS_FILTH, false);
                            events.ScheduleEvent(EVENT_TOSS_FILTH, 15000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new mob_mister_ferociousAI(creature);
        }
};

// Akkalar - 66937
class mob_akkalar : public CreatureScript
{
    public:
        mob_akkalar() : CreatureScript("mob_akkalar") {}

        struct mob_akkalarAI : public ScriptedAI
        {
            mob_akkalarAI(Creature* creature) : ScriptedAI(creature) {}

            EventMap events;

            void Reset() override
            {
                events.Reset();

                events.ScheduleEvent(EVENT_BOUCLIER_D_EAU, 10000);
                events.ScheduleEvent(EVENT_BOUCLIER_D_EAU_2, 20000);
                events.ScheduleEvent(EVENT_BOUCLIER_DE_TERRE, 30000);
                events.ScheduleEvent(EVENT_CARAPACE_BOUCLIER, 40000);
                events.ScheduleEvent(EVENT_MOT_DE_POUVOIR, 50000);
            }

            void UpdateAI(uint32 diff) override
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_BOUCLIER_D_EAU:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_BOUCLIER_D_EAU, false);
                            events.ScheduleEvent(EVENT_BOUCLIER_D_EAU, 62000);
                            break;
                        case EVENT_BOUCLIER_D_EAU_2:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_BOUCLIER_D_EAU_2, false);
                            events.ScheduleEvent(EVENT_BOUCLIER_D_EAU_2, 70000);
                            break;
                        case EVENT_BOUCLIER_DE_TERRE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_BOUCLIER_DE_TERRE, false);
                            events.ScheduleEvent(EVENT_BOUCLIER_DE_TERRE, 80000);
                            break;
                        case EVENT_CARAPACE_BOUCLIER:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_CARAPACE_BOUCLIER, false);
                            events.ScheduleEvent(EVENT_CARAPACE_BOUCLIER, 90000);
                            break;
                        case EVENT_MOT_DE_POUVOIR:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_MOT_DE_POUVOIR, false);
                            events.ScheduleEvent(EVENT_MOT_DE_POUVOIR, 100000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new mob_akkalarAI(creature);
        }
};

// Grookin Outrunner - 67149
class mob_grookin_outrunner : public CreatureScript
{
    public:
        mob_grookin_outrunner() : CreatureScript("mob_grookin_outrunner") {}

        struct mob_grookin_outrunnerAI : public ScriptedAI
        {
            mob_grookin_outrunnerAI(Creature* creature) : ScriptedAI(creature) {}

            EventMap events;

            void Reset() override
            {
                events.Reset();

                events.ScheduleEvent(EVENT_SHOOT, 7000);
                events.ScheduleEvent(EVENT_CLEAVE, 12000);
            }
            void UpdateAI(uint32 diff) override
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_SHOOT:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_SHOOT, false);
                            events.ScheduleEvent(EVENT_SHOOT, 10000);
                            break;
                        case EVENT_CLEAVE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_CLEAVE, false);
                            events.ScheduleEvent(EVENT_CLEAVE, 10000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new mob_grookin_outrunnerAI(creature);
        }
};

// Bamboo Python - 59769
class mob_bamboo_python : public CreatureScript
{
    public:
        mob_bamboo_python() : CreatureScript("mob_bamboo_python") {}

        struct mob_bamboo_pythonAI : public ScriptedAI
        {
            mob_bamboo_pythonAI(Creature* creature) : ScriptedAI(creature) {}

            EventMap events;

            void Reset() override
            {
                events.Reset();

                events.ScheduleEvent(EVENT_SWAMP_FEVER, 7000);
            }

            void EnterCombat(Unit* /*attacker*/) override
            {
                me->RemoveAurasDueToSpell(SPELL_STEALTH);
            }

            void UpdateAI(uint32 diff) override
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    if (eventId == EVENT_SWAMP_FEVER)
                    {
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_SWAMP_FEVER, false);
                            events.ScheduleEvent(EVENT_SWAMP_FEVER, 15000);
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new mob_bamboo_pythonAI(creature);
        }
};

class mob_lurking_tiger : public CreatureScript
{
public:
    mob_lurking_tiger() : CreatureScript("mob_lurking_tiger")
    {
    }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_lurking_tigerAI(creature);
    }

    struct mob_lurking_tigerAI : public ScriptedAI
    {
        mob_lurking_tigerAI(Creature* creature) : ScriptedAI(creature)
        {
        }

        EventMap events;

        void Reset()
        {
            me->CastSpell(me, SPELL_STEALTH, true);
        }

        void UpdateAI(const uint32 /*diff*/)
        {
            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            DoMeleeAttackIfReady();
        }
    };
};

// Rakira - 66622
class mob_rakira : public CreatureScript
{
    public:
        mob_rakira() : CreatureScript("mob_rakira") {}

        struct mob_rakiraAI : public ScriptedAI
        {
            mob_rakiraAI(Creature* creature) : ScriptedAI(creature) {}

            EventMap events;

            void Reset() override
            {
                events.Reset();

                events.ScheduleEvent(EVENT_BLOODY_PREY, 8000);
                events.ScheduleEvent(EVENT_CLAW_RIP, 15000);
                events.ScheduleEvent(EVENT_GROWL, 22000);
                events.ScheduleEvent(EVENT_HOWL_OF_THE_DENMOTHER, 30000);
                events.ScheduleEvent(EVENT_ROAR_OF_COURAGE, 40000);
            }

            void UpdateAI(uint32 diff) override
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_BLOODY_PREY:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_BLOODY_PREY, false);
                            events.ScheduleEvent(EVENT_BLOODY_PREY, 10000);
                            break;
                        case EVENT_CLAW_RIP:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_CLAW_RIP, false);
                            events.ScheduleEvent(EVENT_CLAW_RIP, 15000);
                            break;
                        case EVENT_GROWL:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_GROWL, false);
                            events.ScheduleEvent(EVENT_GROWL, 22000);
                            break;
                        case EVENT_HOWL_OF_THE_DENMOTHER:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_HOWL_OF_THE_DENMOTHER, false);
                            events.ScheduleEvent(EVENT_HOWL_OF_THE_DENMOTHER, 30000);
                            break;
                        case EVENT_ROAR_OF_COURAGE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_ROAR_OF_COURAGE, false);
                            events.ScheduleEvent(EVENT_ROAR_OF_COURAGE, 47000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new mob_rakiraAI(creature);
        }
};

// Ro'shen - 66617
class mob_ro_shen : public CreatureScript
{
    public:
        mob_ro_shen() : CreatureScript("mob_ro_shen") {}

        struct mob_ro_shenAI : public ScriptedAI
        {
            mob_ro_shenAI(Creature* creature) : ScriptedAI(creature) {}

            EventMap events;

            void Reset() override
            {
                events.Reset();

                events.ScheduleEvent(EVENT_BLOODY_PREY, 8000);
                events.ScheduleEvent(EVENT_CLAW_RIP, 15000);
                events.ScheduleEvent(EVENT_GROWL, 22000);
                events.ScheduleEvent(EVENT_ROAR_OF_COURAGE, 30000);
                events.ScheduleEvent(EVENT_BLOOD_CRAZED, 40000);
                events.ScheduleEvent(EVENT_BLOODTHIRST, 50000);
                events.ScheduleEvent(EVENT_CLAW_SLASH, 60000);
                events.ScheduleEvent(EVENT_FRENZY, 70000);
            }

            void UpdateAI(uint32 diff) override
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_BLOODY_PREY:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_BLOODY_PREY, false);
                            events.ScheduleEvent(EVENT_BLOODY_PREY, 10000);
                            break;
                        case EVENT_CLAW_RIP:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_CLAW_RIP, false);
                            events.ScheduleEvent(EVENT_CLAW_RIP, 15000);
                            break;
                        case EVENT_GROWL:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_GROWL, false);
                            events.ScheduleEvent(EVENT_GROWL, 28000);
                            break;
                        case EVENT_ROAR_OF_COURAGE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_ROAR_OF_COURAGE, false);
                            events.ScheduleEvent(EVENT_ROAR_OF_COURAGE, 70000);
                            break;
                        case EVENT_BLOOD_CRAZED:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_BLOOD_CRAZED, false);
                            events.ScheduleEvent(EVENT_BLOOD_CRAZED, 31000);
                            break;
                        case EVENT_BLOODTHIRST:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_BLOODTHIRST, false);
                            events.ScheduleEvent(EVENT_BLOODTHIRST, 40000);
                            break;
                        case EVENT_CLAW_SLASH:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_CLAW_SLASH, false);
                            events.ScheduleEvent(EVENT_CLAW_SLASH, 25000);
                            break;
                        case EVENT_FRENZY:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_FRENZY, false);
                            events.ScheduleEvent(EVENT_FRENZY, 50000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new mob_ro_shenAI(creature);
        }
};

// Sha Remnant - 59434
class mob_sha_reminant : public CreatureScript
{
    public:
        mob_sha_reminant() : CreatureScript("mob_sha_reminant") {}

        struct mob_sha_reminantAI : public ScriptedAI
        {
            mob_sha_reminantAI(Creature* creature) : ScriptedAI(creature) {}

            EventMap events;

            void Reset() override
            {
                events.Reset();

                events.ScheduleEvent(EVENT_SHADOW_CLAW, 10000);
                events.ScheduleEvent(EVENT_OVERWHELMING_GUILT, 15000);
            }

            void UpdateAI(uint32 diff) override
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_SHADOW_CLAW:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_SHADOW_CLAW, false);
                            events.ScheduleEvent(EVENT_SHADOW_CLAW, 10000);
                            break;
                        case EVENT_OVERWHELMING_GUILT:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_OVERWHELMING_GUILT, false);
                            events.ScheduleEvent(EVENT_OVERWHELMING_GUILT, 15000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new mob_sha_reminantAI(creature);
        }
};

#define GOSSIP_CHOICE_1 "Challenge the Patriarch."



class mob_pandriarch_windfur : public CreatureScript
{
public:
    mob_pandriarch_windfur() : CreatureScript("mob_pandriarch_windfur")
    {
    }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_CHOICE_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
		player->SEND_GOSSIP_MENU(61013, creature->GetGUID());

        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
    {
        player->PlayerTalkClass->ClearMenus();

        if (action == GOSSIP_ACTION_INFO_DEF + 1)
        {
            if (player->GetQuestStatus(29920) == QUEST_STATUS_INCOMPLETE)
            {
                creature->setFaction(14);
                creature->SetReactState(REACT_DEFENSIVE);
                creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC);
                creature->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                creature->AI()->Reset();
                creature->CombatStart(player, true);
            }
            player->CLOSE_GOSSIP_MENU();
        }

        return true;
    }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_pandriarch_windfurAI(creature);
    }

    struct mob_pandriarch_windfurAI : public ScriptedAI
    {
        mob_pandriarch_windfurAI(Creature* creature) : ScriptedAI(creature)
        {
        }

        EventMap events;
        uint64 playerGuid;

        void Reset()
        {
            events.Reset();

            events.ScheduleEvent(EVENT_THUNDERING_PALM, urand(3000, 7000));
            events.ScheduleEvent(EVENT_WIND_SWORD, urand(8000, 10000));
            //                events.ScheduleEvent(EVENT_WINDFUR_PUNCH, urand (13000, 17000));
        }

		void DamageTaken(Unit* attacker, uint32 &damage, DamageEffectType /*dmgType*/) override
        {
            if (Player* player = attacker->ToPlayer())
            {
                if (me->HealthBelowPctDamaged(10, damage))
                {
                    damage = 0;
                    me->CombatStop();
                    me->GetMotionMaster()->MovePoint(0, 1996.76001f, -2216.780029f, 247.725006f);
                    me->setFaction(35);
                    me->SetFullHealth();
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC);
                    me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                    player->KilledMonsterCredit(56206);
                }

                if (damage > me->GetHealth())
                    damage = 0;
            }
        }

        void EnterCombat(Unit* who)
        {
            if (Player* player = who->ToPlayer())
            {
                if (player->GetQuestStatus(29920) == QUEST_STATUS_INCOMPLETE)
                    return;
                else
                {
                    me->CombatStop();
                    me->setFaction(35);
                }
            }
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_THUNDERING_PALM:
                    if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                        me->CastSpell(target, SPELL_THUNDERING_PALM, false);
                    events.ScheduleEvent(EVENT_THUNDERING_PALM, 10000);
                    break;
                case EVENT_WIND_SWORD:
                    if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                        me->CastSpell(target, SPELL_WIND_SWORD, false);
                    events.ScheduleEvent(EVENT_WIND_SWORD, 15000);
                    break;
                    /*                        case EVENT_WINDFUR_PUNCH:
                    if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                    me->CastSpell(target, SPELL_WINDFUR_PUNCH, false);
                    events.ScheduleEvent(EVENT_WINDFUR_PUNCH,      15000);
                    break;*/
                default:
                    break;
                }
            }

            DoMeleeAttackIfReady();
        }
    };
};



class mob_pandriarch_bramblestaff : public CreatureScript
{
public:
    mob_pandriarch_bramblestaff() : CreatureScript("mob_pandriarch_bramblestaff")
    {
    }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_CHOICE_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
		player->SEND_GOSSIP_MENU(61013, creature->GetGUID());

        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
    {
        player->PlayerTalkClass->ClearMenus();

        if (action == GOSSIP_ACTION_INFO_DEF + 1)
        {
            if (player->GetQuestStatus(29920) == QUEST_STATUS_INCOMPLETE)
            {
                creature->setFaction(14);
                creature->SetReactState(REACT_DEFENSIVE);
                creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC);
                creature->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                creature->AI()->Reset();
                creature->CombatStart(player, true);
            }
            player->CLOSE_GOSSIP_MENU();
        }

        return true;
    }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_pandriarch_bramblestaffAI(creature);
    }

    struct mob_pandriarch_bramblestaffAI : public ScriptedAI
    {
        mob_pandriarch_bramblestaffAI(Creature* creature) : ScriptedAI(creature)
        {
        }

        EventMap events;
        uint64 playerGuid;

        void Reset()
        {
            events.Reset();

            events.ScheduleEvent(EVENT_ROLL, urand(3000, 7000));
            events.ScheduleEvent(EVENT_STAFF_STRIKE, urand(8000, 10000));
            events.ScheduleEvent(EVENT_THROW_BRAMBLESTAFF, urand(13000, 17000));
            //                events.ScheduleEvent(EVENT_WHIRLWIND, urand (20000, 23000));
            //                events.ScheduleEvent(EVENT_WHIRLWIND_2, urand (24000, 27000));
        }

		void DamageTaken(Unit* attacker, uint32 &damage, DamageEffectType /*dmgType*/) override
        {
            if (Player* player = attacker->ToPlayer())
            {
                if (me->HealthBelowPctDamaged(10, damage) || damage > me->GetHealth())
                {
                    damage = 0;
                    me->CombatStop();
                    me->GetMotionMaster()->MovePoint(0, 1862.300049f, -2325.060059f, 257.062012f);
                    me->setFaction(35);
                    me->SetFullHealth();
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC);
                    me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                    player->KilledMonsterCredit(56209);
                }
            }
        }

        void EnterCombat(Unit* who)
        {
            if (Player* player = who->ToPlayer())
            {
                if (player->GetQuestStatus(29920) == QUEST_STATUS_INCOMPLETE)
                    return;
                else
                {
                    me->CombatStop();
                    me->setFaction(35);
                }
            }
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_ROLL:
                    if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                        me->CastSpell(target, SPELL_ROLL, false);
                    events.ScheduleEvent(EVENT_ROLL, 10000);
                    break;
                case EVENT_STAFF_STRIKE:
                    if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                        me->CastSpell(target, SPELL_STAFF_STRIKE, false);
                    events.ScheduleEvent(EVENT_STAFF_STRIKE, 10000);
                    break;
                case EVENT_THROW_BRAMBLESTAFF:
                    if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                        me->CastSpell(target, SPELL_THROW_BRAMBLESTAFF, false);
                    events.ScheduleEvent(EVENT_THROW_BRAMBLESTAFF, 10000);
                    break;
                    /*                        case EVENT_WHIRLWIND:
                    if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                    me->CastSpell(target, SPELL_WHIRLWIND, false);
                    events.ScheduleEvent(EVENT_WHIRLWIND,      10000);
                    break;
                    case EVENT_WHIRLWIND_2:
                    if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                    me->CastSpell(target, SPELL_WHIRLWIND_2, false);
                    events.ScheduleEvent(EVENT_WHIRLWIND_2,      10000);
                    break;*/
                default:
                    break;
                }
            }

            DoMeleeAttackIfReady();
        }
    };
};



class mob_pandriarch_goldendraft : public CreatureScript
{
public:
    mob_pandriarch_goldendraft() : CreatureScript("mob_pandriarch_goldendraft")
    {
    }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_CHOICE_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
        player->SEND_GOSSIP_MENU(61013, creature->GetGUID());

        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
    {
        player->PlayerTalkClass->ClearMenus();

        if (action == GOSSIP_ACTION_INFO_DEF + 1)
        {
            if (player->GetQuestStatus(29920) == QUEST_STATUS_INCOMPLETE)
            {
                creature->setFaction(14);
                creature->SetReactState(REACT_DEFENSIVE);
                creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC);
                creature->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                creature->AI()->Reset();
                creature->CombatStart(player, true);
            }
            player->CLOSE_GOSSIP_MENU();
        }

        return true;
    }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_pandriarch_goldendraftAI(creature);
    }

    struct mob_pandriarch_goldendraftAI : public ScriptedAI
    {
        mob_pandriarch_goldendraftAI(Creature* creature) : ScriptedAI(creature)
        {
        }

        EventMap events;
        uint64 playerGuid;

        void Reset()
        {
            events.Reset();

            events.ScheduleEvent(EVENT_EXPLOSIVE_LAGER, urand(3000, 7000));
            events.ScheduleEvent(EVENT_FIRE_ALE, urand(8000, 10000));
        }

		void DamageTaken(Unit* attacker, uint32 &damage, DamageEffectType /*dmgType*/) override
        {
            if (Player* player = attacker->ToPlayer())
            {
                if (me->HealthBelowPctDamaged(10, damage) || damage > me->GetHealth())
                {
                    damage = 0;
                    me->CombatStop();
                    me->GetMotionMaster()->MovePoint(0, 1942.630005f, -2290.530029f, 240.429001f);
                    me->setFaction(35);
                    me->SetFullHealth();
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC);
                    me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                    player->KilledMonsterCredit(56210);
                }
            }
        }

        void EnterCombat(Unit* who)
        {
            if (Player* player = who->ToPlayer())
            {
                if (player->GetQuestStatus(29920) == QUEST_STATUS_INCOMPLETE)
                    return;
                else
                {
                    me->CombatStop();
                    me->setFaction(35);
                }
            }
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_EXPLOSIVE_LAGER:
                    if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                        me->CastSpell(target, SPELL_EXPLOSIVE_LAGER, false);
                    events.ScheduleEvent(EVENT_EXPLOSIVE_LAGER, 10000);
                    break;
                case EVENT_FIRE_ALE:
                    if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                        me->CastSpell(target, SPELL_FIRE_ALE, false);
                    events.ScheduleEvent(EVENT_FIRE_ALE, 10000);
                    break;
                default:
                    break;
                }
            }

            DoMeleeAttackIfReady();
        }
    };
};



#define GOSSIP_CHOICE_2 "Let's see which one of us is the better student. I challenge you to a duel !"

class mob_big_bao : public CreatureScript
{
public:
    mob_big_bao() : CreatureScript("mob_big_bao")
    {
    }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_CHOICE_2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
        player->SEND_GOSSIP_MENU(75012, creature->GetGUID());

        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
    {
        player->PlayerTalkClass->ClearMenus();

        if (action == GOSSIP_ACTION_INFO_DEF + 1)
        {
            if (player->GetQuestStatus(31718) == QUEST_STATUS_INCOMPLETE)
            {
                creature->setFaction(14);
                creature->SetReactState(REACT_DEFENSIVE);
                creature->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                creature->AI()->Reset();
                creature->CombatStart(player, true);
            }
            player->CLOSE_GOSSIP_MENU();
        }

        return true;
    }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_big_baoAI(creature);
    }

    struct mob_big_baoAI : public ScriptedAI
    {
        mob_big_baoAI(Creature* creature) : ScriptedAI(creature)
        {
        }

        EventMap events;
        uint64 playerGuid;

        void Reset()
        {
            events.Reset();

            events.ScheduleEvent(EVENT_FORCE_BURST, 3000);
            events.ScheduleEvent(EVENT_KNOCK_AWAY, 8000);
            events.ScheduleEvent(EVENT_PULL_CLOSE, 13000);
            events.ScheduleEvent(EVENT_WAR_STOMP, 18000);
        }

        void EnterCombat(Unit* who)
        {
            if (Player* player = who->ToPlayer())
            {
                if (player->GetQuestStatus(31718) == QUEST_STATUS_INCOMPLETE)
                    player->CastSpell(player, SPELL_RACE_AGAINST_THE_CLOCK);
                else
                {
                    me->CombatStop();
                    me->setFaction(35);
                }
            }
        }

		void DamageTaken(Unit* attacker, uint32 &damage, DamageEffectType /*dmgType*/) override
        {
            if (Player* player = attacker->ToPlayer())
            {
                if (me->HealthBelowPctDamaged(10, damage) || damage > me->GetHealth())
                {
                    damage = 0;
                    me->CombatStop();
                    me->GetMotionMaster()->MovePoint(0, 1604.75f, -2562.139893f, 153.134003f);
                    me->setFaction(35);
                    me->SetFullHealth();
                    me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                    player->KilledMonsterCredit(58508);
                }
            }
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_FORCE_BURST:
                    if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                        me->CastSpell(target, SPELL_FORCE_BURST, false);
                    events.ScheduleEvent(EVENT_FORCE_BURST, 10000);
                    break;
                case EVENT_KNOCK_AWAY:
                    if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                        me->CastSpell(target, SPELL_KNOCK_AWAY, false);
                    events.ScheduleEvent(EVENT_KNOCK_AWAY, 10000);
                    break;
                case EVENT_PULL_CLOSE:
                    if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                        me->CastSpell(target, SPELL_PULL_CLOSE, false);
                    events.ScheduleEvent(EVENT_PULL_CLOSE, 10000);
                    break;
                case EVENT_WAR_STOMP:
                    if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                        me->CastSpell(target, SPELL_FIRE_ALE, false);
                    events.ScheduleEvent(SPELL_WAR_STOMP, 10000);
                    break;
                default:
                    break;
                }
            }

            DoMeleeAttackIfReady();
        }
    };
};

// Elder Anli - 58564
class npc_elder_anli : public CreatureScript
{
public:
    npc_elder_anli() : CreatureScript("npc_elder_anli") {}

    bool OnGossipHello(Player* player, Creature* creature) override
    {
        if (creature->isQuestGiver())
            player->PrepareQuestMenu(creature->GetGUID());

        if (player->GetQuestStatus(QUEST_RIDING_THE_SKIES_FIRST) == QUEST_STATUS_INCOMPLETE || player->GetQuestStatus(QUEST_RIDING_THE_SKIES_SECOND) == QUEST_STATUS_INCOMPLETE || player->GetQuestStatus(QUEST_RIDING_THE_SKIES_THIRD) == QUEST_STATUS_INCOMPLETE)
            if (player->GetReputationRank(REPUTATION_ORDER_OF_THE_CLOUD_SERPENT) == REP_EXALTED)
                player->KilledMonsterCredit(NPC_INSTRUCTOR_WINDBLADE);

        player->PlayerTalkClass->SendGossipMenu(GOSSIP_TEXT_I, creature->GetGUID());

        return true;
    }

    bool OnGossipSelect(Player* player, Creature* /*creature*/, uint32 /*sender*/, uint32 /*action*/) override
    {
        player->PlayerTalkClass->GetQuestMenu();

        return true;
    }

    bool OnQuestAccept(Player* player, Creature* /*p_Creature*/, Quest const* quest) override
    {
        if (quest->GetQuestId() == 31717)
            player->SummonCreature(66025, 1559.40f, -2558.67f, 151.244f, 4.83016f);

        else if (quest->GetQuestId() == 31718)
            player->SummonCreature(66026, 1559.40f, -2558.67f, 151.244f, 4.83016f);

        else if (quest->GetQuestId() == 31719)
            player->SummonCreature(66027, 1559.40f, -2558.67f, 151.244f, 4.83016f);

        else if (quest->GetQuestId() == 31720)
            player->SummonCreature(66029, 1559.40f, -2558.67f, 151.244f, 4.83016f);

        else if (quest->GetQuestId() == 31721)
            player->SummonCreature(66028, 1559.40f, -2558.67f, 151.244f, 4.83016f);

        return true;
    }

    bool OnQuestReward(Player* player, Creature* creature, const Quest* quest, uint32 /*option*/) override
    {
        switch (quest->GetQuestId())
        {
        case 30188:
        case 31810:
        case 31811:
            creature->CastSpell(player, 130487, false);
            break;
        default:
            break;
        }

        return true;
    }
};



// Hutia - 69946
class mob_hutia : public CreatureScript
{
    public:
        mob_hutia() : CreatureScript("mob_hutia") {}

        struct mob_hutiatAI : public ScriptedAI
        {
            mob_hutiatAI(Creature* creature) : ScriptedAI(creature) {}

            EventMap events;

            void Reset() override
            {
                events.Reset();

                me->CastSpell(me, SPELL_SPIRIT_MEND, false);
                me->CastSpell(me, SPELL_STRONG_WILL, false);

                events.ScheduleEvent(EVENT_FRENZY_2, 7000);
                events.ScheduleEvent(EVENT_SPIRIT_BEAST_BLESSING, 12000);
                events.ScheduleEvent(EVENT_SPIRIT_HEAL, 19000);
            }

            void UpdateAI(uint32 diff) override
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_FRENZY_2:
                            me->CastSpell(me, SPELL_FRENZY_2, false);
                            events.ScheduleEvent(EVENT_FRENZY_2, 30000);
                            break;
                        case EVENT_SPIRIT_BEAST_BLESSING:
                            me->CastSpell(me, SPELL_SPIRIT_BEAST_BLESSING, false);
                            events.ScheduleEvent(EVENT_SPIRIT_BEAST_BLESSING, 40000);
                            break;
                        case EVENT_SPIRIT_HEAL:
                            me->CastSpell(me, SPELL_SPIRIT_HEAL_2, false);
                            events.ScheduleEvent(EVENT_SPIRIT_HEAL, 40000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new mob_hutiatAI(creature);
        }
};

class npc_nectarbreeze_farmer : public CreatureScript
{
public:
    npc_nectarbreeze_farmer() : CreatureScript("npc_nectarbreeze_farmer")
    {
    }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        if (player->GetQuestStatus(29579) == QUEST_STATUS_INCOMPLETE)
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Get to Hanae's house. It's safe there.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
        player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());

        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
    {
        player->PlayerTalkClass->ClearMenus();

        if (action == GOSSIP_ACTION_INFO_DEF + 1)
        {
            if (player->GetQuestStatus(29579) == QUEST_STATUS_INCOMPLETE)
            {
                player->CastSpell(player, 102469, true);
                creature->AI()->Talk(0);
                creature->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                creature->ForcedDespawn(2000);
            }
            player->CLOSE_GOSSIP_MENU();
        }

        return true;
    }
};

class npc_windward_hatchling : public CreatureScript
{
public:
    npc_windward_hatchling() : CreatureScript("npc_windward_hatchling") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_windward_hatchlingAI(creature);
    }

    struct npc_windward_hatchlingAI : public ScriptedAI
    {
        npc_windward_hatchlingAI(Creature* creature) : ScriptedAI(creature) { }

        void Reset()
        {
        }

        void SpellHit(Unit* caster, SpellInfo const* spell)
        {
            if (auto player = caster->ToPlayer())
                if (player->GetQuestStatus(30136) == QUEST_STATUS_INCOMPLETE && spell->Id == 110171)
                    if (auto summon = player->SummonCreature(58248, player->GetPositionX(), player->GetPositionY(), player->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 120000))
                    {
                        summon->SetOwnerGUID(player->GetGUID());
                        summon->SetDisplayId(me->GetDisplayId());
                        summon->GetMotionMaster()->MoveFollow(player, PET_FOLLOW_DIST, M_PI);
                        me->ForcedDespawn();
                    }
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            DoMeleeAttackIfReady();
        }
    };
};

class npc_windward_nest_trigger : public CreatureScript
{
public:
    npc_windward_nest_trigger() : CreatureScript("npc_windward_nest_trigger") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_windward_nest_triggerAI(creature);
    }

    struct npc_windward_nest_triggerAI : public ScriptedAI
    {
        npc_windward_nest_triggerAI(Creature* creature) : ScriptedAI(creature) { }

        void Reset() { }

        void MoveInLineOfSight(Unit* who) override
        {
            if (who->GetTypeId() == TYPEID_UNIT && who->GetEntry() == 58248)
            {
                if (who->GetDistance(me) > 5.0f)
                    return;

                if (auto owner = who->GetCharmerOrOwnerPlayerOrPlayerItself())
                {
                    owner->KilledMonsterCredit(58246);
                    who->GetMotionMaster()->MoveFollow(me, 0, 0);
                    who->ToCreature()->ForcedDespawn(1000);
                }
            }
        }
    };
};

class npc_instructor_skythorn : public CreatureScript
{
public:
    npc_instructor_skythorn() : CreatureScript("npc_instructor_skythorn") {}

    struct npc_instructor_skythornAI : public ScriptedAI
    {
        npc_instructor_skythornAI(Creature* creature) : ScriptedAI(creature) {}

        ObjectGuid playerGUID;
        uint8 eventPhase;
        uint32 eventTimer;

        void Reset()
        {
            eventTimer = 0;
            playerGUID;
            eventPhase = 0;
        }

        void BeginEvent(Player* player)
        {
            eventTimer = 10000;
            playerGUID = player->GetGUID();
            Talk(0, playerGUID);
            eventPhase = 1;
        }

        void UpdateAI(uint32 const diff)
        {
            if (eventPhase)
            {
                if (eventTimer < diff)
                {
                    Talk(eventPhase, playerGUID);
                    eventTimer = 8000;
                    ++eventPhase;
                    if (eventPhase == 4)
                    {
                        if (auto player = Player::GetPlayer(*me, playerGUID))
                            player->KilledMonsterCredit(58315);
                        Reset();
                    }
                }
                else
                    eventTimer -= diff;
            }
        }
    };

    bool OnQuestAccept(Player* player, Creature* creature, Quest const* quest)
    {
        if (quest->GetQuestId() == 30142)
            CAST_AI(npc_instructor_skythorn::npc_instructor_skythornAI, creature->AI())->BeginEvent(player);

        return true;
    }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_instructor_skythornAI(creature);
    }
};

class npc_the_pearlfin_situation_q : public CreatureScript
{
public:
    npc_the_pearlfin_situation_q() : CreatureScript("npc_the_pearlfin_situation_q") { }

    bool OnGossipHello(Player* player, Creature* creature) override
    {
        if (creature->isQuestGiver())
            player->PrepareQuestMenu(creature->GetGUID());

        if (creature->isVendor())
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, GOSSIP_TEXT_BROWSE_GOODS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TRADE);

        if (player->GetQuestStatus(29883) == QUEST_STATUS_INCOMPLETE)
        {
            std::string gossip = "";
            switch (creature->GetEntry())
            {
            case 59058:
                gossip = "Please allow us a chance to prove our friendship. We wish you no harm";
                break;
            case 56693:
                gossip = "What are you doing?";
                break;
            case 56690:
                gossip = "My friends and I come with peaceful intentions.";
                break;
            case 54960:
                gossip = "I come from the Alliance. We wish to be allies, not enemies.";
                break;
            }
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, gossip, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
        }

        player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) override
    {
        player->PlayerTalkClass->ClearMenus();
        if (action == GOSSIP_ACTION_INFO_DEF + 1)
        {
            creature->AI()->Talk(0);
            player->KilledMonsterCredit(creature->GetEntry());
            player->CLOSE_GOSSIP_MENU();
        }
        else if (action == GOSSIP_ACTION_TRADE)
            player->GetSession()->SendListInventory(creature->GetGUID());

        return false;
    }
};

enum events
{
    EVENT_LIGHTBREATH = 1,
    EVENT_GENERATE_MOVE = 2
};

// ##npc entry 58213##
class npc_windward_matriarch : public CreatureScript
{
public:
    npc_windward_matriarch() : CreatureScript("npc_windward_matriarch")
    {
    }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_windward_matriarchAI(creature);
    }

    struct npc_windward_matriarchAI : public ScriptedAI
    {
        npc_windward_matriarchAI(Creature* creature) : ScriptedAI(creature) {}

        EventMap events;

        void Reset()
        {
            events.Reset();
            events.ScheduleEvent(EVENT_LIGHTBREATH, 1500);
        }


        void EnterCombat(Unit* /*who*/) { events.ScheduleEvent(EVENT_LIGHTBREATH, 1500); }

        void JustDied(Unit* /*killer*/) {}

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_LIGHTBREATH:
                    DoCast(SelectTarget(SELECT_TARGET_TOPAGGRO), 129013);
                    events.ScheduleEvent(EVENT_LIGHTBREATH, 6000);
                    break;
                default:
                    break;
                }
            }

            DoMeleeAttackIfReady();
        }
    };
};

// ##npc entry 58416##
class npc_injured_hatchling : public CreatureScript
{
public:
    npc_injured_hatchling() : CreatureScript("npc_injured_hatchling")
    {
    }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_injured_hatchlingAI(creature);
    }

    struct npc_injured_hatchlingAI : public ScriptedAI
    {
        npc_injured_hatchlingAI(Creature* creature) : ScriptedAI(creature)
        {
            me->SetHealth((uint32)me->GetMaxHealth()*0.2f);
        }
    };
};


enum SpellsBunny
{
	SPELL_EXPLOTION_BARREL = 130423,
	SPELL_FIRE_ON          = 130755,
	SPELL_FADE_TO_BLACK    = 130411,
};

enum BunnyEvents
{
	EVENT_CAST_FIRE,
};

enum General
{
	QUEST_INTO_THE_MIST = 29690,
};


/*
	NPC ID
*/
class npc_general_nazim_jadeforest_ship : public CreatureScript
{
public:
	npc_general_nazim_jadeforest_ship() : CreatureScript("npc_general_nazim_jadeforest_ship") { }

	CreatureAI* GetAI(Creature* creature) const
	{
		return new npc_general_nazim_jadeforest_shipAI(creature);

	}

	bool OnGossipHello(Player* player, Creature* creature)
	{
		if (creature->isQuestGiver())
			player->PrepareQuestMenu(creature->GetGUID());		

		player->SEND_GOSSIP_MENU(907, creature->GetGUID());
		return true;
	}

	bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
	{
		player->PlayerTalkClass->ClearMenus();
		switch (action)
		{
		case GOSSIP_ACTION_INFO_DEF + 1:
			player->CLOSE_GOSSIP_MENU();

		}
		return true;
	}

	struct npc_general_nazim_jadeforest_shipAI : public ScriptedAI
	{
		npc_general_nazim_jadeforest_shipAI(Creature* creature) : ScriptedAI(creature) {}

		/**/
		EventMap events;
		uint64 PlayerGUID;
		uint32 EventTimer;
		uint8 Phase;
		std::list<uint64> salutePlayerList;
		bool EventStarted;

		void Reset()
		{
			EventTimer = 1000;
			PlayerGUID = 0;
			Phase = 0;
			EventStarted = false;
		}

		bool HasSaluteTo(Player* player)
		{
			for (auto splayer : salutePlayerList)
			{
				if (splayer || player->GetGUID())
				{
					return true;
					break;
				}
			}
			return false;
		}

		void UpdateAI(const uint32 diff)
		{
			events.Update(diff);

			if (EventTimer <= diff)
			{
				std::list<Player*> playerList;
				GetPlayerListInGrid(playerList, me, 100.0f);
				for (auto player : playerList)
				{
					if (!HasSaluteTo(player))
					{
						if (player->GetQuestStatus(QUEST_INTO_THE_MIST) == QUEST_STATUS_INCOMPLETE)
						{
							player->CastSpell(player, SPELL_FADE_TO_BLACK);
							player->CastSpell(player, 122344);
							player->CompleteQuest(QUEST_INTO_THE_MIST);
						}
					}

				}
				EventTimer = 6000;
			}
			else EventTimer -= diff;

		}
	};
};


/*
	Npc ID 
*/
class npc_gunship_fire_bunny : public CreatureScript
{
public:
	npc_gunship_fire_bunny() : CreatureScript("npc_gunship_fire_bunny") { }

	CreatureAI* GetAI(Creature* creature) const
	{
		return new npc_gunship_fire_bunnyAI(creature);

	}

	struct npc_gunship_fire_bunnyAI : public ScriptedAI
	{
		npc_gunship_fire_bunnyAI(Creature* creature) : ScriptedAI(creature) {}


		uint32 EventTimer;
		Player* player;

		bool EventStarted;
		bool HasExploded;
		std::list<uint64> salutePlayerList;

		void Reset()
		{
			EventTimer = 1000;
			EventStarted = false;
			me->SetDisableGravity(true);
			me->SetCanFly(true);
			HasExploded = false;
			me->SetByteFlag(UNIT_FIELD_BYTES_1, 3, 0x02);
			me->AddUnitMovementFlag(MOVEMENTFLAG_FLYING);
		}

		bool HasSaluteTo(Player* player)
		{
			for (auto splayer : salutePlayerList)
			{
				if (splayer || player->GetGUID())
				{
					return true;
					break;
				}
			}
			return false;
		}

		void UpdateAI(const uint32 diff)
		{

			if (EventTimer <= diff)
			{
				std::list<Player*> playerList;
				GetPlayerListInGrid(playerList, me, 10.0f);
				for (auto player : playerList)
				{
					if (!HasSaluteTo(player))
					{
						me->CastSpell(me, 130423);
						me->CastSpell(me, 44681);
						EventStarted = true;
					}

				}

				if (EventStarted)
				{
					me->CastSpell(me, 130755);
					HasExploded = true;
				}

				if (HasExploded && EventStarted)
				{
					me->CastSpell(me, 130423);
					me->CastSpell(me, 44681);
				}
				EventTimer = 6000;
			}
			else EventTimer -= diff;


		}
	};
};


class npc_gunship_cannon : public CreatureScript
{
public:
	npc_gunship_cannon() : CreatureScript("npc_gunship_cannon") { }

	CreatureAI* GetAI(Creature* creature) const
	{
		return new npc_gunship_cannonAI(creature);

	}

	struct npc_gunship_cannonAI : public ScriptedAI
	{
		npc_gunship_cannonAI(Creature* creature) : ScriptedAI(creature) {}


		uint32 EventTimer;
		Player* player;

		bool EventStarted;
		bool HasExploded;
		std::list<uint64> salutePlayerList;

		void Reset()
		{
			EventTimer = 1000;
			EventStarted = false;

		}

		bool HasSaluteTo(Player* player)
		{
			for (auto splayer : salutePlayerList)
			{
				if (splayer || player->GetGUID())
				{
					return true;
					break;
				}
			}
			return false;
		}

		void UpdateAI(const uint32 diff)
		{
			if (EventTimer <= diff)
			{
				std::list<Player*> playerList;
				GetPlayerListInGrid(playerList, me, 1000.0f);
				for (auto player : playerList)
				{
					if (!HasSaluteTo(player))
					{
						me->CastSpell(me, 132002);
						EventStarted = true;
					}

				}

				if (EventStarted)
				{

					HasExploded = true;
				}

				if (HasExploded && EventStarted)
				{
					me->CastSpell(me, 132002);

				}
				EventTimer = 6000;
			}
			else EventTimer -= diff;


		}
	};
};

class npc_gunship_machingun : public CreatureScript
{
public:
	npc_gunship_machingun() : CreatureScript("npc_gunship_machingun") { }

	CreatureAI* GetAI(Creature* creature) const
	{
		return new npc_gunship_machingunAI(creature);

	}

	struct npc_gunship_machingunAI : public ScriptedAI
	{
		npc_gunship_machingunAI(Creature* creature) : ScriptedAI(creature)
		{
			me->AddUnitMovementFlag(MOVEMENTFLAG_ROOT);
		}				

		void Reset()
		{			
		}
	};
};


// Pearlfin Aqualyte - 54959/56585/56591/56592
class mob_pearlfin_aqualyte : public CreatureScript
{
    public:
        mob_pearlfin_aqualyte() : CreatureScript("mob_pearlfin_aqualyte") {}

        bool OnGossipHello(Player* player, Creature* creature) override
        {
            if (player->GetQuestStatus(29903) == QUEST_STATUS_INCOMPLETE)
            {
                player->KilledMonsterCredit(56601);
                creature->DespawnOrUnsummon();
            }

            return true;
        }
};

// Pearlfin Villager - 56233
class mob_pearlfin_villager : public CreatureScript
{
    public:
        mob_pearlfin_villager() : CreatureScript("mob_pearlfin_villager") {}

        bool OnGossipHello(Player* player, Creature* creature) override
        {
            if (player->GetQuestStatus(29762) == QUEST_STATUS_INCOMPLETE)
            {
                player->AddItem(79811, 1);
                creature->DespawnOrUnsummon();
            }

            return true;
        }
};

// Elder Lusshan - 54960 ## Instructor Sharpfin - 56690 ## Ot-Temmdo - 56693 ## Pearlkeeper Fujin - 59058
class mob_pearlfin_situation : public CreatureScript
{
    public:
        mob_pearlfin_situation() : CreatureScript("mob_pearlfin_situation") {}

        bool OnGossipHello(Player* player, Creature* creature) override
        {
                switch (creature->GetEntry())
                {
                    case 59058:
                        if (creature->isQuestGiver())
                            player->PrepareQuestMenu(creature->GetGUID());

                        if (player->GetQuestStatus(29883) == QUEST_STATUS_INCOMPLETE)
                            player->KilledMonsterCredit(59058);

                        player->PlayerTalkClass->SendGossipMenu(GOSSIP_TEXT_I, creature->GetGUID());
                        break;
                    case 56693:
                        if (player->GetQuestStatus(29883) == QUEST_STATUS_INCOMPLETE)
                            player->KilledMonsterCredit(56693);
                        break;
                    case 56690:
                        if (player->GetQuestStatus(29883) == QUEST_STATUS_INCOMPLETE)
                            player->KilledMonsterCredit(56690);
                        break;
                    case 54960:
                        if (creature->isQuestGiver())
                            player->PrepareQuestMenu(creature->GetGUID());

                        if (player->GetQuestStatus(29883) == QUEST_STATUS_INCOMPLETE)
                            player->KilledMonsterCredit(54960);

                        player->PlayerTalkClass->SendGossipMenu(GOSSIP_TEXT_I, creature->GetGUID());
                        break;
                    default:
                        break;
                }

            return true;
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 /*action*/) override
        {
            if (creature->GetEntry() == 59058 || creature->GetEntry() == 54960)
                player->PlayerTalkClass->GetQuestMenu();

            return true;
        }
};

// Hozen Cage - 209586
// Initial LockId : 1979
class gob_hozen_cage : public GameObjectScript
{
    public:
        gob_hozen_cage() : GameObjectScript("gob_hozen_cage") {}

        bool OnGossipHello(Player* player, GameObject* go) override
        {
            if (player->GetQuestStatus(29559) == QUEST_STATUS_INCOMPLETE)
            {
                if (player->HasItemCount(74260, 1))
                {
                    player->KilledMonsterCredit(55201);
                    player->DestroyItemCount(74260, 1, true);
                    go->DestroyForPlayer(player);
                }
            }

            return true;
        }
};

// Widow's Web - 65634
class mob_widow_s_web : public CreatureScript
{
    public:
        mob_widow_s_web() : CreatureScript("mob_widow_s_web") {}

        struct mob_widow_s_webAI : public ScriptedAI
        {
            mob_widow_s_webAI(Creature* creature) : ScriptedAI(creature) {}

            void Reset() override
            {
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_REMOVE_CLIENT_CONTROL);
            }

            void JustDied(Unit* killer) override
            {
                if (Player* player = killer->ToPlayer())
                {
                    if (player->GetQuestStatus(31707) == QUEST_STATUS_INCOMPLETE)
                    {
                        me->SummonCreature(65647, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ());
                        player->KilledMonsterCredit(65647);
                    }
                }
            }

        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new mob_widow_s_webAI(creature);
        }
};

// Freed Sri-La Villager - 65647
class mob_freed_sri_la_villager : public CreatureScript
{
    public:
        mob_freed_sri_la_villager() : CreatureScript("mob_freed_sri_la_villager") {}

        struct mob_freed_sri_la_villagerAI : public ScriptedAI
        {
            mob_freed_sri_la_villagerAI(Creature* creature) : ScriptedAI(creature)
            {
                timer = 0;
            }

            uint32 timer;

            void IsSummonedBy(Unit* /*summoner*/) override
            {
                timer = 3000;
            }

            void UpdateAI(uint32 diff) override
            {
                if (timer)
                {
                    if (timer <= diff)
                    {
                        me->DespawnOrUnsummon();
                    }
                    else
                        timer -= diff;
                }
            }

        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new mob_freed_sri_la_villagerAI(creature);
        }
};

// Windward Hatchling - 58220/58243/58244
class mob_windward_hatchling : public CreatureScript
{
    public:
        mob_windward_hatchling() : CreatureScript("mob_windward_hatchling") {}

        struct mob_windward_hatchlingAI : public ScriptedAI
        {
            mob_windward_hatchlingAI(Creature* creature) : ScriptedAI(creature) {}

            void DoAction(int32 action) override
            {
                if (action == ACTION_SET_FLAG)
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PLAYER_CONTROLLED);
            }

            void Reset() override
            {
                me->SetDefaultMovementType(RANDOM_MOTION_TYPE);
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new mob_windward_hatchlingAI(creature);
        }
};

// Custom npc 1 - 690701
class npc_custom_npc : public CreatureScript
{
    public:
        npc_custom_npc() : CreatureScript("npc_custom_npc") {}

        struct npc_custom_npcAI : public ScriptedAI
        {
            npc_custom_npcAI(Creature* creature) : ScriptedAI(creature)
            {
                hasSummoned = false;
            }

            EventMap events;
            bool hasSummoned;

            void Reset() override
            {
                events.Reset();

                events.ScheduleEvent(EVENT_CHECK_PLAYER, 2000);
            }

            void UpdateAI(uint32 diff) override
            {
                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    if (eventId == EVENT_CHECK_PLAYER)
                    {
                        std::list<Player*> playerList;
                        GetPlayerListInGrid(playerList, me, 30.0f);

                        for (auto player: playerList)
                        {
                            if (!hasSummoned)
                            {
                                if (player->GetQuestStatus(30156) == QUEST_STATUS_INCOMPLETE)
                                {
                                    player->SummonCreature(58527, player->GetPositionX() + 1, player->GetPositionY() + 1.5f, player->GetPositionZ() + 1);
                                    player->SummonCreature(58527, player->GetPositionX() + 2, player->GetPositionY() + 2.5f, player->GetPositionZ() - 1);
                                    player->SummonCreature(58527, player->GetPositionX() + 1.5f, player->GetPositionY() + 1, player->GetPositionZ() + 1.5f);

                                    std::list<Creature*> creatureList;
                                    player->GetCreatureListWithEntryInGrid(creatureList, 58527, 4.0f);

                                    for (auto creature: creatureList)
                                    {
                                        creature->GetMotionMaster()->MoveFollow(player, 4.0f, 4.0f, MOTION_SLOT_ACTIVE);
                                    }
                                }

                                hasSummoned = true;
                            }
                        }

                        if (playerList.empty())
                            hasSummoned = false;
                    }

                    events.ScheduleEvent(EVENT_CHECK_PLAYER, 2000);
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new npc_custom_npcAI(creature);
        }
};

// Instructor Windblade - 58420
class mob_instructor_windblade : public CreatureScript
{
    public:
        mob_instructor_windblade() : CreatureScript("mob_instructor_windblade") {}

        bool OnGossipHello(Player* player, Creature* creature) override
        {
            if (creature->isQuestGiver())
                player->PrepareQuestMenu(creature->GetGUID());

            if (player->GetQuestStatus(30143) == QUEST_STATUS_INCOMPLETE)
                    player->KilledMonsterCredit(58572);

            player->PlayerTalkClass->SendGossipMenu(GOSSIP_TEXT_I, creature->GetGUID());

            return true;
        }

        bool OnGossipSelect(Player* player, Creature* /*creature*/, uint32 /*sender*/, uint32 /*action*/) override
        {
            player->PlayerTalkClass->GetQuestMenu();

            return true;
        }

        bool OnQuestAccept(Player* player, Creature* /*creature*/, Quest const* quest) override
        {
            if (quest->GetQuestId() == 30144) // This quests needs the player to complete it on a mount, but vehicleId is unfindable, so he can complete the quest with any flying mount he already gets.
            {
                player->SummonCreature(58426, 1614.22f, -2733.17f, 172.235f, 4.50949f);
                player->SummonCreature(58426, 1643.95f, -2798.85f, 129.329f, 5.16687f);
                player->SummonCreature(58426, 1647.11f, -2772.71f, 161.497f, 5.72295f);
                player->SummonCreature(58426, 1705.37f, -2815.74f, 141.012f, 5.60907f);
                player->SummonCreature(58426, 1739.69f, -2935.46f, 103.094f, 5.06008f);
                player->SummonCreature(58426, 1853.05f, -2963.5f, 96.546f, 5.59807f);
                player->SummonCreature(58426, 1716.28f, -3080.08f, 39.4494f, 3.61493f);
                player->SummonCreature(58426, 1788.15f, -2927.51f, 112.879f, 1.18805f);
                player->SummonCreature(58426, 1736.29f, -2780.32f, 162.346f, 4.06654f);
                player->SummonCreature(58426, 1752.44f, -2701.74f, 137.955f, 3.12799f);
                player->SummonCreature(58426, 1786.51f, -2722.46f, 142.96f, 0.141875f);
            }

            return true;
        }
};

// Cloud Ring - 58426
class npc_cloud_ring : public CreatureScript
{
    public:
        npc_cloud_ring() : CreatureScript("npc_cloud_ring") {}

        struct npc_cloud_ringcAI : public ScriptedAI
        {
            npc_cloud_ringcAI(Creature* creature) : ScriptedAI(creature)
            {
                playerGUID = ObjectGuid::Empty;
            }

            ObjectGuid playerGUID;

            void Reset() override
            {
                me->SetDisplayId(11686);
                me->AddAura(129282, me);
            }

            void IsSummonedBy(Unit* summoner) override
            {
                if (Player* player = summoner->ToPlayer())
                    playerGUID = player->GetGUID();
            }

            void UpdateAI(uint32 /*diff*/) override
            {
                if (Player* player = ObjectAccessor::GetPlayer(*me, playerGUID))
                {
                    if (player->GetQuestStatus(30144) == QUEST_STATUS_INCOMPLETE)
                    {
                        if (player->GetDistance2d(me) <= 1.0f)
                        {
                            me->DespawnOrUnsummon();
                            player->KilledMonsterCredit(58426);
                        }
                    }

                    else if (player->GetQuestStatus(30144) == QUEST_STATUS_COMPLETE)
                        me->DespawnOrUnsummon();
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new npc_cloud_ringcAI(creature);
        }
};

// Jade Cloud Serpent - 58527
class mob_jade_cloud_serpent : public CreatureScript
{
    public:
        mob_jade_cloud_serpent() : CreatureScript("mob_jade_cloud_serpent")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_jade_cloud_serpentAI(creature);
        }

        struct mob_jade_cloud_serpentAI : public ScriptedAI
        {
            mob_jade_cloud_serpentAI(Creature* creature) : ScriptedAI(creature)
            {
                playerGuid = 0;
            }

            EventMap events;
            uint64 playerGuid;

            void Reset()
            {
                events.Reset();

                events.ScheduleEvent(EVENT_CHECK_TARGET, 2000);
            }

            void IsSummonedBy(Unit* summoner)
            {
                //if (Player* player = summoner->ToPlayer())
                    //playerGuid = player->GetGUID();
            }

            void SpellHit(Unit* caster, SpellInfo const* spell)
            {
                if (Player* player = caster->ToPlayer())
                {
                    if (spell->Id == 110455)
                    {
                        me->DespawnOrUnsummon();
                        player->KilledMonsterCredit(58351);
                    }
                }
            }

            void UpdateAI(const uint32 diff)
            {
                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    if (eventId == EVENT_CHECK_TARGET)
                    {
                        //if (Player* player = Player::GetPlayer(*me, playerGuid))
                        {
                            //if (Unit* victim = player->getVictim())
                            {
                                //me->SetInCombatWith(victim);
                                //AttackStart(victim);
                                //DoMeleeAttackIfReady();
                            }
                        }

                        events.ScheduleEvent(EVENT_CHECK_TARGET, 2000);
                    }
                }
            }
        };
};

// Fei - 56784
class npc_fei : public CreatureScript
{
public:
    npc_fei() : CreatureScript("npc_fei")
    {
    }

    bool OnQuestAccept(Player* player, Creature* creature, Quest const* quest)
    {
        if (quest->GetQuestId() == 30000)
        {
            Position pos;
            player->SendSpellScene(202, nullptr, true, &pos);
            player->KilledMonsterCredit(57243);

            if (Creature* questTaker = GetClosestCreatureWithEntry(creature, 57242, 200.0f, true))
                player->TeleportTo(870, questTaker->GetPositionX() + 1.4f, questTaker->GetPositionY() + 1.5f, questTaker->GetPositionZ(), 0.0f, 0);
        }

        return true;
    }
};


// Suchi The Sweet - 66029
class mob_suchi_the_sweet : public CreatureScript
{
public:
    mob_suchi_the_sweet() : CreatureScript("mob_suchi_the_sweet") {}

    bool OnGossipHello(Player* player, Creature* creature) override
    {
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_CHOICE_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
        player->SEND_GOSSIP_MENU(69970, creature->GetGUID());

        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) override
    {
        player->PlayerTalkClass->ClearMenus();

        if (action == GOSSIP_ACTION_INFO_DEF + 1)
        {
            player->CLOSE_GOSSIP_MENU();

            if (player->GetQuestStatus(31720) == QUEST_STATUS_INCOMPLETE)
            {
                creature->AI()->SetGUID(player ? player->GetGUID() : ObjectGuid::Empty);
                creature->setFaction(14);

                if (creature->GetAI())
                {
                    creature->AI()->Reset();
                    creature->AI()->DoAction(ACTION_REMOVE_FLAG);
                }
            }
        }
        return true;
    }

    struct mob_suchi_the_sweetAI : public ScriptedAI
    {
        mob_suchi_the_sweetAI(Creature* creature) : ScriptedAI(creature)
        {
            playerGUID = ObjectGuid::Empty;
        }

        ObjectGuid playerGUID;
        EventMap events;

        void Reset() override
        {
            events.Reset();
        }

        void DamageTaken(Unit* attacker, uint32& damage, DamageEffectType dmgType)
        {
            if (Player* player = attacker->ToPlayer())
            {
                if (player->GetQuestStatus(31720) == QUEST_STATUS_INCOMPLETE)
                {
                    if (damage > me->GetHealth())
                    {
                        damage = 0;
                        me->SetFullHealth();
                        DoAction(ACTION_REINITIALIZE);
                        player->KilledMonsterCredit(58510);
                        me->DespawnOrUnsummon();
                    }
                }
            }
        }

        void SetGUID(ObjectGuid const& guid, int32 /*param*/) override
        {
            
                playerGUID = guid;
        }

        void IsSummonedBy(Unit* /*summoner*/) override
        {
            events.ScheduleEvent(EVENT_DESPAWN, 600000);
        }

        void DoAction(int32 action) override
        {
            if (action == ACTION_REMOVE_FLAG)
            {
                me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            }

            else if (action == ACTION_REINITIALIZE)
            {
                me->setFaction(35);
                me->CombatStop();
                me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            }
        }

        void UpdateAI(uint32 diff) override
        {
            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                if (eventId == EVENT_DESPAWN)
                    me->DespawnOrUnsummon();
            }

            if (Player* player = ObjectAccessor::FindPlayer(playerGUID))
            {
                if (!player->isAlive())
                {
                    DoAction(ACTION_REINITIALIZE);
                    events.Reset();
                    return;
                }

                if (player->GetQuestStatus(31720) != QUEST_STATUS_INCOMPLETE)
                {
                    me->DespawnOrUnsummon();
                }

                if (me->GetDistance2d(player) >= 25.0f)
                {
                    me->DespawnOrUnsummon();
                }
            }

            if (!UpdateVictim())
                return;

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new mob_suchi_the_sweetAI(creature);
    }
};



class npc_anduin_jadeforest : public CreatureScript
{
public:
	npc_anduin_jadeforest() : CreatureScript("npc_anduin_jadeforest") { }

	CreatureAI* GetAI(Creature* creature) const
	{
		return new npc_anduin_jadeforestAI(creature);

	}

	bool OnGossipHello(Player* player, Creature* creature)
	{
		if (creature->isQuestGiver())
			player->PrepareQuestMenu(creature->GetGUID());

		if (player->GetQuestStatus(29901) == QUEST_STATUS_INCOMPLETE)
			

		player->SEND_GOSSIP_MENU(907, creature->GetGUID());
		return true;
	}

	bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
	{
		player->PlayerTalkClass->ClearMenus();
		switch (action)
		{
		case GOSSIP_ACTION_INFO_DEF + 1:
			player->CLOSE_GOSSIP_MENU();
			player->CompleteQuest(29901);

		}
		return true;
	}

	struct npc_anduin_jadeforestAI : public ScriptedAI
	{
		npc_anduin_jadeforestAI(Creature* creature) : ScriptedAI(creature) {}


		bool EventStarted;

		void Reset()
		{
		}

		void UpdateAI(const uint32 diff)
		{
		}
	};
};



class npc_anduin_prisioner : public CreatureScript
{
public:
	npc_anduin_prisioner() : CreatureScript("npc_anduin_prisioner") { }

	CreatureAI* GetAI(Creature* creature) const
	{
		return new npc_anduin_prisionerAI(creature);

	}

	struct npc_anduin_prisionerAI : public ScriptedAI
	{
		npc_anduin_prisionerAI(Creature* creature) : ScriptedAI(creature)
		{			
		}		
		uint32 EventTimer;	
		bool isCompleted;
		void Reset()
		{		
		  EventTimer = 2000;
		  isCompleted = false;
		}
		
		void UpdateAI(const uint32 diff)
		{
			if (isCompleted)
				return;
			
			if (EventTimer < diff)
			{				
				if (Player* Player = me->FindNearestPlayer(25.0f, true))
				{
					Player->KilledMonsterCredit(55693);
					isCompleted = true;
				}
			}
			else EventTimer -= diff;

		}
	};
};



class npc_sha_remnant : public CreatureScript
{
public:
	npc_sha_remnant() : CreatureScript("npc_sha_remnant") { }

	CreatureAI* GetAI(Creature* creature) const
	{
		return new npc_sha_remnantAI(creature);

	}

	struct npc_sha_remnantAI : public ScriptedAI
	{
		npc_sha_remnantAI(Creature* creature) : ScriptedAI(creature)
		{
		}

		ObjectGuid PlayerGUID;
		uint32 EventTimer;
		uint8 Phase;
		bool EventStarted;

		void Reset()
		{
			Phase = 0;
			PlayerGUID;
			EventTimer = 2000;
			EventStarted = false;
		}

		void SpellHit(Unit* caster, const SpellInfo* spell)
		{
			if (spell->Id == 114297)
			{
				EventStarted = true;
				PlayerGUID = caster->GetGUID();
			}
		}

		void UpdateAI(const uint32 diff)
		{

			if (EventStarted)
			{
				if (EventTimer < diff)
				{
					if (Player* player = me->GetPlayer(*me, PlayerGUID))
					{
						switch (Phase)
						{
						case 0:
							player->KilledMonsterCredit(59454);
							EventTimer = 4000;
							Phase++;
							break;
						case 1:
							me->DespawnOrUnsummon(500);
							EventTimer = 2000;
							Phase++;
							break;

						default:
							break;
						}
					}
				}
				else EventTimer -= diff;
			}

		}
	};
};
       
// Student of Chi-Ji - 60601/60602/60603
class mob_chi_ji_student : public CreatureScript
{
    public:
        mob_chi_ji_student() : CreatureScript("mob_chi_ji_student") {}

        struct mob_chi_ji_studentAI : public ScriptedAI
        {
            mob_chi_ji_studentAI(Creature* creature) : ScriptedAI(creature)
            {
                playerGUID = ObjectGuid::Empty;
            }

            EventMap events;
            ObjectGuid playerGUID;

            void Reset() override
            {
                events.Reset();

                switch (me->GetEntry())
                {
                    case 60601:
                        events.ScheduleEvent(EVENT_GIFT_OF_CHI_JI, 3000);
                        events.ScheduleEvent(EVENT_BLESSING_OF_CHI_JI, 6000);
                        events.ScheduleEvent(EVENT_WRATH_OF_CHI_JI, 11000);
                        break;
                    case 60602:
                        events.ScheduleEvent(EVENT_BLACKOUT_KICK, 3000);
                        events.ScheduleEvent(EVENT_FLYING_SERPENT_KICK, 8000);
                        events.ScheduleEvent(EVENT_JAB, 13000);
                        break;
                    case 60603:
                        events.ScheduleEvent(EVENT_MIGHTY_CHOP, 3000);
                        events.ScheduleEvent(EVENT_STRIKE_FROM_THE_HEAVENS, 6000);
                        break;
                    default:
                        break;
                }
            }

            void EnterCombat(Unit* attacker) override
            {
                if (Player* pAttacker = attacker->ToPlayer())
                    playerGUID = pAttacker->GetGUID();
            }

           

            void UpdateAI(uint32 diff) override
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        // Entry 60601
                        case EVENT_GIFT_OF_CHI_JI:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_GIFT_OF_CHI_JI, false);
                            events.ScheduleEvent(EVENT_GIFT_OF_CHI_JI, 7000);
                            break;
                        case EVENT_BLESSING_OF_CHI_JI:
                            me->CastSpell(me, SPELL_BLESSING_OF_CHI_JI, false);
                            events.ScheduleEvent(EVENT_BLESSING_OF_CHI_JI, 7000);
                            break;
                        case EVENT_WRATH_OF_CHI_JI:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                me->CastSpell(target, SPELL_WRATH_OF_CHI_JI, false);
                            events.ScheduleEvent(EVENT_WRATH_OF_CHI_JI, 7000);
                            break;
                            // Entry 60602
                        case EVENT_BLACKOUT_KICK:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                me->CastSpell(target, SPELL_BLACKOUT_KICK, false);
                            events.ScheduleEvent(EVENT_BLACKOUT_KICK, 15000);
                            break;
                        case EVENT_FLYING_SERPENT_KICK:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                me->CastSpell(target, SPELL_FLYING_SERPENT_KICK, false);
                            events.ScheduleEvent(EVENT_FLYING_SERPENT_KICK, 15000);
                            break;
                        case EVENT_JAB:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                me->CastSpell(target, SPELL_JAB, false);
                            events.ScheduleEvent(EVENT_JAB, 15000);
                            break;
                            // Entry 60603
                        case EVENT_MIGHTY_CHOP:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                me->CastSpell(target, SPELL_MIGHTY_CHOP, false);
                            events.ScheduleEvent(EVENT_MIGHTY_CHOP, 7000);
                            break;
                        case EVENT_STRIKE_FROM_THE_HEAVENS:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                me->CastSpell(target, SPELL_STRIKE_FROM_THE_HEAVENS, false);
                            events.ScheduleEvent(EVENT_STRIKE_FROM_THE_HEAVENS, 7000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new mob_chi_ji_studentAI(creature);
        }
};

// Item 89602 Alliance Flare - Spell 130373 Call In Airstrike - For Quest 31743 Smoke Before Fire
class item_alliance_flare_89602 : public ItemScript
{
public:
    item_alliance_flare_89602() : ItemScript("item_alliance_flare_89602") { }

    bool OnUse(Player* player, Item* item, SpellCastTargets const& targets) override
    {
        if (!player || !item)
            return false;

        player->CastSpell(targets.GetDstPos(), 130373, true);

        if (Creature* gyrocopter = player->SummonCreature(65843, Position(player->GetPositionX(), player->GetPositionY(), player->GetPositionZ() + 20.f), TEMPSUMMON_TIMED_DESPAWN, 1000))
        {
            std::map<int, std::pair<int, int>> gyrocopterTexts;

            gyrocopterTexts.insert(std::make_pair(0, std::make_pair(32534, 67688)));
            gyrocopterTexts.insert(std::make_pair(1, std::make_pair(32535, 67690)));
            gyrocopterTexts.insert(std::make_pair(2, std::make_pair(32536, 67691)));
            gyrocopterTexts.insert(std::make_pair(3, std::make_pair(32537, 67693)));
            gyrocopterTexts.insert(std::make_pair(4, std::make_pair(32538, 67694)));

            int rand = frand(0, 4);
            // text not work but sounds work
            //gyrocopter->Say(gyrocopterTexts[rand].second, LANG_UNIVERSAL, player->GetGUID()); 
            gyrocopter->PlayDirectSound(gyrocopterTexts[rand].first, player);
        }

        player->AddDelayedEvent(7000, [player]() -> void
            {
                // Xhu'Daggab
                if (Creature* xhu = player->FindNearestCreature(66277, 40.f))
                {
                    // explosion 5 times then remove runes
                    for (int i = 1; i <= 5; i++)
                        xhu->AddDelayedEvent(500 * i, [xhu]() -> void
                            {
                                xhu->CastSpell(xhu, explosion, true);
                            });
                    xhu->AddDelayedEvent(3000, [xhu, player]() -> void
                        {
                            if (Creature* genericBunny = xhu->FindNearestCreature(44775, 7.f))
                            {
                                genericBunny->RemoveAurasDueToSpell(xhudaggabrunes);
                                genericBunny->DespawnOrUnsummon(0, Seconds(60));
                            }
                            if (Creature* genericBunny = xhu->FindNearestCreature(41200, 7.f))
                            {
                                genericBunny->RemoveAurasDueToSpell(xhudaggabrunes);
                                genericBunny->DespawnOrUnsummon(0, Seconds(60));
                            }
                            xhu->DespawnOrUnsummon(5000, Seconds(60));
                            player->KilledMonsterCredit(66302);
                        });
                }

                // Wild Imps
                if (Creature* imp = player->FindNearestCreature(66278, 40.f))
                {
                    // explosion 5 times then remove runes
                    for (int i = 1; i <= 5; i++)
                        imp->AddDelayedEvent(500 * i, [imp]() -> void
                            {
                                imp->CastSpell(imp, explosion, true);
                            });
                    imp->AddDelayedEvent(3000, [imp, player]() -> void
                        {
                            if (Creature* genericBunny = imp->FindNearestCreature(44775, 7.f))
                            {
                                genericBunny->RemoveAurasDueToSpell(wildimprunes);
                                genericBunny->DespawnOrUnsummon(0, Seconds(60));
                            }
                            if (Creature* genericBunny = imp->FindNearestCreature(41200, 7.f))
                            {
                                genericBunny->RemoveAurasDueToSpell(wildimprunes);
                                genericBunny->DespawnOrUnsummon(0, Seconds(60));
                            }
                            imp->DespawnOrUnsummon(5000, Seconds(60));
                            player->KilledMonsterCredit(66301);
                        });
                }

                // Twinspire Demolishers
                if (Creature* demo = player->FindNearestCreature(66279, 40.f))
                {
                    // explosion 5 times then destroy
                    for (int i = 1; i <= 5; i++)
                        demo->AddDelayedEvent(500 * i, [demo]() -> void
                            {
                                demo->CastSpell(demo, explosion, true);
                            });
                    demo->AddDelayedEvent(3000, [demo, player]() -> void
                        {
                            demo->Kill(demo);
                            demo->DespawnOrUnsummon(15000, Seconds(60));
                            player->KilledMonsterCredit(66304);
                        });
                }

                // Twinspire Munitions
                if (GameObject* munitions = player->FindNearestGameObject(215254, 40.f))
                {
                    if (Creature* genericBunny = munitions->FindNearestCreature(44775, 7.f))
                    {
                        for (int i = 1; i <= 5; i++)
                            genericBunny->AddDelayedEvent(500 * i, [genericBunny]() -> void
                                {
                                    genericBunny->CastSpell(genericBunny, explosion);
                                });
                    }
                    if (Creature* genericBunny = munitions->FindNearestCreature(41200, 7.f))
                    {
                        for (int i = 1; i <= 5; i++)
                            genericBunny->AddDelayedEvent(500 * i, [genericBunny]() -> void
                                {
                                    genericBunny->CastSpell(genericBunny, explosion);
                                });
                    }

                    munitions->UseDoorOrButton(60000);
                    player->KilledMonsterCredit(66303);
                }
            });


        return true;
    }

private:
    enum
    {
        explosion = 130423,
        xhudaggabrunes = 130456,
        wildimprunes = 130457
    };
};

class player_pandaria_quest_intro : public PlayerScript
{
public:
    player_pandaria_quest_intro() : PlayerScript("player_pandaria_quest_intro") {}

	void OnUpdateArea(Player* player, uint32 NewArea) override
	{
		if (NewArea == 5853 && player->GetQuestStatus(29548) == QUEST_STATUS_INCOMPLETE && player->GetTeam() == ALLIANCE)
		{
            Position pos;
            player->GetPosition(&pos);
            player->KilledMonsterCredit(66292);
            player->SendSpellScene(94, nullptr, true, &pos);
		}
	}
};

void AddSC_jade_forest()
{
    // Rare mobs
    new mob_kor_nas_nightsavage();
    new mob_krax_ik();
    new mob_mister_ferocious();
    // Elite mobs
    new mob_shadow_of_doubt();
    new mob_akkalar();
    new mob_grookin_outrunner();
    new mob_bamboo_python();
    new mob_rakira();
    new mob_ro_shen();
    new mob_sha_reminant();
    new mob_hutia();
    // Standard Mobs
    new mob_pandriarch_windfur();
    new mob_pandriarch_bramblestaff();
    new mob_pandriarch_goldendraft();
    new mob_big_bao();
    new npc_elder_anli();
    new mob_pearlfin_aqualyte();
    new mob_pearlfin_villager();
    new mob_pearlfin_situation();
    new mob_widow_s_web();
    new mob_freed_sri_la_villager();
    new mob_windward_hatchling();
    new npc_custom_npc();
    new mob_jade_cloud_serpent();
    new mob_instructor_windblade();
    new npc_cloud_ring();
    new npc_instructor_skythorn();
    new mob_suchi_the_sweet();
    new npc_fei();
    new mob_chi_ji_student();
    //Quest scripts
    new npc_anduin_prisioner();
    new npc_sha_remnant();
    new npc_nectarbreeze_farmer();
    new npc_windward_hatchling();
    new npc_windward_nest_trigger();
    new npc_the_pearlfin_situation_q();
    new npc_windward_matriarch();
    new npc_injured_hatchling();
    new npc_general_nazim_jadeforest_ship();
    new npc_gunship_fire_bunny();
    new npc_anduin_jadeforest();
    // Game Objects
    new gob_hozen_cage();
	
	// Items
    new item_alliance_flare_89602();
	
	RegisterPlayerScript(player_pandaria_quest_intro);
}
