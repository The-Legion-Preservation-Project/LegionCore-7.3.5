/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
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

#ifndef TRINITY_WAYPOINTMANAGER_H
#define TRINITY_WAYPOINTMANAGER_H

#include "Containers.h"

enum WaypointMoveType
{
    WAYPOINT_MOVE_TYPE_WALK,
    WAYPOINT_MOVE_TYPE_RUN,
    WAYPOINT_MOVE_TYPE_LAND,
    WAYPOINT_MOVE_TYPE_TAKEOFF,
    WAYPOINT_MOVE_TYPE_MAX
};

struct WaypointData
{
    WaypointData() : id(0), x(0.f), y(0.f), z(0.f), orientation(0.f), delay(0), delay_chance(0), event_id(0), move_type(WAYPOINT_MOVE_TYPE_RUN), speed(0), event_chance(0) { }

    uint32 id;
    float x, y, z, orientation;
    uint32 move_type;
    float speed;
    uint32 delay;
    uint8 delay_chance;
    uint32 event_id;
    uint8 event_chance;
};

typedef std::vector<WaypointData> WaypointPath;
typedef std::unordered_map<uint32, WaypointPath> WaypointPathContainer;

class TC_GAME_API WaypointMgr
{
    public:
        static WaypointMgr* instance();

        // Attempts to reload a single path from database
        void ReloadPath(uint32 id);

        // Loads all paths from database, should only run on startup
        void Load();

        // Returns the path from a given id
        WaypointPath const* GetPath(uint32 id) const
        {
            return Trinity::Containers::MapGetValuePtr(_waypointStore, id);
        }

        // Returns the path from a given id
        WaypointPath const* GetPathScript(uint32 id) const
        {
            return Trinity::Containers::MapGetValuePtr(_waypointScriptStore, id);
        }

    private:
        WaypointMgr() = default;

        WaypointPathContainer _waypointStore;
        WaypointPathContainer _waypointScriptStore;
};

#define sWaypointMgr WaypointMgr::instance()

#endif

