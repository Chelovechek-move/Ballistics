#include "ballistics/integrators/explicitRungeKutta/ExplicitRungeKutta.hpp"
#include "ballistics/integrators/explicitRungeKutta/ButcherTableERK4.hpp"
#include "ballistics/integrators/nestedRungeKutta/NestedRungeKutta.hpp"

#include "ballistics/forces/GravityForce.hpp"
#include "ballistics/forces/ThirdBodyGravity.hpp"
#include "ballistics/forces/SolarPressureForse.hpp"
#include "ballistics/forces/DragForce.hpp"

#include "ballistics/shadow/Shadow.hpp"
#include "ballistics/atmosphere/Atmosphere.hpp"

#include "ballistics/time/TimeConverter.hpp"
#include "ballistics/frame/FrameConverter.hpp"
#include "ballistics/ephemeris/Ephemeris.hpp"

#include "ballistics/orbit/OrbitConverter.hpp"
#include "ballistics/orbit/CartesianElements.hpp"
#include "ballistics/orbit/KeplerMeanElements.hpp"

#include <fstream>

const std::string FILE_PATH = __FILE__;
const int amountOfSymbolsBeforeRootDirectory = 44;
const std::string DIR_PATH = FILE_PATH.substr(0, FILE_PATH.size() - amountOfSymbolsBeforeRootDirectory);

using namespace Ballistics;

class quaternionCalculator {
    Frame::FrameConverter frameConverter_;
    Time::TimeConverter timeConverter_;
public:
    quaternionCalculator(const std::string &csvTimeLocation,
                         const std::string &csvFrameLocation) noexcept: frameConverter_(csvFrameLocation),
                                                                        timeConverter_(csvTimeLocation) {};

    [[nodiscard]] quaterniond calcQuaternion(Time::Time<Ballistics::Time::Scale::TT> &timeTT) const {
        const auto timeUT1 = timeConverter_.convert<Time::Scale::TT, Ballistics::Time::Scale::UT1>(timeTT);
        const auto timeUTC = timeConverter_.convert<Time::Scale::TT, Ballistics::Time::Scale::UTC>(timeTT);
        return frameConverter_.calcQuaternion<Frame::Frame::GCRS, Frame::Frame::ITRS>(timeTT,
                                                                                      timeUT1,
                                                                                      timeUTC);
    }
};

class RightPart32 {
public:
    constexpr static Ballistics::index size = 6;

    struct Params {
        scalar mass;
        scalar solarRadius;
        scalar earthRadius;
        scalar S;
        scalar TSI;
        scalar AU;
        scalar Cd;
        std::vector<integer> bodiesNumbers;
        std::vector<scalar> bodiesGravParams;
        quaternionCalculator quatCalculator;
        Time::Time<Time::Scale::TT> &timeTT;
        Time::Time<Time::Scale::UT1> &timeUT1;
        Time::Time<Time::Scale::UTC> &timeUTC;
        Time::Time<Time::Scale::TDB> &timeTDB;
        Forces::GravityForce model;
        Ephemeris::Ephemeris ephems;
        Atmosphere::Atmosphere atmosphere;
    };

    using Time = Time::Time<Time::Scale::TT>;

    struct State {
        vector<scalar, size> u;
        Time t;
    };

