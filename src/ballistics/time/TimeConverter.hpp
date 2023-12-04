#ifndef PROJECT_BALLISTICS_TIMECONVERTER_HPP
#define PROJECT_BALLISTICS_TIMECONVERTER_HPP

#include "Time.hpp"
#include "../types/BasicTypes.hpp"
#include "../../../third_party/include/rapidcsv.h"
#include <array>
#include <vector>
#include <string>

namespace Ballistics::Time
{
    class TimeConverter
    {
        std::string csvLocation_;
        std::vector<int> mjdCol_;
        std::vector<double> dutCol_;
    public:
        /** Constructor of a TimeConverter
        * @param csvLocation location of the csv file
        */
        inline TimeConverter(const std::string csvLocation) noexcept : csvLocation_(csvLocation)
        {
            parse();
        }

        /** A method for parsing a csv file and filling in private class fields
        */
        void parse() noexcept
        {
            rapidcsv::Document doc(csvLocation_);
            mjdCol_ = doc.GetColumn<int>("mjd");
            dutCol_ = doc.GetColumn<double>("UT1-UTC s");
        }

        /** Method for interpolating dut by a given time value in mjd
        * @param mgd time values in mjd
        * @return interpolated dut value
        */
        [[nodiscard]] scalar interpolate(const scalar mjd) const noexcept
        {
            scalar dut = 0.;
            for(int i = 0; i < mjdCol_.size(); ++i)
            {
                if(mjdCol_[i] <= mjd && mjdCol_[i + 1] > mjd)
                {
                    dut = ((mjd - mjdCol_[i]) / (mjdCol_[i + 1] - mjdCol_[i])) * (dutCol_[i + 1] - dutCol_[i]) + dutCol_[i];
                }
            }
            return dut;
        }

        /** Method for getting dut at a given time value in mjd
        * @param mgd time values in mjd
        * @return dut value
        */
        [[nodiscard]] scalar getDut(const scalar mjd) const noexcept
        {
            return interpolate(mjd);
        }

        /** Method for converting time in jd format from one scale to another
        * @param t time in the original format
        * @return time in the new format
        */
        template<Scale From, Scale To>
        [[nodiscard]] Time<To> convert(Time<From> const &t) const;
    };

///////////////////////////////////////////////CONVERSION FROM TT///////////////////////////////////////////////////////
    template<>
    inline Time<Scale::TAI> TimeConverter::convert<Scale::TT, Scale::TAI>(Time<Scale::TT> const &t) const
    {
        scalar tai1, tai2;
        integer j = iauTttai(t.getDay(), t.getPart(), &tai1, &tai2);
        if (j != 0)
        {
            std::stringstream buff;
            buff << "The return value of the iauTttai() function must be equal to 0. The received value: " << j << ". File: " << __FILE__ << ". Line: " << __LINE__;
            throw ProjectException(buff.str());
        }
        return {tai1, tai2};
    }

    template<>
    inline Time<Scale::UTC> TimeConverter::convert<Scale::TT, Scale::UTC>(Time<Scale::TT> const &t) const
    {
        Time<Scale::TAI> TAI = convert<Scale::TT, Scale::TAI>(t);
        double utc1, utc2;
        int j = iauTaiutc(TAI.getDay(), TAI.getPart(), &utc1, &utc2);
        if (j != 0)
        {
            std::stringstream buff;
            buff << "The return value of the iauTaiutc() function must be equal to 0. The received value: " << j << ". File: " << __FILE__ << ". Line: " << __LINE__;
            throw ProjectException(buff.str());
        }
        return {utc1, utc2};
    }

