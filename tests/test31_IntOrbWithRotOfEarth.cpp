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
#include "ballistics/frame/FrameConverter.hpp"
#include <functional>
#include <fstream>

const std::string FILE_PATH = __FILE__;
const int amountOfSymbolsBeforeRootDirectory = 37;
const std::string DIR_PATH = FILE_PATH.substr(0, FILE_PATH.size() - amountOfSymbolsBeforeRootDirectory);

using namespace Ballistics;

class quaternionCalculator {
    Frame::FrameConverter frameConverter_;
    Time::TimeConverter timeConverter_;
public:
    quaternionCalculator(const std::string &csvTimeLocation,
                         const std::string &csvFrameLocation) noexcept: frameConverter_(csvFrameLocation),
                                                                        timeConverter_(csvTimeLocation) {};

    [[nodiscard]] const quaterniond calcQuaternion(Time::Time<Ballistics::Time::Scale::TT> &timeTT) const {
        auto timeUT1 = timeConverter_.convert<Time::Scale::TT, Ballistics::Time::Scale::UT1>(timeTT);
        auto timeUTC = timeConverter_.convert<Time::Scale::TT, Ballistics::Time::Scale::UTC>(timeTT);
        return frameConverter_.calcQuaternion<Frame::Frame::GCRS, Frame::Frame::ITRS>(timeTT,
                                                                                      timeUT1,
                                                                                      timeUTC);
    }
};

class RightPart31 {
public:
    constexpr static Ballistics::index size = 6;

    struct Params {
        Ballistics::scalar mass;
        quaternionCalculator quatCalculator;
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
                                                                  params.quatCalculator.calcQuaternion(params.timeTT),
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
    // initial values
    const scalar gravParam = 3.986004415e14;
    const Orbit::KeplerMean meanElements = {0.,
                                            0.,
                                            6800e3,
                                            0.,
                                            0.,
                                            0.
    };
    const Orbit::PosVel CartesianElements = Orbit::convert<Orbit::PosVel>(meanElements, gravParam);

    // let's integrate this stuff
    const Ballistics::Time::TimeConverter timeConverter = Ballistics::Time::TimeConverter("../../resources/earth_rotation.csv");
    auto timeTT = Ballistics::Time::Time<Ballistics::Time::Scale::TT>(2459842, 0.);
    auto timeUT1 = timeConverter.convert<Ballistics::Time::Scale::TT, Ballistics::Time::Scale::UT1>(timeTT);
    const RightPart31::State initState = {vector<scalar, 6>{CartesianElements.position[0],
                                                      CartesianElements.position[1],
                                                      CartesianElements.position[2],
                                                      CartesianElements.velocity[0],
                                                      CartesianElements.velocity[1],
                                                      CartesianElements.velocity[2]}, timeTT};

    const quaternionCalculator quatCalculator = quaternionCalculator("../../resources/earth_rotation.csv",
                                                                     "../../resources/earth_rotation.csv");
    const RightPart31::Params params = {1.,
                                        quatCalculator,
                                        timeTT,
                                        timeUT1,
                                        {"egm2008", "../../resources/geographicLib", 16, 16}};
    const scalar step = 100;
    const auto tFinal = Ballistics::Time::Time<Ballistics::Time::Scale::TT>(2459843, 0.);
    const Ballistics::Integrators::ExplicitRK::IntegrationParameters<RightPart31> intParams = {step,
                                                                                               tFinal};
    const std::vector<RightPart31::State> solution =
            Integrators::ExplicitRK::ExplicitRungeKutta<Integrators::ExplicitRK::ButcherTableERK4>::
                    calc<RightPart31>(initState,
                                      params,
                                      intParams);
    std::ofstream out;
    out.open("/home/vladimir/Ballistics_Project/ballistics_chiniaev/tests/utility/test31Data.txt");
    for (const auto &el: solution) {
        out << el.u[0] << " ";
        out << el.u[1] << " ";
        out << el.u[2] << std::endl;
    }

    // Uncomment next rows to automatically build a graph
//    std::string filename = DIR_PATH + "tests/test31_IntOrbWithRotOfEarth.py";
//    std::string command = "python3 ";
//    command += filename;
//    system(command.c_str());
}
