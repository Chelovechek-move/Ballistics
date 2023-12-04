#include "../src/ballistics/ephemeris/Ephemeris.hpp"
#include "ballistics/types/BasicTypes.hpp"
#include "../../third_party/include/rapidcsv.h"
#include "../src/ballistics/time/TimeConverter.hpp"
#include <iostream>

int main()
{
    using namespace Ballistics;
    rapidcsv::Document doc("/home/vladimir/Ballistics_Project/ballistics_chiniaev/tests/utility/ephems.csv");

    std::vector<integer> bodyNumCol = doc.GetColumn<integer>("body");
    std::vector<scalar> jdDayCol = doc.GetColumn<scalar>("jdDay");
    std::vector<scalar> jdPartCol = doc.GetColumn<scalar>("jdDayPart");
    std::vector<scalar> xCol = doc.GetColumn<scalar>("x");
    std::vector<scalar> yCol = doc.GetColumn<scalar>("y");
    std::vector<scalar> zCol = doc.GetColumn<scalar>("z");
    std::vector<scalar> vxCol = doc.GetColumn<scalar>("vx");
    std::vector<scalar> vyCol = doc.GetColumn<scalar>("vy");
    std::vector<scalar> vzCol = doc.GetColumn<scalar>("vz");

    Ballistics::Time::TimeConverter converter = Ballistics::Time::TimeConverter("../../resources/earth_rotation.csv");
    Ephemeris::Ephemeris Ephems;

    auto n = static_cast<integer>(xCol.size());
    scalar tolerance = 5e-15;

    for (int i = 0; i < n; ++i)
    {
        const Time::Time<Time::Scale::TT> timeTT = {jdDayCol[i], jdPartCol[i]};
        const Time::Time<Time::Scale::TDB> timeTDB = converter.convert<Ballistics::Time::Scale::TT, Ballistics::Time::Scale::TDB>(timeTT);
        const vector3d position = {xCol[i], yCol[i], zCol[i]};
        const vector3d velocity = {vxCol[i], vyCol[i], vzCol[i]};

        if (!isEqualByRelativeError(Ephems.calcCoordinates(timeTDB, bodyNumCol[i], Ephemeris::Body::Earth) * 1000, position, tolerance))
        {
            std::stringstream buff;
            buff << "Error in line " << i + 1;
            throw Ballistics::ProjectException(buff.str());
        }

        if (!isEqualByRelativeError(Ephems.calcVelocities(timeTDB, bodyNumCol[i], Ephemeris::Body::Earth) * 1000, velocity, tolerance))
        {
            std::stringstream buff;
            buff << "Error in line " << i + 1;
            throw Ballistics::ProjectException(buff.str());
        }
    }

    std::cout << "Test 14 passed successfully!" << std::endl;
}