////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ObjectMgr.h"
#include "ScriptMgr.h"

#define BOSS_YU_LON 0

enum eSpells
{
};

enum eEvents
{
};

enum eCreatures
{
};

enum eSays
{
};

enum eActions
{
};

class world_boss_yu_lon : public CreatureScript
{
    public:
        world_boss_yu_lon() : CreatureScript("world_boss_yu_lon") { }

        struct world_boss_yu_lonAI : public BossAI
        {
            world_boss_yu_lonAI(Creature* creature) : BossAI(creature, BOSS_YU_LON) { }

            EventMap events;

            void Reset()
            {
                events.Reset();

                _Reset();
            }

            void KilledUnit(Unit* who)
            {
                if (who->IsPlayer())
                    //Talk(TALK_SLAY);
                        return;
            }

            void EnterCombat(Unit* /*p_Attacker*/)
            {
            }

            void JustDied(Unit* /*p_Killer*/)
            {
            }

            void DoAction(const int32 /*p_Action*/)
            {
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                /*switch (events.ExecuteEvent())
                {
                    default:
                        break;
                }*/

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new world_boss_yu_lonAI(creature);
        }
};

#ifndef __clang_analyzer__
void AddSC_boss_yu_lon()
{
    new world_boss_yu_lon();
}
#endif
