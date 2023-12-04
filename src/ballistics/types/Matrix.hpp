#ifndef PROJECT_BALLISTICS_MATRIX_HPP
#define PROJECT_BALLISTICS_MATRIX_HPP

#include "../third_party/Eigen/Dense"
#include "ballistics/types/BasicTypes.hpp"

namespace Ballistics
{
    template<typename T>
    Eigen::Matrix<T, 3, 3> toMatrix3(T m[3][3]) noexcept
    {
        Eigen::Matrix<T, 3, 3> M {{m[0][0], m[0][1], m[0][2]},
                                  {m[1][0], m[1][1], m[1][2]},
                                  {m[2][0], m[2][1], m[2][2]}
        };
        return M;
    }

    template<typename T>
    Eigen::Vector3<T> toVector3(T vec[3]) noexcept
    {
        Eigen::Vector3<T> vector = {vec[0], vec[1], vec[2]};
        return vector;
    }

    static Eigen::Quaterniond toQuaternion(scalar m[3][3]) noexcept
    {
        Eigen::Matrix M = toMatrix3(m);
        Eigen::Quaterniond Q(M);
        return Q;
    }

    static bool isEqualByRelativeError(const Eigen::Vector3d &l, const Eigen::Vector3d &r, const scalar tolerance)
    {
        return (l - r).norm() <= tolerance * r.norm();
    }

    template<typename T, index size>
    static bool isEqualByRelativeError(const vector<T, size> &l, const vector<T, size> &r, const scalar tolerance)
    {
        return (l - r).norm() <= tolerance * r.norm();
    }

    static bool isEqualByAbsoluteError(const Eigen::Vector3d &l,const Eigen::Vector3d &r, const scalar tolerance)
    {
        return (l - r).norm() < tolerance;
    }
}

#endif //PROJECT_BALLISTICS_MATRIX_HPP
