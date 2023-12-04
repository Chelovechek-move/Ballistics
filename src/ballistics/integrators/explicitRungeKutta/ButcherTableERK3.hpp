#ifndef PROJECT_BALLISTICS_BUTCHERTABLEERK3_HPP
#define PROJECT_BALLISTICS_BUTCHERTABLEERK3_HPP

#include "ballistics/types/BasicTypes.hpp"
#include <array>

namespace Ballistics::Integrators::ExplicitRK {
    // Hoyne's method
    class ButcherTableERK3 {
    public:
        constexpr static index size = 3;
        constexpr static index numOfSubDiagEl = (size * size - size) / 2;
        constexpr static std::array<scalar, size> column = {
                static_cast<scalar>(0),
                static_cast<scalar>(1) / static_cast<scalar>(3),
                static_cast<scalar>(2) / static_cast<scalar>(3)
        };
        constexpr static std::array<scalar, size> row = {
                static_cast<scalar>(1) / static_cast<scalar>(4),
                static_cast<scalar>(0),
                static_cast<scalar>(3) / static_cast<scalar>(4)
        };
        constexpr static std::array<scalar, numOfSubDiagEl> matrix = {
                static_cast<scalar>(1) / static_cast<scalar>(3),
                static_cast<scalar>(0),
                static_cast<scalar>(2) / static_cast<scalar>(3)
        };
    };
}


#endif //PROJECT_BALLISTICS_BUTCHERTABLEERK3_HPP
