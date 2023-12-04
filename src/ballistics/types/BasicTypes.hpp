#ifndef PROJECT_BALLISTICS_BASICTYPES_HPP
#define PROJECT_BALLISTICS_BASICTYPES_HPP

#include <cstdint>
#include "../third_party/Eigen/Dense"

namespace Ballistics
{
    using scalar = double;
    using integer = std::int32_t;
    using index = unsigned int;
    using vector3d = Eigen::Vector3d;
    using vector4d = Eigen::Vector4d;
    using vectorxd = Eigen::VectorXd;
    using matrix3d = Eigen::Matrix3d;
    using quaterniond = Eigen::Quaterniond;

    template<typename T, integer size>
    using vector = Eigen::Vector<T, size>;

    template<typename T>
    using vectorX = Eigen::VectorX<T>;

    template<typename T, index n, index m>
    using matrix = Eigen::Matrix<T, n, m>;

    template<typename T>
    using matrixX = Eigen::MatrixX<T>;

    template<typename T, index n, index m>
    using diagMatrix = Eigen::DiagonalMatrix<T, n, m>;

}

#endif //PROJECT_BALLISTICS_BASICTYPES_HPP
