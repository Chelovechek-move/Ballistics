//#include "utility/satelliteData.h"
#include "ballistics/GNSS/PositionDetermination.h"
#include "ballistics/GNSS/RinexParser.h"
#include "ballistics/GNSS/SP3Parser.h"
#include <iostream>

const std::string FILE_PATH = __FILE__;
const int amountOfSymbolsBeforeRootDirectory = 38;
const std::string DIR_PATH = FILE_PATH.substr(0, FILE_PATH.size() - amountOfSymbolsBeforeRootDirectory);

using namespace Ballistics;

int main() {
    const integer numberOfSatellites = 4;
    const std::string time1 = "2022-11-20 23:41:00";
    std::vector<GNSS::externalParameters> externalData1(numberOfSatellites);
    const std::string time2 = "2022-11-20 23:40:30";
    std::vector<GNSS::externalParameters> externalData2(numberOfSatellites);
    struct RinexData {
        scalar C1W;
        scalar C2W;
        scalar L1W;
        scalar L2W;
    };
    const Rinex::RinexParser G13_Rinex(DIR_PATH + "resources/GNSS/RINEX/G13_RINEX.csv",
                                       {"C1W", "C2W", "L1W", "L2W"});
    const Rinex::RinexParser G15_Rinex(DIR_PATH + "resources/GNSS/RINEX/G15_RINEX.csv",
                                       {"C1W", "C2W", "L1W", "L2W"});
    const Rinex::RinexParser G16_Rinex(DIR_PATH + "resources/GNSS/RINEX/G16_RINEX.csv",
                                       {"C1W", "C2W", "L1W", "L2W"});
    const Rinex::RinexParser G18_Rinex(DIR_PATH + "resources/GNSS/RINEX/G18_RINEX.csv",
                                       {"C1W", "C2W", "L1W", "L2W"});
    const std::vector<Rinex::RinexParser> satellites_RINEX = {G13_Rinex, G15_Rinex, G16_Rinex, G18_Rinex};
    std::vector<RinexData> rowRinexData1;
    for (auto sat: satellites_RINEX) {
        rowRinexData1.push_back({sat.getObservation("C1W", time1),
                                 sat.getObservation("C2W", time1),
                                 sat.getObservation("L1W", time1),
                                 sat.getObservation("L2W", time1)});
    }
    std::vector<RinexData> rowRinexData2;
    for (auto sat: satellites_RINEX) {
        rowRinexData2.push_back({sat.getObservation("C1W", time2),
                                 sat.getObservation("C2W", time2),
                                 sat.getObservation("L1W", time2),
                                 sat.getObservation("L2W", time2)});
    }
    const scalar frequencyL1 = 1575.42 * 1e6;
    const scalar frequencyL2 = 1227.6 * 1e6;

    std::vector<scalar> P_IF1(numberOfSatellites);
    for (int i = 0; i < numberOfSatellites; ++i) {
        P_IF1[i] = (frequencyL1 * frequencyL1 * rowRinexData1[i].C1W -
                    frequencyL2 * frequencyL2 * rowRinexData1[i].C2W) /
                   (frequencyL1 * frequencyL1 - frequencyL2 * frequencyL2);
    }
    std::vector<scalar> P_IF2(numberOfSatellites);
    for (int i = 0; i < numberOfSatellites; ++i) {
        P_IF2[i] = (frequencyL1 * frequencyL1 * rowRinexData2[i].C1W -
                    frequencyL2 * frequencyL2 * rowRinexData2[i].C2W) /
                   (frequencyL1 * frequencyL1 - frequencyL2 * frequencyL2);
    }

    struct SP3Data {
        vector3d position;
        scalar time;
    };

    const SP3::SP3Parser G13_SP3(DIR_PATH + "resources/GNSS/SP3/G13_SP3.csv");
    const SP3::SP3Parser G15_SP3(DIR_PATH + "resources/GNSS/SP3/G15_SP3.csv");
    const SP3::SP3Parser G16_SP3(DIR_PATH + "resources/GNSS/SP3/G16_SP3.csv");
    const SP3::SP3Parser G18_SP3(DIR_PATH + "resources/GNSS/SP3/G18_SP3.csv");

    const std::vector<SP3::SP3Parser> satellites_SP3 = {G13_SP3, G15_SP3, G16_SP3, G18_SP3};
    std::vector<SP3Data> rowSP3Data1;
    for (auto sat: satellites_SP3) {
        const std::pair<vector3d, scalar> observation = sat.getObservation(time1);
        rowSP3Data1.push_back({observation.first, observation.second});
    }
    std::vector<SP3Data> rowSP3Data2;
    for (auto sat: satellites_SP3) {
        const std::pair<vector3d, scalar> observation = sat.getObservation(time2);
        rowSP3Data2.push_back({observation.first, observation.second});
    }

    for (int i = 0; i < numberOfSatellites; ++i) {
        externalData1[i] = {P_IF1[i],
                            rowSP3Data1[i].position.x() * 1000,
                            rowSP3Data1[i].position.y() * 1000,
                            rowSP3Data1[i].position.z() * 1000,
                            rowSP3Data1[i].time * 1e-6};
    }
    for (int i = 0; i < numberOfSatellites; ++i) {
        externalData2[i] = {P_IF2[i],
                            rowSP3Data2[i].position.x() * 1000,
                            rowSP3Data2[i].position.y() * 1000,
                            rowSP3Data2[i].position.z() * 1000,
                            rowSP3Data2[i].time * 1e-6};
    }

    const GNSS::determinedParameters &initialApproximation = {0., 0., 0., 0.};
    const scalar sigma = 1.;
    const scalar sigmaSqRev = 1. / (sigma * sigma);
    vector<scalar, numberOfSatellites> sigmaVector;
    for (int i = 0; i < numberOfSatellites; ++i) {
        sigmaVector[i] = sigmaSqRev;
    }
    const diagMatrix<scalar, numberOfSatellites, numberOfSatellites> P(sigmaVector);
    const scalar tolerance = 1e-10;

    const auto result1 = GNSS::determinePosition<numberOfSatellites>(externalData1,
                                                                                           initialApproximation,
                                                                                           P,
                                                                                           tolerance);
    const auto result2 = GNSS::determinePosition<numberOfSatellites>(externalData2,
                                                                                           initialApproximation,
                                                                                           P,
                                                                                           tolerance);
    std::cout << result1.first.deltaXr << std::endl << result1.first.deltaYr << std::endl << result1.first.deltaZr << std::endl
              << result1.first.deltaTr << std::endl;
    std::cout << "--------------------------------------------------" << std::endl;
    std::cout << result2.first.deltaXr << std::endl << result2.first.deltaYr << std::endl << result2.first.deltaZr << std::endl
              << result2.first.deltaTr << std::endl;
    auto res1Vec = toVector(result1.first);
    auto res2Vec = toVector(result2.first);
    std::cout << "--------------------------------------------------" << std::endl;
    std::cout << (res1Vec.head(3) - res2Vec.head(3)).norm();
}

