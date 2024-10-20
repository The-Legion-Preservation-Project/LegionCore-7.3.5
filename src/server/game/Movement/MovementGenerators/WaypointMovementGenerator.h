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

#ifndef TRINITY_WAYPOINTMOVEMENTGENERATOR_H
#define TRINITY_WAYPOINTMOVEMENTGENERATOR_H

/**
 * @page PathMovementGenerator is used to generate movements
 * of waypoints and flight paths.  Each serves the purpose
 * of generate activities so that it generates updated
 * packets for the players.
 */

#include "MovementGenerator.h"
#include "WaypointManager.h"

class Creature;
class Player;

template<class Entity, class BasePath>
class PathMovementBase
{
    public:
        PathMovementBase() : _path(), _currentNode(0) { }
        virtual ~PathMovementBase() = default;

        uint32 GetCurrentNode() const { return _currentNode; }

    protected:
        BasePath _path;
        uint32 _currentNode;
};

template<class T>
class WaypointMovementGenerator;

template<>
class WaypointMovementGenerator<Creature> : public MovementGeneratorMedium<Creature, WaypointMovementGenerator<Creature>>, public PathMovementBase<Creature, WaypointPath const*>
{
    public:
        explicit WaypointMovementGenerator(uint32 pathId = 0, bool repeating = true, float randomMoveX = 0, float randomMoveY = 0, bool forceTpToStart = false) : _nextMoveTime(0), _isArrivalDone(false), _pathId(pathId), _repeating(repeating), _goBack(false), _randomMoveX(randomMoveX), _randomMoveY(randomMoveY), _forceTPToStart(forceTpToStart), _recalculateSpeed(false), _loadedFromDB(true), _stalled(false) { }
        explicit WaypointMovementGenerator(WaypointPath& path, bool repeating = true) : _nextMoveTime(0), _recalculateSpeed(false), _isArrivalDone(false), _pathId(0), _repeating(repeating), _loadedFromDB(false), _stalled(false), _goBack(false), _forceTPToStart(false), _randomMoveX(0.f), _randomMoveY(0.f)
        {
            _path = &path;
        }

        ~WaypointMovementGenerator() { _path = nullptr; }

        void DoInitialize(Creature&);
        void DoFinalize(Creature&);
        void DoReset(Creature&);
        bool DoUpdate(Creature&, const uint32& diff);

        MovementGeneratorType GetMovementGeneratorType() override { return WAYPOINT_MOTION_TYPE; }

        void UnitSpeedChanged() override { _recalculateSpeed = true; }
        void Pause() override;
        void Resume() override;

        void MovementInform(Creature&);

        bool GetResetPosition(Unit&, float& x, float& y, float& z) override;

    private:
        void LoadPath(Creature&);
        void OnArrived(Creature&);
        bool StartMove(Creature&);
        bool CanMove(Creature&);
        bool StartMoveNow(Creature& creature)
        {
            _nextMoveTime.Reset(0);
            return StartMove(creature);
        }

        TimeTrackerSmall _nextMoveTime;
        bool _recalculateSpeed;
        bool _isArrivalDone;
        uint32 _pathId;
        bool _repeating;
        bool _loadedFromDB;
        bool _stalled;

        bool _goBack;
        float _randomMoveX;
        float _randomMoveY;
        bool _forceTPToStart;
};

template<>
class WaypointMovementGenerator<Player> : public MovementGeneratorMedium<Player, WaypointMovementGenerator<Player>>, public PathMovementBase<Player, WaypointPath const*>
{
    public:
        explicit WaypointMovementGenerator(uint32 pathId = 0) : _nextMoveTime(0), _isArrivalDone(false), _pathId(pathId)  { }

        ~WaypointMovementGenerator() { _path = nullptr; }

        void DoInitialize(Player &);
        void DoFinalize(Player &);
        void DoReset(Player &);
        bool DoUpdate(Player &, const uint32 &diff);
        MovementGeneratorType GetMovementGeneratorType() override { return WAYPOINT_MOTION_TYPE; }

    private:
        void LoadPath(Player&);
        bool StartMove(Player&);
        void Stop(int32 time) { _nextMoveTime.Reset(time);}
        bool Stopped() { return !_nextMoveTime.Passed();}
        void OnArrived(Player&);

        bool CanMove(int32 diff)
        {
            _nextMoveTime.Update(diff);
            return _nextMoveTime.Passed();
        }

        void StartMoveNow(Player& player)
        {
            _nextMoveTime.Reset(0);
            StartMove(player);
        }

        TimeTrackerSmall _nextMoveTime;
        bool _isArrivalDone;
        uint32 _pathId;
};

/** FlightPathMovementGenerator generates movement of the player for the paths
 * and hence generates ground and activities for the player.
 */
class FlightPathMovementGenerator : public MovementGeneratorMedium<Player, FlightPathMovementGenerator>, public PathMovementBase<Player, TaxiPathNodeList>
{
    public:
        explicit FlightPathMovementGenerator()
        {
            _currentNode = 0;
            _endGridX = 0.0f;
            _endGridY = 0.0f;
            _endGridZ = 0.0f;
            _endMapId = 0;
            _preloadTargetNode = 0;
        }
        void LoadPath(Player& player, uint32 startNode = 0);
        void DoInitialize(Player& );
        void DoReset(Player& );
        void DoFinalize(Player& );
        bool DoUpdate(Player& , const uint32&);
        MovementGeneratorType GetMovementGeneratorType() override { return FLIGHT_MOTION_TYPE; }

        TaxiPathNodeList const& GetPath() { return _path; }
        uint32 GetPathAtMapEnd() const;
        bool HasArrived() const { return (_currentNode >= _path.size()); }
        void SetCurrentNodeAfterTeleport();
        void SkipCurrentNode() { ++_currentNode; }
        void DoEventIfAny(Player& player, TaxiPathNodeEntry const& node, bool departure);

        bool GetResetPosition(Unit&, float& x, float& y, float& z) override;
        void InitEndGridInfo();
        void PreloadEndGrid();

    private:

        float _endGridX;                            //! X coord of last node location
        float _endGridY;                            //! Y coord of last node location
        float _endGridZ;                            //! Z coord of last node location
        uint32 _endMapId;                           //! map Id of last node location
        uint32 _preloadTargetNode;                  //! node index where preloading starts

        struct TaxiNodeChangeInfo
        {
            uint32 PathIndex;
            int32 Cost;
        };

        std::deque<TaxiNodeChangeInfo> _pointsForPathSwitch;    //! node indexes and costs where TaxiPath changes
};

#endif
