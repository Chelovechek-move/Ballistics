#include "../src/ballistics/time/Time.hpp"
#include "iostream"

int main()
{
    Ballistics::Time::Time jd1Time = Ballistics::Time::Time(2400000, 0.5);
    Ballistics::Time::Time jd2Time = Ballistics::Time::Time(2400001, 0.0);
    Ballistics::Time::Time jd3Time = Ballistics::Time::Time(2400002, 0.3);

    Ballistics::scalar mjd1Time = jd1Time.toMJD();
    Ballistics::scalar mjd2Time = jd2Time.toMJD();
    Ballistics::scalar mjd3Time = jd3Time.toMJD();

    Ballistics::scalar tolerance = 10e-15;

    if (mjd1Time - 0. < tolerance &&
        mjd2Time - 0.5 < tolerance &&
        mjd3Time - 1.8 < tolerance &&
        jd1Time.toMJD() - 0. < tolerance &&
        jd2Time.toMJD() - 0.5 < tolerance &&
        jd3Time.toMJD() - 1.8 < tolerance)
    {
        std::cout << "Test 4 completed successfully!";
    } else {
        throw "Error! Test 4 failed";
    }
}