#include "ballistics/integrators/nestedRungeKutta/NestedRungeKutta.hpp"
#include "ballistics/integrators/nestedRungeKutta/ButcherTableNRK5_4.hpp"
#include "ballistics/integrators/nestedRungeKutta/ButcherTableNRK7_8.hpp"
#include "ballistics/integrators/RightPartOfDiffEq.hpp"
#include "ballistics/types/BasicTypes.hpp"
#include <fstream>
#include "iostream"

const std::string FILE_PATH = __FILE__;
const int amountOfSymbolsBeforeRootDirectory = 33;
const std::string DIR_PATH = FILE_PATH.substr(0, FILE_PATH.size() - amountOfSymbolsBeforeRootDirectory);

int main() {
    using namespace Ballistics;

    const Integrators::RightPart4::State initState = {vector<scalar, 1>{1.}, 1.};
    const Integrators::RightPart4::Params params = {0.};
    const scalar tFinal = 4 * M_PI;
    const scalar stepMin = 0.0001;
    const scalar stepMax = 1.;
    const scalar tolerance = 1e-10;
    const scalar p = Integrators::NestedRungeKutta::ButcherTableNRK5_4::approxOrder;
    const Integrators::NestedRungeKutta::IntegrationParameters<Integrators::RightPart4> intParams = {tFinal,
                                                                                                     stepMin,
                                                                                                     stepMax,
                                                                                                     tolerance,
                                                                                                     p};
    const std::vector<Integrators::RightPart4::State> solution =
            Integrators::NestedRungeKutta::NestedRungeKutta<Integrators::NestedRungeKutta::ButcherTableNRK5_4>::
                    calc<Integrators::RightPart4>(initState,
                                                  params,
                                                  intParams);

    std::ofstream out;
    out.open(DIR_PATH + "tests/utility/test26Data.txt");
    for (const auto &el : solution)
    {
        out << el.t << " ";
        out << el.u[0] << std::endl;
    }

    // Uncomment next rows to automatically build a graph
//    std::string filename = DIR_PATH + "tests/test26_NestedRungeKutta.py";
//    std::string command = "python3 ";
//    command += filename;
//    system(command.c_str());
}