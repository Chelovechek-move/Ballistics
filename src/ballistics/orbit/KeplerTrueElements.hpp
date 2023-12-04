#ifndef PROJECT_BALLISTICS_KEPLERTRUEELEMENTS_HPP
#define PROJECT_BALLISTICS_KEPLERTRUEELEMENTS_HPP

#include "ballistics/types/BasicTypes.hpp"

namespace Ballistics::Orbit
{
    struct KeplerTrue
    {
        scalar inclination;
        scalar longitudeOfAscendingNode;
        scalar largeSemiAxis;
        scalar eccentricity;
        scalar pericenterArgument;
        scalar trueAnomaly;
    };
}

#endif //PROJECT_BALLISTICS_KEPLERTRUEELEMENTS_HPP
