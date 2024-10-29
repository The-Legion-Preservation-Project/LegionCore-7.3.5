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
    if (!_path || _path->nodes.empty())
        return;

    WaypointNode const &waypoint = _path->nodes.at(_currentNode);

    // For WP from sniff
    if (waypoint.delayChance && roll_chance_i(waypoint.delayChance))
    {
        creature.ClearUnitState(UNIT_STATE_ROAMING_MOVE);
        _nextMoveTime.Reset(urand(1000, 10000));
    }
    else if (waypoint.delay)
    {
        creature.ClearUnitState(UNIT_STATE_ROAMING_MOVE);
        _nextMoveTime.Reset(waypoint.delay);
    }

    if (waypoint.eventId && roll_chance_i(waypoint.eventChance))
    {
        TC_LOG_DEBUG("maps.scripts", "Creature movement start script %u at point %u for %s.", waypoint.eventId, _currentNode, creature.GetGUID().ToString().c_str());
        creature.ClearUnitState(UNIT_STATE_ROAMING_MOVE);
        creature.GetMap()->ScriptsStart(sWaypointScripts, waypoint.eventId, &creature, nullptr);
    }

    // Inform script
    MovementInform(creature);
    creature.UpdateWaypointID(_currentNode);

    creature.SetWalk(waypoint.moveType != WAYPOINT_MOVE_TYPE_RUN);
}

bool WaypointMovementGenerator<Creature>::StartMove(Creature& creature)
{
    if (!creature.IsAlive())
        return false;

    if (!_path || _path->nodes.empty())
        return false;

    bool transportPath = creature.HasUnitMovementFlag(MOVEMENTFLAG_ONTRANSPORT) && !creature.GetTransGUID().IsEmpty();

    if (_isArrivalDone)
    {
        if ((_currentNode == _path->nodes.size() - 1) && !_repeating) // If that's our last waypoint
        {
            WaypointNode const &waypoint = _path->nodes.at(_currentNode);

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

            return false;
        }

        _currentNode = (_currentNode + 1) % _path->nodes.size();
    }

    float finalOrient = 0.0f;
    uint8 finalMove = WAYPOINT_MOVE_TYPE_WALK;

    Movement::PointsArray pathing;
    pathing.reserve((_path->nodes.size() - _currentNode) + 1);

    pathing.emplace_back(creature.GetPositionX(), creature.GetPositionY(), creature.GetPositionZ());
    for (uint32 i = _currentNode; i < _path->nodes.size(); ++i)
    {
        WaypointNode const &waypoint = _path->nodes.at(i);

        pathing.emplace_back(waypoint.x, waypoint.y, waypoint.z);

        finalOrient = waypoint.orientation;
        finalMove = waypoint.moveType;

        if (waypoint.delay)
            break;
    }

    // if we have only 1 point, only current position, we shall return
    if (pathing.size() < 2)
        return false;

    _isArrivalDone = false;
    _recalculateSpeed = false;

    creature.AddUnitState(UNIT_STATE_ROAMING_MOVE);

    Movement::MoveSplineInit init(creature);

    WaypointNode const &waypoint = _path->nodes.at(_currentNode);
    Position formationDest(waypoint.x, waypoint.y, waypoint.z, 0.0f);

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

    init.MovebyPath(pathing, _currentNode);

//    // TODO: is this needed?
//    if (waypoint.speed > 0)
//        init.SetVelocity(waypoint.speed);
//
    switch (finalMove)
    {
        case WAYPOINT_MOVE_TYPE_LAND:
            init.SetAnimation(Movement::ToGround);
            break;
        case WAYPOINT_MOVE_TYPE_TAKEOFF:
            init.SetAnimation(Movement::ToFly);
            break;
        case WAYPOINT_MOVE_TYPE_RUN:
            init.SetWalk(false);
            break;
        case WAYPOINT_MOVE_TYPE_WALK:
            init.SetWalk(true);
            break;
    }

    if (finalOrient != 0.0f)
        init.SetFacing(finalOrient);

    init.Launch();

    // Call for creature group update
    if (creature.GetFormation() && creature.GetFormation()->getLeader() == &creature)
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
    if (!_path || _path->nodes.empty())
        return false;

    if (Stopped())
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
            Stop(_loadedFromDB ? sWorld->getIntConfig(CONFIG_CREATURE_STOP_FOR_PLAYER) : 2 * HOUR * IN_MILLISECONDS);
        else if (creature.movespline->Finalized())
        {
            OnArrived(creature);

            _isArrivalDone = true;

            if (!Stopped())
            {
                if (creature.IsStopped())
                    Stop(_loadedFromDB ? sWorld->getIntConfig(CONFIG_CREATURE_STOP_FOR_PLAYER) : 2 * HOUR * IN_MILLISECONDS);
                else
                    return StartMove(creature);
            }
        }
        else
        {
            // speed changed during path execution, calculate remaining path and launch it once more
            if (_recalculateSpeed)
            {
                _recalculateSpeed = false;

                if (!Stopped())
                    return StartMove(creature);
            }
            else
            {
                auto pointId = uint32(creature.movespline->currentPathIdx());
                if (pointId > _currentNode)
                {
                    OnArrived(creature);
                    _currentNode = pointId;
                    //FormationMove(creature);
                }
            }
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
        if (_currentNode == _path->nodes.size() - 1)
            creature.AI()->LastWPReached();
    }
}

bool WaypointMovementGenerator<Creature>::GetResetPosition(Unit&, float& x, float& y, float& z)
{
    // prevent a crash at empty waypoint path.
    if (!_path || _path->nodes.empty())
        return false;

    WaypointNode const &waypoint = _path->nodes.at(_currentNode);

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
    if (!_path || _path->nodes.empty() || _currentNode >= _path->nodes.size() || _isArrivalDone)
        return;

    _isArrivalDone = true;
    WaypointNode const &waypoint = _path->nodes.at(_currentNode);

    if (waypoint.eventId && roll_chance_i(waypoint.eventChance))
    {
        TC_LOG_DEBUG("maps.script", "Player movement start script %u at point %u for %lu.", waypoint.eventId, _currentNode, player.GetGUID().GetCounter());
        player.ClearUnitState(UNIT_STATE_ROAMING_MOVE);
        player.GetMap()->ScriptsStart(sWaypointScripts, waypoint.eventId, &player, nullptr);
    }
}

bool WaypointMovementGenerator<Player>::StartMove(Player &player)
{
    if (!_path || _path->nodes.empty())
        return false;

    if (Stopped())
        return true;

    if (_isArrivalDone)
    {
        if (_currentNode == _path->nodes.size() - 1) // If that's our last waypoint
        {
            player.GetMotionMaster()->Initialize();
            return false;
        }

        _currentNode = (_currentNode+1) % _path->nodes.size();
    }

    if (_currentNode >= _path->nodes.size())
        return false;

    WaypointNode const &waypoint = _path->nodes.at(_currentNode);

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

    init.SetWalk(waypoint.moveType != WAYPOINT_MOVE_TYPE_RUN);
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
    if (!_path || _path->nodes.empty())
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
