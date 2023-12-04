#ifndef BALLISTICS_CHINIAEV_TIME_HPP
#define BALLISTICS_CHINIAEV_TIME_HPP

#include "../types/BasicTypes.hpp"
#include "../constants/TimeConstants.hpp"
#include "../exception/ProjectException.hpp"
#include "../../../third_party/include/sofa.h"
#include <cmath>
#include <sstream>

namespace Ballistics::Time
{
    enum class Scale
    {
        TAI = 0,
        UTC = 1,
        UT1 = 2,
        TT = 3,
        TCG = 4,
        TCB = 5,
        TDB = 6
    };

    template<Scale scale = Scale::TAI>
    class Time
    {
        integer jdDay_;
        scalar jdPart_;
    public:
        /** The default constructor of a Time class
        */
        inline constexpr Time() noexcept = default;

        /** Constructor of a Time class by integer and scalar
        * @param jdDay whole part of the days
        * @param jdPart fractional part of the day
        * @return Time class object
        */
        inline constexpr Time(const integer jdDay, const scalar jdPart) : jdDay_(jdDay), jdPart_(jdPart)
        {
            checkTimeFormat();
        }

        /** Constructor of a Time class by two scalars
        * @param jdDay whole part of the days
        * @param jdPart fractional part of the day
        * @return Time class object
        */
        inline constexpr Time(const scalar jdDay, const scalar jdPart)
        {
            jdDay_ = static_cast<integer>(jdDay);
            jdPart_ = jdDay - std::floor(jdDay) + jdPart;

            checkTimeFormat();
        }

        /** A method for converting an object of the Time type to the correct format
        */
        void checkTimeFormat()
        {
            if (static_cast<scalar>(jdDay_) + jdPart_ < 0.)
            {
                std::stringstream buff;
                buff << "The time value cannot be less than zero" << ". File: " << __FILE__ << ". Line: " << __LINE__;
                throw ProjectException(buff.str());
            }
            while(jdPart_ >= 1.)
            {
                jdPart_ -= 1.;
                jdDay_ += 1;
            }
            while(jdPart_ < 0.)
            {
                jdPart_ += 1.;
                jdDay_ -= 1;
            }
        }

        /** Operator for adding seconds to Time class object
        * @param s number of seconds
        * @return new moment of time
        */
        [[nodiscard]] constexpr Time operator+(const scalar s) const noexcept
        {
            integer jdDay = jdDay_;
            scalar jdPart = jdPart_;
            return {jdDay, jdPart + s / SECONDS_IN_DAY};
        }

        /** Operator for subtracting seconds from Time class object
        * @param s number of seconds
        * @return new moment of time
        */
        [[nodiscard]] constexpr Time operator-(const scalar s) const noexcept
        {
            integer jdDay = jdDay_;
            scalar jdPart = jdPart_;
            return {jdDay, jdPart - s / SECONDS_IN_DAY};
        }

        /** Operator for subtracting one moment of time from another
        * @param r subtracted point in time
        * @return the difference between two points in time in seconds
        */
        [[nodiscard]] constexpr scalar operator-(const Time &r) const noexcept
        {
            integer jdDay = jdDay_;
            scalar jdPart = jdPart_;
            return ((jdDay - r.jdDay_) + (jdPart - r.jdPart_)) * SECONDS_IN_DAY;
        }

        /** The " < " operator for comparing two moments in time
        * @param r moment in time on the right from " < "
        * @return result of the operator " < " for two moments in time
        */
        [[nodiscard]] constexpr bool operator<(const Time &r) const noexcept
        {
            return jdDay_ + jdPart_ < r.jdDay_ + r.jdPart_;
        }

        /** The " > " operator for comparing two moments in time
        * @param r moment in time on the right from " > "
        * @return result of the operator " > " for two moments in time
        */
        [[nodiscard]] constexpr bool operator>(const Time &r) const noexcept
        {
            return jdDay_ + jdPart_ > r.jdDay_ + r.jdPart_;
        }