    template<>
    inline Time<Scale::UT1> TimeConverter::convert<Scale::TT, Scale::UT1>(Time<Scale::TT> const &t) const
    {
        Time<Scale::UTC> UTC = convert<Scale::TT, Scale::UTC>(t);
        scalar ut11, ut12;
        int j = iauUtcut1(UTC.getDay(), UTC.getPart(), getDut(UTC.toMJD()), &ut11, &ut12);
        if (j != 0)
        {
            std::stringstream buff;
            buff << "The return value of the iauUtcut1() function must be equal to 0. The received value: " << j << ". File: " << __FILE__ << ". Line: " << __LINE__;
            throw ProjectException(buff.str());
        }
        return {ut11, ut12};
    }

    template<>
    inline Time<Scale::TCG> TimeConverter::convert<Scale::TT, Scale::TCG>(Time<Scale::TT> const &t) const
    {
        scalar tcg1, tcg2;
        integer j = iauTttcg(t.getDay(), t.getPart(), &tcg1, &tcg2);
        if (j != 0)
        {
            std::stringstream buff;
            buff << "The return value of the iauTttcg() function must be equal to 0. The received value: " << j << ". File: " << __FILE__ << ". Line: " << __LINE__;
            throw ProjectException(buff.str());
        }
        return {tcg1, tcg2};
    }

    template<>
    inline Time<Scale::TDB> TimeConverter::convert<Scale::TT, Scale::TDB>(Time<Scale::TT> const &t) const
    {
        scalar tdb1, tdb2;
        Time<Scale::UT1> UT1 = convert<Scale::TT, Scale::UT1>(t);

        scalar ut = fmod(UT1.getPart() + 0.5, 1.0);
        scalar dtr = iauDtdb(t.getDay(), t.getPart(), ut, 0., 0., 0.);

        integer j = iauTttdb(t.getDay(), t.getPart(), dtr, &tdb1, &tdb2);
        if (j != 0)
        {
            std::stringstream buff;
            buff << "The return value of the iauTttdb() function must be equal to 0. The received value: " << j << ". File: " << __FILE__ << ". Line: " << __LINE__;
            throw ProjectException(buff.str());
        }
        return {tdb1, tdb2};
    }

    template<>
    inline Time<Scale::TCB> TimeConverter::convert<Scale::TT, Scale::TCB>(Time<Scale::TT> const &t) const
    {
        Time<Scale::TDB> TDB = convert<Scale::TT, Scale::TDB>(t);
        scalar tcb1, tcb2;
        integer j = iauTdbtcb(TDB.getDay(), TDB.getPart(), &tcb1, &tcb2);
        if (j != 0)
        {
            std::stringstream buff;
            buff << "The return value of the iauTdbtcb() function must be equal to 0. The received value: " << j << ". File: " << __FILE__ << ". Line: " << __LINE__;
            throw ProjectException(buff.str());
        }
        return {tcb1, tcb2};
    }

    template<>
    inline Time<Scale::TT> TimeConverter::convert<Scale::TT, Scale::TT>(Time<Scale::TT> const &t) const
    {
        return t;
    }

///////////////////////////////////////////////CONVERSION FROM UTC//////////////////////////////////////////////////////
    template<>
    inline Time<Scale::UT1> TimeConverter::convert<Scale::UTC, Scale::UT1>(Time<Scale::UTC> const &t) const
    {
        scalar ut11, ut12;
        integer j = iauUtcut1(t.getDay(), t.getPart(), getDut(t.toMJD()), &ut11, &ut12);
        if (j != 0)
        {
            std::stringstream buff;
            buff << "The return value of the iauUtcut1() function must be equal to 0. The received value: " << j << ". File: " << __FILE__ << ". Line: " << __LINE__;
            throw ProjectException(buff.str());
        }
        return {ut11, ut12};
    }

    template<>
    inline Time<Scale::TAI> TimeConverter::convert<Scale::UTC, Scale::TAI>(Time<Scale::UTC> const &t) const
    {
        scalar tai1, tai2;
        integer j = iauUtctai(t.getDay(), t.getPart(), &tai1, &tai2);
        if (j != 0)
        {
            std::stringstream buff;
            buff << "The return value of the iauUtctai() function must be equal to 0. The received value: " << j << ". File: " << __FILE__ << ". Line: " << __LINE__;
            throw ProjectException(buff.str());
        }
        return {tai1, tai2};
    }

