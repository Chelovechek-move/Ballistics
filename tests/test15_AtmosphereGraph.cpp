#include "../src/ballistics/atmosphere/Atmosphere.hpp"
#include "../../third_party/include/rapidcsv.h"
#include "ballistics/time/Time.hpp"
#include <iostream>
#include <vector>

int main()
{
    using namespace Ballistics;

    Atmosphere::Atmosphere atmosphere = Atmosphere::Atmosphere("../../resources/atmosphere/SW-Last5Years.csv");
//    std::ofstream out;
//    out.open("test.txt");
//    integer x = 6380000;
//    for (int i = 0; i < 1600; ++i)
//    {
//        out << x - 6378000 << " ";
//        out << atmosphere.getDensity({x, 0, 0}, {2459277.5, 0.0}) << std::endl;
//        x += 1000;
//    }

    Eigen::Vector3d a = {-1752019.43992093, -1388716.46829728, 6178991.45919616};

    std::cout << atmosphere.getDensity({-1752019.43992093, -1388716.46829728, 6178991.45919616}, {2459277.5, 0.0}) << std::endl;

//    std::cout << atmosphere.getDensity({6380000 + 107000, 0, 0}, {2459277.5, 0.0}) << std::endl;
}