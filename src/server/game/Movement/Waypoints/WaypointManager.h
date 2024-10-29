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
#include "WaypointDefines.h"

class WaypointMgr
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
        ~WaypointMgr() = default;

        std::unordered_map<uint32, WaypointPath> _waypointStore;
        std::unordered_map<uint32, WaypointPath> _waypointScriptStore;
};

#define sWaypointMgr WaypointMgr::instance()

#endif

