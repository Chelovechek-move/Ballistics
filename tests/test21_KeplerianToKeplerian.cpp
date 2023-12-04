#include "ballistics/orbit/OrbitConverter.hpp"
#include "../../third_party/include/rapidcsv.h"
#include "ballistics/exception/ProjectException.hpp"
#include "ballistics/types/Matrix.hpp"

int main()
{
    using namespace Ballistics;
    const rapidcsv::Document doc("/home/vladimir/Ballistics_Project/ballistics_chiniaev/tests/utility/KeplerianAndCartesian.csv");

    const std::vector<scalar> muCol       = doc.GetColumn<scalar>("mu");
    const std::vector<scalar> xCol        = doc.GetColumn<scalar>("x");
    const std::vector<scalar> yCol        = doc.GetColumn<scalar>("y");
    const std::vector<scalar> zCol        = doc.GetColumn<scalar>("z");
    const std::vector<scalar> vxCol       = doc.GetColumn<scalar>("vx");
    const std::vector<scalar> vyCol       = doc.GetColumn<scalar>("vy");
    const std::vector<scalar> vzCol       = doc.GetColumn<scalar>("vz");
    const std::vector<scalar> iCol        = doc.GetColumn<scalar>("i");
    const std::vector<scalar> RAANCol     = doc.GetColumn<scalar>("RAAN");
    const std::vector<scalar> aCol        = doc.GetColumn<scalar>("a");
    const std::vector<scalar> eCol        = doc.GetColumn<scalar>("e");
    const std::vector<scalar> argPerCol   = doc.GetColumn<scalar>("argPer");
    const std::vector<scalar> trueAnomCol = doc.GetColumn<scalar>("trueAnom");
    const std::vector<scalar> meanAnomCol = doc.GetColumn<scalar>("meanAnom");

    const auto n = static_cast<integer>(xCol.size());
    const scalar tolerance = 1e-15;

    for (int i = 0; i < n; ++i) {
        const Orbit::PosVel CartesianElements = {{xCol[i],  yCol[i],  zCol[i]},
                                                 {vxCol[i], vyCol[i], vzCol[i]}};
        const Orbit::KeplerMean KeplerMeanElements = {iCol[i], RAANCol[i], aCol[i], eCol[i], argPerCol[i],
                                                      meanAnomCol[i]};
        const Orbit::KeplerTrue KeplerTrueElements = {iCol[i], RAANCol[i], aCol[i], eCol[i], argPerCol[i],
                                                      trueAnomCol[i]};

        const Orbit::KeplerTrue newKeplerTrueElements = Orbit::convert<Orbit::KeplerTrue>(KeplerMeanElements, muCol[i]);
        const Orbit::KeplerMean newKeplerMeanElements = Orbit::convert<Orbit::KeplerMean>(KeplerTrueElements, muCol[i]);

        if (!(abs(newKeplerTrueElements.inclination              - KeplerTrueElements.inclination             ) < tolerance &&
              abs(newKeplerTrueElements.longitudeOfAscendingNode - KeplerTrueElements.longitudeOfAscendingNode) < tolerance &&
              abs(newKeplerTrueElements.largeSemiAxis            - KeplerTrueElements.largeSemiAxis           ) < tolerance &&
              abs(newKeplerTrueElements.eccentricity             - KeplerTrueElements.eccentricity            ) < tolerance &&
              abs(newKeplerTrueElements.pericenterArgument       - KeplerTrueElements.pericenterArgument      ) < tolerance &&

              (abs(newKeplerTrueElements.trueAnomaly             - KeplerTrueElements.trueAnomaly             ) < tolerance ||
               abs(newKeplerTrueElements.trueAnomaly             - KeplerTrueElements.trueAnomaly + 2 * M_PI  ) < tolerance ||
               abs(newKeplerTrueElements.trueAnomaly             - KeplerTrueElements.trueAnomaly - 2 * M_PI  ) < tolerance)
              ) )
        {
            std::stringstream buff;
            buff << "Error in conversion from Kepler mean elements to Kepler true elements. Line " << i + 1;
            throw Ballistics::ProjectException(buff.str());
        }

        if (!(abs(newKeplerMeanElements.inclination              - KeplerMeanElements.inclination             ) < tolerance &&
              abs(newKeplerMeanElements.longitudeOfAscendingNode - KeplerMeanElements.longitudeOfAscendingNode) < tolerance &&
              abs(newKeplerMeanElements.largeSemiAxis            - KeplerMeanElements.largeSemiAxis           ) < tolerance &&
              abs(newKeplerMeanElements.eccentricity             - KeplerMeanElements.eccentricity            ) < tolerance &&
              abs(newKeplerMeanElements.pericenterArgument       - KeplerMeanElements.pericenterArgument      ) < tolerance &&

              (abs(newKeplerMeanElements.meanAnomaly             - KeplerMeanElements.meanAnomaly             ) < tolerance ||
               abs(newKeplerMeanElements.meanAnomaly             - KeplerMeanElements.meanAnomaly + 2 * M_PI  ) < tolerance ||
               abs(newKeplerMeanElements.meanAnomaly             - KeplerMeanElements.meanAnomaly - 2 * M_PI  ) < tolerance)
        ) )
        {
            std::stringstream buff;
            buff << "Error in conversion from Kepler true elements to Kepler mean elements. Line " << i + 1;
            throw Ballistics::ProjectException(buff.str());
        }
    }
    std::cout << "Test 21 passed successfully!" << std::endl;
}