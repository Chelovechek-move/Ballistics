#include "DragForce.hpp"

namespace Ballistics::Forces
{
    vector3d calcDragForce(const scalar density,
                           const vector3d &velocity,
                           const scalar S,
                           const scalar Cd) noexcept
    {
        return (1. / 2.) * density * velocity.norm() * velocity.norm() * Cd * S * (-velocity / velocity.norm());
    }
}