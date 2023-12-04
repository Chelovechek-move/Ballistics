#include "ballistics/integrators/explicitRungeKutta/ExplicitRungeKutta.hpp"
#include "ballistics/integrators/explicitRungeKutta/ButcherTableERK4.hpp"
#include "ballistics/integrators/implicitRungeKutta/ImplicitRungeKutta.hpp"
#include "ballistics/integrators/implicitRungeKutta/ButcherTableIRK6.hpp"
#include "ballistics/integrators/nestedRungeKutta/NestedRungeKutta.hpp"
#include "ballistics/integrators/nestedRungeKutta/ButcherTableNRK5_4.hpp"
#include "ballistics/integrators/RightPartOfDiffEq.hpp"

#include "ballistics/orbit/OrbitConverter.hpp"
#include "ballistics/orbit/CartesianElements.hpp"
#include "ballistics/orbit/KeplerMeanElements.hpp"

#include <fstream>

int main() {
    using namespace Ballistics;
    // initial values
    const scalar gravParam = 1;
    const Orbit::KeplerMean meanElements = {5.,
                                            0.,
                                            2.,
                                            0.8,
                                            0.,
                                            0.
    };
    const Orbit::PosVel CartesianElements = Orbit::convert<Orbit::PosVel>(meanElements, gravParam);
    // let's integrate this stuff
    const Integrators::RightPart5::State initState = {vector<scalar, 6>{CartesianElements.position[0],
                                                                        CartesianElements.position[1],
                                                                        CartesianElements.position[2],
                                                                        CartesianElements.velocity[0],
                                                                        CartesianElements.velocity[1],
                                                                        CartesianElements.velocity[2]}, 0.};
    const Integrators::RightPart5::Params params = {gravParam};
    const scalar step1 = 0.1;
    const scalar tFinal = 18;
    const Integrators::ExplicitRK::IntegrationParameters<Integrators::RightPart5> intParams1 = {step1,
                                                                                                tFinal};
    const std::vector<Integrators::RightPart5::State> solution1 =
            Integrators::ExplicitRK::ExplicitRungeKutta<Integrators::ExplicitRK::ButcherTableERK4>::
                    calc<Integrators::RightPart5>(initState,
                                                  params,
                                                  intParams1);
    const scalar step2 = 0.4;
    const scalar toleranceOfSNAE = 1e-15;
    const Integrators::ImplicitRK::IntegrationParameters<Integrators::RightPart5> intParams2 = {step2,
                                                                                                tFinal,
                                                                                                toleranceOfSNAE};
    const std::vector<Integrators::RightPart5::State> solution2 =
            Integrators::ImplicitRK::ImplicitRungeKutta<Integrators::ImplicitRK::ButcherTableIRK6>::
                    calc<Integrators::RightPart5>(initState,
                                                  params,
                                                  intParams2);
    const scalar stepMin = 0.1;
    const scalar stepMax = 2.;
    const scalar tolerance = 1e-3;
    const scalar p = 5;
    const Integrators::NestedRungeKutta::IntegrationParameters<Integrators::RightPart5> intParams3 = {tFinal,
                                                                                                      stepMin,
                                                                                                      stepMax,
                                                                                                      tolerance,
                                                                                                      p};
    const std::vector<Integrators::RightPart5::State> solution3 =
            Integrators::NestedRungeKutta::NestedRungeKutta<Integrators::NestedRungeKutta::ButcherTableNRK5_4>::
                    calc<Integrators::RightPart5>(initState,
                                                  params,
                                                  intParams3);

    std::ofstream out;
    out.open("/home/vladimir/Ballistics_Project/ballistics_chiniaev/tests/utility/test28Data.txt");

    for (const auto &el : solution1) {
        out << el.u[0] << " ";
    }
    out << "/n";
    for (const auto &el : solution1) {
        out << el.u[1] << " ";
    }
    out << "/n";
    for (const auto &el : solution1) {
        out << el.u[2] << " ";
    }
    out << "/n";
    for (const auto &el : solution2) {
        out << el.u[0] << " ";
    }
    out << "/n";
    for (const auto &el : solution2) {
        out << el.u[1] << " ";
    }
    out << "/n";
    for (const auto &el : solution2) {
        out << el.u[2] << " ";
    }
    out << "/n";
    for (const auto &el : solution3) {
        out << el.u[0] << " ";
    }
    out << "/n";
    for (const auto &el : solution3) {
        out << el.u[1] << " ";
    }
    out << "/n";
    for (const auto &el : solution3) {
        out << el.u[2] << " ";
    }
}