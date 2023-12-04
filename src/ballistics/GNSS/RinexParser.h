#ifndef PROJECT_BALLISTICS_RINEXPARSER_H
#define PROJECT_BALLISTICS_RINEXPARSER_H

#include "../../../third_party/include/rapidcsv.h"
#include "ballistics/types/BasicTypes.hpp"
#include "../time/Time.hpp"
#include <string>

namespace Ballistics::Rinex {

    class RinexParser {
    private:
        std::map<std::string, std::vector<scalar>> data;
        std::vector<std::string> timeColumn;

    public:
        // if "time" not in a columnNames then it will be added
        // It means that "time" column MUST be in a csv file
        RinexParser(const std::string &csvAddress,
                    const std::vector<std::string> &columnNames) {
            const rapidcsv::Document doc(csvAddress);
            timeColumn = doc.GetColumn<std::string>("time");

            for (const auto &name: columnNames) {
                if (name != "time") {
                    data[name] = doc.GetColumn<scalar>(name);
                }
            }

            for (const auto &time: timeColumn) {
                const std::string year = {time[0], time[1], time[2], time[3]};
                const std::string month = {time[5], time[6]};
                const std::string day = {time[8], time[9]};
                const std::string hour = {time[11], time[12]};
                const std::string minute = {time[14], time[15]};
                const std::string second = {time[17], time[18]};

                scalar Day, Part;
                const std::string Scale = "UTC";
                int j = iauDtf2d(Scale.c_str(), atoi(year.c_str()), atoi(month.c_str()), atoi(day.c_str()),
                                 atoi(hour.c_str()), atoi(minute.c_str()), atoi(second.c_str()), &Day, &Part);

                if (j != 0) {
                    std::stringstream buff;
                    buff << "The return value of the iauDtf2d() function must be equal to 0. The received value: " << j
                         << ". File: " << __FILE__ << ". Line: " << __LINE__;
                    throw ProjectException(buff.str());
                }
                data["time"].push_back(Day + Part);
            }
        }

        [[nodiscard]] scalar getObservation(const std::string &obsType,
                                            const std::string &time) {
            const std::string year = {time[0], time[1], time[2], time[3]};
            const std::string month = {time[5], time[6]};
            const std::string day = {time[8], time[9]};
            const std::string hour = {time[11], time[12]};
            const std::string minute = {time[14], time[15]};
            const std::string second = {time[17], time[18]};

            scalar Day, Part;
            const std::string Scale = "UTC";
            int j = iauDtf2d(Scale.c_str(), atoi(year.c_str()), atoi(month.c_str()), atoi(day.c_str()),
                             atoi(hour.c_str()), atoi(minute.c_str()), atoi(second.c_str()), &Day, &Part);

            if (j != 0) {
                std::stringstream buff;
                buff << "The return value of the iauDtf2d() function must be equal to 0. The received value: " << j
                     << ". File: " << __FILE__ << ". Line: " << __LINE__;
                throw ProjectException(buff.str());
            }

            const scalar jd = Day + Part;
            const integer index = std::find(data["time"].begin(), data["time"].end(), jd) - data["time"].begin();

            return data[obsType][index];
        }
    };
}

#endif //PROJECT_BALLISTICS_RINEXPARSER_H
