#include "ballistics/forces/ThirdBodyGravity.hpp"
#include "../../third_party/include/rapidcsv.h"
#include "ballistics/types/BasicTypes.hpp"
#include "ballistics/exception/ProjectException.hpp"
#include "ballistics/types/Matrix.hpp"

int main()
{
    using namespace Ballistics;
    const rapidcsv::Document doc("/home/vladimir/Ballistics_Project/ballistics_chiniaev/tests/utility/third_body_gravity.csv");

    const std::vector<scalar> xCol = doc.GetColumn<scalar>("x");
    const std::vector<scalar> yCol = doc.GetColumn<scalar>("y");
    const std::vector<scalar> zCol = doc.GetColumn<scalar>("z");
    const std::vector<scalar> fxCol = doc.GetColumn<scalar>("fx");
    const std::vector<scalar> fyCol = doc.GetColumn<scalar>("fy");
    const std::vector<scalar> fzCol = doc.GetColumn<scalar>("fz");

    const auto n = static_cast<integer>(xCol.size());
    const scalar tolerance = 5e-6;

    const std::vector<vector3d> SunAndMoonPositions = {{-9.66955e+10, -1.02917e+11, -4.46137e+10},
                                                       {-3.63564e+08, 6.23951e+06,3.86832e+07}};
    const std::vector<scalar> SunAndMoonParameters = {1.32712e+20, 4.9028e+12};
    const scalar mass = 1;

    for (int i = 0; i < n; ++i)
    {
        const vector3d force = Forces::calcThirdPartyGravityForce(SunAndMoonPositions,
                                                                  SunAndMoonParameters,
                                                                  {xCol[i], yCol[i], zCol[i]},
                                                                  mass);

        if (!isEqualByRelativeError(force, {fxCol[i], fyCol[i], fzCol[i]}, tolerance))
        {
            std::stringstream buff;
            buff << "Error in line " << i + 1;
            throw Ballistics::ProjectException(buff.str());
        }
    }

    std::cout << "Test 18 passed successfully!" << std::endl;
}