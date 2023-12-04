#ifndef PROJECT_BALLISTICS_BUTCHERTABLEERK2_HPP
#define PROJECT_BALLISTICS_BUTCHERTABLEERK2_HPP

#include "ballistics/types/BasicTypes.hpp"
#include <array>

namespace Ballistics::Integrators::ExplicitRK {
    // Euler's method
    class ButcherTableERK2 {
    public:
        constexpr static index size = 2;
        constexpr static index numOfSubDiagEl = (size * size - size) / 2;
        constexpr static std::array<scalar, size> column = {
                static_cast<scalar>(0),
                static_cast<scalar>(1) / static_cast<scalar>(2)
        };
        constexpr static std::array<scalar, size> row = {
                static_cast<scalar>(0),
                static_cast<scalar>(1)
        };
        constexpr static std::array<scalar, numOfSubDiagEl> matrix = {static_cast<scalar>(1) / static_cast<scalar>(2)};
    };
}


#endif //PROJECT_BALLISTICS_BUTCHERTABLEERK2_HPP
