#include "ballistics/integrators/explicitRungeKutta/ExplicitRungeKutta.hpp"
#include "ballistics/integrators/explicitRungeKutta/ButcherTableERK4.hpp"
//#include "ballistics/integrators/explicitRungeKutta/ButcherTableERK3.hpp"
//#include "ballistics/integrators/explicitRungeKutta/ButcherTableERK2.hpp"
#include "ballistics/integrators/RightPartOfDiffEq.hpp"
#include <iostream>
#include <fstream>

int main() {
    using namespace Ballistics;

    const scalar step = 0.001;
    const scalar tFinal = 4 * M_PI;
    const Integrators::RightPart4::State initState = {vector<scalar, 1>{0.}, 0.};
    const Integrators::RightPart4::Params params = {0.};
    const Integrators::ExplicitRK::IntegrationParameters<Integrators::RightPart4> intParams = {step,
                                                                                               tFinal};
    const std::vector<Integrators::RightPart4::State> solution =
            Integrators::ExplicitRK::ExplicitRungeKutta<Integrators::ExplicitRK::ButcherTableERK4>::
                    calc<Integrators::RightPart4>(initState,
                                                  params,
                                                  intParams);

    std::ofstream out;
    out.open("/home/vladimir/Ballistics_Project/ballistics_chiniaev/tests/utility/test22Data.txt");
    for (const auto &el : solution)
    {
        out << el.t << " ";
        out << el.u[0] << std::endl;
    }
}