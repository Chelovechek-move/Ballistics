#ifndef PROJECT_BALLISTICS_THIRDBODYGRAVITY_HPP
#define PROJECT_BALLISTICS_THIRDBODYGRAVITY_HPP

#include "ballistics/types/BasicTypes.hpp"

namespace Ballistics::Forces
{
    /** Function for calculating gravity force from space bodies except Earth
     * @param otherBodyPosGCRS vector of vectors each of which is the position of the i-th body in the GCRS system
     * @param gravParam vector of gravitational parameters of the space bodies
     * @param positionGCRS position of the satellite in the system GCRS
     * @param mass mass of the satellite
     * @return gravity force from space bodies except Earth
     */
    [[nodiscard]] vector3d calcThirdPartyGravityForce(const std::vector<vector3d> &otherBodyPosGCRS,
                                                      const std::vector<scalar> &gravParam,
                                                      const vector3d &positionGCRS,
                                                      const scalar mass) noexcept
    {
        vector3d res = vector3d::Zero();
        for (int i = 0; i < otherBodyPosGCRS.size(); ++i)
        {
            const vector3d deltaR = otherBodyPosGCRS[i] - positionGCRS;
            const scalar deltaRNormSqr = deltaR.squaredNorm();
            const scalar deltaRNorm = std::sqrt(deltaRNormSqr);
            const scalar riNormSqr = otherBodyPosGCRS[i].squaredNorm();
            const scalar riNorm = std::sqrt(riNormSqr);
            res += ((deltaR) / (deltaRNormSqr * deltaRNorm) -
                                    otherBodyPosGCRS[i] / (riNormSqr * riNorm)) * gravParam[i];
        }
        return res * mass;
    }
}

#endif //PROJECT_BALLISTICS_THIRDBODYGRAVITY_HPP
