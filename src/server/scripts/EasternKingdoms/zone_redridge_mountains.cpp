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
    SPELL_CONTROL_ETTIN_2 = 80702
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
    EVENT_BRIDGE_WORKERS_RESPONSE = 6
};

enum HugeBoulderPhases
{
    PHASE_IDLE = 0
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

    private:
        EventMap _events;
        ObjectGuid _foremanGUID;
        ObjectGuid _alexGUID;
        ObjectGuid _matthewGUID;
        ObjectGuid _trentGUID;
        ObjectGuid _dmitriGUID;
        ObjectGuid _jessGUID;
        ObjectGuid _danielGUID;
    };
};

void AddSC_redridge_mountains()
{
    RegisterSpellScript(spell_redridge_control_ettin);
    RegisterSpellScript(spell_redridge_lift_huge_boulder);
    new npc_redridge_huge_boulder();
}
