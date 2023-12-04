#ifndef PROJECT_BALLISTICS_SP3PARSER_H
#define PROJECT_BALLISTICS_SP3PARSER_H

#include "../../../third_party/include/rapidcsv.h"
#include "ballistics/types/BasicTypes.hpp"
#include "../time/Time.hpp"
#include <string>
#include <utility>

namespace Ballistics::SP3 {

    class SP3Parser {
    private:
        std::vector<vector3d> position_;
        std::vector<scalar> time_;
        std::vector<std::string> dateString_;
        std::vector<scalar> date_;

    public:
        SP3Parser(const std::string &csvAddress) {
            const rapidcsv::Document doc(csvAddress);
            dateString_ = doc.GetColumn<std::string>("time");

            for (int i = 0; i < dateString_.size(); i += 3) {
                const std::string year = {dateString_[i][0], dateString_[i][1], dateString_[i][2], dateString_[i][3]};
                const std::string month = {dateString_[i][5], dateString_[i][6]};
                const std::string day = {dateString_[i][8], dateString_[i][9]};
                const std::string hour = {dateString_[i][11], dateString_[i][12]};
                const std::string minute = {dateString_[i][14], dateString_[i][15]};
                const std::string second = {dateString_[i][17], dateString_[i][18]};

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
                date_.push_back(Day + Part);
            }

            std::vector<scalar> pseudorange = doc.GetColumn<scalar>("position");
            std::vector<scalar> time = doc.GetColumn<scalar>("clock");
            for (int i = 0; i < pseudorange.size(); i += 3) {
                position_.push_back({pseudorange[i], pseudorange[i + 1], pseudorange[i + 2]});
                time_.push_back(time[i]);
            }
        }

        [[nodiscard]] std::pair<vector3d, scalar> getObservation(const std::string& time) {
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
            const integer index = std::find(date_.begin(), date_.end(), jd) - date_.begin();

            return {position_[index], time_[index]};
        }
    };
}

#endif //PROJECT_BALLISTICS_SP3PARSER_H
