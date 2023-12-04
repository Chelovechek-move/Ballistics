#include "../src/ballistics/time/TimeConverter.hpp"
#include "utility/TimeResult.hpp"
#include "iostream"

int main()
{
    using namespace Ballistics::Time;
    struct scales
    {
        Time<Scale::UT1> UT1;
        Time<Scale::UTC> UTC;
        Time<Scale::TAI> TAI;
        Time<Scale::TT> TT;
        Time<Scale::TCG> TCG;
        Time<Scale::TCB> TCB;
        Time<Scale::TDB> TDB;
    };

    Ballistics::Time::TimeConverter converter = Ballistics::Time::TimeConverter("../../resources/earth_rotation.csv");

    for(const auto &el : timeResult)
    {
        scales example = {{el[1], el[2]}, {el[3], el[4]}, {el[5], el[6]}, {el[7], el[8]}, {el[9], el[10]}, {el[11], el[12]}, {el[13], el[14]}};

        if (converter.convert<Scale::UT1, Scale::UT1>(example.UT1).isEqual(example.UT1) &&
            converter.convert<Scale::UT1, Scale::UTC>(example.UT1).isEqual(example.UTC) &&
            converter.convert<Scale::UT1, Scale::TAI>(example.UT1).isEqual(example.TAI) &&
            converter.convert<Scale::UT1, Scale::TT>(example.UT1).isEqual(example.TT) &&
            converter.convert<Scale::UT1, Scale::TCG>(example.UT1).isEqual(example.TCG) &&
            converter.convert<Scale::UT1, Scale::TCB>(example.UT1).isEqual(example.TCB) &&
            converter.convert<Scale::UT1, Scale::TDB>(example.UT1).isEqual(example.TDB)
            )
        {
        } else
        {
            std::cout << "Error in UT1 conversion!" << std::endl;
            throw 1;
        }

        if (converter.convert<Scale::UTC, Scale::UTC>(example.UTC).isEqual(example.UTC) &&
            converter.convert<Scale::UTC, Scale::UT1>(example.UTC).isEqual(example.UT1) &&
            converter.convert<Scale::UTC, Scale::TAI>(example.UTC).isEqual(example.TAI) &&
            converter.convert<Scale::UTC, Scale::TT>(example.UTC).isEqual(example.TT) &&
            converter.convert<Scale::UTC, Scale::TCG>(example.UTC).isEqual(example.TCG) &&
            converter.convert<Scale::UTC, Scale::TCB>(example.UTC).isEqual(example.TCB) &&
            converter.convert<Scale::UTC, Scale::TDB>(example.UTC).isEqual(example.TDB)
            )
        {
        } else
        {
            std::cout << "Error in UTC conversion!" << std::endl;
            throw 1;
        }

        if (converter.convert<Scale::TAI, Scale::TAI>(example.TAI).isEqual(example.TAI) &&
            converter.convert<Scale::TAI, Scale::UT1>(example.TAI).isEqual(example.UT1) &&
            converter.convert<Scale::TAI, Scale::UTC>(example.TAI).isEqual(example.UTC) &&
            converter.convert<Scale::TAI, Scale::TT>(example.TAI).isEqual(example.TT) &&
            converter.convert<Scale::TAI, Scale::TCG>(example.TAI).isEqual(example.TCG) &&
            converter.convert<Scale::TAI, Scale::TCB>(example.TAI).isEqual(example.TCB) &&
            converter.convert<Scale::TAI, Scale::TDB>(example.TAI).isEqual(example.TDB)
            )
        {
        } else
        {
            std::cout << "Error in TAI conversion!" << std::endl;
            throw 1;
        }

        if (converter.convert<Scale::TT, Scale::TT>(example.TT).isEqual(example.TT) &&
            converter.convert<Scale::TT, Scale::UT1>(example.TT).isEqual(example.UT1) &&
            converter.convert<Scale::TT, Scale::UTC>(example.TT).isEqual(example.UTC) &&
            converter.convert<Scale::TT, Scale::TAI>(example.TT).isEqual(example.TAI) &&
            converter.convert<Scale::TT, Scale::TCG>(example.TT).isEqual(example.TCG) &&
            converter.convert<Scale::TT, Scale::TCB>(example.TT).isEqual(example.TCB) &&
            converter.convert<Scale::TT, Scale::TDB>(example.TT).isEqual(example.TDB)
            )
        {
        } else
        {
            std::cout << "Error in TT conversion!" << std::endl;
            throw 1;
        }

        if (converter.convert<Scale::TCG, Scale::TCG>(example.TCG).isEqual(example.TCG) &&
            converter.convert<Scale::TCG, Scale::UT1>(example.TCG).isEqual(example.UT1) &&
            converter.convert<Scale::TCG, Scale::UTC>(example.TCG).isEqual(example.UTC) &&
            converter.convert<Scale::TCG, Scale::TAI>(example.TCG).isEqual(example.TAI) &&
            converter.convert<Scale::TCG, Scale::TT>(example.TCG).isEqual(example.TT) &&
            converter.convert<Scale::TCG, Scale::TCB>(example.TCG).isEqual(example.TCB) &&
            converter.convert<Scale::TCG, Scale::TDB>(example.TCG).isEqual(example.TDB)
            )
        {
        } else
        {
            std::cout << "Error in TCG conversion!" << std::endl;
            throw 1;
        }

        if (converter.convert<Scale::TCB, Scale::TCB>(example.TCB).isEqual(example.TCB) &&
            converter.convert<Scale::TCB, Scale::UT1>(example.TCB).isEqual(example.UT1) &&
            converter.convert<Scale::TCB, Scale::UTC>(example.TCB).isEqual(example.UTC) &&
            converter.convert<Scale::TCB, Scale::TAI>(example.TCB).isEqual(example.TAI) &&
            converter.convert<Scale::TCB, Scale::TT>(example.TCB).isEqual(example.TT) &&
            converter.convert<Scale::TCB, Scale::TCG>(example.TCB).isEqual(example.TCG) &&
            converter.convert<Scale::TCB, Scale::TDB>(example.TCB).isEqual(example.TDB)
            )
        {
        } else
        {
            std::cout << "Error in TCB conversion!" << std::endl;
            throw 1;
        }

        if (converter.convert<Scale::TDB, Scale::TDB>(example.TDB).isEqual(example.TDB) &&
            converter.convert<Scale::TDB, Scale::UT1>(example.TDB).isEqual(example.UT1) &&
            converter.convert<Scale::TDB, Scale::UTC>(example.TDB).isEqual(example.UTC) &&
            converter.convert<Scale::TDB, Scale::TAI>(example.TDB).isEqual(example.TAI) &&
            converter.convert<Scale::TDB, Scale::TT>(example.TDB).isEqual(example.TT) &&
            converter.convert<Scale::TDB, Scale::TCG>(example.TDB).isEqual(example.TCG) &&
            converter.convert<Scale::TDB, Scale::TCB>(example.TDB).isEqual(example.TCB)
            )
        {
        } else
        {
            std::cout << "Error in TDB conversion!" << std::endl;
            throw 1;
        }
    }
}