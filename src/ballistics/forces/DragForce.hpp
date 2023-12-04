#ifndef PROJECT_BALLISTICS_DRAGFORCE_HPP
#define PROJECT_BALLISTICS_DRAGFORCE_HPP

#include "../types/BasicTypes.hpp"

namespace Ballistics::Forces
{
    /** Method for calculating drag force
     * @param density density of the atmosphere
     * @param velocity velocity of the satellite
     * @param S surface area
     * @param Cd atmospheric drag coefficient
     * @return value of the drag force
     */
    [[nodiscard]] vector3d calcDragForce(const scalar density,
                                         const vector3d &velocity,
                                         const scalar S,
                                         const scalar Cd) noexcept;
}

#endif //PROJECT_BALLISTICS_DRAGFORCE_HPP