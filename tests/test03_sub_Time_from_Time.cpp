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

    if ((oneAndHalfDays - oneDay - 0.5 * Ballistics::SECONDS_IN_DAY  < tolerance) &&
        (twoDays - oneDay - 1 * Ballistics::SECONDS_IN_DAY < tolerance) &&
        (fiveAndHalfDays - oneAndHalfDays - 4 * Ballistics::SECONDS_IN_DAY < tolerance) &&
        (fiveDays - fourDays - 1 * Ballistics::SECONDS_IN_DAY < tolerance)) {
        std::cout << "Test 2 completed successfully!";
    } else {
        throw "Error! Test 2 failed";
    }
}