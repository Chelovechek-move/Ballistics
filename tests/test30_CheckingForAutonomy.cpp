#include "ballistics/integrators/explicitRungeKutta/ExplicitRungeKutta.hpp"
#include "ballistics/integrators/explicitRungeKutta/ButcherTableERK4.hpp"
#include "ballistics/integrators/implicitRungeKutta/ImplicitRungeKutta.hpp"
#include "ballistics/integrators/implicitRungeKutta/ButcherTableIRK6.hpp"
#include "ballistics/integrators/nestedRungeKutta/NestedRungeKutta.hpp"
#include "ballistics/integrators/nestedRungeKutta/ButcherTableNRK5_4.hpp"
#include "ballistics/integrators/RightPartOfDiffEq.hpp"
#include "ballistics/forces/GravityForce.hpp"
#include "../src/ballistics/time/TimeConverter.hpp"

#include "ballistics/orbit/OrbitConverter.hpp"
#include "ballistics/orbit/CartesianElements.hpp"
#include "ballistics/orbit/KeplerMeanElements.hpp"

#include <fstream>

class RightPart30 {
public:
    constexpr static Ballistics::index size = 6;

    struct Params {
        Ballistics::scalar mass;
        Ballistics::quaterniond q;
        Ballistics::Time::Time<Ballistics::Time::Scale::TT> &timeTT;
        Ballistics::Time::Time<Ballistics::Time::Scale::UT1> &timeUT1;
        Ballistics::Forces::GravityForce model;
    };

    using Time = Ballistics::Time::Time<Ballistics::Time::Scale::TT>;

    struct State {
        Ballistics::vector<Ballistics::scalar, size> u;
        Time t;
    };

    [[nodiscard]] static inline Ballistics::vector<Ballistics::scalar, size> calc(const State &stateVector,
                                                                                  const Params &params) {
        const Ballistics::vector3d force = params.model.calcForce(params.mass,
                                                                  {stateVector.u[0], stateVector.u[1], stateVector.u[2]},
                                                                  params.q,
                                                                  params.timeTT,
                                                                  params.timeUT1);
        return Ballistics::vector<Ballistics::scalar, size>{stateVector.u[3],
                                                            stateVector.u[4],
                                                            stateVector.u[5],
                                                            force[0] / params.mass,
                                                            force[1] / params.mass,
                                                            force[2] / params.mass};
    };
};

int main() {
    using namespace Ballistics;
    // initial values
    const scalar gravParam =  3.986004415e14;
    const Orbit::KeplerMean meanElements = {0.,
                                            0.,
                                            6800e3,
                                            0.,
                                            0.,
                                            0.
    };
    const Orbit::PosVel CartesianElements = Orbit::convert<Orbit::PosVel>(meanElements, gravParam);

    // let's integrate this stuff
    const Ballistics::Time::TimeConverter converter = Ballistics::Time::TimeConverter("../../resources/earth_rotation.csv");
    auto timeTT = Ballistics::Time::Time<Ballistics::Time::Scale::TT>(2459842, 0.);
    auto timeUT1 = converter.convert<Ballistics::Time::Scale::TT, Ballistics::Time::Scale::UT1>(timeTT);
    const RightPart30::State initState = {vector<scalar, 6>{CartesianElements.position[0],
                                                            CartesianElements.position[1],
                                                            CartesianElements.position[2],
                                                            CartesianElements.velocity[0],
                                                            CartesianElements.velocity[1],
                                                            CartesianElements.velocity[2]}, timeTT};
    const RightPart30::Params params = {1.,
                                        {1., 0., 0., 0.},
                                        timeTT,
                                        timeUT1,
                                        {"egm2008", "../../resources/geographicLib", 16, 16}};
    const scalar step = 100;
    const auto tFinal = Ballistics::Time::Time<Ballistics::Time::Scale::TT>(2459892, 0.);
    const Ballistics::Integrators::ExplicitRK::IntegrationParameters<RightPart30> intParams = {step,
                                                                                               tFinal};
    const std::vector<RightPart30::State> solution =
            Integrators::ExplicitRK::ExplicitRungeKutta<Integrators::ExplicitRK::ButcherTableERK4>::
                    calc<RightPart30>(initState,
                                    params,
                                    intParams);

    std::ofstream out;
    out.open("/home/vladimir/Ballistics_Project/ballistics_chiniaev/tests/utility/test30Data.txt");

    std::vector<scalar> res;
    const vector<scalar, 3> vInit = {solution[0].u[3],
                                     solution[0].u[4],
                                     solution[0].u[5]};
    const vector<scalar, 3> rInit = {solution[0].u[0],
                                     solution[0].u[1],
                                     solution[0].u[2]};
    const scalar initH = vInit.squaredNorm() - (2 * gravParam) / rInit.norm();
    for (integer i = 0; i < solution.size(); ++i) {
        const vector<scalar, 3> v = {solution[i].u[3],
                                     solution[i].u[4],
                                     solution[i].u[5]};
        const vector<scalar, 3> r = {solution[i].u[0],
                                     solution[i].u[1],
                                     solution[i].u[2]};
        const scalar H = v.squaredNorm() - (2 * gravParam) / r.norm();
        out << abs(H - initH) / abs(H) << " " << (solution[i].t - solution[0].t) / 86400 << std::endl;
    }
}