#include "../src/ballistics/time/TimeConverter.hpp"
#include "iostream"

int main() {

    Ballistics::Time::TimeConverter converter = Ballistics::Time::TimeConverter("../../resources/earth_rotation.csv");
    Ballistics::Time::Time<Ballistics::Time::Scale::TDB> timeTDB = Ballistics::Time::Time<Ballistics::Time::Scale::TDB>(2459842, 0.635587);

    double tolerance = 10e-11;

    auto const timeTDB1 = converter.convert<Ballistics::Time::Scale::TDB, Ballistics::Time::Scale::TDB>(timeTDB);
    auto const timeUTC = converter.convert<Ballistics::Time::Scale::TDB, Ballistics::Time::Scale::UTC>(timeTDB);
    auto const timeTAI = converter.convert<Ballistics::Time::Scale::TDB, Ballistics::Time::Scale::TAI>(timeTDB);
    auto const timeUT1 = converter.convert<Ballistics::Time::Scale::TDB, Ballistics::Time::Scale::UT1>(timeTDB);
    auto const timeTT = converter.convert<Ballistics::Time::Scale::TDB, Ballistics::Time::Scale::TT>(timeTDB);
    auto const timeTCG = converter.convert<Ballistics::Time::Scale::TDB, Ballistics::Time::Scale::TCG>(timeTDB);
    auto const timeTCB = converter.convert<Ballistics::Time::Scale::TDB, Ballistics::Time::Scale::TCB>(timeTDB);

    std::cout << std::fixed;
    std::cout.precision(53);

    std::cout << "___TDB __ " << timeTDB.getDay() << " // " << timeTDB.getPart() << std::endl;
    std::cout << "TDB->TDB: " << timeTDB1.getDay() << " // " << timeTDB1.getPart() << std::endl;
    std::cout << "TDB->UTC: " << timeUTC.getDay() << " // " << timeUTC.getPart() << std::endl;
    std::cout << "TDB->TAI: " << timeTAI.getDay() << " // " << timeTAI.getPart() << std::endl;
    std::cout << "TDB->UT1: " << timeUT1.getDay() << " // " << timeUT1.getPart() << std::endl;
    std::cout << "TDB->TT:  " << timeTT.getDay() << " // " << timeTT.getPart() << std::endl;
    std::cout << "TDB->TCG: " << timeTCG.getDay() << " // " << timeTCG.getPart() << std::endl;
    std::cout << "TDB->TCB: " << timeTCB.getDay() << " // " << timeTCB.getPart() << std::endl;

    std::cout << "--------------------------" << std::endl;

    auto const timeUTC_TDB = converter.convert<Ballistics::Time::Scale::UTC, Ballistics::Time::Scale::TDB>(timeUTC);
    auto const timeTAI_TDB = converter.convert<Ballistics::Time::Scale::TAI, Ballistics::Time::Scale::TDB>(timeTAI);
    auto const timeUT1_TDB = converter.convert<Ballistics::Time::Scale::UT1, Ballistics::Time::Scale::TDB>(timeUT1);
    auto const timeTT_TDB = converter.convert<Ballistics::Time::Scale::TT, Ballistics::Time::Scale::TDB>(timeTT);
    auto const timeTCG_TDB = converter.convert<Ballistics::Time::Scale::TCG, Ballistics::Time::Scale::TDB>(timeTCG);
    auto const timeTCB_TDB = converter.convert<Ballistics::Time::Scale::TCB, Ballistics::Time::Scale::TDB>(timeTCB);

    std::cout << "UTC->TDB: " << timeUTC_TDB.getDay() << " // " << timeUTC_TDB.getPart() << std::endl;
    std::cout << "TAI->TDB: " << timeTAI_TDB.getDay() << " // " << timeTAI_TDB.getPart() << std::endl;
    std::cout << "UT1->TDB: " << timeUT1_TDB.getDay() << " // " << timeUT1_TDB.getPart() << std::endl;
    std::cout << "TT->TDB:  " << timeTT_TDB.getDay() << " // " << timeTT_TDB.getPart() << std::endl;
    std::cout << "TCG->TDB: " << timeTCG_TDB.getDay() << " // " << timeTCG_TDB.getPart() << std::endl;
    std::cout << "TCB->TDB: " << timeTCB_TDB.getDay() << " // " << timeTCB_TDB.getPart() << std::endl;

    if (timeTDB1.isEqual(timeTDB) &&
        timeUTC_TDB.isEqual(timeTDB, tolerance) &&
        timeTAI_TDB.isEqual(timeTDB, tolerance) &&
        timeUT1_TDB.isEqual(timeTDB, tolerance) &&
        timeTT_TDB.isEqual(timeTDB, tolerance) &&
        timeTCG_TDB.isEqual(timeTDB, tolerance) &&
        timeTCB_TDB.isEqual(timeTDB))
    {
        std::cout << "Test 10 completed successfully!";
    } else {
        throw "Error! Test 10 failed";
    }
}