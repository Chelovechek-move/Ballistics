#include "../src/ballistics/time/TimeConverter.hpp"
#include "iostream"

int main() {

    Ballistics::Time::TimeConverter converter = Ballistics::Time::TimeConverter("../../resources/earth_rotation.csv");
    Ballistics::Time::Time<Ballistics::Time::Scale::TCG> timeTCG = Ballistics::Time::Time<Ballistics::Time::Scale::TCG>(2459842, 0.635599);

    double tolerance = 10e-11;

    auto const timeTCG1 = converter.convert<Ballistics::Time::Scale::TCG, Ballistics::Time::Scale::TCG>(timeTCG);
    auto const timeUTC = converter.convert<Ballistics::Time::Scale::TCG, Ballistics::Time::Scale::UTC>(timeTCG);
    auto const timeTAI = converter.convert<Ballistics::Time::Scale::TCG, Ballistics::Time::Scale::TAI>(timeTCG);
    auto const timeUT1 = converter.convert<Ballistics::Time::Scale::TCG, Ballistics::Time::Scale::UT1>(timeTCG);
    auto const timeTT = converter.convert<Ballistics::Time::Scale::TCG, Ballistics::Time::Scale::TT>(timeTCG);
    auto const timeTDB = converter.convert<Ballistics::Time::Scale::TCG, Ballistics::Time::Scale::TDB>(timeTCG);
    auto const timeTCB = converter.convert<Ballistics::Time::Scale::TCG, Ballistics::Time::Scale::TCB>(timeTCG);

    std::cout << std::fixed;
    std::cout.precision(53);

    std::cout << "___TCG__ " << timeTCG.getDay() << " // " << timeTCG.getPart() << std::endl;
    std::cout << "TCG->TCG: " << timeTCG1.getDay() << " // " << timeTCG1.getPart() << std::endl;
    std::cout << "TCG->UTC: " << timeUTC.getDay() << " // " << timeUTC.getPart() << std::endl;
    std::cout << "TCG->TAI: " << timeTAI.getDay() << " // " << timeTAI.getPart() << std::endl;
    std::cout << "TCG->UT1: " << timeUT1.getDay() << " // " << timeUT1.getPart() << std::endl;
    std::cout << "TCG->TT: " << timeTT.getDay() << " // " << timeTT.getPart() << std::endl;
    std::cout << "TCG->TDB: " << timeTDB.getDay() << " // " << timeTDB.getPart() << std::endl;
    std::cout << "TCG->TCB: " << timeTCB.getDay() << " // " << timeTCB.getPart() << std::endl;

    std::cout << "--------------------------" << std::endl;

    auto const timeUTC_TCG = converter.convert<Ballistics::Time::Scale::UTC, Ballistics::Time::Scale::TCG>(timeUTC);
    auto const timeTAI_TCG = converter.convert<Ballistics::Time::Scale::TAI, Ballistics::Time::Scale::TCG>(timeTAI);
    auto const timeUT1_TCG = converter.convert<Ballistics::Time::Scale::UT1, Ballistics::Time::Scale::TCG>(timeUT1);
    auto const timeTT_TCG = converter.convert<Ballistics::Time::Scale::TT, Ballistics::Time::Scale::TCG>(timeTT);
    auto const timeTDB_TCG = converter.convert<Ballistics::Time::Scale::TDB, Ballistics::Time::Scale::TCG>(timeTDB);
    auto const timeTCB_TCG = converter.convert<Ballistics::Time::Scale::TCB, Ballistics::Time::Scale::TCG>(timeTCB);

    std::cout << "UTC->TCG: " << timeUTC_TCG.getDay() << " // " << timeUTC_TCG.getPart() << std::endl;
    std::cout << "TAI->TCG: " << timeTAI_TCG.getDay() << " // " << timeTAI_TCG.getPart() << std::endl;
    std::cout << "UT1->TCG: " << timeUT1_TCG.getDay() << " // " << timeUT1_TCG.getPart() << std::endl;
    std::cout << "TT->TCG: " << timeTT_TCG.getDay() << " // " << timeTT_TCG.getPart() << std::endl;
    std::cout << "TDB->TCG: " << timeTDB_TCG.getDay() << " // " << timeTDB_TCG.getPart() << std::endl;
    std::cout << "TCB->TCG: " << timeTCB_TCG.getDay() << " // " << timeTCB_TCG.getPart() << std::endl;

    if (timeTCG1.isEqual(timeTCG) &&
        timeUTC_TCG.isEqual(timeTCG) &&
        timeTAI_TCG.isEqual(timeTCG) &&
        timeUT1_TCG.isEqual(timeTCG) &&
        timeTT_TCG.isEqual(timeTCG) &&
        timeTDB_TCG.isEqual(timeTCG, tolerance) &&
        timeTCB_TCG.isEqual(timeTCG, tolerance))
    {
        std::cout << "Test 9 completed successfully!";
    } else {
        throw "Error! Test 9 failed";
    }
}