    template<>
    inline Time<Scale::TT> TimeConverter::convert<Scale::UTC, Scale::TT>(Time<Scale::UTC> const &t) const
    {
        Time<Scale::TAI> TAI = convert<Scale::UTC, Scale::TAI>(t);
        scalar tt1, tt2;
        integer j = iauTaitt(TAI.getDay(), TAI.getPart(), &tt1, &tt2);
        if (j != 0)
        {
            std::stringstream buff;
            buff << "The return value of the iauTaitt() function must be equal to 0. The received value: " << j << ". File: " << __FILE__ << ". Line: " << __LINE__;
            throw ProjectException(buff.str());
        }
        return {tt1, tt2};
    }

    template<>
    inline Time<Scale::TCG> TimeConverter::convert<Scale::UTC, Scale::TCG>(Time<Scale::UTC> const &t) const
    {
        Time<Scale::TT> TT = convert<Scale::UTC, Scale::TT>(t);
        scalar tcg1, tcg2;
        integer j = iauTttcg(TT.getDay(), TT.getPart(), &tcg1, &tcg2);
        if (j != 0)
        {
            std::stringstream buff;
            buff << "The return value of the iauTttcg() function must be equal to 0. The received value: " << j << ". File: " << __FILE__ << ". Line: " << __LINE__;
            throw ProjectException(buff.str());
        }
        return {tcg1, tcg2};
    }

    template<>
    inline Time<Scale::TDB> TimeConverter::convert<Scale::UTC, Scale::TDB>(Time<Scale::UTC> const &t) const
    {
        Time<Scale::TT> TT = convert<Scale::UTC, Scale::TT>(t);
        Time<Scale::TDB> TDB = convert<Scale::TT, Scale::TDB>(TT);
        return TDB;
    }

    template<>
    inline Time<Scale::TCB> TimeConverter::convert<Scale::UTC, Scale::TCB>(Time<Scale::UTC> const &t) const
    {
        Time<Scale::TDB> TDB = convert<Scale::UTC, Scale::TDB>(t);
        scalar tcb1, tcb2;
        integer j = iauTdbtcb(TDB.getDay(), TDB.getPart(), &tcb1, &tcb2);
        if (j != 0)
        {
            std::stringstream buff;
            buff << "The return value of the iauTdbtcb() function must be equal to 0. The received value: " << j << ". File: " << __FILE__ << ". Line: " << __LINE__;
            throw ProjectException(buff.str());
        }
        return {tcb1, tcb2};
    }

    template<>
    inline Time<Scale::UTC> TimeConverter::convert<Scale::UTC, Scale::UTC>(Time<Scale::UTC> const &t) const
    {
        return t;
    }

///////////////////////////////////////////////CONVERSION FROM TAI//////////////////////////////////////////////////////
    template<>
    inline Time<Scale::UTC> TimeConverter::convert<Scale::TAI, Scale::UTC>(Time<Scale::TAI> const &t) const
    {
        scalar utc1, utc2;
        integer j = iauTaiutc(t.getDay(), t.getPart(), &utc1, &utc2);
        if (j != 0)
        {
            std::stringstream buff;
            buff << "The return value of the iauTaiutc() function must be equal to 0. The received value: " << j << ". File: " << __FILE__ << ". Line: " << __LINE__;
            throw ProjectException(buff.str());
        }
        return {utc1, utc2};
    }

    template<>
    inline Time<Scale::UT1> TimeConverter::convert<Scale::TAI, Scale::UT1>(Time<Scale::TAI> const &t) const
    {
        Time<Scale::UTC> UTC = convert<Scale::TAI, Scale::UTC>(t);
        Time<Scale::UT1> UT1 = convert<Scale::UTC, Scale::UT1>(UTC);
        return UT1;
    }

