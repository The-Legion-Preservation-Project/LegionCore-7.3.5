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

#ifndef TRINITY_RANDOMMOTIONGENERATOR_H
#define TRINITY_RANDOMMOTIONGENERATOR_H

#include "MovementGenerator.h"
#include "PathGenerator.h"
#include "Timer.h"

template<class T>
class RandomMovementGenerator : public MovementGeneratorMedium<T, RandomMovementGenerator<T>>
{
    public:
        explicit RandomMovementGenerator(float distance = 0.0f) : _path(nullptr), _timer(0), _reference(0.f, 0.f, 0.f), _wanderDistance(distance), _wanderSteps(0), _interrupt(false) { }

        MovementGeneratorType GetMovementGeneratorType() override { return RANDOM_MOTION_TYPE; }

        void DoInitialize(T &);
        void DoFinalize(T &);
        void DoReset(T &);
        bool DoUpdate(T &, const uint32);

    private:
        void SetRandomLocation(T &);

        std::unique_ptr<PathGenerator> _path;
        TimeTracker _timer;
        G3D::Vector3 _reference;
        float _wanderDistance;
        uint8 _wanderSteps;
        bool _interrupt;
};
#endif

