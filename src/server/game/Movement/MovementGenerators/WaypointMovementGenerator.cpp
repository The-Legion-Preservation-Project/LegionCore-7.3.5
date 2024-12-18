/*
 * Copyright (C) 2008-2015 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
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

#include "WaypointMovementGenerator.h"
#include "Creature.h"
#include "CreatureAI.h"
#include "CreatureGroups.h"
#include "MapManager.h"
#include "MoveSpline.h"
#include "MoveSplineInit.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "PlayerDefines.h"
#include "ScriptsData.h"
#include "SpellScript.h"
#include "Transport.h"

void WaypointMovementGenerator<Creature>::LoadPath(Creature& creature)
{
    if (_loadedFromDB)
    {
        if (!_pathId)
        {
            _pathId = creature.GetWaypointPath();
            _path = sWaypointMgr->GetPath(_pathId);
        }
        else
            _path = sWaypointMgr->GetPathScript(_pathId);

        if (creature.GetEntry() == 120132 || creature.GetEntry() == 120131) // hack
            _forceTPToStart = true;
    }

    if (!_path)
    {
        // No movement found for entry
        TC_LOG_WARN("sql.sql", "WaypointMovementGenerator::LoadPath: creature %s (Entry: %u GUID: %u) doesn't have waypoint path id: %u", creature.GetName(), creature.GetEntry(), creature.GetGUIDLow(), _pathId);
        return;
    }

    _nextMoveTime.Reset(3000);

    if (CanMove(creature))
        StartMoveNow(creature);
}

void WaypointMovementGenerator<Creature>::DoInitialize(Creature& creature)
{
    LoadPath(creature);
}

void WaypointMovementGenerator<Creature>::DoFinalize(Creature& creature)
{
    creature.ClearUnitState(UNIT_STATE_ROAMING | UNIT_STATE_ROAMING_MOVE);
    creature.SetWalk(false);
}

void WaypointMovementGenerator<Creature>::DoReset(Creature& creature)
{
    if (CanMove(creature))
        StartMoveNow(creature);
}

void WaypointMovementGenerator<Creature>::OnArrived(Creature& creature)
{
    if (!_path || _path->empty())
        return;

    WaypointData const &waypoint = _path->at(_currentNode);

    // For WP from sniff
    if (waypoint.delay_chance && roll_chance_i(waypoint.delay_chance))
    {
        creature.ClearUnitState(UNIT_STATE_ROAMING_MOVE);
        _nextMoveTime.Reset(urand(1000, 10000));
    }
    else if (waypoint.delay)
    {
        creature.ClearUnitState(UNIT_STATE_ROAMING_MOVE);
        _nextMoveTime.Reset(waypoint.delay);
    }

    if (waypoint.event_id && roll_chance_i(waypoint.event_chance))
    {
        TC_LOG_DEBUG("maps.scripts", "Creature movement start script %u at point %u for %s.", waypoint.event_id, _currentNode, creature.GetGUID().ToString().c_str());
        creature.ClearUnitState(UNIT_STATE_ROAMING_MOVE);
        creature.GetMap()->ScriptsStart(sWaypointScripts, waypoint.event_id, &creature, nullptr);
    }

    // Inform script
    MovementInform(creature);
    creature.UpdateWaypointID(_currentNode);
}

bool WaypointMovementGenerator<Creature>::StartMove(Creature& creature)
{
    if (!creature.IsAlive())
        return false;

    if (!_path || _path->empty())
        return false;

    bool transportPath = creature.HasUnitMovementFlag(MOVEMENTFLAG_ONTRANSPORT) && !creature.GetTransGUID().IsEmpty();

    bool waitingInLast = false;
    if (_isArrivalDone)
    {
        if ((_currentNode == _path->size() - 1) && !_repeating) // If that's our last waypoint
        {
            WaypointData const &waypoint = _path->at(_currentNode);

            float x = waypoint.x + _randomMoveX;
            float y = waypoint.y + _randomMoveY;
            float z = waypoint.z;
            float o = creature.GetOrientation();

            if (!transportPath)
                creature.SetHomePosition(x, y, z, o);
            else
            {
                if (Transport* trans = creature.GetTransport())
                {
                    o -= trans->GetOrientation();
                    creature.SetTransportHomePosition(x, y, z, o);
                    trans->CalculatePassengerPosition(x, y, z, &o);
                    creature.SetHomePosition(x, y, z, o);
                }
                else
                    transportPath = false;
                // else if (vehicle) - this should never happen, vehicle offsets are const
            }

            //creature.GetMotionMaster()->Initialize();
            return false;
        }

//        if (_currentNode == 0 && _goBack)
//        {
//            _goBack = false;
//            waitingInLast = true;
//        }
//
//        if (_goBack)
//            _currentNode = (_currentNode-1) % _path->size();
//        else
//            _currentNode = (_currentNode+1) % _path->size();
//
//        // Revers go
//        if ((_currentNode == _path->size() - 1) && !_goBack)
//        {
//            if (!_forceTPToStart)
//                _goBack = true;
//            else
//            {
//                _currentNode = 0;
//                WaypointData const* node = _path->at(_currentNode);
//                creature.NearTeleportTo(node->x, node->y, node->z, 0.0f);
//                return true;
//            }
//        }

        _currentNode = (_currentNode + 1) % _path->size();
    }

    WaypointData const &waypoint = _path->at(_currentNode);
    Position formationDest(waypoint.x, waypoint.y, waypoint.z, (waypoint.orientation && waypoint.delay) ? waypoint.orientation : 0.0f);

    _isArrivalDone = false;
    _recalculateSpeed = false;

    creature.AddUnitState(UNIT_STATE_ROAMING_MOVE);

    Movement::MoveSplineInit init(creature);

    //! If creature is on transport, we assume waypoints set in DB are already transport offsets
    if (transportPath)
    {
        init.DisableTransportPathTransformations();
        if (TransportBase* trans = creature.GetDirectTransport())
        {
            float orientation = formationDest.GetOrientation();
            trans->CalculatePassengerPosition(formationDest.m_positionX, formationDest.m_positionY, formationDest.m_positionZ, &orientation);
            formationDest.SetOrientation(orientation);
        }
    }

    //! Do not use formationDest here, MoveTo requires transport offsets due to DisableTransportPathTransformations() call
    //! but formationDest contains global coordinates
    init.MoveTo(waypoint.x + _randomMoveX, waypoint.y + _randomMoveY, waypoint.z);

    //! Accepts angles such as 0.00001 and -0.00001, 0 must be ignored, default value in waypoint table
    if (waypoint.orientation && waypoint.delay)
        init.SetFacing(waypoint.orientation);

    // TODO: is this needed?
    if (waypoint.speed > 0)
        init.SetVelocity(waypoint.speed);


    switch (waypoint.move_type)
    {
        case WAYPOINT_MOVE_TYPE_LAND:
            init.SetAnimation(AnimTier::Ground);
            break;
        case WAYPOINT_MOVE_TYPE_TAKEOFF:
            init.SetAnimation(AnimTier::Hover);
            break;
        case WAYPOINT_MOVE_TYPE_RUN:
            init.SetWalk(false);
            break;
        case WAYPOINT_MOVE_TYPE_WALK:
            init.SetWalk(true);
            break;
    }

    init.Launch();

    //Call for creature group update
    if (creature.GetFormation() && creature.GetFormation()->getLeader() == &creature)
        //creature.GetFormation()->LeaderMoveTo(formationDest, waypoint.id, waypoint.move_type, (waypoint.orientation && waypoint.delay) ? true : false);
        creature.GetFormation()->LeaderMoveTo(formationDest.m_positionX, formationDest.m_positionY, formationDest.m_positionZ);

    return true;
}

bool WaypointMovementGenerator<Creature>::DoUpdate(Creature& creature, uint32 diff)
{
    if (!creature.IsAlive())
        return false;

    // Creature's movement has been paused.
    if (_pauseTime > 0)
        _pauseTime -= diff;

    if (_stalled || _pauseTime > 0 || creature.HasUnitState(UNIT_STATE_NOT_MOVE) || creature.IsMovementPreventedByCasting())
    {
        creature.StopMoving();
        return true;
    }

    // prevent a crash at empty waypoint path.
    if (!_path || _path->empty())
        return false;

    if (!_nextMoveTime.Passed())
    {
        _nextMoveTime.Update(diff);
        if (_nextMoveTime.Passed())
            return StartMoveNow(creature);
    }
    else
    {
        // Set home position at place on waypoint movement.
        if (!creature.HasUnitMovementFlag(MOVEMENTFLAG_ONTRANSPORT) || creature.GetTransGUID().IsEmpty())
            creature.SetHomePosition(creature.GetPosition());

        if (creature.IsStopped())
            _nextMoveTime.Reset(sWorld->getIntConfig(CONFIG_CREATURE_STOP_FOR_PLAYER));
        else if (creature.movespline->Finalized())
        {
            OnArrived(creature);

            _isArrivalDone = true;

            if (_nextMoveTime.Passed())
            {
                if (creature.IsStopped())
                    _nextMoveTime.Reset(sWorld->getIntConfig(CONFIG_CREATURE_STOP_FOR_PLAYER));
                else
                    return StartMove(creature);
            }
        }
        else if (_recalculateSpeed)
        {
            if (_nextMoveTime.Passed())
                StartMove(creature);
        }
    }

    return true;
 }

void WaypointMovementGenerator<Creature>::MovementInform(Creature &creature)
{
    if (creature.AI())
    {
        creature.AI()->MovementInform(WAYPOINT_MOTION_TYPE, _currentNode);

        // TODO: is this needed?
        if (_currentNode == _path->size() - 1)
            creature.AI()->LastWPReached();
    }
}

bool WaypointMovementGenerator<Creature>::GetResetPosition(Unit&, float& x, float& y, float& z)
{
    // prevent a crash at empty waypoint path.
    if (!_path || _path->empty())
        return false;

    WaypointData const &waypoint = _path->at(_currentNode);

    x = waypoint.x;
    y = waypoint.y;
    z = waypoint.z;
    return true;
}

void WaypointMovementGenerator<Creature>::Pause(uint32 timer /*= 0*/)
{
    _stalled = timer ? false : true;
    _pauseTime = timer;
}