    template<>
    inline Time<Scale::TT> TimeConverter::convert<Scale::TAI, Scale::TT>(Time<Scale::TAI> const &t) const
    {
        scalar tt1, tt2;
        integer j = iauTaitt(t.getDay(), t.getPart(), &tt1, &tt2);
        if (j != 0)
        {
            std::stringstream buff;
            buff << "The return value of the iauTaitt() function must be equal to 0. The received value: " << j << ". File: " << __FILE__ << ". Line: " << __LINE__;
            throw ProjectException(buff.str());
        }
        return {tt1, tt2};
    }

    template<>
    inline Time<Scale::TDB> TimeConverter::convert<Scale::TAI, Scale::TDB>(Time<Scale::TAI> const &t) const
    {
        Time<Scale::TT> TT = convert<Scale::TAI, Scale::TT>(t);
        Time<Scale::TDB> TDB = convert<Scale::TT, Scale::TDB>(TT);
        return TDB;
    }

    template<>
    inline Time<Scale::TCG> TimeConverter::convert<Scale::TAI, Scale::TCG>(Time<Scale::TAI> const &t) const
    {
        Time<Scale::TT> TT = convert<Scale::TAI, Scale::TT>(t);
        scalar tcg1, tcg2;
        integer j = iauTttcg(TT.getDay(), TT.getPart(), &tcg1, &tcg2);
        if (j != 0)
        {
            std::stringstream buff;
            buff << "The return value of the iauTttcg() function must be equal to 0. The received value: " << j << ". File: " << __FILE__ << ". Line: " << __LINE__;
            throw ProjectException(buff.str());
        }
        return {tcg1, tcg2};
    }

    template<>
    inline Time<Scale::TCB> TimeConverter::convert<Scale::TAI, Scale::TCB>(Time<Scale::TAI> const &t) const
    {
        Time<Scale::TDB> TDB = convert<Scale::TAI, Scale::TDB>(t);
        scalar tcb1, tcb2;
        integer j = iauTdbtcb(TDB.getDay(), TDB.getPart(), &tcb1, &tcb2);
        if (j != 0)
        {
            std::stringstream buff;
            buff << "The return value of the iauTdbtcb() function must be equal to 0. The received value: " << j << ". File: " << __FILE__ << ". Line: " << __LINE__;
            throw ProjectException(buff.str());
        }
        return {tcb1, tcb2};
    }

    template<>
    inline Time<Scale::TAI> TimeConverter::convert<Scale::TAI, Scale::TAI>(Time<Scale::TAI> const &t) const
    {
        return t;
    }

///////////////////////////////////////////////CONVERSION FROM UT1//////////////////////////////////////////////////////
    template<>
    inline Time<Scale::UTC> TimeConverter::convert<Scale::UT1, Scale::UTC>(Time<Scale::UT1> const &t) const
    {
        scalar utc1, utc2;
        scalar dut = getDut(t.toMJD());
        // Let's determine the time in UTC using the iterative method
        for (int i = 0; i < 2; ++i)
        {
            integer j = iauUt1utc(t.getDay(), t.getPart(), dut, &utc1, &utc2);
            if (j != 0)
            {
                std::stringstream buff;
                buff << "The return value of the iauUt1utc() function must be equal to 0. The received value: " << j << ". File: " << __FILE__ << ". Line: " << __LINE__;
                throw ProjectException(buff.str());
            }
            Time<Scale::UTC> UTC = {utc1, utc2};
            dut = getDut(UTC.toMJD());
        }
        return {utc1, utc2};
    }

    template<>
    inline Time<Scale::TAI> TimeConverter::convert<Scale::UT1, Scale::TAI>(Time<Scale::UT1> const &t) const
    {
        Time<Scale::UTC> UTC = convert<Scale::UT1, Scale::UTC>(t);
        Time<Scale::TAI> TAI = convert<Scale::UTC, Scale::TAI>(UTC);
        return TAI;
    }

