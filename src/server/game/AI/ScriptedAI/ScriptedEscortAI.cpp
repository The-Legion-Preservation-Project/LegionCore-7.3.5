/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

 /* ScriptData
 SDName: Npc_EscortAI
 SD%Complete: 100
 SDComment:
 SDCategory: Npc
 EndScriptData */

#include "CreatureGroups.h"
#include "Group.h"
#include "ScriptedCreature.h"
#include "ScriptedEscortAI.h"
#include "ScriptSystem.h"
#include "WaypointDefines.h"
#include "WaypointMovementGenerator.h"

enum ePoints
{
    POINT_LAST_POINT = 0xFFFFFF,
    POINT_HOME = 0xFFFFFE
};

npc_escortAI::npc_escortAI(Creature* creature) : ScriptedAI(creature), m_uifollowerGUID(ObjectGuid::Empty), m_uiPlayerGUID(ObjectGuid::Empty), m_uiWPWaitTimer(1000), m_uiPlayerCheckTimer(0),
m_uiEscortState(STATE_ESCORT_NONE), MaxPlayerDistance(DEFAULT_MAX_PLAYER_DISTANCE), LastWP(0), m_pQuestForEscort(nullptr), m_bIsActiveAttacker(true), m_bIsRunning(false), m_bCanInstantRespawn(false),
m_bCanReturnToStart(false), DespawnAtEnd(true), DespawnAtFar(true), ScriptWP(false), HasImmuneToNPCFlags(false), GeneratePath(true), _started(false), _ended(false)
{ }

Player* npc_escortAI::GetPlayerForEscort()
{
    if (Unit* unit = Unit::GetUnit(*me, m_uiPlayerGUID))
        return unit->ToPlayer();
    return nullptr;
}

//see followerAI
bool npc_escortAI::AssistPlayerInCombat(Unit* who)
{
    if (!who || !who->getVictim())
        return false;

    //experimental (unknown) flag not present
    if (!(me->GetCreatureTemplate()->TypeFlags[0] & CREATURE_TYPEFLAGS_CAN_ASSIST))
        return false;

    //not a player
    if (!who->getVictim()->GetCharmerOrOwnerPlayerOrPlayerItself())
        return false;

    //never attack friendly
    if (me->IsFriendlyTo(who))
        return false;

    //too far away and no free sight?
    if (me->IsWithinDistInMap(who, GetMaxPlayerDistance()) && me->IsWithinLOSInMap(who))
    {
        //already fighting someone?
        if (!me->getVictim())
        {
            AttackStart(who);
            return true;
        }
        who->SetInCombatWith(me);
        me->AddThreat(who, 0.0f);
        return true;
    }

    return false;
}

void npc_escortAI::MoveInLineOfSight(Unit* who)
{
    if (me->getVictim())
        return;

    if (me->HasReactState(REACT_AGGRESSIVE) && !me->HasUnitState(UNIT_STATE_STUNNED) && who->isTargetableForAttack() && who->isInAccessiblePlaceFor(me))
        if (HasEscortState(STATE_ESCORT_ESCORTING) && AssistPlayerInCombat(who))
            return;

    if (me->canStartAttack(who, false))
        AttackStart(who);
}

