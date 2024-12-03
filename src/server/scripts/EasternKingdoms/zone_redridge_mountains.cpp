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

/* Script Data Start
SDName: Redridge Mountains
SD%Complete: 0
SDComment:
Script Data End */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedEscortAI.h"

enum RedridgeCreature
{
    NPC_CANYON_ETTIN = 43094,
    NPC_SUBDUED_CANYON_ETTIN = 43197,
    NPC_FOREMAN_OSLOW = 341,
    NPC_BRIDGE_WORKER_TRENT = 648,
    NPC_BRIDGE_WORKER_DMITRI = 649,
    NPC_BRIDGE_WORKER_JESS = 650,
    NPC_BRIDGE_WORKER_DANIEL = 651,
    NPC_BRIDGE_WORKER_MATTHEW = 652,
    NPC_BRIDGE_WORKER_ALEX = 653,
};

enum RedridgeSpell
{
    SPELL_CONTROL_ETTIN = 80704,
    SPELL_CONTROL_ETTIN_2 = 80702,
    SPELL_LIFT_HUGE_BOULDER = 80739,
    SPELL_EJECT_PASSENGER_1 = 80743
};

class spell_redridge_control_ettin : public SpellScript
{
    PrepareSpellScript(spell_redridge_control_ettin);

    void HandleScriptEffect(SpellEffIndex /*effIndex*/)
    {
        Unit* caster = GetCaster();
        if (!caster || !caster->IsPlayer())
            return;

        if (Creature* target = GetHitCreature())
            if (target->GetEntry() == NPC_CANYON_ETTIN)
                caster->CastSpell(target, SPELL_CONTROL_ETTIN_2, false);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_redridge_control_ettin::HandleScriptEffect, EFFECT_1, SPELL_EFFECT_DUMMY);
    }
};

class spell_redridge_lift_huge_boulder : public SpellScript
{
    PrepareSpellScript(spell_redridge_lift_huge_boulder);

    void HandleScriptEffect(SpellEffIndex /*effIndex*/)
    {
        if (Unit* caster = GetCaster())
            if (Creature* target = GetHitCreature())
                caster->CastSpell(target, uint32(GetSpellInfo()->GetEffect(EFFECT_0)->BasePoints), false);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_redridge_lift_huge_boulder::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
    }
};

enum HugeBoulderEvents
{
    EVENT_START_PHASE_IDLE = 1,
    EVENT_FOREMAN_OSLOW_IDLE_TALK = 2,
    EVENT_BRIDGE_WORKER_ALEX_IDLE_TALK = 3,
    EVENT_BRIDGE_WORKER_ALEX_HARDER = 4,
    EVENT_BRIDGE_WORKER_MATTHEW_HARDER_RESPONSE = 5,
    EVENT_BRIDGE_WORKERS_RESPONSE = 6,
    EVENT_REPOSITION = 7,
    EVENT_LIFT_BOULDER = 8,

    EVENT_DEBUG_RESET = 99
};

enum HugeBoulderPhases
{
    PHASE_IDLE = 0,
    PHASE_REMOVE_ROCK = 1
};