    template<>
    inline Time<Scale::TT> TimeConverter::convert<Scale::UT1, Scale::TT>(Time<Scale::UT1> const &t) const
    {
        Time<Scale::TAI> TAI = convert<Scale::UT1, Scale::TAI>(t);
        Time<Scale::TT> TT = convert<Scale::TAI, Scale::TT>(TAI);
        return TT;
    }

    template<>
    inline Time<Scale::TCG> TimeConverter::convert<Scale::UT1, Scale::TCG>(Time<Scale::UT1> const &t) const
    {
        Time<Scale::TT> TT = convert<Scale::UT1, Scale::TT>(t);
        scalar tcg1, tcg2;
        integer j = iauTttcg(TT.getDay(), TT.getPart(), &tcg1, &tcg2);
        if (j != 0)
        {
            std::stringstream buff;
            buff << "The return value of the iauTttcg() function must be equal to 0. The received value: " << j << ". File: " << __FILE__ << ". Line: " << __LINE__;
            throw ProjectException(buff.str());
        }
        return {tcg1, tcg2};
    }

    template<>
    inline Time<Scale::TDB> TimeConverter::convert<Scale::UT1, Scale::TDB>(Time<Scale::UT1> const &t) const
    {
        Time<Scale::TT> TT = convert<Scale::UT1, Scale::TT>(t);
        Time<Scale::TDB> TDB = convert<Scale::TT, Scale::TDB>(TT);
        return TDB;
    }

    template<>
    inline Time<Scale::TCB> TimeConverter::convert<Scale::UT1, Scale::TCB>(Time<Scale::UT1> const &t) const
    {
        Time<Scale::TDB> TDB = convert<Scale::UT1, Scale::TDB>(t);
        scalar tcb1, tcb2;
        integer j = iauTdbtcb(TDB.getDay(), TDB.getPart(), &tcb1, &tcb2);
        if (j != 0)
        {
            std::stringstream buff;
            buff << "The return value of the iauTdbtcb() function must be equal to 0. The received value: " << j << ". File: " << __FILE__ << ". Line: " << __LINE__;
            throw ProjectException(buff.str());
        }
        return {tcb1, tcb2};
    }

    template<>
    inline Time<Scale::UT1> TimeConverter::convert<Scale::UT1, Scale::UT1>(Time<Scale::UT1> const &t) const
    {
        return t;
    }

///////////////////////////////////////////////CONVERSION FROM TCG//////////////////////////////////////////////////////
    template<>
    inline Time<Scale::TT> TimeConverter::convert<Scale::TCG, Scale::TT>(Time<Scale::TCG> const &t) const
    {
        scalar tt1, tt2;
        integer j = iauTcgtt(t.getDay(), t.getPart(), &tt1, &tt2);
        if (j != 0)
        {
            std::stringstream buff;
            buff << "The return value of the iauTcgtt() function must be equal to 0. The received value: " << j << ". File: " << __FILE__ << ". Line: " << __LINE__;
            throw ProjectException(buff.str());
        }
        return {tt1, tt2};
    }

    template<>
    inline Time<Scale::TDB> TimeConverter::convert<Scale::TCG, Scale::TDB>(Time<Scale::TCG> const &t) const
    {
        Time<Scale::TT> TT = convert<Scale::TCG, Scale::TT>(t);
        Time<Scale::TDB> TDB = convert<Scale::TT, Scale::TDB>(TT);
        return TDB;
    }

    template<>
    inline Time<Scale::TCB> TimeConverter::convert<Scale::TCG, Scale::TCB>(Time<Scale::TCG> const &t) const
    {
        Time<Scale::TT> TT = convert<Scale::TCG, Scale::TT>(t);
        Time<Scale::TCB> TCB = convert<Scale::TT, Scale::TCB>(TT);
        return TCB;
    }

    template<>
    inline Time<Scale::UT1> TimeConverter::convert<Scale::TCG, Scale::UT1>(Time<Scale::TCG> const &t) const
    {
        Time<Scale::TT> TT = convert<Scale::TCG, Scale::TT>(t);
        Time<Scale::UT1> UT1 = convert<Scale::TT, Scale::UT1>(TT);
        return UT1;
    }

