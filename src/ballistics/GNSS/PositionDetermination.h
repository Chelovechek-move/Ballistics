#ifndef PROJECT_BALLISTICS_POSITIONDETERMINATION_H
#define PROJECT_BALLISTICS_POSITIONDETERMINATION_H

#include <iostream>
#include "ballistics/types/BasicTypes.hpp"
#include "ballistics/constants/FundamentalConstants.hpp"
#include "ballistics/types/Matrix.hpp"

namespace Ballistics::GNSS
{
    struct determinedParameters {
        scalar deltaXr;
        scalar deltaYr;
        scalar deltaZr;
        scalar deltaTr;
    };

    [[nodiscard]] vector4d toVector(const determinedParameters& parameters) noexcept {
        return {parameters.deltaXr, parameters.deltaYr, parameters.deltaZr, parameters.deltaTr};
    }

    [[nodiscard]] determinedParameters toStruct(const vector4d& vector) noexcept {
        return {vector[0], vector[1], vector[2], vector[3]};
    }

    struct externalParameters {
        scalar pseudorange;
        scalar deltaXs;
        scalar deltaYs;
        scalar deltaZs;
        scalar deltaTs;
    };

    [[nodiscard]] vector3d getPositionVector(const externalParameters& parameters) noexcept {
        return {parameters.deltaXs, parameters.deltaYs, parameters.deltaZs};
    }

    [[nodiscard]] scalar calcTroposphericCorrection(const vector3d& receiverPosition,
                                                    const vector3d& satellitePosition,
                                                    const scalar ZHD,
                                                    const scalar ZWD) noexcept {
        const scalar m = 1. /
                (receiverPosition.dot(satellitePosition) / (receiverPosition.norm() * satellitePosition.norm()));
        return m * (ZHD + ZWD);
    }

    [[nodiscard]] scalar calcGeometricDistance(const vector3d& receiverPosition,
                                               const vector3d& satellitePosition) noexcept {
        return (satellitePosition - receiverPosition).norm();
    }

    [[nodiscard]] scalar calcRelativisticCorrection(const vector3d& satellitePosition,
                                                    const vector3d& satelliteVelocity,
                                                    const scalar speedOfLight) noexcept {
        return satellitePosition.dot(satelliteVelocity) / (speedOfLight * speedOfLight);
    }

    [[nodiscard]] vector3d calcVelocity(const std::vector<externalParameters>& externalData) noexcept {
        // TODO: Тут заглушка, дописать функцию
        return {0., 0., 0.};
    }

    enum class DifferentiationVariable {
        deltaX = 0,
        deltaY = 1,
        deltaZ = 2
    };

    [[nodiscard]] scalar calcGeometricDistanceDerivative(const vector3d& receiverPosition,
                                                         const vector3d& satellitePosition,
                                                         const DifferentiationVariable diffVariable) noexcept {
        const vector3d deltaPositions = satellitePosition - receiverPosition;
        return -deltaPositions[static_cast<integer>(diffVariable)] / sqrt(deltaPositions.x() * deltaPositions.x() +
                                                                         deltaPositions.y() * deltaPositions.y() +
                                                                         deltaPositions.z() * deltaPositions.z());
    }

    template<integer numberOfSatellites>
    [[nodiscard]] determinedParameters determinePosition(const std::vector<externalParameters>& externalData,
                                                         const determinedParameters& initialApproximation,
                                                         const diagMatrix<scalar, numberOfSatellites, numberOfSatellites> P,
                                                         const scalar tolerance) noexcept {
        vector4d x = toVector(initialApproximation);
        vector4d xPrev;
        matrix<scalar, numberOfSatellites, 4> A;
        vector<scalar, numberOfSatellites> b;
        matrix<scalar, 4, 4> covarianceMatrix;
        bool isItFirstIteration = true;
        integer numberOfIterations = 0;
        while(!isEqualByRelativeError<scalar, 4>(x, xPrev, tolerance) || isItFirstIteration) {
            xPrev = x;
            // Заполняем матрицу частными производными геометрической дальности и скоростью света
            for (int i = 0; i < numberOfSatellites; ++i) {
                A.row(i) = vector<scalar, 4>{calcGeometricDistanceDerivative(x.head(3),
                                                                             getPositionVector(externalData[i]),
                                                                             DifferentiationVariable::deltaX),
                                             calcGeometricDistanceDerivative(x.head(3),
                                                                             getPositionVector(externalData[i]),
                                                                             DifferentiationVariable::deltaY),
                                             calcGeometricDistanceDerivative(x.head(3),
                                                                             getPositionVector(externalData[i]),
                                                                             DifferentiationVariable::deltaZ),
                                             speedOfLight
                };
            }

            const scalar ZHD = 2.5;
            const scalar ZWD = 0.;
            // Заполняем столбец правой части
            for (int i = 0; i < numberOfSatellites; ++i) {
                scalar lol1 = calcTroposphericCorrection(x.head(3),{externalData[i].deltaXs,
                                                                    externalData[i].deltaYs,
                                                                    externalData[i].deltaZs}, ZHD, ZWD);
                scalar lol2 = speedOfLight * externalData[i].deltaTs;
                const scalar correction =
                        calcGeometricDistance(x.head(3),{externalData[i].deltaXs,
                                                                                       externalData[i].deltaYs,
                                                                                       externalData[i].deltaZs})
//                        calcRelativisticCorrection({externalData[i].deltaXs,
//                                                    externalData[i].deltaYs,
//                                                    externalData[i].deltaZs},
//                                                   calcVelocity(externalData),
//                                                   Ballistics::speedOfLight)
                        -
                        speedOfLight * externalData[i].deltaTs
                        +
                        (isItFirstIteration ? 0. : calcTroposphericCorrection(x.head(3),
                                                                              {externalData[i].deltaXs,
                                                                               externalData[i].deltaYs,
                                                                               externalData[i].deltaZs}, ZHD, ZWD));

                b[i] = externalData[i].pseudorange - correction;
            }
            isItFirstIteration = false;

            matrix<scalar, 4, numberOfSatellites> ATransposedP = A.transpose() * P;
            matrix<scalar, 4, 4> N = ATransposedP * A;
            vector<scalar, 4> g = ATransposedP * b;
            covarianceMatrix = N.inverse();
            x += covarianceMatrix * g;

            std::cout << "Number of iteration:" << numberOfIterations << std::endl;
//            std::cout << "Matrix A: " << std::endl << A << std::endl;
            std::cout << "Vector x:" << std::endl << x << std::endl;
            std::cout << "Covariance matrix:" << std::endl << covarianceMatrix << std::endl;
            numberOfIterations++;
        }

        return toStruct(x);
    }
}

#endif //PROJECT_BALLISTICS_POSITIONDETERMINATION_H
