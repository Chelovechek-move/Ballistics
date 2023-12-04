#ifndef PROJECT_BALLISTICS_RIGHTPARTOFDIFFEQ_HPP
#define PROJECT_BALLISTICS_RIGHTPARTOFDIFFEQ_HPP

#include "ballistics/types/BasicTypes.hpp"

namespace Ballistics::Integrators {
    // du   u
    // -- = --
    // dt   t
    class RightPart1 {
    public:
        constexpr static index size = 1;

        struct Params {
            scalar gravParam;
        };

        using Time = scalar;

        struct State {
            vector<scalar, size> u;
            Time t;
        };

        [[nodiscard]] static inline vector<scalar, size> calc(const State &stateVector,
                                                              const Params &params) {
            return vector<scalar, size>{stateVector.u / stateVector.t};
        };
    };

    // du   1
    // -- = --
    // dt   u
    class RightPart2 {
    public:
        constexpr static index size = 1;

        struct Params {
            scalar gravParam;
        };

        using Time = scalar;

        struct State {
            vector<scalar, size> u;
            Time t;
        };

        [[nodiscard]] static inline vector<scalar, size> calc(const State &stateVector,
                                                              const Params &params) {
            return vector<scalar, size>{1. / stateVector.u[0]};
        };
    };

    // du
    // -- = sin(t) * u^2
    // dt
    class RightPart3 {
    public:
        constexpr static index size = 1;

        struct Params {
            scalar gravParam;
        };

        using Time = scalar;

        struct State {
            vector<scalar, size> u;
            Time t;
        };

        [[nodiscard]] static inline vector<scalar, size> calc(const State &stateVector,
                                                              const Params &params) {
            return vector<scalar, size>{sin(stateVector.t) * stateVector.u.dot(stateVector.u)};
        };
    };

    // du
    // -- = cos(t)
    // dt
    class RightPart4 {
    public:
        constexpr static index size = 1;

        struct Params {
            scalar gravParam;
        };

        using Time = scalar;

        struct State {
            vector<scalar, size> u;
            Time t;
        };

        [[nodiscard]] static inline vector<scalar, size> calc(const State &stateVector,
                                                              const Params &params) {
            return vector<scalar, size>{cos(stateVector.t)};
        };
    };

    // dr
    // -- = v
    // dt
    // dv      /  mu \
    // -- = - (  ---  ) * r
    // dt      \ r^3 /
    class RightPart5 {
    public:
        constexpr static index size = 6;

        struct Params {
            scalar gravParam;
        };

        using Time = scalar;

        struct State {
            vector<scalar, size> u;
            Time t;
        };

        [[nodiscard]] static inline vector<scalar, size> calc(const State &stateVector,
                                                              const Params &params) {
            const scalar rSquaredNorm =
                    vector<scalar, size / 2>{stateVector.u[0], stateVector.u[1], stateVector.u[2]}.squaredNorm();
            const scalar coeff = (-1) * (params.gravParam / (sqrt(rSquaredNorm) * rSquaredNorm));
            return vector<scalar, size>{stateVector.u[3],
                                        stateVector.u[4],
                                        stateVector.u[5],
                                        coeff * stateVector.u[0],
                                        coeff * stateVector.u[1],
                                        coeff * stateVector.u[2]};
        };
    };

    // Arenstorff orbit
    class RightPart6 {
    public:
        constexpr static index size = 4;

        struct Params {
            static constexpr scalar mu = 0.012277471;
            static constexpr scalar eta = 1 - mu;
        };

        using Time = scalar;

        struct State {
            vector<scalar, size> u;
            Time t;
        };

        [[nodiscard]] static inline vector<scalar, size> calc(const State &stateVector,
                                                              const Params &params) {
            const scalar x = stateVector.u[0];
            const scalar u = stateVector.u[1];
            const scalar y = stateVector.u[2];
            const scalar nu = stateVector.u[3];
            const scalar multiplier1 = (x + params.mu) * (x + params.mu) + y * y;
            const scalar A = sqrt(multiplier1 * multiplier1 * multiplier1);
            const scalar multiplier2 = (x - params.eta) * (x - params.eta) + y * y;
            const scalar B = sqrt(multiplier2 * multiplier2 * multiplier2);
            return vector<scalar, size>{u,
                                        x + 2 * nu - params.eta * ((x + params.mu) / A) - params.mu * ((x - params.eta) / B),
                                        nu,
                                        y - 2 * u - params.eta * (y / A) - params.mu * (y / B)};
        };
    };
}

#endif //PROJECT_BALLISTICS_RIGHTPARTOFDIFFEQ_HPP
