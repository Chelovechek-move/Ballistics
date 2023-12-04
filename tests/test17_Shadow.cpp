#include "ballistics/shadow/Shadow.hpp"
#include "../../third_party/include/rapidcsv.h"
#include "ballistics/exception/ProjectException.hpp"

int main()
{
    using namespace Ballistics;
    const rapidcsv::Document doc("/home/vladimir/Ballistics_Project/ballistics_chiniaev/tests/utility/penumbra_shadow.csv");
    const std::vector<scalar> xCol = doc.GetColumn<scalar>("x");
    const std::vector<scalar> yCol = doc.GetColumn<scalar>("y");
    const std::vector<scalar> zCol = doc.GetColumn<scalar>("z");
    const std::vector<scalar> shadowCol = doc.GetColumn<scalar>("shadow");

    const auto n = static_cast<integer>(xCol.size());
    const scalar tolerance = 5e-14;

    for (int i = 0; i < n; ++i)
    {
        if ((abs(Shadow::calcShadowFunction({xCol[i], yCol[i], zCol[i]}, {-1.47098291e10, 0., 0.}, 695.700e6,  6.371e6)
            - shadowCol[i]) > tolerance))
        {
            std::stringstream buff;
            buff << "Error in line " << i + 1;
            throw Ballistics::ProjectException(buff.str());
        }
    }

    std::cout << "Test 17 passed successfully!" << std::endl;
}