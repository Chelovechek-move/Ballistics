#ifndef PROJECT_BALLISTICS_BUTCHERTABLENRK7_8_HPP
#define PROJECT_BALLISTICS_BUTCHERTABLENRK7_8_HPP

#include "ballistics/types/BasicTypes.hpp"
#include <array>

namespace Ballistics::Integrators::NestedRungeKutta {
    // The Felberg method of order 7(8)
    class ButcherTableNRK7_8 {
    public:
        constexpr static index approxOrder = 8;
        constexpr static index size = 13;
        constexpr static index numOfSubDiagEl = (size * size - size) / 2;
        constexpr static std::array<scalar, size> column = {
                static_cast<scalar>(0),
                static_cast<scalar>(2) / static_cast<scalar>(27),
                static_cast<scalar>(1) / static_cast<scalar>(9),
                static_cast<scalar>(1) / static_cast<scalar>(6),
                static_cast<scalar>(5) / static_cast<scalar>(12),
                static_cast<scalar>(1) / static_cast<scalar>(2),
                static_cast<scalar>(5) / static_cast<scalar>(6),
                static_cast<scalar>(1) / static_cast<scalar>(6),
                static_cast<scalar>(2) / static_cast<scalar>(3),
                static_cast<scalar>(1) / static_cast<scalar>(3),
                static_cast<scalar>(1),
                static_cast<scalar>(0),
                static_cast<scalar>(1)
        };
        constexpr static std::array<scalar, size> row1 = {
                static_cast<scalar>(41) / static_cast<scalar>(840),
                static_cast<scalar>(0),
                static_cast<scalar>(0),
                static_cast<scalar>(0),
                static_cast<scalar>(0),
                static_cast<scalar>(34) / static_cast<scalar>(105),
                static_cast<scalar>(9)  / static_cast<scalar>(35),
                static_cast<scalar>(9)  / static_cast<scalar>(35),
                static_cast<scalar>(9)  / static_cast<scalar>(280),
                static_cast<scalar>(9)  / static_cast<scalar>(280),
                static_cast<scalar>(41) / static_cast<scalar>(840),
                static_cast<scalar>(0),
                static_cast<scalar>(0)
        };
        constexpr static std::array<scalar, size> row2 = {
                static_cast<scalar>(0),
                static_cast<scalar>(0),
                static_cast<scalar>(0),
                static_cast<scalar>(0),
                static_cast<scalar>(0),
                static_cast<scalar>(34) / static_cast<scalar>(105),
                static_cast<scalar>(9)  / static_cast<scalar>(35),
                static_cast<scalar>(9)  / static_cast<scalar>(35),
                static_cast<scalar>(9)  / static_cast<scalar>(280),
                static_cast<scalar>(9)  / static_cast<scalar>(280),
                static_cast<scalar>(0),
                static_cast<scalar>(41) / static_cast<scalar>(840),
                static_cast<scalar>(41) / static_cast<scalar>(840)
        };
        constexpr static std::array<scalar, numOfSubDiagEl> matrix = {
                static_cast<scalar>(2)     / static_cast<scalar>(27),

                static_cast<scalar>(1)     / static_cast<scalar>(36),
                static_cast<scalar>(1)     / static_cast<scalar>(12),

                static_cast<scalar>(1)     / static_cast<scalar>(24),
                static_cast<scalar>(0),
                static_cast<scalar>(1)     / static_cast<scalar>(8),

                static_cast<scalar>(5)     / static_cast<scalar>(12),
                static_cast<scalar>(0),
                static_cast<scalar>(-25)   / static_cast<scalar>(16),
                static_cast<scalar>(25)    / static_cast<scalar>(16),

                static_cast<scalar>(1)     / static_cast<scalar>(20),
                static_cast<scalar>(0),
                static_cast<scalar>(0),
                static_cast<scalar>(1)     / static_cast<scalar>(4),
                static_cast<scalar>(1)     / static_cast<scalar>(5),

                static_cast<scalar>(-25)   / static_cast<scalar>(108),
                static_cast<scalar>(0),
                static_cast<scalar>(0),
                static_cast<scalar>(125)   / static_cast<scalar>(108),
                static_cast<scalar>(-65)   / static_cast<scalar>(27),
                static_cast<scalar>(125)   / static_cast<scalar>(54),

                static_cast<scalar>(31)    / static_cast<scalar>(300),
                static_cast<scalar>(0),
                static_cast<scalar>(0),
                static_cast<scalar>(0),
                static_cast<scalar>(61)    / static_cast<scalar>(225),
                static_cast<scalar>(-2)    / static_cast<scalar>(9),
                static_cast<scalar>(13)    / static_cast<scalar>(900),

                static_cast<scalar>(2),
                static_cast<scalar>(0),
                static_cast<scalar>(0),
                static_cast<scalar>(-53)   / static_cast<scalar>(6),
                static_cast<scalar>(704)   / static_cast<scalar>(45),
                static_cast<scalar>(-107)  / static_cast<scalar>(9),
                static_cast<scalar>(67)    / static_cast<scalar>(90),
                static_cast<scalar>(3),

                static_cast<scalar>(-91)   / static_cast<scalar>(108),
                static_cast<scalar>(0),
                static_cast<scalar>(0),
                static_cast<scalar>(23)    / static_cast<scalar>(108),
                static_cast<scalar>(-976)  / static_cast<scalar>(135),
                static_cast<scalar>(311)   / static_cast<scalar>(54),
                static_cast<scalar>(-19)   / static_cast<scalar>(60),
                static_cast<scalar>(17)    / static_cast<scalar>(6),
                static_cast<scalar>(-1)    / static_cast<scalar>(12),

                static_cast<scalar>(2383)  / static_cast<scalar>(4100),
                static_cast<scalar>(0),
                static_cast<scalar>(0),
                static_cast<scalar>(-341)  / static_cast<scalar>(164),
                static_cast<scalar>(4496)  / static_cast<scalar>(1025),
                static_cast<scalar>(-301)  / static_cast<scalar>(82),
                static_cast<scalar>(2133)  / static_cast<scalar>(4100),
                static_cast<scalar>(45)    / static_cast<scalar>(82),
                static_cast<scalar>(45)    / static_cast<scalar>(164),
                static_cast<scalar>(18)    / static_cast<scalar>(41),

                static_cast<scalar>(3)     / static_cast<scalar>(205),
                static_cast<scalar>(0),
                static_cast<scalar>(0),
                static_cast<scalar>(0),
                static_cast<scalar>(0),
                static_cast<scalar>(-6)    / static_cast<scalar>(41),
                static_cast<scalar>(-3)    / static_cast<scalar>(205),
                static_cast<scalar>(-3)    / static_cast<scalar>(41),
                static_cast<scalar>(3)     / static_cast<scalar>(41),
                static_cast<scalar>(6)     / static_cast<scalar>(41),
                static_cast<scalar>(0),

                static_cast<scalar>(-1777) / static_cast<scalar>(4100),
                static_cast<scalar>(0),
                static_cast<scalar>(0),
                static_cast<scalar>(-341)  / static_cast<scalar>(164),
                static_cast<scalar>(4496)  / static_cast<scalar>(1025),
                static_cast<scalar>(-289)  / static_cast<scalar>(82),
                static_cast<scalar>(2193)  / static_cast<scalar>(4100),
                static_cast<scalar>(51)    / static_cast<scalar>(82),
                static_cast<scalar>(33)    / static_cast<scalar>(164),
                static_cast<scalar>(12)    / static_cast<scalar>(41),
                static_cast<scalar>(0),
                static_cast<scalar>(1)
        };
    };
}
#endif //PROJECT_BALLISTICS_BUTCHERTABLENRK7_8_HPP