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

#include "Creature.h"
#include "CreatureAI.h"
#include "WorldPacket.h"
#include "MoveSplineInit.h"
#include "MoveSpline.h"
#include "HomeMovementGenerator.h"

template<class T>
HomeMovementGenerator<T>::~HomeMovementGenerator() { }

template<>
HomeMovementGenerator<Creature>::~HomeMovementGenerator()
{
    delete _path;
}

template<class T>
void HomeMovementGenerator<T>::SetTargetLocation(T &) { }

template<>
void HomeMovementGenerator<Creature>::SetTargetLocation(Creature &owner)
{
    if (owner.HasUnitState(UNIT_STATE_ROOT | UNIT_STATE_STUNNED | UNIT_STATE_DISTRACTED))
    {
        _skipToHome = true;
        return;
    }


    Movement::MoveSplineInit init(owner);
    float x, y, z, o;
    // at apply we can select more nice return points base at current movegen
    if (owner.GetMotionMaster()->empty() || !owner.GetMotionMaster()->top()->GetResetPosition(owner,x,y,z))
    {
        owner.GetHomePosition(x, y, z, o);
        init.SetFacing(o);
    }
    init.MoveTo(x, y, z);
    init.SetWalk(false);
    init.Launch();

    _skipToHome = false;
    _arrived = false;

    owner.ClearUnitState(UNIT_STATE_ALL_STATE & ~UNIT_STATE_EVADE);
}

template<class T>
void HomeMovementGenerator<T>::DoInitialize(T &) { }

template<>
void HomeMovementGenerator<Creature>::DoInitialize(Creature &owner)
{
    SetTargetLocation(owner);
}

template<class T>
void HomeMovementGenerator<T>::DoFinalize(T &) { }

template<>
void HomeMovementGenerator<Creature>::DoFinalize(Creature &owner)
{
    if (_arrived)
    {
        owner.ClearUnitState(UNIT_STATE_EVADE);
        owner.SetWalk(true);
        owner.LoadCreaturesAddon(true);
        owner.AI()->JustReachedHome();
        owner.SetSpawnHealth();
    }
}

template<class T>
void HomeMovementGenerator<T>::DoReset(T &) { }

template<>
void HomeMovementGenerator<Creature>::DoReset(Creature &owner)
{
    DoInitialize(owner);
}

template<class T>
bool HomeMovementGenerator<T>::DoUpdate(T &, const uint32)
{
    return false;
}

template<>
bool HomeMovementGenerator<Creature>::DoUpdate(Creature &owner, const uint32 /*time_diff*/)
{
    _arrived = _skipToHome || owner.movespline->Finalized();
    return !_arrived;
}

