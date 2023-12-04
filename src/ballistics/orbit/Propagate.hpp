#ifndef PROJECT_BALLISTICS_PROPAGATE_HPP
#define PROJECT_BALLISTICS_PROPAGATE_HPP

#include "ballistics/types/BasicTypes.hpp"
#include "ballistics/orbit/KeplerMeanElements.hpp"

namespace Ballistics::Orbit
{
    [[nodiscard]] KeplerMean propagate(const KeplerMean &initSetOfElements,
                                       const scalar gravParam,
                                       const scalar deltaT)
    {
        const scalar deltaMeanAnomaly = sqrt(gravParam / (initSetOfElements.eccentricity *
                                                          initSetOfElements.eccentricity *
                                                          initSetOfElements.eccentricity)) * deltaT;
        return {initSetOfElements.inclination,
                initSetOfElements.longitudeOfAscendingNode,
                initSetOfElements.largeSemiAxis,
                initSetOfElements.eccentricity,
                initSetOfElements.pericenterArgument,
                initSetOfElements.meanAnomaly + deltaMeanAnomaly};
    }
}

#endif //PROJECT_BALLISTICS_PROPAGATE_HPP
