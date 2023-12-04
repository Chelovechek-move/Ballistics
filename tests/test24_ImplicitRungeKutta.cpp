#include "ballistics/integrators/implicitRungeKutta/ImplicitRungeKutta.hpp"
#include "ballistics/integrators/implicitRungeKutta/ButcherTableIRK6.hpp"
#include "ballistics/integrators/RightPartOfDiffEq.hpp"
#include <fstream>

int main() {
    using namespace Ballistics;

    const scalar step = 0.1;
    const scalar tFinal = 4 * M_PI;
    const scalar toleranceOfSNAE = 1e-15;
    const Integrators::RightPart4::State initState = {vector<scalar, 1>{0.}, 0.};
    const Integrators::RightPart4::Params params = {0.};
    const Integrators::ImplicitRK::IntegrationParameters<Integrators::RightPart4> intParams = {step, tFinal, toleranceOfSNAE};
    const std::vector<Integrators::RightPart4::State> solution =
            Integrators::ImplicitRK::ImplicitRungeKutta<Integrators::ImplicitRK::ButcherTableIRK6>::
                    calc<Integrators::RightPart4>(initState,
                                                  params,
                                                  intParams);
    std::ofstream out;
    out.open("/home/vladimir/Ballistics_Project/ballistics_chiniaev/tests/utility/test24Data.txt");
    for (const auto &el : solution)
    {
        out << el.t << " ";
        out << el.u[0] << std::endl;
    }
}