    template<>
    inline Time<Scale::UTC> TimeConverter::convert<Scale::TCG, Scale::UTC>(Time<Scale::TCG> const &t) const
    {
        Time<Scale::TT> TT = convert<Scale::TCG, Scale::TT>(t);
        Time<Scale::UTC> UTC = convert<Scale::TT, Scale::UTC>(TT);
        return UTC;
    }

    template<>
    inline Time<Scale::TAI> TimeConverter::convert<Scale::TCG, Scale::TAI>(Time<Scale::TCG> const &t) const
    {
        Time<Scale::TT> TT = convert<Scale::TCG, Scale::TT>(t);
        Time<Scale::TAI> TAI = convert<Scale::TT, Scale::TAI>(TT);
        return TAI;
    }

    template<>
    inline Time<Scale::TCG> TimeConverter::convert<Scale::TCG, Scale::TCG>(Time<Scale::TCG> const &t) const
    {
        return t;
    }

///////////////////////////////////////////////CONVERSION FROM TDB//////////////////////////////////////////////////////
    template<>
    inline Time<Scale::TCB> TimeConverter::convert<Scale::TDB, Scale::TCB>(Time<Scale::TDB> const &t) const
    {
        scalar tcb1, tcb2;
        integer j = iauTdbtcb(t.getDay(), t.getPart(), &tcb1, &tcb2);
        if (j != 0)
        {
            std::stringstream buff;
            buff << "The return value of the iauTdbtcb() function must be equal to 0. The received value: " << j << ". File: " << __FILE__ << ". Line: " << __LINE__;
            throw ProjectException(buff.str());
        }
        return {tcb1, tcb2};
    }

    template<>
    inline Time<Scale::TT> TimeConverter::convert<Scale::TDB, Scale::TT>(Time<Scale::TDB> const &t) const
    {
        scalar tt1, tt2;
        scalar dtr = 0.001657 * sin(6.24 + 0.017202 * (static_cast<scalar>(t.getDay()) + t.getPart() - 2451545.));
        integer j = iauTdbtt(t.getDay(), t.getPart(), dtr, &tt1, &tt2);
        if (j != 0)
        {
            std::stringstream buff;
            buff << "The return value of the iauTdbtt() function must be equal to 0. The received value: " << j << ". File: " << __FILE__ << ". Line: " << __LINE__;
            throw ProjectException(buff.str());
        }
        Time<Scale::UT1> UT1 = convert<Scale::TT, Scale::UT1>({tt1, tt2});

        scalar ut = fmod(UT1.getPart() + 0.5, 1.0);
        dtr = iauDtdb(tt1, tt2, ut, 0., 0., 0.);

        j = iauTdbtt(t.getDay(), t.getPart(), dtr, &tt1, &tt2);
        if (j != 0)
        {
            std::stringstream buff;
            buff << "The return value of the iauTdbtt() function must be equal to 0. The received value: " << j << ". File: " << __FILE__ << ". Line: " << __LINE__;
            throw ProjectException(buff.str());
        }

        return {tt1, tt2};
    }

    template<>
    inline Time<Scale::TCG> TimeConverter::convert<Scale::TDB, Scale::TCG>(Time<Scale::TDB> const &t) const
    {
        Time<Scale::TT> TT = convert<Scale::TDB, Scale::TT>(t);
        Time<Scale::TCG> TCG = convert<Scale::TT, Scale::TCG>(TT);
        return TCG;
    }

    template<>
    inline Time<Scale::UT1> TimeConverter::convert<Scale::TDB, Scale::UT1>(Time<Scale::TDB> const &t) const
    {
        Time<Scale::TT> TT = convert<Scale::TDB, Scale::TT>(t);
        Time<Scale::UT1> UT1 = convert<Scale::TT, Scale::UT1>(TT);
        return UT1;
    }

