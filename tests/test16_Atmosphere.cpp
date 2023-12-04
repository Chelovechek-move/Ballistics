#include "../src/ballistics/atmosphere/Atmosphere.hpp"
#include "ballistics/time/Time.hpp"
#include <iostream>
#include <vector>

int main()
{
    using namespace Ballistics;

    Atmosphere::Atmosphere atmosphere = Atmosphere::Atmosphere("../../resources/atmosphere/SW-Last5Years.csv");
    rapidcsv::Document doc("/home/vladimir/Ballistics_Project/ballistics_chiniaev/tests/utility/gost_density.csv");
    std::vector<scalar> xCol = doc.GetColumn<scalar>("x");
    std::vector<scalar> yCol = doc.GetColumn<scalar>("y");
    std::vector<scalar> zCol = doc.GetColumn<scalar>("z");
    std::vector<scalar> rhoCol = doc.GetColumn<scalar>("rho");

    integer n = xCol.size();
    scalar tolerance = 7e-2;

    for (int i = 0; i < n; ++i)
    {
        if ((abs(atmosphere.getDensity({xCol[i], yCol[i], zCol[i]}, {2459277.5, 0.0}) - rhoCol[i]) / rhoCol[i]) > tolerance)
        {
            std::stringstream buff;
            buff << "Error in line " << i + 1;
            throw Ballistics::ProjectException(buff.str());
        }
    }

    std::cout << "Test 16 passed successfully! Relative accuracy: " << tolerance * 100 << "%." << std::endl;
}