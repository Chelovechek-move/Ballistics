#ifndef PROJECT_BALLISTICS_BUTCHERTABLENRK5_4_HPP
#define PROJECT_BALLISTICS_BUTCHERTABLENRK5_4_HPP

#include "ballistics/types/BasicTypes.hpp"
#include <array>

namespace Ballistics::Integrators::NestedRungeKutta{
    // The Dormand–Prince method of order 5(4)
    class ButcherTableNRK5_4 {
    public:
        constexpr static index approxOrder = 5;
        constexpr static index size = 7;
        constexpr static index numOfSubDiagEl = (size * size - size) / 2;
        constexpr static std::array<scalar, size> column = {
                static_cast<scalar>(0),
                static_cast<scalar>(1) / static_cast<scalar>(5),
                static_cast<scalar>(3) / static_cast<scalar>(10),
                static_cast<scalar>(4) / static_cast<scalar>(5),
                static_cast<scalar>(8) / static_cast<scalar>(9),
                static_cast<scalar>(1),
                static_cast<scalar>(1)
        };
        constexpr static std::array<scalar, size> row1 = {
                static_cast<scalar>(35)    / static_cast<scalar>(384),
                static_cast<scalar>(0),
                static_cast<scalar>(500)   / static_cast<scalar>(1113),
                static_cast<scalar>(125)   / static_cast<scalar>(192),
                static_cast<scalar>(-2187) / static_cast<scalar>(6784),
                static_cast<scalar>(11)    / static_cast<scalar>(84),
                static_cast<scalar>(0)
        };
        constexpr static std::array<scalar, size> row2 = {
                static_cast<scalar>(5179)   / static_cast<scalar>(57600),
                static_cast<scalar>(0),
                static_cast<scalar>(7571)   / static_cast<scalar>(16695),
                static_cast<scalar>(393)    / static_cast<scalar>(640),
                static_cast<scalar>(-92097) / static_cast<scalar>(339200),
                static_cast<scalar>(187)    / static_cast<scalar>(2100),
                static_cast<scalar>(1)      / static_cast<scalar>(40)
        };
        constexpr static std::array<scalar, numOfSubDiagEl> matrix = {
                static_cast<scalar>(1)      / static_cast<scalar>(5),

                static_cast<scalar>(3)      / static_cast<scalar>(40),
                static_cast<scalar>(9)      / static_cast<scalar>(40),

                static_cast<scalar>(44)     / static_cast<scalar>(45),
                static_cast<scalar>(-56)    / static_cast<scalar>(15),
                static_cast<scalar>(32)     / static_cast<scalar>(9),

                static_cast<scalar>(19372)  / static_cast<scalar>(6561),
                static_cast<scalar>(-25360) / static_cast<scalar>(2187),
                static_cast<scalar>(64448)  / static_cast<scalar>(6561),
                static_cast<scalar>(-212)   / static_cast<scalar>(729),

                static_cast<scalar>(9017)   / static_cast<scalar>(3168),
                static_cast<scalar>(-355)   / static_cast<scalar>(33),
                static_cast<scalar>(46732)  / static_cast<scalar>(5247),
                static_cast<scalar>(49)     / static_cast<scalar>(176),
                static_cast<scalar>(-5163)  / static_cast<scalar>(18656),

                static_cast<scalar>(35)     / static_cast<scalar>(384),
                static_cast<scalar>(0),
                static_cast<scalar>(500)    / static_cast<scalar>(1113),
                static_cast<scalar>(125)    / static_cast<scalar>(192),
                static_cast<scalar>(-2187)  / static_cast<scalar>(6784),
                static_cast<scalar>(11)     / static_cast<scalar>(84)
        };
    };
}


#endif //PROJECT_BALLISTICS_BUTCHERTABLENRK5_4_HPP
