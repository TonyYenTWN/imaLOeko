// time series process and functions
#pragma once

// STL
#include <chrono>
//#include <iostream>

class time_series_class {
    private:

    public:
        std::vector <std::chrono::sys_days> get_past_days(unsigned int num_day, int time_zone_hour = 0, int time_zone_minute = 0){
            // Current time and date
            auto now = std::chrono::system_clock::now() + std::chrono::hours{time_zone_hour} + std::chrono::minutes{time_zone_minute};
            auto today = std::chrono::floor <std::chrono::days> (now);
            auto start_day = today - std::chrono::days{num_day};

            std::vector <std::chrono::sys_days> results (2);
            results[0] = start_day;
            results[1] = today;

            return results;
        }

        dataset create_ts_test(){
            dataset output;

            auto days_range = get_past_days(1, -4, 0);

            std::vector <unsigned int> year;
            std::vector <unsigned int> month;
            std::vector <unsigned int> day;
            std::vector <unsigned int> hour;
            std::vector <unsigned int> minute;
            std::vector <unsigned int> second;
            year.reserve(50);
            month.reserve(50);
            day.reserve(50);
            hour.reserve(50);
            minute.reserve(50);
            second.reserve(50);

            // First tick
            {
                auto earliest_day = days_range[0] - std::chrono::days{1};
                auto time_temp = days_range[0] - std::chrono::minutes{15};
                std::chrono::year_month_day date_temp {earliest_day};
                int year_temp {date_temp.year()};
                int month_temp = unsigned{date_temp.month()};
                int day_temp = unsigned{date_temp.day()};
                year.push_back(year_temp);
                month.push_back(month_temp);
                day.push_back(day_temp);

                std::chrono::hh_mm_ss clock_temp {time_temp - earliest_day};
                hour.push_back(clock_temp.hours().count());
                minute.push_back(clock_temp.minutes().count());
                second.push_back(clock_temp.seconds().count());

                std::cout << year_temp;
                std::cout << "-";
                std::cout << month_temp;
                std::cout << "-";
                std::cout << day_temp;
                std::cout << "\t";
                std::cout << hour[hour.size() - 1];
                std::cout << ":";
                std::cout << minute[minute.size() - 1];
                std::cout << ":";
                std::cout << second[second.size() - 1];
                std::cout << "\n";
            }

            // Middle ticks
            for(unsigned int tick = 0; tick < 48; ++ tick){
                auto time_temp = days_range[0] + std::chrono::minutes{15 + 30 * tick};
                std::chrono::year_month_day date_temp {days_range[0]};
                int year_temp {date_temp.year()};
                int month_temp = unsigned{date_temp.month()};
                int day_temp = unsigned{date_temp.day()};
                year.push_back(year_temp);
                month.push_back(month_temp);
                day.push_back(day_temp);

                std::chrono::hh_mm_ss clock_temp {time_temp - days_range[0]};
                hour.push_back(clock_temp.hours().count());
                minute.push_back(clock_temp.minutes().count());
                second.push_back(clock_temp.seconds().count());

                std::cout << year_temp;
                std::cout << "-";
                std::cout << month_temp;
                std::cout << "-";
                std::cout << day_temp;
                std::cout << "\t";
                std::cout << hour[hour.size() - 1];
                std::cout << ":";
                std::cout << minute[minute.size() - 1];
                std::cout << ":";
                std::cout << second[second.size() - 1];
                std::cout << "\n";
            }

            // Last tick
            {
                auto time_temp = days_range[1] + std::chrono::minutes{15};
                std::chrono::year_month_day date_temp {days_range[1]};
                int year_temp {date_temp.year()};
                int month_temp = unsigned{date_temp.month()};
                int day_temp = unsigned{date_temp.day()};
                year.push_back(year_temp);
                month.push_back(month_temp);
                day.push_back(day_temp);

                std::chrono::hh_mm_ss clock_temp {time_temp - days_range[1]};
                hour.push_back(clock_temp.hours().count());
                minute.push_back(clock_temp.minutes().count());
                second.push_back(clock_temp.seconds().count());

                std::cout << year_temp;
                std::cout << "-";
                std::cout << month_temp;
                std::cout << "-";
                std::cout << day_temp;
                std::cout << "\t";
                std::cout << hour[hour.size() - 1];
                std::cout << ":";
                std::cout << minute[minute.size() - 1];
                std::cout << ":";
                std::cout << second[second.size() - 1];
                std::cout << "\n";
            }

            // Store output time series data
            output["year"] = year;
            output["month"] = month;
            output["day"] = day;
            output["hour"] = hour;
            output["minute"] = minute;
            output["second"] = second;

            return output;
        }
};



