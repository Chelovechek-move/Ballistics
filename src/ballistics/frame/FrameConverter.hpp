#ifndef PROJECT_BALLISTICS_FRAMECONVERTER_HPP
#define PROJECT_BALLISTICS_FRAMECONVERTER_HPP

#include "include/sofa.h"
#include "include/sofam.h"
#include "../time/Time.hpp"
#include "../types/Matrix.hpp"
#include "../types/BasicTypes.hpp"
#include "../../../third_party/include/rapidcsv.h"

namespace Ballistics::Frame
{
    enum class Frame
    {
        ITRS = 0,
        GCRS = 1,
    };

    struct Pole
    {
        scalar xp;
        scalar yp;
    };

    class FrameConverter
    {
        std::string csvLocation_;
        std::vector<int> mjdCol_;
        std::vector<double> xCol_;
        std::vector<double> yCol_;
    public:
        /** Constructor of a FrameConverter
        * @param csvLocation location of the csv file
        */
        inline FrameConverter(const std::string &csvLocation) noexcept : csvLocation_(csvLocation)
        {
            parse();
        }

        /** A method for parsing a csv file and filling in private class fields
        */
        void parse() noexcept
        {
            rapidcsv::Document doc(csvLocation_);
            mjdCol_ = doc.GetColumn<int>("mjd");
            xCol_ = doc.GetColumn<double>("x arcsec");
            yCol_ = doc.GetColumn<double>("y arcsec");
        }

        /** Method for interpolating pole coordinates by a given time value in mjd
        * @param mgd time values in mjd
        * @return the interpolated value of the pole coordinates
        */
        [[nodiscard]] Pole interpolate(const scalar mjd) const noexcept
        {
            scalar xp = 0.;
            scalar yp = 0.;
            for(int i = 0; i < mjdCol_.size(); ++i)
            {
                if(mjdCol_[i] <= mjd && mjdCol_[i + 1] > mjd)
                {
                    xp = ((mjd - mjdCol_[i]) / (mjdCol_[i + 1] - mjdCol_[i])) * (xCol_[i + 1] - xCol_[i]) + xCol_[i];
                    yp = ((mjd - mjdCol_[i]) / (mjdCol_[i + 1] - mjdCol_[i])) * (yCol_[i + 1] - yCol_[i]) + yCol_[i];
                }
            }
            return {xp * DAS2R, yp * DAS2R};
        }

        /** Method for getting pole coordinates at a given time value in mjd
        * @param mgd time values in mjd
        * @return value of the pole coordinates
        */
        [[nodiscard]] Pole getPole(const scalar mjd) const noexcept
        {
            return interpolate(mjd);
        }

        template<Frame From, Frame To>
        Eigen::Quaterniond calcQuaternion(const Time::Time<Time::Scale::TT> &TT,
                                          const Time::Time<Time::Scale::UT1> &UT1,
                                          const Time::Time<Time::Scale::UTC> &UTC)
                                          const noexcept;
    };

    template<>
    Eigen::Quaterniond FrameConverter::calcQuaternion<Frame::GCRS, Frame::ITRS>(const Time::Time<Time::Scale::TT> &TT,
                                                                                const Time::Time<Time::Scale::UT1> &UT1,
                                                                                const Time::Time<Time::Scale::UTC> &UTC)
                                                                                const noexcept
    {
        scalar x, y, s;
        iauXys06a(TT.getDay(), TT.getPart(), &x, &y, &s);

        scalar rc2i[3][3];
        iauC2ixys(x, y, s, rc2i);

        scalar era = iauEra00(UT1.getDay(), UT1.getPart());
        scalar rc2ti[3][3];
        iauCr(rc2i, rc2ti);
        iauRz(era, rc2ti);

        scalar sp = iauSp00(TT.getDay(), TT.getPart());
        scalar rpom[3][3];
        Pole pole = getPole(UTC.toMJD());
        iauPom00(pole.xp, pole.yp, sp, rpom);
        scalar rc2it[3][3];
        iauRxr(rpom, rc2ti, rc2it);

        return toQuaternion(rc2it);
    }

    template<>
    Eigen::Quaterniond FrameConverter::calcQuaternion<Frame::ITRS, Frame::GCRS>(const Time::Time<Time::Scale::TT> &TT,
                                                                                const Time::Time<Time::Scale::UT1> &UT1,
                                                                                const Time::Time<Time::Scale::UTC> &UTC)
                                                                                const noexcept
    {
        return calcQuaternion<Frame::GCRS, Frame::ITRS>(TT, UT1, UTC).inverse();
    }
}

#endif //PROJECT_BALLISTICS_FRAMECONVERTER_HPP