void WaypointMovementGenerator<Creature>::Resume(uint32 overrideTimer /*= 0*/)
{
    _stalled = false;
    _nextMoveTime.Reset(1);
    _pauseTime = overrideTimer;
}

bool WaypointMovementGenerator<Creature>::CanMove(Creature& creature)
{
    return _nextMoveTime.Passed() && !creature.HasUnitState(UNIT_STATE_NOT_MOVE) && !creature.IsMovementPreventedByCasting();
}

//----------------------------------------------------//

void WaypointMovementGenerator<Player>::LoadPath(Player& player)
{
    _path = sWaypointMgr->GetPathScript(_pathId);

    if (!_path)
    {
        // No movement found for entry
        TC_LOG_WARN("sql.sql", "WaypointMovementGenerator::LoadPath: player %s (Entry: %u GUID: %u) doesn't have waypoint path id: %u", player.GetName(), player.GetEntry(), player.GetGUIDLow(), _pathId);
        return;
    }

    StartMoveNow(player);
}

void WaypointMovementGenerator<Player>::DoInitialize(Player& player)
{
    LoadPath(player);
    player.AddUnitState(UNIT_STATE_ROAMING | UNIT_STATE_ROAMING_MOVE);
}

void WaypointMovementGenerator<Player>::DoFinalize(Player& player)
{
    player.ClearUnitState(UNIT_STATE_ROAMING | UNIT_STATE_ROAMING_MOVE);
    player.SetWalk(false);
}

