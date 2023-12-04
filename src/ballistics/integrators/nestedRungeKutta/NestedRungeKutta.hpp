#ifndef PROJECT_BALLISTICS_NESTEDRUNGEKUTTA_HPP
#define PROJECT_BALLISTICS_NESTEDRUNGEKUTTA_HPP

#include "ballistics/exception/ProjectException.hpp"
#include "ballistics/types/BasicTypes.hpp"
#include <vector>

namespace Ballistics::Integrators::NestedRungeKutta {
    template<typename RightPart>
    struct IntegrationParameters {
        const typename RightPart::Time tFinal;
        const scalar stepMin;
        const scalar stepMax;
        const scalar tolerance;
        const scalar p;
    };

    template<typename ButcherTable>
    class NestedRungeKutta {
    public:
        template<typename RightPart>
        [[nodiscard]] static inline std::vector<typename RightPart::State> calc(const typename RightPart::State &initState,
                                                                                const typename RightPart::Params &params,
                                                                                const IntegrationParameters<RightPart> &intParams) {
            vector<scalar, RightPart::size> uTmp = initState.u;
            typename RightPart::Time tTmp = initState.t;
            std::vector<typename RightPart::State> solution = {initState};
            scalar step = intParams.stepMin;

            while (tTmp <= intParams.tFinal) {

                if (step < intParams.stepMin) {
                    std::stringstream buff;
                    buff << "The calculated step is less than the minimum step. Step = " << step << ". Minimum step = " << intParams.stepMin << ". File: " << __FILE__ << ". Line: " << __LINE__;
                    throw ProjectException(buff.str());
                }

                if (step > intParams.stepMax) {
                    std::stringstream buff;
                    buff << "The calculated step is more than the maximum step. Step = " << step << ". Maximum step = " << intParams.stepMax << ". File: " << __FILE__ << ". Line: " << __LINE__;
                    throw ProjectException(buff.str());
                }

                // calculating k[i]
                std::vector<vector<scalar, RightPart::size>> kMas = {RightPart::calc({uTmp, tTmp}, params)};
                index n = 0;
                for (integer i = 1; i < ButcherTable::size; ++i) {
                    vector<scalar, RightPart::size> sum = vector<scalar, RightPart::size>::Zero();
                    for (integer k = 0; k < i; ++k)
                    {
                        const scalar lol = (ButcherTable::matrix[n + k] * kMas[k])[0];
                        sum += ButcherTable::matrix[n + k] * kMas[k];
                    }
                    n += i;

                    kMas.push_back(RightPart::calc({uTmp + sum * step, tTmp + ButcherTable::column[i] * step},
                                                   params));
                }
                // calculating error = (b[i] - b[i]`) * k[i];
                vector<scalar, RightPart::size> error = vector<scalar, RightPart::size>::Zero();
                for (integer i = 0; i < ButcherTable::size; ++i) {
                    error += (ButcherTable::row1[i] - ButcherTable::row2[i]) * kMas[i];
                }

                const scalar errorNorm = error.norm() * step;
                if (errorNorm < intParams.tolerance) {
                    vector<scalar, RightPart::size> sum = vector<scalar, RightPart::size>::Zero();
                    for (integer i = 0; i < ButcherTable::size; ++i) {
                        sum += kMas[i] * ButcherTable::row1[i];
                    }

                    uTmp = uTmp + step * sum;
                    tTmp = tTmp + step;
                    solution.push_back({uTmp, tTmp});

                    const scalar q1 = 1.3;
                    const scalar q2 = 0.95;
                    scalar stepNew = std::min(q1 * step, intParams.stepMax);
                    stepNew = q2 * std::max(stepNew, intParams.stepMin);
                    step = stepNew;
                } else {
                    step = step * pow(intParams.tolerance / errorNorm, 1. / intParams.p);

//                    if (step < intParams.stepMin) {
//                        std::stringstream buff;
//                        buff << "The calculated step is less than the minimum step. Step = " << step << ". Minimum step = " << intParams.stepMin << ". File: " << __FILE__ << ". Line: " << __LINE__;
//                        throw ProjectException(buff.str());
//                    }
//
//                    if (step > intParams.stepMax) {
//                        std::stringstream buff;
//                        buff << "The calculated step is more than the maximum step. Step = " << step << ". Maximum step = " << intParams.stepMax << ". File: " << __FILE__ << ". Line: " << __LINE__;
//                        throw ProjectException(buff.str());
//                    }
                }
            }

            return solution;
        }
    };
}

#endif //PROJECT_BALLISTICS_NESTEDRUNGEKUTTA_HPP