void npc_escortAI::JustDied(Unit* /*killer*/)
{
    if (!HasEscortState(STATE_ESCORT_ESCORTING) || m_uiPlayerGUID.IsEmpty() || !m_pQuestForEscort)
        return;

    if (Player* player = GetPlayerForEscort())
    {
        if (Group* group = player->GetGroup())
        {
            for (GroupReference* groupRef = group->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
                if (Player* member = groupRef->getSource())
                    if (member->GetQuestStatus(m_pQuestForEscort->GetQuestId()) == QUEST_STATUS_INCOMPLETE)
                        member->FailQuest(m_pQuestForEscort->GetQuestId());
        }
        else
        {
            if (player->GetQuestStatus(m_pQuestForEscort->GetQuestId()) == QUEST_STATUS_INCOMPLETE)
                player->FailQuest(m_pQuestForEscort->GetQuestId());
        }
    }
}

void npc_escortAI::JustRespawned()
{
    RemoveEscortState(STATE_ESCORT_ESCORTING | STATE_ESCORT_RETURNING | STATE_ESCORT_PAUSED);

    if (!IsCombatMovementAllowed())
        SetCombatMovement(true);

    //add a small delay before going to first waypoint, normal in near all cases
    m_uiWPWaitTimer = 1000;

    if (me->getFaction() != me->GetCreatureTemplate()->faction)
        me->RestoreFaction();

    Reset();
}

void npc_escortAI::ReturnToLastPoint()
{
    me->SetWalk(false);
    float x, y, z, o;
    me->GetHomePosition(x, y, z, o);
    MovePoint(POINT_LAST_POINT, x, y, z);
}

void npc_escortAI::MovePoint(uint32 point, float x, float y, float z)
{
    me->GetMotionMaster()->MovePoint(point, x, y, z, GeneratePath);

    //Call for creature group update
    if (me->GetFormation() && me->GetFormation()->getLeader() == me)
        me->GetFormation()->LeaderMoveTo(x, y, z);

    //ToDo: find fast way for find behind point in 3-5 range
    if (Creature* follower = Unit::GetCreature(*me, m_uifollowerGUID))
        follower->GetMotionMaster()->MovePoint(point, x, y, z);
}

void npc_escortAI::EnterEvadeMode()
{
    me->RemoveAllAurasExceptType(SPELL_AURA_CONTROL_VEHICLE);
    me->DeleteThreatList();
    me->ClearSaveThreatTarget();
    me->CombatStop(true);
    me->SetLootRecipient(nullptr);

    if (HasEscortState(STATE_ESCORT_ESCORTING))
    {
        AddEscortState(STATE_ESCORT_RETURNING);
        ReturnToLastPoint();
        TC_LOG_DEBUG("scripts", "EscortAI has left combat and is now returning to last point");
    }
    else
    {
        me->GetMotionMaster()->MoveTargetedHome();
        if (Creature* follower = Unit::GetCreature(*me, m_uifollowerGUID))
            follower->GetMotionMaster()->MoveTargetedHome();
        if (HasImmuneToNPCFlags)
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC);
        Reset();
    }
}