    template<>
    inline Time<Scale::UTC> TimeConverter::convert<Scale::TDB, Scale::UTC>(Time<Scale::TDB> const &t) const
    {
        Time<Scale::TT> TT = convert<Scale::TDB, Scale::TT>(t);
        Time<Scale::UTC> UTC = convert<Scale::TT, Scale::UTC>(TT);
        return UTC;
    }

    template<>
    inline Time<Scale::TAI> TimeConverter::convert<Scale::TDB, Scale::TAI>(Time<Scale::TDB> const &t) const
    {
        Time<Scale::TT> TT = convert<Scale::TDB, Scale::TT>(t);
        Time<Scale::TAI> TAI = convert<Scale::TT, Scale::TAI>(TT);
        return TAI;
    }

    template<>
    inline Time<Scale::TDB> TimeConverter::convert<Scale::TDB, Scale::TDB>(Time<Scale::TDB> const &t) const
    {
        return t;
    }

///////////////////////////////////////////////CONVERSION FROM TCB//////////////////////////////////////////////////////
    template<>
    inline Time<Scale::TDB> TimeConverter::convert<Scale::TCB, Scale::TDB>(Time<Scale::TCB> const &t) const
    {
        scalar tdb1, tdb2;
        integer j = iauTcbtdb(t.getDay(), t.getPart(), &tdb1, &tdb2);
        if (j != 0)
        {
            std::stringstream buff;
            buff << "The return value of the iauTcbtdb() function must be equal to 0. The received value: " << j << ". File: " << __FILE__ << ". Line: " << __LINE__;
            throw ProjectException(buff.str());
        }
        return {tdb1, tdb2};
    }

    template<>
    inline Time<Scale::TT> TimeConverter::convert<Scale::TCB, Scale::TT>(Time<Scale::TCB> const &t) const
    {
        Time<Scale::TDB> TDB = convert<Scale::TCB, Scale::TDB>(t);
        Time<Scale::TT> TT = convert<Scale::TDB, Scale::TT>(TDB);
        return TT;
    }

    template<>
    inline Time<Scale::TCG> TimeConverter::convert<Scale::TCB, Scale::TCG>(Time<Scale::TCB> const &t) const
    {
        Time<Scale::TDB> TDB = convert<Scale::TCB, Scale::TDB>(t);
        Time<Scale::TCG> TCG = convert<Scale::TDB, Scale::TCG>(TDB);
        return TCG;
    }

    template<>
    inline Time<Scale::UT1> TimeConverter::convert<Scale::TCB, Scale::UT1>(Time<Scale::TCB> const &t) const
    {
        Time<Scale::TDB> TDB = convert<Scale::TCB, Scale::TDB>(t);
        Time<Scale::UT1> UT1 = convert<Scale::TDB, Scale::UT1>(TDB);
        return UT1;
    }

    template<>
    inline Time<Scale::UTC> TimeConverter::convert<Scale::TCB, Scale::UTC>(Time<Scale::TCB> const &t) const
    {
        Time<Scale::TDB> TDB = convert<Scale::TCB, Scale::TDB>(t);
        Time<Scale::UTC> UTC = convert<Scale::TDB, Scale::UTC>(TDB);
        return UTC;
    }

    template<>
    inline Time<Scale::TAI> TimeConverter::convert<Scale::TCB, Scale::TAI>(Time<Scale::TCB> const &t) const
    {
        Time<Scale::TDB> TDB = convert<Scale::TCB, Scale::TDB>(t);
        Time<Scale::TAI> TAI = convert<Scale::TDB, Scale::TAI>(TDB);
        return TAI;
    }

    template<>
    inline Time<Scale::TCB> TimeConverter::convert<Scale::TCB, Scale::TCB>(Time<Scale::TCB> const &t) const
    {
        return t;
    }
}
#endif //PROJECT_BALLISTICS_TIMECONVERTER_HPP
