#include "../src/ballistics/time/TimeConverter.hpp"
#include "iostream"

int main() {

    Ballistics::Time::TimeConverter converter = Ballistics::Time::TimeConverter("../../resources/earth_rotation.csv");
    Ballistics::Time::Time<Ballistics::Time::Scale::TT> timeTT = Ballistics::Time::Time<Ballistics::Time::Scale::TT>(2459842, 0.635587);

    double tolerance = 10e-11;

    auto const timeTT1 = converter.convert<Ballistics::Time::Scale::TT, Ballistics::Time::Scale::TT>(timeTT);
    auto const timeUTC = converter.convert<Ballistics::Time::Scale::TT, Ballistics::Time::Scale::UTC>(timeTT);
    auto const timeTAI = converter.convert<Ballistics::Time::Scale::TT, Ballistics::Time::Scale::TAI>(timeTT);
    auto const timeUT1 = converter.convert<Ballistics::Time::Scale::TT, Ballistics::Time::Scale::UT1>(timeTT);
    auto const timeTCG = converter.convert<Ballistics::Time::Scale::TT, Ballistics::Time::Scale::TCG>(timeTT);
    auto const timeTDB = converter.convert<Ballistics::Time::Scale::TT, Ballistics::Time::Scale::TDB>(timeTT);
    auto const timeTCB = converter.convert<Ballistics::Time::Scale::TT, Ballistics::Time::Scale::TCB>(timeTT);

    std::cout << std::fixed;
    std::cout.precision(53);

    std::cout << "___TT___ " << timeTT.getDay() << " // " << timeTT.getPart() << std::endl;
    std::cout << "TT->TT : " << timeTT1.getDay() << " // " << timeTT1.getPart() << std::endl;
    std::cout << "TT->UTC: " << timeUTC.getDay() << " // " << timeUTC.getPart() << std::endl;
    std::cout << "TT->TAI: " << timeTAI.getDay() << " // " << timeTAI.getPart() << std::endl;
    std::cout << "TT->UT1: " << timeTT.getDay() << " // " << timeTT.getPart() << std::endl;
    std::cout << "TT->TCG: " << timeTCG.getDay() << " // " << timeTCG.getPart() << std::endl;
    std::cout << "TT->TDB: " << timeTDB.getDay() << " // " << timeTDB.getPart() << std::endl;
    std::cout << "TT->TCB: " << timeTCB.getDay() << " // " << timeTCB.getPart() << std::endl;

    std::cout << "--------------------------" << std::endl;

    auto const timeUTC_TT = converter.convert<Ballistics::Time::Scale::UTC, Ballistics::Time::Scale::TT>(timeUTC);
    auto const timeTAI_TT = converter.convert<Ballistics::Time::Scale::TAI, Ballistics::Time::Scale::TT>(timeTAI);
    auto const timeUT1_TT = converter.convert<Ballistics::Time::Scale::UT1, Ballistics::Time::Scale::TT>(timeUT1);
    auto const timeTCG_TT = converter.convert<Ballistics::Time::Scale::TCG, Ballistics::Time::Scale::TT>(timeTCG);
    auto const timeTDB_TT = converter.convert<Ballistics::Time::Scale::TDB, Ballistics::Time::Scale::TT>(timeTDB);
    auto const timeTCB_TT = converter.convert<Ballistics::Time::Scale::TCB, Ballistics::Time::Scale::TT>(timeTCB);

    std::cout << "UTC->TT: " << timeUTC_TT.getDay() << " // " << timeUTC_TT.getPart() << std::endl;
    std::cout << "TAI->TT: " << timeTAI_TT.getDay() << " // " << timeTAI_TT.getPart() << std::endl;
    std::cout << "UT1->TT: " << timeUT1_TT.getDay() << " // " << timeUT1_TT.getPart() << std::endl;
    std::cout << "TCG->TT: " << timeTCG_TT.getDay() << " // " << timeTCG_TT.getPart() << std::endl;
    std::cout << "TDB->TT: " << timeTDB_TT.getDay() << " // " << timeTDB_TT.getPart() << std::endl;
    std::cout << "TCB->TT: " << timeTCB_TT.getDay() << " // " << timeTCB_TT.getPart() << std::endl;

    if (timeTT1.isEqual(timeTT) &&
        timeUTC_TT.isEqual(timeTT) &&
        timeTAI_TT.isEqual(timeTT) &&
        timeUT1_TT.isEqual(timeTT) &&
        timeTCG_TT.isEqual(timeTT) &&
        timeTDB_TT.isEqual(timeTT, tolerance) &&
        timeTCB_TT.isEqual(timeTT, tolerance))
    {
        std::cout << "Test 8 completed successfully!";
    } else {
        throw "Error! Test 8 failed";
    }
}