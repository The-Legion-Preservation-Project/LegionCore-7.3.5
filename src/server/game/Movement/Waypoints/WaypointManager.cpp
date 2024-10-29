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

#include "WaypointManager.h"
#include "DatabaseEnv.h"
#include "GridDefines.h"
#include "Log.h"

WaypointMgr* WaypointMgr::instance()
{
    static WaypointMgr instance;
    return &instance;
}

void WaypointMgr::Load()
{
    uint32 oldMSTime = getMSTime();

    //                                                0    1         2           3          4            5           6        7      8       9        10              11
    QueryResult result = WorldDatabase.Query("SELECT id, point, position_x, position_y, position_z, orientation, move_type, speed, delay, action, action_chance, delay_chance FROM waypoint_data ORDER BY id, point");

    if (!result)
    {
        TC_LOG_ERROR("server.loading", ">> Loaded 0 waypoints. DB table `waypoint_data` is empty!");

        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();

        uint32 pathId = fields[0].GetUInt32();
        WaypointPath& path = _waypointStore[pathId];

        float x = fields[2].GetFloat();
        float y = fields[3].GetFloat();
        float z = fields[4].GetFloat();
        float o = fields[5].GetFloat();

        Trinity::NormalizeMapCoord(x);
        Trinity::NormalizeMapCoord(y);

        WaypointNode wp;
        wp.id = fields[1].GetUInt32();
        wp.x = x;
        wp.y = y;
        wp.z = z;
        wp.orientation = o;
        wp.moveType = fields[6].GetUInt32();

        if (wp.moveType >= WAYPOINT_MOVE_TYPE_MAX)
        {
            TC_LOG_ERROR("sql.sql", "Waypoint %u in waypoint_data has invalid move_type, ignoring", wp.id);
            continue;
        }

        wp.speed = fields[7].GetFloat();
        wp.delay = fields[8].GetUInt32();
        wp.eventId = fields[9].GetUInt32();
        wp.eventChance = fields[10].GetInt16();
        wp.delayChance = fields[11].GetInt16();

        path.nodes.push_back(std::move(wp));
        ++count;
    }
    while (result->NextRow());

    //                                    0    1         2           3          4            5           6        7      8       9        10
    result = WorldDatabase.Query("SELECT id, point, position_x, position_y, position_z, orientation, move_type, speed, delay, action, action_chance FROM waypoint_data_script ORDER BY id, point");

    if (!result)
    {
        TC_LOG_ERROR("server.loading", ">> Loaded 0 waypoints. DB table `waypoint_data_script` is empty!");
        return;
    }

    do
    {
        Field* fields = result->Fetch();

        uint32 pathId = fields[0].GetUInt32();
        WaypointPath& path = _waypointScriptStore[pathId];

        float x = fields[2].GetFloat();
        float y = fields[3].GetFloat();
        float z = fields[4].GetFloat();
        float o = fields[5].GetFloat();

        Trinity::NormalizeMapCoord(x);
        Trinity::NormalizeMapCoord(y);

        WaypointNode wp;
        wp.id = fields[1].GetUInt32();
        wp.x = x;
        wp.y = y;
        wp.z = z;
        wp.orientation = o;
        wp.moveType = fields[6].GetUInt32();

        if (wp.moveType >= WAYPOINT_MOVE_TYPE_MAX)
        {
            TC_LOG_ERROR("sql.sql", "Waypoint %u in waypoint_data_script has invalid move_type, ignoring", wp.id);
            continue;
        }

        wp.speed = fields[7].GetFloat();
        wp.delay = fields[8].GetUInt32();
        wp.eventId = fields[9].GetUInt32();
        wp.eventChance = fields[10].GetInt16();

        path.nodes.push_back(std::move(wp));
        ++count;
    }
    while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u waypoints in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void WaypointMgr::ReloadPath(uint32 id)
{
    auto itr = _waypointStore.find(id);
    if (itr != _waypointStore.end())
        _waypointStore.erase(itr);

    WorldDatabasePreparedStatement* stmt = WorldDatabase.GetPreparedStatement(WORLD_SEL_WAYPOINT_DATA_BY_ID);
    stmt->setUInt32(0, id);
    PreparedQueryResult result = WorldDatabase.Query(stmt);
    if (!result)
        return;

    WaypointPath& path = _waypointStore[id];

    do
    {
        Field* fields = result->Fetch();

        float x = fields[1].GetFloat();
        float y = fields[2].GetFloat();
        float z = fields[3].GetFloat();
        float o = fields[4].GetFloat();

        Trinity::NormalizeMapCoord(x);
        Trinity::NormalizeMapCoord(y);

        WaypointNode wp;
        wp.id = fields[0].GetUInt32();
        wp.x = x;
        wp.y = y;
        wp.z = z;
        wp.orientation = o;
        wp.moveType = fields[5].GetUInt32();

        if (wp.moveType >= WAYPOINT_MOVE_TYPE_MAX)
        {
            TC_LOG_ERROR("sql.sql", "Waypoint %u in waypoint_data has invalid move_type, ignoring", wp.id);
            continue;
        }

        wp.speed = fields[6].GetFloat();
        wp.delay = fields[7].GetUInt32();
        wp.eventId = fields[8].GetUInt32();
        wp.eventChance = fields[9].GetUInt8();
        wp.delayChance = fields[10].GetUInt8();

        path.nodes.push_back(std::move(wp));

    }
    while (result->NextRow());

    itr = _waypointScriptStore.find(id);
    if (itr != _waypointScriptStore.end())
        _waypointScriptStore.erase(itr);

    stmt = WorldDatabase.GetPreparedStatement(WORLD_SEL_WAYPOINT_DATA_SCRIPT_BY_ID);
    stmt->setUInt32(0, id);
    result = WorldDatabase.Query(stmt);
    if (!result)
        return;

    WaypointPath& pathS = _waypointScriptStore[id];

    do
    {
        Field* fields = result->Fetch();

        float x = fields[1].GetFloat();
        float y = fields[2].GetFloat();
        float z = fields[3].GetFloat();
        float o = fields[4].GetFloat();

        Trinity::NormalizeMapCoord(x);
        Trinity::NormalizeMapCoord(y);

        WaypointNode wp;
        wp.id = fields[0].GetUInt32();
        wp.x = x;
        wp.y = y;
        wp.z = z;
        wp.orientation = o;
        wp.moveType = fields[5].GetUInt32();

        if (wp.moveType >= WAYPOINT_MOVE_TYPE_MAX)
        {
            TC_LOG_ERROR("sql.sql", "Waypoint %u in waypoint_data_script has invalid move_type, ignoring", wp.id);
            continue;
        }

        wp.speed = fields[6].GetFloat();
        wp.delay = fields[7].GetUInt32();
        wp.eventId = fields[8].GetUInt32();
        wp.eventChance = fields[9].GetUInt8();

        pathS.nodes.push_back(std::move(wp));

    }
    while (result->NextRow());
}
