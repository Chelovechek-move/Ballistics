//#include "utility/satelliteData.h"
#include "ballistics/GNSS/PositionDetermination.h"
#include "ballistics/GNSS/RinexParser.h"
#include "ballistics/GNSS/SP3Parser.h"
#include <iostream>

using namespace Ballistics;

int main() {
    const integer numberOfSatellites = 4;
    const std::string time = "2022-11-20 23:59:00";
    std::vector<GNSS::externalParameters> externalData(numberOfSatellites);
    struct RinexData {
        scalar C1W;
        scalar C2W;
        scalar L1W;
        scalar L2W;
    };
    const Rinex::RinexParser G13_Rinex("/home/vladimir/CLionProjects/ballistics_chiniaev/resources/GNSS/RINEX/G13_RINEX.csv",
                           {"C1W", "C2W", "L1W", "L2W"});
    const Rinex::RinexParser G15_Rinex("/home/vladimir/CLionProjects/ballistics_chiniaev/resources/GNSS/RINEX/G15_RINEX.csv",
                           {"C1W", "C2W", "L1W", "L2W"});
    const Rinex::RinexParser G16_Rinex("/home/vladimir/CLionProjects/ballistics_chiniaev/resources/GNSS/RINEX/G16_RINEX.csv",
                           {"C1W", "C2W", "L1W", "L2W"});
    const Rinex::RinexParser G18_Rinex("/home/vladimir/CLionProjects/ballistics_chiniaev/resources/GNSS/RINEX/G18_RINEX.csv",
                           {"C1W", "C2W", "L1W", "L2W"});
    const std::vector<Rinex::RinexParser> satellites_RINEX = {G13_Rinex, G15_Rinex, G16_Rinex, G18_Rinex};
    std::vector<RinexData> rowRinexData;
    for (auto sat : satellites_RINEX) {
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

    struct SP3Data {
        vector3d position;
        scalar time;
    };

    const SP3::SP3Parser G13_SP3("/home/vladimir/CLionProjects/ballistics_chiniaev/resources/GNSS/SP3/G13_SP3.csv");
    const SP3::SP3Parser G15_SP3("/home/vladimir/CLionProjects/ballistics_chiniaev/resources/GNSS/SP3/G15_SP3.csv");
    const SP3::SP3Parser G16_SP3("/home/vladimir/CLionProjects/ballistics_chiniaev/resources/GNSS/SP3/G16_SP3.csv");
    const SP3::SP3Parser G18_SP3("/home/vladimir/CLionProjects/ballistics_chiniaev/resources/GNSS/SP3/G18_SP3.csv");

    const std::vector<SP3::SP3Parser> satellites_SP3 = {G13_SP3, G15_SP3, G16_SP3, G18_SP3};
    std::vector<SP3Data> rowSP3Data;
    for (auto sat : satellites_SP3) {
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

    const GNSS::determinedParameters& initialApproximation = {0., 0., 0., 0.};
    const scalar sigma = 1.;
    const scalar sigmaSqRev = 1. / (sigma * sigma);
    vector<scalar, numberOfSatellites> sigmaVector;
    for (int i = 0; i < numberOfSatellites; ++i) {
        sigmaVector[i] = sigmaSqRev;
    }
    const diagMatrix<scalar, numberOfSatellites, numberOfSatellites> P(sigmaVector);
    const scalar tolerance = 1e-10;

    auto const [result, P1] = GNSS::determinePosition<numberOfSatellites>(externalData,
                                                                          initialApproximation,
                                                                          P,
                                                                          tolerance);
    std::cout << result.deltaXr << std::endl << result.deltaYr << std::endl << result.deltaZr << std::endl << result.deltaTr << std::endl;
}


