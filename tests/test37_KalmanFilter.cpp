//#include "utility/satelliteData.h"
#include "ballistics/GNSS/PositionDetermination.h"
#include "ballistics/GNSS/RinexParser.h"
#include "ballistics/GNSS/SP3Parser.h"
#include <iostream>
#include <fstream>

using namespace Ballistics;

struct RinexData {
    scalar C1W;
    scalar C2W;
    scalar L1W;
    scalar L2W;
};

struct SP3Data {
    vector3d position;
    scalar time;
};

const Rinex::RinexParser G13_Rinex(
        "/home/vladimir/CLionProjects/ballistics_chiniaev/resources/GNSS/RINEX/G13_RINEX.csv",
        {"C1W", "C2W", "L1W", "L2W"});
const Rinex::RinexParser G15_Rinex(
        "/home/vladimir/CLionProjects/ballistics_chiniaev/resources/GNSS/RINEX/G15_RINEX.csv",
        {"C1W", "C2W", "L1W", "L2W"});
const Rinex::RinexParser G16_Rinex(
        "/home/vladimir/CLionProjects/ballistics_chiniaev/resources/GNSS/RINEX/G16_RINEX.csv",
        {"C1W", "C2W", "L1W", "L2W"});
const Rinex::RinexParser G18_Rinex(
        "/home/vladimir/CLionProjects/ballistics_chiniaev/resources/GNSS/RINEX/G18_RINEX.csv",
        {"C1W", "C2W", "L1W", "L2W"});

const SP3::SP3Parser G13_SP3("/home/vladimir/CLionProjects/ballistics_chiniaev/resources/GNSS/SP3/G13_SP3.csv");
const SP3::SP3Parser G15_SP3("/home/vladimir/CLionProjects/ballistics_chiniaev/resources/GNSS/SP3/G15_SP3.csv");
const SP3::SP3Parser G16_SP3("/home/vladimir/CLionProjects/ballistics_chiniaev/resources/GNSS/SP3/G16_SP3.csv");
const SP3::SP3Parser G18_SP3("/home/vladimir/CLionProjects/ballistics_chiniaev/resources/GNSS/SP3/G18_SP3.csv");

const std::vector<Rinex::RinexParser> satellites_RINEX = {G13_Rinex, G15_Rinex, G16_Rinex, G18_Rinex};
const std::vector<SP3::SP3Parser> satellites_SP3 = {G13_SP3, G15_SP3, G16_SP3, G18_SP3};

const integer numberOfSatellites = 4;

auto closure(unsigned int const N,
             std::string const &init) noexcept -> std::vector<std::pair<std::string, std::vector<GNSS::externalParameters>>> {
    auto const rinexTimes = G13_Rinex.timeColumn;
    auto const sp3Times = G13_SP3.dateString_;
    auto const itR = std::find(G13_Rinex.timeColumn.begin(), G13_Rinex.timeColumn.end(), init);
    assert(itR != G13_Rinex.timeColumn.end());
    unsigned int const startR = itR - G13_Rinex.timeColumn.begin();

    auto const itS = std::find(G13_SP3.dateString_.begin(), G13_SP3.dateString_.end(), init);
    assert(itS != G13_SP3.dateString_.end());
    unsigned int const startS = itS - G13_SP3.dateString_.begin();

    std::vector<std::pair<std::string, std::vector<GNSS::externalParameters>>> res;

    for (unsigned int j = 0; j < N; ++j) {
        std::vector<GNSS::externalParameters> externalData(numberOfSatellites);
        std::vector<RinexData> rowRinexData;
        for (auto sat: satellites_RINEX) {
            rowRinexData.push_back({sat.getObservation("C1W", rinexTimes[j + startR]),
                                    sat.getObservation("C2W", rinexTimes[j + startR]),
                                    sat.getObservation("L1W", rinexTimes[j + startR]),
                                    sat.getObservation("L2W", rinexTimes[j + startR])});
        }
        const scalar frequencyL1 = 1575.42 * 1e6;
        const scalar frequencyL2 = 1227.6 * 1e6;

        std::vector<scalar> P_IF(numberOfSatellites);
        for (int i = 0; i < numberOfSatellites; ++i) {
            P_IF[i] = (frequencyL1 * frequencyL1 * rowRinexData[i].C1W -
                       frequencyL2 * frequencyL2 * rowRinexData[i].C2W) /
                      (frequencyL1 * frequencyL1 - frequencyL2 * frequencyL2);
        }


        std::vector<SP3Data> rowSP3Data;
        for (auto sat: satellites_SP3) {
            const std::pair<vector3d, scalar> observation = sat.getObservation(sp3Times[j + startS]);
            rowSP3Data.push_back({observation.first, observation.second});
        }

        for (int i = 0; i < numberOfSatellites; ++i) {
            externalData[i] = {P_IF[i],
                               rowSP3Data[i].position.x() * 1000,
                               rowSP3Data[i].position.y() * 1000,
                               rowSP3Data[i].position.z() * 1000,
                               rowSP3Data[i].time * 1e-6};
        }

        res.emplace_back(rinexTimes[j + startR], externalData);
    }
    return res;
}


