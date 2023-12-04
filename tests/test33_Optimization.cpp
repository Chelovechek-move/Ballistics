#include "ballistics/optimization/OneDimOptimization.hpp"
#include "ballistics/exception/ProjectException.hpp"
#include <iostream>

using namespace Ballistics;

class FuncType1 {
public:
    using ArgType = scalar;
    [[nodiscard]] static inline ArgType calcFunc(const ArgType arg) {
        return arg * arg - 4. * arg + 4.;
    }
};

class FuncType2 {
public:
    using ArgType = scalar;
    [[nodiscard]] static inline ArgType calcFunc(const ArgType arg) {
        return arg;
    }
};

class FuncType3 {
public:
    using ArgType = scalar;
    [[nodiscard]] static inline ArgType calcFunc(const ArgType arg) {
        return cosh(arg - 2.);
    }
};

class FuncType4 {
public:
    using ArgType = scalar;
    [[nodiscard]] static inline ArgType calcFunc(const ArgType arg) {
        return (arg - 1.) * (arg - 1.) * (arg - 1.) * (arg - 1.) + 1.;
    }
};

class Stopper {
public:
    constexpr static scalar tolerance = 1e-15;

    [[nodiscard]] static inline bool isFinish(const scalar lBorder,
                                              const scalar rBorder) {
        return abs(rBorder - lBorder) < tolerance;
    }
};

int main() {
    const scalar lBorder = -5.;
    const scalar rBorder = 10.;

    const scalar min1 = Optimization::Dichotomy::Optimize<FuncType1, Stopper>(lBorder, rBorder);
    const scalar min2 = Optimization::Dichotomy::Optimize<FuncType2, Stopper>(lBorder, rBorder);
    const scalar min3 = Optimization::Dichotomy::Optimize<FuncType3, Stopper>(lBorder, rBorder);
    const scalar min4 = Optimization::Dichotomy::Optimize<FuncType4, Stopper>(lBorder, rBorder);

    const scalar tolerance1 = 1e-15;
    const scalar tolerance2 = 1e-7;
    const scalar tolerance3 = 1e-3;

    if (abs(min1 - 2.)    > tolerance2 ||
        abs(min2 - (-5.)) > tolerance1 ||
        abs(min3 - 2.)    > tolerance2 ||
        abs(min4 - 1.)    > tolerance3) {
        std::stringstream buff;
        buff << "Error in optimization!" << std::endl;
        throw Ballistics::ProjectException(buff.str());
    }

    std::cout << "Test 33 passed successfully!" << std::endl;
}