void WaypointMovementGenerator<Player>::DoReset(Player &player)
{
    player.AddUnitState(UNIT_STATE_ROAMING | UNIT_STATE_ROAMING_MOVE);
    StartMoveNow(player);
}

void WaypointMovementGenerator<Player>::OnArrived(Player& player)
{
    if (!_path || _path->empty() || _currentNode >= _path->size() || _isArrivalDone)
        return;

    _isArrivalDone = true;
    WaypointData const &waypoint = _path->at(_currentNode);

    if (waypoint.event_id && roll_chance_i(waypoint.event_chance))
    {
        TC_LOG_DEBUG("maps.script", "Player movement start script %u at point %u for %lu.", waypoint.event_id, _currentNode, player.GetGUID().GetCounter());
        player.ClearUnitState(UNIT_STATE_ROAMING_MOVE);
        player.GetMap()->ScriptsStart(sWaypointScripts, waypoint.event_id, &player, nullptr);
    }
}

bool WaypointMovementGenerator<Player>::StartMove(Player &player)
{
    if (!_path || _path->empty())
        return false;

    if (Stopped())
        return true;

    if (_isArrivalDone)
    {
        if (_currentNode == _path->size() - 1) // If that's our last waypoint
        {
            player.GetMotionMaster()->Initialize();
            return false;
        }

        _currentNode = (_currentNode+1) % _path->size();
    }

    if (_currentNode >= _path->size())
        return false;

    WaypointData const &waypoint = _path->at(_currentNode);

    _isArrivalDone = false;

    player.AddUnitState(UNIT_STATE_ROAMING_MOVE);

    Movement::Location formationDest(waypoint.x, waypoint.y, waypoint.z, 0.0f);
    Movement::MoveSplineInit init(player);

    //! Do not use formationDest here, MoveTo requires transport offsets due to DisableTransportPathTransformations() call
    //! but formationDest contains global coordinates

    init.MoveTo(waypoint.x, waypoint.y, waypoint.z);

    //! Accepts angles such as 0.00001 and -0.00001, 0 must be ignored, default value in waypoint table
    if (waypoint.orientation && waypoint.delay)
        init.SetFacing(waypoint.orientation);

    if (waypoint.speed > 0)
        init.SetVelocity(waypoint.speed);

    init.SetWalk(waypoint.move_type != WAYPOINT_MOVE_TYPE_RUN);
    init.Launch();

    return true;
}

bool WaypointMovementGenerator<Player>::DoUpdate(Player& player, uint32 diff)
{
    // Waypoint movement can be switched on/off
    // This is quite handy for escort quests and other stuff
    if (player.HasUnitState(UNIT_STATE_NOT_MOVE))
    {
        player.ClearUnitState(UNIT_STATE_ROAMING_MOVE);
        return true;
    }
    // prevent a crash at empty waypoint path.
    if (!_path || _path->empty())
        return false;

    if (Stopped())
    {
        if (CanMove(diff))
            return StartMove(player);
    }
    else
    {
        if (player.IsStopped())
            Stop(sWorld->getIntConfig(CONFIG_CREATURE_STOP_FOR_PLAYER)); // TODO: is this too long for player waypoint movement?
        else if (player.movespline->Finalized())
        {
            OnArrived(player);
            return StartMove(player);
        }
    }
     return true;
}