int main() {
    std::ofstream out1;
    out1.open("/home/vladimir/PycharmProjects/SatelliteSystems_Testing/resourses/lol.txt");
    const std::string time = "2022-11-20 23:30:00";

    const GNSS::determinedParameters &initialApproximation = {0., 0., 0., 0.};
    const scalar sigma = 1.;
    const scalar sigmaSqRev = 1. / (sigma * sigma);
    vector<scalar, numberOfSatellites> sigmaVector;
    for (int i = 0; i < numberOfSatellites; ++i) {
        sigmaVector[i] = sigmaSqRev;
    }
    const diagMatrix<scalar, numberOfSatellites, numberOfSatellites> P(sigmaVector);
    const scalar tolerance = 1e-10;

    std::vector<GNSS::externalParameters> externalData(numberOfSatellites);
    std::vector<RinexData> rowRinexData;
    for (auto sat: satellites_RINEX) {
        rowRinexData.push_back({sat.getObservation("C1W", time),
                                sat.getObservation("C2W", time),
                                sat.getObservation("L1W", time),
                                sat.getObservation("L2W", time)});
    }
    const scalar frequencyL1 = 1575.42 * 1e6;
    const scalar frequencyL2 = 1227.6 * 1e6;

    std::vector<scalar> P_IF(numberOfSatellites);
    for (int i = 0; i < numberOfSatellites; ++i) {
        P_IF[i] = (frequencyL1 * frequencyL1 * rowRinexData[i].C1W - frequencyL2 * frequencyL2 * rowRinexData[i].C2W) /
                  (frequencyL1 * frequencyL1 - frequencyL2 * frequencyL2);
    }


    std::vector<SP3Data> rowSP3Data;
    for (auto sat: satellites_SP3) {
        const std::pair<vector3d, scalar> observation = sat.getObservation(time);
        rowSP3Data.push_back({observation.first, observation.second});
    }

    for (int i = 0; i < numberOfSatellites; ++i) {
        externalData[i] = {P_IF[i],
                           rowSP3Data[i].position.x() * 1000,
                           rowSP3Data[i].position.y() * 1000,
                           rowSP3Data[i].position.z() * 1000,
                           rowSP3Data[i].time * 1e-6};
    }

    auto const [result, P1] = GNSS::determinePosition<numberOfSatellites>(externalData,
                                                                          initialApproximation,
                                                                          P,
                                                                          tolerance);
    std::cout << result.deltaXr << std::endl << result.deltaYr << std::endl << result.deltaZr << std::endl
              << result.deltaTr << std::endl;


    auto const I = matrix<scalar, 4, 4>::Identity();
    auto const doFilterStep = [&](vector4d const &xp, matrix<scalar, 4, 4> const &pp, std::string const &epoch,
                                  std::vector<GNSS::externalParameters> const &params) noexcept
            -> std::pair<vector4d, matrix<scalar, 4, 4>> {
        matrix<scalar, numberOfSatellites, 4> A;
        vector4d x = vector4d{0., 0., 0., 0.};
        vector<scalar, numberOfSatellites> b;
        for (int i = 0; i < numberOfSatellites; ++i) {
            auto const [pr, dx, dy, dz, dt] = params[i];
            A.row(i) = vector<scalar, 4>{calcGeometricDistanceDerivative(x.head(3),
                                                                         vector3d{dx, dy, dz},
                                                                         GNSS::DifferentiationVariable::deltaX),
                                         calcGeometricDistanceDerivative(x.head(3),
                                                                         vector3d{dx, dy, dz},
                                                                         GNSS::DifferentiationVariable::deltaY),
                                         calcGeometricDistanceDerivative(x.head(3),
                                                                         vector3d{dx, dy, dz},
                                                                         GNSS::DifferentiationVariable::deltaZ),
                                         speedOfLight
            };
            const scalar correction = GNSS::calcGeometricDistance(x.head(3), {dx, dy, dz}) - speedOfLight * dt;
            b[i] = pr - correction;
        }


        auto const Pnew = pp;
        auto const K = Pnew * A.transpose() * (I + A * Pnew * A.transpose()).inverse();
        auto const tmp = I - K * A;
        return {xp + K * (b - A * xp), tmp * Pnew};
    };

    vector4d xPrev = GNSS::toVector(result);
    matrix<scalar, 4, 4> PPrev = P1;

    auto const from = closure(40u, time);
    for (auto const &group: from) {
        auto const [newX, newP] = doFilterStep(xPrev, PPrev, group.first, group.second);

        out1 << (xPrev.head(3) - newX.head(3)).norm() << ";" << group.first << std::endl;

        xPrev = newX;
        PPrev = newP;
        std::cout << "X / P: " << std::endl;
        std::cout << xPrev << std::endl;
        std::cout << PPrev << std::endl;
        std::cout << "----------------------------------------" << std::endl;
    }
    out1.close();
}