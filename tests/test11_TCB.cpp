#include "../src/ballistics/time/TimeConverter.hpp"
#include "iostream"

int main() {

    Ballistics::Time::TimeConverter converter = Ballistics::Time::TimeConverter("../../resources/earth_rotation.csv");
    Ballistics::Time::Time<Ballistics::Time::Scale::TCB> timeTCB = Ballistics::Time::Time<Ballistics::Time::Scale::TCB>(2459842, 0.635846);

    double tolerance = 10e-11;

    auto const timeTCB1 = converter.convert<Ballistics::Time::Scale::TCB, Ballistics::Time::Scale::TCB>(timeTCB);
    auto const timeUTC = converter.convert<Ballistics::Time::Scale::TCB, Ballistics::Time::Scale::UTC>(timeTCB);
    auto const timeTAI = converter.convert<Ballistics::Time::Scale::TCB, Ballistics::Time::Scale::TAI>(timeTCB);
    auto const timeUT1 = converter.convert<Ballistics::Time::Scale::TCB, Ballistics::Time::Scale::UT1>(timeTCB);
    auto const timeTT = converter.convert<Ballistics::Time::Scale::TCB, Ballistics::Time::Scale::TT>(timeTCB);
    auto const timeTCG = converter.convert<Ballistics::Time::Scale::TCB, Ballistics::Time::Scale::TCG>(timeTCB);
    auto const timeTDB = converter.convert<Ballistics::Time::Scale::TCB, Ballistics::Time::Scale::TDB>(timeTCB);

    std::cout << std::fixed;
    std::cout.precision(53);

    std::cout << "___TCB___ " << timeTCB.getDay() << " // " << timeTCB.getPart() << std::endl;
    std::cout << "TCB->TCB: " << timeTCB1.getDay() << " // " << timeTCB1.getPart() << std::endl;
    std::cout << "TCB->UTC: " << timeUTC.getDay() << " // " << timeUTC.getPart() << std::endl;
    std::cout << "TCB->TAI: " << timeTAI.getDay() << " // " << timeTAI.getPart() << std::endl;
    std::cout << "TCB->UT1: " << timeUT1.getDay() << " // " << timeUT1.getPart() << std::endl;
    std::cout << "TCB->TT:  " << timeTT.getDay() << " // " << timeTT.getPart() << std::endl;
    std::cout << "TCB->TCG: " << timeTCG.getDay() << " // " << timeTCG.getPart() << std::endl;
    std::cout << "TCB->TDB: " << timeTDB.getDay() << " // " << timeTDB.getPart() << std::endl;

    std::cout << "--------------------------" << std::endl;

    auto const timeUTC_TCB = converter.convert<Ballistics::Time::Scale::UTC, Ballistics::Time::Scale::TCB>(timeUTC);
    auto const timeTAI_TCB = converter.convert<Ballistics::Time::Scale::TAI, Ballistics::Time::Scale::TCB>(timeTAI);
    auto const timeUT1_TCB = converter.convert<Ballistics::Time::Scale::UT1, Ballistics::Time::Scale::TCB>(timeUT1);
    auto const timeTT_TCB = converter.convert<Ballistics::Time::Scale::TT, Ballistics::Time::Scale::TCB>(timeTT);
    auto const timeTCG_TCB = converter.convert<Ballistics::Time::Scale::TCG, Ballistics::Time::Scale::TCB>(timeTCG);
    auto const timeTDB_TCB = converter.convert<Ballistics::Time::Scale::TDB, Ballistics::Time::Scale::TCB>(timeTDB);

    std::cout << "UTC->TCB: " << timeUTC_TCB.getDay() << " // " << timeUTC_TCB.getPart() << std::endl;
    std::cout << "TAI->TCB: " << timeTAI_TCB.getDay() << " // " << timeTAI_TCB.getPart() << std::endl;
    std::cout << "UT1->TCB: " << timeUT1_TCB.getDay() << " // " << timeUT1_TCB.getPart() << std::endl;
    std::cout << "TT->TCB:  " << timeTT_TCB.getDay() << " // " << timeTT_TCB.getPart() << std::endl;
    std::cout << "TCG->TCB: " << timeTCG_TCB.getDay() << " // " << timeTCG_TCB.getPart() << std::endl;
    std::cout << "TDB->TCB: " << timeTDB_TCB.getDay() << " // " << timeTDB_TCB.getPart() << std::endl;

    if (timeTCB1.isEqual(timeTCB) &&
        timeUTC_TCB.isEqual(timeTCB, tolerance) &&
        timeTAI_TCB.isEqual(timeTCB, tolerance) &&
        timeUT1_TCB.isEqual(timeTCB, tolerance) &&
        timeTT_TCB.isEqual(timeTCB, tolerance) &&
        timeTCG_TCB.isEqual(timeTCB, tolerance) &&
        timeTDB_TCB.isEqual(timeTCB))
    {
        std::cout << "Test 11 completed successfully!";
    } else {
        throw "Error! Test 11 failed";
    }
}