bool npc_escortAI::IsPlayerOrGroupInRange()
{
    if (Player* player = GetPlayerForEscort())
    {
        if (Group* group = player->GetGroup())
        {
            for (GroupReference* groupRef = group->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
                if (Player* member = groupRef->getSource())
                    if (me->IsWithinDistInMap(member, GetMaxPlayerDistance()))
                        return true;
        }
        else if (me->IsWithinDistInMap(player, GetMaxPlayerDistance()))
            return true;
    }

    return false;
}

void npc_escortAI::UpdateAI(uint32 diff)
{
    if (HasEscortState(STATE_ESCORT_ESCORTING) && !me->getVictim() && m_uiWPWaitTimer && !HasEscortState(STATE_ESCORT_RETURNING))
    {
        if (m_uiWPWaitTimer <= diff)
        {
            if (!HasEscortState(STATE_ESCORT_PAUSED))
            {
                m_uiWPWaitTimer = 0;

                if (_ended)
                {
                    me->StopMoving();
                    me->GetMotionMaster()->Clear(false);
                    me->GetMotionMaster()->MoveIdle();

                    _ended = false;

                    LastWaypointReached();

                    if (DespawnAtEnd)
                    {
                        TC_LOG_DEBUG("scripts", "EscortAI reached end of waypoints");

                        if (m_bCanReturnToStart)
                        {
                            float fRetX, fRetY, fRetZ;
                            me->GetRespawnPosition(fRetX, fRetY, fRetZ);

                            me->GetMotionMaster()->MovePoint(POINT_HOME, fRetX, fRetY, fRetZ);

                            TC_LOG_DEBUG("scripts", "EscortAI are returning home to spawn location: %u, %f, %f, %f", POINT_HOME, fRetX, fRetY, fRetZ);
                        }
                        else if (m_bCanInstantRespawn)
                        {
                            me->setDeathState(JUST_DIED);
                            me->Respawn();

                            if (Creature* follower = Unit::GetCreature(*me, m_uifollowerGUID))
                            {
                                follower->setDeathState(JUST_DIED);
                                follower->Respawn();
                            }
                        }
                        else
                        {
                            me->DespawnOrUnsummon();

                            if (Creature* follower = Unit::GetCreature(*me, m_uifollowerGUID))
                                follower->DespawnOrUnsummon();
                        }
                    }
                    else
                        TC_LOG_DEBUG("scripts", "EscortAI reached end of waypoints with Despawn off");

                    RemoveEscortState(STATE_ESCORT_ESCORTING);
                    return;
                }

                if (!_started)
                {
                    _started = true;
                    me->GetMotionMaster()->MovePath(_path, false);
                }
                else if (auto move = dynamic_cast<WaypointMovementGenerator<Creature>*>(me->GetMotionMaster()->top()))
                    WaypointStart(move->GetCurrentNode());

            }
        }
        else
            m_uiWPWaitTimer -= diff;
    }

    //Check if player or any member of his group is within range
    if (HasEscortState(STATE_ESCORT_ESCORTING) && !m_uiPlayerGUID.IsEmpty() && !me->getVictim() && !HasEscortState(STATE_ESCORT_RETURNING))
    {
        m_uiPlayerCheckTimer += diff;
        if (m_uiPlayerCheckTimer > 1000)
        {
            if (DespawnAtFar && !IsPlayerOrGroupInRange())
            {
                if (m_bCanInstantRespawn)
                {
                    me->setDeathState(JUST_DIED);
                    me->Respawn();
                }
                else
                    me->DespawnOrUnsummon();

                return;
            }

            m_uiPlayerCheckTimer = 0;
        }
    }

    UpdateEscortAI(diff);
}

void npc_escortAI::UpdateEscortAI(uint32 const /*diff*/)
{
    if (!UpdateVictim())
        return;

    DoMeleeAttackIfReady();
}

void npc_escortAI::MovementInform(uint32 moveType, uint32 pointId)
{
    if (moveType != POINT_MOTION_TYPE || !HasEscortState(STATE_ESCORT_ESCORTING))
        return;

    // no action allowed if there is no escort
    if (!HasEscortState(STATE_ESCORT_ESCORTING))
        return;

    if (moveType == POINT_MOTION_TYPE)
    {
        if (!m_uiWPWaitTimer)
            m_uiWPWaitTimer = 1;

        if (pointId == POINT_LAST_POINT)
        {
            TC_LOG_DEBUG("scripts", "EscortAI has returned to original position before combat");

            me->SetWalk(!m_bIsRunning);
            RemoveEscortState(STATE_ESCORT_RETURNING);
        }
        else if (pointId == POINT_HOME)
        {
            TC_LOG_DEBUG("scripts", "EscortAI has returned to original home location and will continue from beginning of waypoint list.");

            _started = false;
        }
    }
    else if (moveType == WAYPOINT_MOTION_TYPE)
    {
        WaypointReached(pointId);

        // End of the line
        if (LastWP && LastWP == pointId)
        {
            LastWP = 0;

            _started = false;
            _ended = true;

            m_uiWPWaitTimer = 50;

            return;
        }

        TC_LOG_DEBUG("scripts", "EscortAI Waypoint %u reached", pointId);

        auto move = dynamic_cast<WaypointMovementGenerator<Creature>*>(me->GetMotionMaster()->top());

        if (move)
            m_uiWPWaitTimer = move->GetTrackerTimer().GetExpiry();

        // Call WP start function
        if (!m_uiWPWaitTimer && !HasEscortState(STATE_ESCORT_PAUSED) && move)
            WaypointReached(move->GetCurrentNode());

        if (m_bIsRunning)
            me->SetWalk(false);
        else
            me->SetWalk(true);
    }
}

/*
void npc_escortAI::OnPossess(bool apply)
{
    // We got possessed in the middle of being escorted, store the point
    // where we left off to come back to when possess is removed
    if (HasEscortState(STATE_ESCORT_ESCORTING))
    {
        if (apply)
            me->GetPosition(LastPos.x, LastPos.y, LastPos.z);
        else
        {
            Returning = true;
            me->GetMotionMaster()->MovementExpired();
            me->GetMotionMaster()->MovePoint(WP_LAST_POINT, LastPos.x, LastPos.y, LastPos.z);
        }
    }
}
*/

void npc_escortAI::AddWaypoint(uint32 id, float x, float y, float z, uint32 waitTime)
{
    Trinity::NormalizeMapCoord(x);
    Trinity::NormalizeMapCoord(y);

    WaypointNode wp;

    wp.id = id;
    wp.x = x;
    wp.y = y;
    wp.z = z;
    wp.orientation = 0.f;
    wp.moveType = m_bIsRunning ? WAYPOINT_MOVE_TYPE_RUN : WAYPOINT_MOVE_TYPE_WALK;
    wp.delay = waitTime;
    wp.eventId = 0;
    wp.eventChance = 100;

    _path.nodes.push_back(std::move(wp));

    // i think SD2 no longer uses this function
    ScriptWP = true;
    /*PointMovement wp;
    wp.m_uiCreatureEntry = me->GetEntry();
    wp.m_uiPointId = id;
    wp.m_fX = x;
    wp.m_fY = y;
    wp.m_fZ = z;
    wp.m_uiWaitTime = WaitTimeMs;
    PointMovementMap[wp.m_uiCreatureEntry].push_back(wp);*/
}

void npc_escortAI::FillPointMovementListForCreature()
{
    ScriptPointVector const& movePoints = sScriptSystemMgr->GetPointMoveList(me->GetEntry());
    if (movePoints.empty())
        return;

    LastWP = movePoints.back().uiPointId;

    for (const ScriptPointMove &point : movePoints)
    {
        WaypointNode wp;

        float x = point.fX;
        float y = point.fY;
        float z = point.fZ;

        Trinity::NormalizeMapCoord(x);
        Trinity::NormalizeMapCoord(y);

        wp.id = point.uiPointId;
        wp.x = x;
        wp.y = y;
        wp.z = z;
        wp.orientation = 0.f;
        wp.moveType = m_bIsRunning ? WAYPOINT_MOVE_TYPE_RUN : WAYPOINT_MOVE_TYPE_WALK;
        wp.delay = point.uiWaitTime;
        wp.eventId = 0;
        wp.eventChance = 100;

        _path.nodes.push_back(std::move(wp));
    }
}

void npc_escortAI::SetRun(bool on)
{
    if (on)
    {
        if (!m_bIsRunning)
            me->SetWalk(false);
        else
            TC_LOG_DEBUG("scripts", "EscortAI attempt to set run mode, but is already running.");
    }
    else
    {
        if (m_bIsRunning)
            me->SetWalk(true);
        else
            TC_LOG_DEBUG("scripts", "EscortAI attempt to set walk mode, but is already walking.");
    }

    m_bIsRunning = on;
}

//TODO: get rid of this many variables passed in function.
void npc_escortAI::Start(bool isActiveAttacker /* = true*/, bool run /* = false */, ObjectGuid playerGUID /* = 0 */, Quest const* quest /* = NULL */, bool instantRespawn /* = false */, bool canLoopPath /* = false */, bool resetWaypoints /* = true */)
{
    if (me->getVictim())
    {
        TC_LOG_ERROR("misc", "TSCR ERROR: EscortAI (script: %s, creature entry: %u) attempts to Start while in combat", me->GetScriptName().c_str(), me->GetEntry());
        return;
    }

    if (HasEscortState(STATE_ESCORT_ESCORTING))
    {
        TC_LOG_ERROR("scripts", "EscortAI (script: %s, creature entry: %u) attempts to Start while already escorting", me->GetScriptName().c_str(), me->GetEntry());
        return;
    }

    //set variables
    m_bIsActiveAttacker = isActiveAttacker;
    m_bIsRunning = run;

    m_uiPlayerGUID = playerGUID;
    m_pQuestForEscort = quest;

    m_bCanInstantRespawn = instantRespawn;
    m_bCanReturnToStart = canLoopPath;

    if (!ScriptWP && resetWaypoints) // sd2 never adds wp in script, but tc does
        FillPointMovementListForCreature();

    if (m_bCanReturnToStart && m_bCanInstantRespawn)
        TC_LOG_DEBUG("scripts", "EscortAI is set to return home after waypoint end and instant respawn at waypoint end. Creature will never despawn.");

    if (me->GetMotionMaster()->GetCurrentMovementGeneratorType() == WAYPOINT_MOTION_TYPE)
    {
        me->StopMoving();
        me->GetMotionMaster()->Clear(false);
        me->GetMotionMaster()->MoveIdle();
        TC_LOG_DEBUG("scripts", "EscortAI start with WAYPOINT_MOTION_TYPE, changed to MoveIdle.");
    }

    //disable npcflags
    me->SetUInt32Value(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_NONE);
    if (me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC))
    {
        HasImmuneToNPCFlags = true;
        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC);
    }

    TC_LOG_DEBUG("scripts", "EscortAI started. ActiveAttacker = %d, Run = %d, PlayerGUID = %s", uint32(m_bIsActiveAttacker), uint32(m_bIsRunning), m_uiPlayerGUID.ToString().c_str());

    // Set initial speed
    if (m_bIsRunning)
        me->SetWalk(false);
    else
        me->SetWalk(true);

    _started = false;

    AddEscortState(STATE_ESCORT_ESCORTING);
}

void npc_escortAI::SetEscortPaused(bool on)
{
    if (!HasEscortState(STATE_ESCORT_ESCORTING))
        return;

    if (on)
    {
        AddEscortState(STATE_ESCORT_PAUSED);
        me->StopMoving();
    }
    else
    {
        RemoveEscortState(STATE_ESCORT_PAUSED);
        if (auto move = dynamic_cast<WaypointMovementGenerator<Creature>*>(me->GetMotionMaster()->top()))
            move->GetTrackerTimer().Reset(1);
    }
}
