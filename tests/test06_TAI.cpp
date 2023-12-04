#include "../src/ballistics/time/TimeConverter.hpp"
#include "iostream"

int main() {

    Ballistics::Time::TimeConverter converter = Ballistics::Time::TimeConverter("../../resources/earth_rotation.csv");
    Ballistics::Time::Time<Ballistics::Time::Scale::TAI> timeTAI = Ballistics::Time::Time<Ballistics::Time::Scale::TAI>(2459842, 0.635215);

    double tolerance = 10e-11;

    auto const timeTAI1 = converter.convert<Ballistics::Time::Scale::TAI, Ballistics::Time::Scale::TAI>(timeTAI);
    auto const timeUTC = converter.convert<Ballistics::Time::Scale::TAI, Ballistics::Time::Scale::UTC>(timeTAI);
    auto const timeUT1 = converter.convert<Ballistics::Time::Scale::TAI, Ballistics::Time::Scale::UT1>(timeTAI);
    auto const timeTT = converter.convert<Ballistics::Time::Scale::TAI, Ballistics::Time::Scale::TT>(timeTAI);
    auto const timeTCG = converter.convert<Ballistics::Time::Scale::TAI, Ballistics::Time::Scale::TCG>(timeTAI);
    auto const timeTDB = converter.convert<Ballistics::Time::Scale::TAI, Ballistics::Time::Scale::TDB>(timeTAI);
    auto const timeTCB = converter.convert<Ballistics::Time::Scale::TAI, Ballistics::Time::Scale::TCB>(timeTAI);

    std::cout << std::fixed;
    std::cout.precision(53);

    std::cout << "___TAI___ " << timeTAI.getDay() << " // " << timeTAI.getPart() << std::endl;
    std::cout << "TAI->TAI: " << timeTAI1.getDay() << " // " << timeTAI1.getPart() << std::endl;
    std::cout << "TAI->UTC: " << timeUTC.getDay() << " // " << timeUTC.getPart() << std::endl;
    std::cout << "TAI->UT1: " << timeUT1.getDay() << " // " << timeUT1.getPart() << std::endl;
    std::cout << "TAI->TT:  " << timeTT.getDay() << " // " << timeTT.getPart() << std::endl;
    std::cout << "TAI->TCG: " << timeTCG.getDay() << " // " << timeTCG.getPart() << std::endl;
    std::cout << "TAI->TDB: " << timeTDB.getDay() << " // " << timeTDB.getPart() << std::endl;
    std::cout << "TAI->TCB: " << timeTCB.getDay() << " // " << timeTCB.getPart() << std::endl;

    std::cout << "--------------------------" << std::endl;

    auto const timeUTC_TAI = converter.convert<Ballistics::Time::Scale::UTC, Ballistics::Time::Scale::TAI>(timeUTC);
    auto const timeUT1_TAI = converter.convert<Ballistics::Time::Scale::UT1, Ballistics::Time::Scale::TAI>(timeUT1);
    auto const timeTT_TAI = converter.convert<Ballistics::Time::Scale::TT, Ballistics::Time::Scale::TAI>(timeTT);
    auto const timeTCG_TAI = converter.convert<Ballistics::Time::Scale::TCG, Ballistics::Time::Scale::TAI>(timeTCG);
    auto const timeTDB_TAI = converter.convert<Ballistics::Time::Scale::TDB, Ballistics::Time::Scale::TAI>(timeTDB);
    auto const timeTCB_TAI = converter.convert<Ballistics::Time::Scale::TCB, Ballistics::Time::Scale::TAI>(timeTCB);

    std::cout << "UTC->TAI: " << timeUTC_TAI.getDay() << " // " << timeUTC_TAI.getPart() << std::endl;
    std::cout << "UT1->TAI: " << timeUT1_TAI.getDay() << " // " << timeUT1_TAI.getPart() << std::endl;
    std::cout << "TT->TAI:  " << timeTT_TAI.getDay() << " // " << timeTT_TAI.getPart() << std::endl;
    std::cout << "TCG->TAI: " << timeTCG_TAI.getDay() << " // " << timeTCG_TAI.getPart() << std::endl;
    std::cout << "TDB->TAI: " << timeTDB_TAI.getDay() << " // " << timeTDB_TAI.getPart() << std::endl;
    std::cout << "TCB->TAI: " << timeTCB_TAI.getDay() << " // " << timeTCB_TAI.getPart() << std::endl;



    if (timeTAI1.isEqual(timeTAI) &&
        timeUTC_TAI.isEqual(timeTAI) &&
        timeUT1_TAI.isEqual(timeTAI) &&
        timeTT_TAI.isEqual(timeTAI) &&
        timeTCG_TAI.isEqual(timeTAI) &&
        timeTDB_TAI.isEqual(timeTAI, tolerance) &&
        timeTCB_TAI.isEqual(timeTAI, tolerance))
    {
        std::cout << "Test 6 completed successfully!";
    } else {
        throw "Error! Test 6 failed";
    }
}