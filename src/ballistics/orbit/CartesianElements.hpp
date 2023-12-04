#ifndef PROJECT_BALLISTICS_POSITIONVELOCITYELEMENTS_HPP
#define PROJECT_BALLISTICS_POSITIONVELOCITYELEMENTS_HPP

#include "ballistics/types/BasicTypes.hpp"

namespace Ballistics::Orbit
{
    struct PosVel
    {
        vector3d position;
        vector3d velocity;
    };
}

#endif //PROJECT_BALLISTICS_POSITIONVELOCITYELEMENTS_HPP