class npc_redridge_huge_boulder : public CreatureScript
{
public:
    npc_redridge_huge_boulder() : CreatureScript("npc_redridge_huge_boulder") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_redridge_huge_boulderAI(creature);
    }

    struct npc_redridge_huge_boulderAI : public CreatureAI
    {
        npc_redridge_huge_boulderAI(Creature* creature) : CreatureAI(creature) { }

        void Reset() override
        {
            _events.Reset();
            _events.SetPhase(PHASE_IDLE);
            _events.ScheduleEvent(EVENT_START_PHASE_IDLE, 0, 0, PHASE_IDLE);
        }

        void JustSummoned(Creature* summoned) override
        {
            Reset();
        }

        void SpellHit(Unit* caster, SpellInfo const* spell) override
        {
            if (!caster || !caster->IsPlayer())
                return;

            if (spell->Id != SPELL_CONTROL_ETTIN)
                return;

            if (!_events.IsInPhase(PHASE_IDLE))
                return;

            if (Creature* ettin = me->FindNearestCreature(NPC_SUBDUED_CANYON_ETTIN, 20.0f))
            {
                _ettinGUID = ettin->GetGUID();
                _events.Reset();
                _events.SetPhase(PHASE_REMOVE_ROCK);
                _events.ScheduleEvent(EVENT_REPOSITION, 1s, 0, PHASE_REMOVE_ROCK);
            }
        }

        void UpdateAI(uint32 diff) override
        {
            _events.Update(diff);

            while (uint32 eventId = _events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_START_PHASE_IDLE:
                        // wait for everyone to appear
                        if (!LocateGUIDs())
                            _events.Repeat(500);

                        _events.ScheduleEvent(EVENT_FOREMAN_OSLOW_IDLE_TALK, 45s, 60s, 0, PHASE_IDLE);
                        _events.ScheduleEvent(EVENT_BRIDGE_WORKER_ALEX_IDLE_TALK, 20s, 30s, 0, PHASE_IDLE);
                        break;
                    case EVENT_FOREMAN_OSLOW_IDLE_TALK:
                        if (Creature* foreman = GetForeman())
                            sCreatureTextMgr->SendChat(foreman, 0);
                        _events.Repeat(45s, 60s);
                        break;
                    case EVENT_BRIDGE_WORKER_ALEX_IDLE_TALK:
                        if (Creature* alex = GetAlex())
                        {
                            uint32 text = urand(0, 3);
                            switch (text)
                            {
                                case 1:
                                    sCreatureTextMgr->SendChat(alex, text);
                                    _events.ScheduleEvent(EVENT_BRIDGE_WORKERS_RESPONSE, 1s, 0, PHASE_IDLE);
                                    break;
                                case 2:
                                    sCreatureTextMgr->SendChat(alex, text);
                                    _events.ScheduleEvent(EVENT_BRIDGE_WORKER_ALEX_HARDER, 3s, 0, PHASE_IDLE);
                                    break;
                                default:
                                    sCreatureTextMgr->SendChat(alex, text);
                                    _events.Repeat(10s);
                                    break;
                            }
                        }
                        break;
                    case EVENT_BRIDGE_WORKER_ALEX_HARDER:
                        if (Creature* alex = GetAlex())
                        {
                            sCreatureTextMgr->SendChat(alex, 4);
                            _events.ScheduleEvent(EVENT_BRIDGE_WORKER_MATTHEW_HARDER_RESPONSE, 4s, 0, PHASE_IDLE);
                        }
                        break;
                    case EVENT_BRIDGE_WORKER_MATTHEW_HARDER_RESPONSE:
                        if (Creature* matthew = GetMatthew())
                            sCreatureTextMgr->SendChat(matthew, 1, _foremanGUID);
                        _events.ScheduleEvent(EVENT_BRIDGE_WORKER_ALEX_IDLE_TALK, 20s, 30s, 0, PHASE_IDLE);
                        break;
                    case EVENT_BRIDGE_WORKERS_RESPONSE:
                        if (Creature* matthew = GetMatthew())
                            sCreatureTextMgr->SendChat(matthew, 0, _foremanGUID);
                        if (Creature* trent = GetTrent())
                            sCreatureTextMgr->SendChat(trent, 0, _foremanGUID);
                        if (Creature* dmitri = GetDmitri())
                            sCreatureTextMgr->SendChat(dmitri, 0, _foremanGUID);
                        if (Creature* jess = GetJess())
                            sCreatureTextMgr->SendChat(jess, 0, _foremanGUID);
                        if (Creature* daniel = GetDaniel())
                            sCreatureTextMgr->SendChat(daniel, 0, _foremanGUID);
                        _events.ScheduleEvent(EVENT_BRIDGE_WORKER_ALEX_IDLE_TALK, 20s, 30s, 0, PHASE_IDLE);
                        break;
                    case EVENT_REPOSITION:
                        if (GetEttin())
                            GetEttin()->GetMotionMaster()->MovePoint(0, me->GetPosition());
                        if (GetTrent())
                            GetTrent()->GetMotionMaster()->MovePoint(0, -9281.44, -2285.27, 67.5123);
                        if (GetDmitri())
                            GetDmitri()->GetMotionMaster()->MovePoint(0, -9282.8, -2293.28, 67.5089);
                        if (GetJess())
                            GetJess()->GetMotionMaster()->MovePoint(0, -9282.27, -2290.95, 67.5319);
                        if (GetDaniel())
                            GetDaniel()->GetMotionMaster()->MovePoint(0, -9281.77, -2287.55, 67.5869);
                        if (GetMatthew())
                            GetMatthew()->GetMotionMaster()->MovePoint(0, -9280.71, -2283.21, 67.5747);
                        if (GetAlex())
                            GetAlex()->GetMotionMaster()->MovePoint(0, -9279.86, -2281.42, 67.5854);
                        _events.ScheduleEvent(EVENT_LIFT_BOULDER, 200ms, 0, PHASE_REMOVE_ROCK);
                        break;
                    case EVENT_LIFT_BOULDER:
                        if (GetEttin())
                        {
                            if (!GetEttin()->IsStopped() ||
                                !GetTrent()   || !GetTrent()->IsStopped()   ||
                                !GetDmitri()  || !GetDmitri()->IsStopped()  ||
                                !GetJess()    || !GetJess()->IsStopped()    ||
                                !GetDaniel()  || !GetDaniel()->IsStopped()  ||
                                !GetMatthew() || !GetMatthew()->IsStopped() ||
                                !GetAlex()    || !GetAlex()->IsStopped())
                            {
                                _events.Repeat(200ms);
                            }
                            else
                            {
                                GetTrent()->SetFacingTo(GetEttin());
                                GetDmitri()->SetFacingTo(GetEttin());
                                GetJess()->SetFacingTo(GetEttin());
                                GetDaniel()->SetFacingTo(GetEttin());
                                GetMatthew()->SetFacingTo(GetEttin());
                                GetAlex()->SetFacingTo(GetEttin());
                                me->CastSpell(GetEttin(), SPELL_LIFT_HUGE_BOULDER, false);
                                _events.ScheduleEvent(EVENT_DEBUG_RESET, 5s, 0, PHASE_REMOVE_ROCK);
                            }
                        }
                        break;
                    case EVENT_DEBUG_RESET:
                        if (GetEttin())
                        {
                            GetEttin()->CastSpell(me, SPELL_EJECT_PASSENGER_1, false);
                            GetEttin()->DisappearAndDie();
                        }
                        _events.ScheduleEvent(EVENT_START_PHASE_IDLE, 1s, 0, PHASE_REMOVE_ROCK);
                        break;
                }
            }
        }

        bool LocateGUIDs()
        {
            if (Creature* foreman = me->FindNearestCreature(NPC_FOREMAN_OSLOW, 10.0f))
                if (Creature* alex = me->FindNearestCreature(NPC_BRIDGE_WORKER_ALEX, 10.0f))
                    if (Creature* matthew = me->FindNearestCreature(NPC_BRIDGE_WORKER_MATTHEW, 10.0f))
                        if (Creature* trent = me->FindNearestCreature(NPC_BRIDGE_WORKER_TRENT, 10.0f))
                            if (Creature* dmitri = me->FindNearestCreature(NPC_BRIDGE_WORKER_DMITRI, 10.0f))
                                if (Creature* jess = me->FindNearestCreature(NPC_BRIDGE_WORKER_JESS, 10.0f))
                                    if (Creature* daniel = me->FindNearestCreature(NPC_BRIDGE_WORKER_DANIEL, 10.0f))
                                    {
                                        _foremanGUID = foreman->GetGUID();
                                        _alexGUID = alex->GetGUID();
                                        _matthewGUID = matthew->GetGUID();
                                        _trentGUID = trent->GetGUID();
                                        _dmitriGUID = dmitri->GetGUID();
                                        _jessGUID = jess->GetGUID();
                                        _danielGUID = daniel->GetGUID();
                                        return true;
                                    }

            return false;
        }

        Creature* GetForeman()
        {
            return me->GetMap()->GetCreature(_foremanGUID);
        }

        Creature* GetAlex()
        {
            return me->GetMap()->GetCreature(_alexGUID);
        }

        Creature* GetMatthew()
        {
            return me->GetMap()->GetCreature(_matthewGUID);
        }

        Creature* GetTrent()
        {
            return me->GetMap()->GetCreature(_trentGUID);
        }

        Creature* GetDmitri()
        {
            return me->GetMap()->GetCreature(_dmitriGUID);
        }

        Creature* GetJess()
        {
            return me->GetMap()->GetCreature(_jessGUID);
        }

        Creature* GetDaniel()
        {
            return me->GetMap()->GetCreature(_danielGUID);
        }

        Creature* GetEttin()
        {
            return me->GetMap()->GetCreature(_ettinGUID);
        }

    private:
        EventMap _events;
        ObjectGuid _foremanGUID;
        ObjectGuid _alexGUID;
        ObjectGuid _matthewGUID;
        ObjectGuid _trentGUID;
        ObjectGuid _dmitriGUID;
        ObjectGuid _jessGUID;
        ObjectGuid _danielGUID;
        ObjectGuid _ettinGUID;
    };
};

void AddSC_redridge_mountains()
{
    RegisterSpellScript(spell_redridge_control_ettin);
    RegisterSpellScript(spell_redridge_lift_huge_boulder);
    new npc_redridge_huge_boulder();
}
