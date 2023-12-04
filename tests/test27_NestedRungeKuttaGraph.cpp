#include "ballistics/integrators/nestedRungeKutta/NestedRungeKutta.hpp"
#include "ballistics/integrators/nestedRungeKutta/ButcherTableNRK7_8.hpp"
#include "ballistics/integrators/RightPartOfDiffEq.hpp"
#include "ballistics/types/BasicTypes.hpp"
#include <iostream>
#include <fstream>

const std::string FILE_PATH = __FILE__;
const int amountOfSymbolsBeforeRootDirectory = 38;
const std::string DIR_PATH = FILE_PATH.substr(0, FILE_PATH.size() - amountOfSymbolsBeforeRootDirectory);

[[nodiscard]] Ballistics::scalar max(const std::vector<Ballistics::scalar> &mas) {
    Ballistics::scalar max = mas[0];
    for (const auto el : mas) {
        if (el > max) {
            max = el;
        }
    }
    return max;
}

struct Point {
    Ballistics::scalar error;
    Ballistics::scalar step;
};

int main() {
    using namespace Ballistics;

    std::vector<Point> res{};

    const scalar tolInit = 1e-6;
    const Ballistics::index numberOfSteps = 7;

    const scalar tFinal = 10 * M_PI;
    const scalar stepMin = 0.0001;
    const scalar stepMax = 0.1;
    scalar tolerance = tolInit;
    const scalar p = Integrators::NestedRungeKutta::ButcherTableNRK7_8::approxOrder;;

    const Integrators::RightPart4::State initState = {vector<scalar, 1>{0.}, 0.};

    for (integer i = 0; i < numberOfSteps; ++i) {
        const Integrators::NestedRungeKutta::IntegrationParameters<Integrators::RightPart4> intParams = {tFinal,
                                                                                                         stepMin,
                                                                                                         stepMax,
                                                                                                         tolerance,
                                                                                                         p};
        const Integrators::RightPart4::Params params = {0.};
        const std::vector<Integrators::RightPart4::State> solution =
                Integrators::NestedRungeKutta::NestedRungeKutta<Integrators::NestedRungeKutta::ButcherTableNRK7_8>::
                        calc<Integrators::RightPart4>(initState,
                                                      params,
                                                      intParams);
        std::vector<scalar> exactSolution{};
        std::vector<scalar> delta{};
        for (integer j = 0; j <= solution.size(); ++j) {
            exactSolution.push_back(sin(solution[j].t));
            delta.push_back(abs(solution[j].u[0] - exactSolution[j]));
        }

        const scalar maxError = max(delta);
        const scalar stepMean = abs(solution[solution.size() - 1].t - solution[0].t) /
                                                                                static_cast<scalar>(solution.size());
        res.push_back({maxError, stepMean});

        tolerance /= 10.;
    }

    std::ofstream out;
    out.open(DIR_PATH + "tests/utility/test27Data.txt");
    for (const auto el : res)
    {
        out << el.error << " ";
        out << el.step << std::endl;
    }

    // Uncomment next rows to automatically build a graph
//    std::string filename = DIR_PATH + "tests/test27_NestedRungeKuttaGraph.py";
//    std::string command = "python3 ";
//    command += filename;
//    system(command.c_str());
}