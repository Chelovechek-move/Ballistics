#include "GravityForce.hpp"

namespace Ballistics::Forces
{
    [[nodiscard]] vector3d GravityForce::calcForce(const Ballistics::scalar mass,
                                                   const Ballistics::vector3d &rGCRS,
                                                   const Ballistics::quaterniond &q,
                                                   const Time::Time<Time::Scale::TT> &timeTT,
                                                   const Time::Time<Time::Scale::UT1> &timeUT1) const noexcept
    {
        const vector3d rITRS = q._transformVector(rGCRS);
        vector3d aITRS;
        model_.V(rITRS.x(), rITRS.y(), rITRS.z(), aITRS.x(), aITRS.y(), aITRS.z());
        const vector3d aGCRS = q.conjugate()._transformVector(aITRS);
        return aGCRS * mass;
    }
}