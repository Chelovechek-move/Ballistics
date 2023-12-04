#include "ballistics/interpolation/NewtonInterpolator.h"
#include <iostream>

using namespace Ballistics;

int main() {
    // Пример использования функций
    std::vector<double> x = {1.0, 2.0, 3.0, 4.0};
    std::vector<double> y = {1.0, 4.0, 9.0, 16.0};

    // Построение интерполяционного полинома Ньютона и получение коэффициентов
    std::vector<double> coefficients = Interpolation::Newton::calcCoefficients(x, y);

    // Вычисление значения интерполяционного полинома в точке
    double point = 1.;
    double interpolatedValue = Interpolation::Newton::evaluateNewtonPolynomial(coefficients, x, point);

    // Вычисление значения производной интерполяционного полинома в точке
    double derivativeValue = Interpolation::Newton::evaluateNewtonPolynomialDerivative(coefficients, x, point);

    std::cout << "Interpolated value at point " << point << ": " << interpolatedValue << std::endl;
    std::cout << "Derivative value at point " << point << ": " << derivativeValue << std::endl;

    return 0;
}