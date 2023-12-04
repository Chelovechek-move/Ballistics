#ifndef PROJECT_BALLISTICS_GRAVITYFORCE_HPP
#define PROJECT_BALLISTICS_GRAVITYFORCE_HPP

#include "../types/BasicTypes.hpp"
#include "ballistics/time/Time.hpp"
#include "../third_party/GeographicLib/include/GravityModel.hpp"

namespace Ballistics::Forces
{
    class GravityForce
    {
    private:
        GeographicLib::GravityModel model_;
    public:
        /** Constructor of a GravityForce class
         * Detailed description: third_party/GeographicLib/include/GravityModel.hpp
         * @param modelName name of the model
         * @param fileLocation directory for data file
         * @param n (optional) limitation on the degree of the model
         * @param m (optional) limitation on the order of the model
         * @return Gravity class object
         */
        GravityForce(const std::string &modelName,
                     const std::string &fileLocation,
                     const integer n = -1,
                     const integer m = -1) noexcept
                     : model_(modelName, fileLocation, n, m) {}

        /** Method for calculating gravity force from Earth
         * @param mass mass of the satellite
         * @param rGCRS radius vector of the body in the GCRS system
         * @param q transition quaternion from GCRS to ITRS system
         * @param timeTT moment of time in TT scale
         * @param timeUT1 moment of time in UT1 scale
         * @return value of the gravity force
         */
        [[nodiscard]] vector3d calcForce(const scalar mass,
                                         const vector3d &rGCRS,
                                         const quaterniond &q,
                                         const Time::Time<Time::Scale::TT> &timeTT,
                                         const Time::Time<Time::Scale::UT1> &timeUT1) const noexcept;
    };
}

#endif //PROJECT_BALLISTICS_GRAVITYFORCE_HPP
