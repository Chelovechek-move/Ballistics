#include "ballistics/integrators/explicitRungeKutta/ExplicitRungeKutta.hpp"
#include "ballistics/integrators/explicitRungeKutta/ButcherTableERK4.hpp"
#include "ballistics/integrators/explicitRungeKutta/ButcherTableERK3.hpp"
#include "ballistics/integrators/explicitRungeKutta/ButcherTableERK2.hpp"
#include "ballistics/integrators/RightPartOfDiffEq.hpp"
#include <iostream>
#include <fstream>

[[nodiscard]] Ballistics::scalar max(const std::vector<Ballistics::scalar> &mas) {
    Ballistics::scalar max = mas[0];
    for (const auto el : mas) {
        if (el > max) {
            max = el;
        }
    }
    return max;
}

struct Point {
    Ballistics::scalar error;
    Ballistics::scalar step;
};

int main() {
    using namespace Ballistics;

    std::vector<Point> res{};

    const scalar stepInit = 0.1;
    const scalar stepFinal = 1;
    const Ballistics::index numberOfSteps = 1000;
    const scalar stepDelta = (stepFinal - stepInit) / numberOfSteps;

    scalar step = stepInit;
    const scalar tFinal = 4 * M_PI;

    const Integrators::RightPart4::State initState = {vector<scalar, 1>{0.}, 0.};

    for (integer i = 0; i < numberOfSteps; ++i) {
        const auto numOfIter = static_cast<Ballistics::index>(abs(initState.t - tFinal) / step);
        const Integrators::ExplicitRK::IntegrationParameters<Integrators::RightPart4> intParams = {step,
                                                                                                   tFinal};
        const Integrators::RightPart4::Params params = {0.};
        const std::vector<Integrators::RightPart4::State> solution =
                Integrators::ExplicitRK::ExplicitRungeKutta<Integrators::ExplicitRK::ButcherTableERK4>::
                        calc<Integrators::RightPart4>(initState,
                                                      params,
                                                      intParams);
        std::vector<scalar> exactSolution{};
        for (integer j = 0; j <= numOfIter; ++j) {
            exactSolution.push_back(sin(step * j));
        }

        std::vector<scalar> delta{};
        for (integer j = 0; j <= numOfIter; ++j) {
            delta.push_back(abs(solution[j].u[0] - exactSolution[j]));
        }

        const scalar maxError = max(delta);

        res.push_back({maxError, step});

        step += stepDelta;
    }

    std::ofstream out;
    out.open("/home/vladimir/Ballistics_Project/ballistics_chiniaev/tests/utility/test23Data.txt");
    for (const auto el : res)
    {
        out << el.error << " ";
        out << el.step << std::endl;
    }
}