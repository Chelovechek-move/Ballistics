#ifndef PROJECT_BALLISTICS_SATELLITEDATA_H
#define PROJECT_BALLISTICS_SATELLITEDATA_H

#include "ballistics/types/BasicTypes.hpp"
#include <string>

struct Epoch {
    Ballistics::integer year;
    Ballistics::integer month;
    Ballistics::integer day;
    Ballistics::integer hour;
    Ballistics::integer minute;
    Ballistics::scalar  second;
};

struct RinexData {
    Epoch epoch;
    std::string nameOfSatellite;
    Ballistics::scalar pseudorange;
};

std::vector<RinexData> rinexData = {
        {{2022, 11, 20, 10, 41, 00.0000000}, "R01", 19377286.800},
        {{2022, 11, 20, 10, 41, 00.0000000}, "R02", 22984983.920},
        {{2022, 11, 20, 10, 41, 00.0000000}, "R08", 20596983.680},
        {{2022, 11, 20, 10, 41, 00.0000000}, "R09", 23148707.100},
        {{2022, 11, 20, 10, 41, 00.0000000}, "R11", 20176780.960},
        {{2022, 11, 20, 10, 41, 00.0000000}, "R17", 23287699.280}
};

struct SP3Data {
    Epoch epoch;
    std::string nameOfSatellite;
    Ballistics::scalar x;
    Ballistics::scalar y;
    Ballistics::scalar z;
    Ballistics::scalar t;
};

std::vector<SP3Data> sp3Data = {
        {{2022, 11, 20, 10, 41, 00.0000000}, "R01",  11976.468008,   4224.310335, 22117.344056, 18.155286 },
        {{2022, 11, 20, 10, 41, 00.0000000}, "R02",   6018.286356, -15504.093777, 19404.158747, -23.385996},
        {{2022, 11, 20, 10, 41, 00.0000000}, "R08",  10229.992513,  20070.812451, 11981.597134, -68.210335},
        {{2022, 11, 20, 10, 41, 00.0000000}, "R09", -14329.470349,   6235.295928, 20153.606391, 88.570347 },
        {{2022, 11, 20, 10, 41, 00.0000000}, "R11",  19076.165493,  11726.754011, 12238.374969, 19.632028 },
        {{2022, 11, 20, 10, 41, 00.0000000}, "R17",   7893.628443, -17466.291417, 16820.283141, 579.743149}
};

#endif //PROJECT_BALLISTICS_SATELLITEDATA_H
