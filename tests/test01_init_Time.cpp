#include "../src/ballistics/time/Time.hpp"
#include "iostream"

int main() {
    Ballistics::Time::Time defaultDay = Ballistics::Time::Time();
    Ballistics::Time::Time oneDay = Ballistics::Time::Time(1, 0);
    Ballistics::Time::Time twoDays = Ballistics::Time::Time(2, 0);
    Ballistics::Time::Time twoDaysAndHalf = Ballistics::Time::Time(2, 0.5);

    double tolerance = 10e-15;

    if ((defaultDay.getDay() == 0 && (defaultDay.getPart() - 0.) < tolerance) &&
        (oneDay.getDay() == 1 && (oneDay.getPart() - 0.) < tolerance) &&
        (twoDays.getDay() == 2 && twoDays.getPart() - 0. < tolerance) &&
        (twoDaysAndHalf.getDay() == 2 && twoDaysAndHalf.getPart() - 0.5 < tolerance)) {
        std::cout << "Test 1 completed successfully!";
    } else {
        throw "Error! Test 1 failed";
    }
}
