#include "../src/ballistics/time/TimeConverter.hpp"
#include "iostream"

int main() {

    Ballistics::Time::TimeConverter converter = Ballistics::Time::TimeConverter("../../resources/earth_rotation.csv");
    Ballistics::Time::Time<Ballistics::Time::Scale::UTC> timeUTC = Ballistics::Time::Time<Ballistics::Time::Scale::UTC>(2459842, 0.634786578);

    double tolerance = 10e-11;

    auto const timeUTC1 = converter.convert<Ballistics::Time::Scale::UTC, Ballistics::Time::Scale::UTC>(timeUTC);
    auto const timeTAI = converter.convert<Ballistics::Time::Scale::UTC, Ballistics::Time::Scale::TAI>(timeUTC);
    auto const timeUT1 = converter.convert<Ballistics::Time::Scale::UTC, Ballistics::Time::Scale::UT1>(timeUTC);
    auto const timeTT = converter.convert<Ballistics::Time::Scale::UTC, Ballistics::Time::Scale::TT>(timeUTC);
    auto const timeTCG = converter.convert<Ballistics::Time::Scale::UTC, Ballistics::Time::Scale::TCG>(timeUTC);
    auto const timeTDB = converter.convert<Ballistics::Time::Scale::UTC, Ballistics::Time::Scale::TDB>(timeUTC);
    auto const timeTCB = converter.convert<Ballistics::Time::Scale::UTC, Ballistics::Time::Scale::TCB>(timeUTC);

    std::cout << std::fixed;
    std::cout.precision(53);

    std::cout << "___UTC___ " << timeUTC.getDay() << " // " << timeUTC.getPart() << std::endl;
    std::cout << "UTC->UTC: " << timeUTC1.getDay() << " // " << timeUTC1.getPart() << std::endl;
    std::cout << "UTC->TAI: " << timeTAI.getDay() << " // " << timeTAI.getPart() << std::endl;
    std::cout << "UTC->UT1: " << timeUT1.getDay() << " // " << timeUT1.getPart() << std::endl;
    std::cout << "UTC->TT:  " << timeTT.getDay() << " // " << timeTT.getPart() << std::endl;
    std::cout << "UTC->TCG: " << timeTCG.getDay() << " // " << timeTCG.getPart() << std::endl;
    std::cout << "UTC->TDB: " << timeTDB.getDay() << " // " << timeTDB.getPart() << std::endl;
    std::cout << "UTC->TCB: " << timeTCB.getDay() << " // " << timeTCB.getPart() << std::endl;

    std::cout << "--------------------------" << std::endl;

    auto const timeTAI_UTC = converter.convert<Ballistics::Time::Scale::TAI, Ballistics::Time::Scale::UTC>(timeTAI);
    auto const timeUT1_UTC = converter.convert<Ballistics::Time::Scale::UT1, Ballistics::Time::Scale::UTC>(timeUT1);
    auto const timeTT_UTC = converter.convert<Ballistics::Time::Scale::TT, Ballistics::Time::Scale::UTC>(timeTT);
    auto const timeTCG_UTC = converter.convert<Ballistics::Time::Scale::TCG, Ballistics::Time::Scale::UTC>(timeTCG);
    auto const timeTDB_UTC = converter.convert<Ballistics::Time::Scale::TDB, Ballistics::Time::Scale::UTC>(timeTDB);
    auto const timeTCB_UTC = converter.convert<Ballistics::Time::Scale::TCB, Ballistics::Time::Scale::UTC>(timeTCB);

    std::cout << "TAI->UTC: " << timeTAI_UTC.getDay() << " // " << timeTAI_UTC.getPart() << std::endl;
    std::cout << "UT1->UTC: " << timeUT1_UTC.getDay() << " // " << timeUT1_UTC.getPart() << std::endl;
    std::cout << "TT->UTC:  " << timeTT_UTC.getDay() << " // " << timeTT_UTC.getPart() << std::endl;
    std::cout << "TCG->UTC: " << timeTCG_UTC.getDay() << " // " << timeTCG_UTC.getPart() << std::endl;
    std::cout << "TDB->UTC: " << timeTDB_UTC.getDay() << " // " << timeTDB_UTC.getPart() << std::endl;
    std::cout << "TCB->UTC: " << timeTCB_UTC.getDay() << " // " << timeTCB_UTC.getPart() << std::endl;



    if (timeUTC1.isEqual(timeUTC) &&
        timeTAI_UTC.isEqual(timeUTC) &&
        timeUT1_UTC.isEqual(timeUTC) &&
        timeTT_UTC.isEqual(timeUTC) &&
        timeTCG_UTC.isEqual(timeUTC) &&
        timeTDB_UTC.isEqual(timeUTC, tolerance) &&
        timeTCB_UTC.isEqual(timeUTC, tolerance))
    {
        std::cout << "Test 5 completed successfully!";
    } else {
        throw "Error! Test 5 failed";
    }
}