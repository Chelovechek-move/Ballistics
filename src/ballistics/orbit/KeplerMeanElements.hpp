#ifndef PROJECT_BALLISTICS_KEPLERMEANELEMENTS_HPP
#define PROJECT_BALLISTICS_KEPLERMEANELEMENTS_HPP

#include "ballistics/types/BasicTypes.hpp"

namespace Ballistics::Orbit
{
    struct KeplerMean
    {
        scalar inclination;
        scalar longitudeOfAscendingNode;
        scalar largeSemiAxis;
        scalar eccentricity;
        scalar pericenterArgument;
        scalar meanAnomaly;
    };
}

#endif //PROJECT_BALLISTICS_KEPLERMEANELEMENTS_HPP
