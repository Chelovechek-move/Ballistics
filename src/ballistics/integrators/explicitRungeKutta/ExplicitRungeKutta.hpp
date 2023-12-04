#ifndef PROJECT_BALLISTICS_EXPLICITRUNGEKUTTA_HPP
#define PROJECT_BALLISTICS_EXPLICITRUNGEKUTTA_HPP

#include "ballistics/types/BasicTypes.hpp"
#include <vector>

namespace Ballistics::Integrators::ExplicitRK
{
    template<typename RightPart>
    struct IntegrationParameters
    {
        const scalar step;
        const typename RightPart::Time tFinal;
    };

    template<typename ButcherTable>
    class ExplicitRungeKutta
    {
    public:
        template<typename RightPart>
        [[nodiscard]] static inline bool correctStep(scalar &currentStep,
                                                     const scalar step,
                                                     const typename RightPart::Time t,
                                                     const typename RightPart::Time tFinal)
        {
            if (tFinal - t < step)
            {
                currentStep = tFinal - t;
            }

            return t < tFinal;
        }

        template<typename RightPart>
        [[nodiscard]] static inline std::vector<typename RightPart::State> calc(const typename RightPart::State &initState,
                                                                                const typename RightPart::Params &params,
                                                                                const IntegrationParameters<RightPart> &intParams)
        {
            vector<scalar, RightPart::size> uTmp = initState.u;
            typename RightPart::Time tTmp = initState.t;
            std::vector<typename RightPart::State> solution = {initState};
            scalar currentStep = intParams.step;

            while(correctStep<RightPart>(currentStep, intParams.step, tTmp, intParams.tFinal))
            {
                // Calculating k[i]
                std::vector<vector<scalar, RightPart::size>> kMas = {RightPart::calc({uTmp, tTmp}, params)};
                index n = 0;
                for (integer i = 1; i < ButcherTable::size; ++i)
                {
                    vector<scalar, RightPart::size> sum = vector<scalar, RightPart::size>::Zero();
                    for (integer k = 0; k < i; ++k)
                    {
                        sum += ButcherTable::matrix[n + k] * kMas[k];
                    }
                    n += i;
                    kMas.push_back(RightPart::calc({uTmp + sum * currentStep, tTmp + ButcherTable::column[i] * currentStep},
                                                   params));
                }
                // Calculating sum k[i] * b[i];
                vector<scalar, RightPart::size> sum = vector<scalar, RightPart::size>::Zero();
                for (integer i = 0; i < ButcherTable::size; ++i)
                {
                    sum += kMas[i] * ButcherTable::row[i];
                }

                uTmp = uTmp + currentStep * sum;
                tTmp = tTmp + currentStep;

                solution.push_back({uTmp, tTmp});
            }

            return solution;
        }
    };
}

#endif //PROJECT_BALLISTICS_EXPLICITRUNGEKUTTA_HPP
