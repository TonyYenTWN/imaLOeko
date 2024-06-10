// time series process and functions
#pragma once

// STL
#include <chrono>
//#include <iostream>

class time_series_class {
    private:

    public:
        std::vector <std::chrono::sys_days> get_past_days(unsigned int num_day){
            // Current time and date
            auto now = std::chrono::system_clock::now();
            auto today = std::chrono::floor <std::chrono::days> (now);
            auto start_day = today - std::chrono::days{num_day};

            std::vector <std::chrono::sys_days> results (2);
            results[0] = start_day;
            results[1] = today;

            return results;
        }
};



