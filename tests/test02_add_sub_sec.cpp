#include "../src/ballistics/time/Time.hpp"
#include "iostream"

int main() {

    Ballistics::Time::Time oneDay = Ballistics::Time::Time(1, 0);
    Ballistics::Time::Time oneAndHalfDays = oneDay + 43200;
    Ballistics::Time::Time twoDays = oneDay + 86400;
    Ballistics::Time::Time fiveAndHalfDays = Ballistics::Time::Time(5, 0.5);
    Ballistics::Time::Time fiveDays = fiveAndHalfDays - 43200;
    Ballistics::Time::Time fourDays = fiveDays - 86400;


    double tolerance = 10e-15;

    if ((twoDays.getDay() == 2 && twoDays.getPart() - 0. < tolerance) &&
        (oneAndHalfDays.getDay() == 1 && oneAndHalfDays.getPart() - 0.5 < tolerance) &&
        (fiveAndHalfDays.getDay() == 5 && fiveAndHalfDays.getPart() - 0.5 < tolerance) &&
        (fiveDays.getDay() == 5 && fiveDays.getPart() - 0. < tolerance) &&
        (fourDays.getDay() == 4 && fourDays.getPart() - 0. < tolerance))
    {
        std::cout << "Test 2.1 completed successfully!" << std::endl;
    } else {
        throw "Error! Test 2.1 failed";
    }

    Ballistics::Time::Time testDay1 = Ballistics::Time::Time(1, 95.5);
    Ballistics::Time::Time testDay3 = Ballistics::Time::Time(1, 95.5);
    Ballistics::Time::Time testDay2 = testDay1 + 86400 *  5;
    testDay3 = testDay3 + 86400 *  5;

    if (testDay1.isEqual({96, 0.5}) &&
        testDay2.isEqual({101, 0.5}) &&
        testDay3.isEqual({101, 0.5}))
    {
        std::cout << "Test 2.2 completed successfully!" << std::endl;
    } else {
        throw "Error! Test 2.2 failed";
    }
}