        /** A method for comparing two objects of the Time type with a given tolerance accuracy
        * @return time in mjd
        */
        [[nodiscard]] constexpr scalar toMJD() const noexcept
        {
            return static_cast<scalar>(jdDay_) + jdPart_ - DELTA_JD_MJD;
        }

        /** A method for comparing two Time objects with a certain precision
        * @param time a moment of time for comparison
        * @param tolerance accuracy
        * @return 1 if two Time type objects are equal, 0 if not
        */
        [[nodiscard]] constexpr bool isEqual(const Time &time, const scalar tolerance = 1e-15) const noexcept
        {
            return std::abs(static_cast<scalar>(jdDay_ - time.getDay()) + (jdPart_ - time.getPart())) < tolerance;
        }

        /** Operator for comparing two Time objects
        * @param time a moment of time for comparison
        * @return 1 if two Time type objects are equal, 0 if not
        */
        [[nodiscard]] constexpr bool operator==(const Time &time) const noexcept
        {
            return jdDay_ == time.getDay() && jdPart_ == time.getPart();
        }

        /** A method that returns the whole part of the days
        * @return whole part of the days
        */
        [[nodiscard]] constexpr integer getDay() const noexcept {return jdDay_;}

        /** A method that returns the fractional part of days
        * @return the fractional part of days
        */
        [[nodiscard]] constexpr scalar getPart() const noexcept {return jdPart_;}
    };

    /** Function for calculating number of days since the beginning of the year
     * @param t moment of time in scale UTC
     * @return number of days since the beginning of the year
     */
    [[nodiscard]] static  scalar calcDaysSinceJan1(const Time<Scale::UTC> &t)
    {
        int year, month, day;
        double fd;
        const int i = iauJd2cal(t.getDay(), t.getPart(), &year, &month, &day, &fd);
        if (i != 0)
        {
            std::stringstream buff;
            buff << "The return value of the iauJd2cal() function must be equal to 0. The received value: " << i << ". File: " << __FILE__ << ". Line: " << __LINE__;
            throw ProjectException(buff.str());
        }

        scalar t1, t2;
        const int j = iauDtf2d("UTC", year - 1, 12, 31, 0, 0, 0., &t1, &t2);
        if (j != 0)
        {
            std::stringstream buff;
            buff << "The return value of the iauDtf2d() function must be equal to 0. The received value: " << j << ". File: " << __FILE__ << ". Line: " << __LINE__;
            throw ProjectException(buff.str());
        }

        return static_cast<scalar>(t.getDay()) + t.getPart() - t1 - t2;
    }

    /** Function for calculating the moment of time at midnight of a given day
     * @param t moment of time in scale UTC
     * @return moment of time at midnight of a given day in scale UTC
     */
    [[nodiscard]] static Time<Scale::UTC> calcTimeInMidnight(const Time<Scale::UTC> &t)
    {
        int year, month, day;
        double fd;
        const int i = iauJd2cal(t.getDay(), t.getPart(), &year, &month, &day, &fd);
        if (i != 0)
        {
            std::stringstream buff;
            buff << "The return value of the iauJd2cal() function must be equal to 0. The received value: " << i << ". File: " << __FILE__ << ". Line: " << __LINE__;
            throw ProjectException(buff.str());
        }

        scalar t1, t2;
        const int j = iauDtf2d("UTC", year, month, day, 0, 0, 0., &t1, &t2);
        if (j != 0)
        {
            std::stringstream buff;
            buff << "The return value of the iauDtf2d() function must be equal to 0. The received value: " << j << ". File: " << __FILE__ << ". Line: " << __LINE__;
            throw ProjectException(buff.str());
        }

        return {t1, t2};
    }
}

#endif //BALLISTICS_CHINIAEV_TIME_HPP
