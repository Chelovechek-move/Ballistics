#ifndef PROJECT_BALLISTICS_NEWTONINTERPOLATOR_H
#define PROJECT_BALLISTICS_NEWTONINTERPOLATOR_H

#include "ballistics/types/BasicTypes.hpp"
#include <vector>

namespace Ballistics::Interpolation::Newton
{
    [[nodiscard]] std::vector<scalar> calcCoefficients(const std::vector<scalar>& x,
                                                       const std::vector<scalar>& y) {
        integer n = x.size();
        std::vector<scalar> coefficients(n);
        std::vector<std::vector<scalar>> dividedDifferences(n, std::vector<scalar>(n));

        for (int i = 0; i < n; ++i) {
            dividedDifferences[i][0] = y[i];
        }

        for (int j = 1; j < n; ++j) {
            for (int i = 0; i < n - j; ++i) {
                dividedDifferences[i][j] = (dividedDifferences[i + 1][j - 1] - dividedDifferences[i][j - 1]) / (x[i + j] - x[i]);
            }
        }

        for (int i = 0; i < n; ++i) {
            coefficients[i] = dividedDifferences[0][i];
        }

        return coefficients;
    }

    [[nodiscard]] scalar evaluateNewtonPolynomial(const std::vector<scalar>& coefficients,
                                                  const std::vector<scalar>& x,
                                                  double point) {
        integer n = coefficients.size();
        scalar result = coefficients[n - 1];

        for (int i = n - 2; i >= 0; --i) {
            result = result * (point - x[i]) + coefficients[i];
        }

        return result;
    }

    [[nodiscard]] scalar evaluateNewtonPolynomialDerivative(const std::vector<scalar>& coefficients,
                                                            const std::vector<scalar>& x,
                                                            const scalar point) {
        integer n = x.size();
        scalar result = coefficients[n - 1];

        for (int i = n - 2; i >= 1; --i) {
            result = result * (point - x[i]) + coefficients[i] * i;
        }

        return result;
    }
}

#endif //PROJECT_BALLISTICS_NEWTONINTERPOLATOR_H