    [[nodiscard]] static inline vector<scalar, size> calc(const State &stateVector,
                                                          const Params &params) {
        const vector3d geoSatPos = {stateVector.u[0], stateVector.u[1], stateVector.u[2]};
        // Calculating of the gravitational force from Earth
        const quaterniond GCRStoITRSquat = params.quatCalculator.calcQuaternion(params.timeTT);
        const vector3d gravityForceEarth = params.model.calcForce(params.mass,
                                                                  geoSatPos,
                                                                  GCRStoITRSquat,
                                                                  params.timeTT,
                                                                  params.timeUT1);
        // Calculating of the gravitational force from space bodies except Earth
        std::vector<vector3d> BodyPositions;
        for (integer i = 0; i < params.bodiesNumbers.size(); ++i) {
            BodyPositions.emplace_back(params.ephems.calcCoordinates(params.timeTDB,
                                                                     params.bodiesNumbers[i],
                                                                     Ephemeris::Body::Earth) * 1000);
        }
        const vector3d gravityForceThirdBodies = Forces::calcThirdPartyGravityForce(BodyPositions,
                                                                                    params.bodiesGravParams,
                                                                                    geoSatPos,
                                                                                    params.mass);
        // Calculating of the solar pressure force
        const vector3d geoSunPos = params.ephems.calcCoordinates(params.timeTDB,
                                                                 Ephemeris::Body::Sun,
                                                                 Ephemeris::Body::Earth) * 1000;
        const vector3d helioSatPos = geoSatPos - geoSunPos;
        const scalar F = Shadow::calcShadowFunction(geoSatPos,
                                                    geoSunPos,
                                                    params.solarRadius,
                                                    params.earthRadius);
        const vector3d solarPressureForce = Forces::calcSolarPressureForce(helioSatPos,
                                                                           F,
                                                                           params.S,
                                                                           params.TSI,
                                                                           params.AU);
        // Calculating of the drag force
        const vector3d ITRSSatPos = GCRStoITRSquat._transformVector(geoSatPos);
        const vector3d dragForce = Forces::calcDragForce(params.atmosphere.getDensity(ITRSSatPos,
                                                                                      params.timeUTC),
                                                         {stateVector.u[3], stateVector.u[4], stateVector.u[5]},
                                                         params.S,
                                                         params.Cd);
        // Calculating sum of all forces
        const vector3d force = gravityForceEarth + gravityForceThirdBodies + solarPressureForce + dragForce;
        return vector<scalar, size>{stateVector.u[3],
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
                                            0.};
    const Orbit::PosVel CartesianElements = Orbit::convert<Orbit::PosVel>(meanElements, gravParam);

    // let's integrate this stuff
    const Ballistics::Time::TimeConverter timeConverter = Ballistics::Time::TimeConverter("../../resources/earth_rotation.csv");
    auto timeTT = Ballistics::Time::Time<Ballistics::Time::Scale::TT>(2459842, 0.);
    auto timeUT1 = timeConverter.convert<Ballistics::Time::Scale::TT, Ballistics::Time::Scale::UT1>(timeTT);
    auto timeUTC = timeConverter.convert<Ballistics::Time::Scale::TT, Ballistics::Time::Scale::UTC>(timeTT);
    auto timeTDB = timeConverter.convert<Ballistics::Time::Scale::TT, Ballistics::Time::Scale::TDB>(timeTT);
    const RightPart32::State initState = {vector<scalar, 6>{CartesianElements.position[0],
                                                            CartesianElements.position[1],
                                                            CartesianElements.position[2],
                                                            CartesianElements.velocity[0],
                                                            CartesianElements.velocity[1],
                                                            CartesianElements.velocity[2]}, timeTT};

    const quaternionCalculator quatCalculator = quaternionCalculator("../../resources/earth_rotation.csv",
                                                                     "../../resources/earth_rotation.csv");
    const RightPart32::Params params = {1.,
                                        695.700e+6,
                                        6.371e+6,
                                        0.1,
                                        1366,
                                        149597870700,
                                        2.2,
                                        {Ephemeris::Body::Sun, Ephemeris::Body::MarsBarycenter,
                                         Ephemeris::Body::Moon, Ephemeris::Body::JupiterBarycenter,
                                         Ephemeris::Body::SaturnBarycenter},
                                        {1.3271244e+20, 4.2828e+13,
                                         4.9028e+12, 1.26686534e+17,
                                         3.7931187e+16},
                                        quatCalculator,
                                        timeTT,
                                        timeUT1,
                                        timeUTC,
                                        timeTDB,
                                        {"egm2008", "../../resources/geographicLib", 16, 16},
                                        Ephemeris::Ephemeris(),
                                        Atmosphere::Atmosphere("../../resources/atmosphere/SW-Last5Years.csv")};
    const scalar step = 100;
    const auto tFinal = Ballistics::Time::Time<Ballistics::Time::Scale::TT>(2459843, 0.);
    const Integrators::ExplicitRK::IntegrationParameters<RightPart32> intParams = {step,
                                                                                   tFinal};
    const std::vector<RightPart32::State> solution =
            Integrators::ExplicitRK::ExplicitRungeKutta<Integrators::ExplicitRK::ButcherTableERK4>::
                    calc<RightPart32>(initState,
                                      params,
                                      intParams);
    std::ofstream out;
    out.open(DIR_PATH + "tests/utility/test32Data.txt");
    for (const auto &el: solution) {
        out << el.u[0] << " ";
        out << el.u[1] << " ";
        out << el.u[2] << std::endl;
    }

    // Uncomment next rows to automatically build a graph
    std::string filename = DIR_PATH + "tests/test32_IntegrateOrbitWithAllForces.py";
    std::string command = "python3 ";
    command += filename;
    system(command.c_str());
}
