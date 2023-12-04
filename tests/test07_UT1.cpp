#include "../src/ballistics/time/TimeConverter.hpp"
#include "iostream"

int main() {

    Ballistics::Time::TimeConverter converter = Ballistics::Time::TimeConverter("../../resources/earth_rotation.csv");
    Ballistics::Time::Time<Ballistics::Time::Scale::UT1> timeUT1 = Ballistics::Time::Time<Ballistics::Time::Scale::UT1>(2459842, 0.634791);

    double tolerance = 10e-11;

    auto const timeUT11 = converter.convert<Ballistics::Time::Scale::UT1, Ballistics::Time::Scale::UT1>(timeUT1);
    auto const timeUTC = converter.convert<Ballistics::Time::Scale::UT1, Ballistics::Time::Scale::UTC>(timeUT1);
    auto const timeTAI = converter.convert<Ballistics::Time::Scale::UT1, Ballistics::Time::Scale::TAI>(timeUT1);
    auto const timeTT = converter.convert<Ballistics::Time::Scale::UT1, Ballistics::Time::Scale::TT>(timeUT1);
    auto const timeTCG = converter.convert<Ballistics::Time::Scale::UT1, Ballistics::Time::Scale::TCG>(timeUT1);
    auto const timeTDB = converter.convert<Ballistics::Time::Scale::UT1, Ballistics::Time::Scale::TDB>(timeUT1);
    auto const timeTCB = converter.convert<Ballistics::Time::Scale::UT1, Ballistics::Time::Scale::TCB>(timeUT1);

    std::cout << std::fixed;
    std::cout.precision(53);

    std::cout << "___UT1___ " << timeUT1.getDay() << " // " << timeUT1.getPart() << std::endl;
    std::cout << "UT1->UT1: " << timeUT11.getDay() << " // " << timeUT11.getPart() << std::endl;
    std::cout << "UT1->UTC: " << timeUTC.getDay() << " // " << timeUTC.getPart() << std::endl;
    std::cout << "UT1->TAI: " << timeTAI.getDay() << " // " << timeTAI.getPart() << std::endl;
    std::cout << "UT1->TT:  " << timeTT.getDay() << " // " << timeTT.getPart() << std::endl;
    std::cout << "UT1->TCG: " << timeTCG.getDay() << " // " << timeTCG.getPart() << std::endl;
    std::cout << "UT1->TDB: " << timeTDB.getDay() << " // " << timeTDB.getPart() << std::endl;
    std::cout << "UT1->TCB: " << timeTCB.getDay() << " // " << timeTCB.getPart() << std::endl;

    std::cout << "--------------------------" << std::endl;

    auto const timeUTC_UT1 = converter.convert<Ballistics::Time::Scale::UTC, Ballistics::Time::Scale::UT1>(timeUTC);
    auto const timeTAI_UT1 = converter.convert<Ballistics::Time::Scale::TAI, Ballistics::Time::Scale::UT1>(timeTAI);
    auto const timeTT_UT1 = converter.convert<Ballistics::Time::Scale::TT, Ballistics::Time::Scale::UT1>(timeTT);
    auto const timeTCG_UT1 = converter.convert<Ballistics::Time::Scale::TCG, Ballistics::Time::Scale::UT1>(timeTCG);
    auto const timeTDB_UT1 = converter.convert<Ballistics::Time::Scale::TDB, Ballistics::Time::Scale::UT1>(timeTDB);
    auto const timeTCB_UT1 = converter.convert<Ballistics::Time::Scale::TCB, Ballistics::Time::Scale::UT1>(timeTCB);

    std::cout << "UTC->UT1: " << timeUTC_UT1.getDay() << " // " << timeUTC_UT1.getPart() << std::endl;
    std::cout << "TAI->UT1: " << timeTAI_UT1.getDay() << " // " << timeTAI_UT1.getPart() << std::endl;
    std::cout << "TT->UT1:  " << timeTT_UT1.getDay() << " // " << timeTT_UT1.getPart() << std::endl;
    std::cout << "TCG->UT1: " << timeTCG_UT1.getDay() << " // " << timeTCG_UT1.getPart() << std::endl;
    std::cout << "TDB->UT1: " << timeTDB_UT1.getDay() << " // " << timeTDB_UT1.getPart() << std::endl;
    std::cout << "TCB->UT1: " << timeTCB_UT1.getDay() << " // " << timeTCB_UT1.getPart() << std::endl;



    if (timeUT11.isEqual(timeUT1) &&
        timeUTC_UT1.isEqual(timeUT1) &&
        timeTAI_UT1.isEqual(timeUT1) &&
        timeTT_UT1.isEqual(timeUT1) &&
        timeTCG_UT1.isEqual(timeUT1) &&
        timeTDB_UT1.isEqual(timeUT1, tolerance) &&
        timeTCB_UT1.isEqual(timeUT1, tolerance))
    {
        std::cout << "Test 7 completed successfully!";
    } else {
        throw "Error! Test 7 failed";
    }
}