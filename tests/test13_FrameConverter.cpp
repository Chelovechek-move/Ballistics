#include "../src/ballistics/frame/FrameConverter.hpp"
#include "../src/ballistics/time/TimeConverter.hpp"
#include "utility/EarthRotationResult.hpp"

int main()
{
    using namespace Ballistics::Time;
    using namespace Ballistics;
    struct Line
    {
        scalar mjd;
        Eigen::Vector3d v1;
        Eigen::Vector3d v2;
        Eigen::Vector3d v3;
    };

    Ballistics::Frame::FrameConverter FrameConverter = Ballistics::Frame::FrameConverter("../../resources/earth_rotation.csv");
    Ballistics::Time::TimeConverter TimeConverter = Ballistics::Time::TimeConverter("../../resources/earth_rotation.csv");
    Eigen::Vector3d initVec1 = {6700e3, 0., 0.};
    Eigen::Vector3d initVec2 = {0., 6700e3, 0.};
    Eigen::Vector3d initVec3 = {0., 0., 6700e3};
    scalar tolerance = 5e-14;

    for (const auto el : earthRotationResult)
    {
        Line line = {el[0], {el[1], el[2], el[3]}, {el[4], el[5], el[6]}, {el[7], el[8], el[9]}};
        auto const timeTT = Ballistics::Time::Time<Ballistics::Time::Scale::TT>(line.mjd, 0.);
        auto const timeUTC = TimeConverter.convert<Ballistics::Time::Scale::TT, Ballistics::Time::Scale::UTC>(timeTT);
        auto const timeUT1 = TimeConverter.convert<Ballistics::Time::Scale::TT, Ballistics::Time::Scale::UT1>(timeTT);

        Eigen::Quaterniond Q = FrameConverter.calcQuaternion<Frame::Frame::GCRS, Frame::Frame::ITRS>(timeTT,
                                                                                                     timeUT1,
                                                                                                     timeUTC);

        Eigen::Vector3d newVec1 = Q * initVec1;
        Eigen::Vector3d newVec2 = Q * initVec2;
        Eigen::Vector3d newVec3 = Q * initVec3;

        if (isEqualByRelativeError(newVec1, line.v1, tolerance) &&
            isEqualByRelativeError(newVec2, line.v2, tolerance) &&
            isEqualByRelativeError(newVec3, line.v3, tolerance))
        {}
        else
        {
            throw Ballistics::ProjectException("Error!!!");
        }

    }
    std::cout << "Test 13 passed" << std::endl;
}