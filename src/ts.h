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
            std::vector <double> value;
            year.reserve(50);
            month.reserve(50);
            day.reserve(50);
            hour.reserve(50);
            minute.reserve(50);
            second.reserve(50);
            value.reserve(50);

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
                value.push_back((double) (value.size() % 2 == 0));
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
                value.push_back((double) (value.size() % 2 == 0));

//                std::cout << year_temp;
//                std::cout << "-";
//                std::cout << month_temp;
//                std::cout << "-";
//                std::cout << day_temp;
//                std::cout << "\t";
//                std::cout << hour[hour.size() - 1];
//                std::cout << ":";
//                std::cout << minute[minute.size() - 1];
//                std::cout << ":";
//                std::cout << second[second.size() - 1];
//                std::cout << "\t";
//                std::cout << value[value.size() - 1];
//                std::cout << "\n";
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
                value.push_back((double) (value.size() % 2 == 0));
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

        void temporal_integration(dataset &data){
            auto year_data = std::any_cast <std::vector <unsigned int>> (data["year"]);
            auto month_data = std::any_cast <std::vector <unsigned int>> (data["month"]);
            auto day_data = std::any_cast <std::vector <unsigned int>> (data["day"]);
            auto hour_data = std::any_cast <std::vector <unsigned int>> (data["hour"]);
            auto minute_data = std::any_cast <std::vector <unsigned int>> (data["minute"]);
            auto second_data = std::any_cast <std::vector <unsigned int>> (data["second"]);

            std::chrono::year_month_day first_date{
                std::chrono::year(year_data[0]),
                std::chrono::month(month_data[0]),
                std::chrono::day(day_data[0])
            };

            std::vector <unsigned int> year_sample;
            std::vector <unsigned int> month_sample;
            std::vector <unsigned int> day_sample;
            std::vector <unsigned int> hour_sample;
            std::vector <unsigned int> minute_sample;
            std::vector <unsigned int> second_sample;
            std::vector <double> value_sample;
            year_sample.reserve(1440);
            month_sample.reserve(1440);
            day_sample.reserve(1440);
            hour_sample.reserve(1440);
            minute_sample.reserve(1440);
            second_sample.reserve(1440);
            value_sample.reserve(1440);

            // Create sampling time intervals
            for(unsigned int tick = 0; tick < 1440; ++ tick){
                auto sample_time_temp  = std::chrono::sys_days{first_date} + std::chrono::days{1} + std::chrono::minutes{tick};
                std::chrono::year_month_day sample_date_temp {std::chrono::floor <std::chrono::days> (sample_time_temp)};

                int year_temp {sample_date_temp.year()};
                int month_temp = unsigned{sample_date_temp.month()};
                int day_temp = unsigned{sample_date_temp.day()};
                year_sample.push_back(year_temp);
                month_sample.push_back(month_temp);
                day_sample.push_back(day_temp);

                std::chrono::hh_mm_ss clock_temp {sample_time_temp - std::chrono::sys_days{sample_date_temp}};
                hour_sample.push_back(clock_temp.hours().count());
                minute_sample.push_back(clock_temp.minutes().count());
                second_sample.push_back(clock_temp.seconds().count());
            }

            unsigned int left_time_ID = 0;
            unsigned int right_time_ID = 0;
            for(unsigned int tick = 0; tick < 1440; ++ tick){
                std::chrono::year_month_day sample_date_temp{
                    std::chrono::year(year_sample[tick]),
                    std::chrono::month(month_sample[tick]),
                    std::chrono::day(day_sample[tick])
                };
                auto sample_time_temp = std::chrono::sys_days{sample_date_temp} + std::chrono::hours{hour_sample[tick]} + std::chrono::minutes{minute_sample[tick]} + std::chrono::seconds{second_sample[tick]};

                while(true){
                    std::chrono::year_month_day left_date_temp{
                        std::chrono::year(year_data[left_time_ID]),
                        std::chrono::month(month_data[left_time_ID]),
                        std::chrono::day(day_data[left_time_ID])
                    };
                    auto left_time_temp = std::chrono::sys_days{left_date_temp} + std::chrono::hours{hour_data[left_time_ID]} + std::chrono::minutes{minute_data[left_time_ID]} + std::chrono::seconds{second_data[left_time_ID]};

                    if(left_time_temp > sample_time_temp){
                        left_time_ID -= 1;
                        break;
                    }
                    left_time_ID += 1;
                }
                std::chrono::year_month_day left_date{
                    std::chrono::year(year_data[left_time_ID]),
                    std::chrono::month(month_data[left_time_ID]),
                    std::chrono::day(day_data[left_time_ID])
                };
                auto left_time = std::chrono::sys_days{left_date} + std::chrono::hours{hour_data[left_time_ID]} + std::chrono::minutes{minute_data[left_time_ID]} + std::chrono::seconds{second_data[left_time_ID]};

                while(true){
                    std::chrono::year_month_day right_date_temp{
                        std::chrono::year(year_data[right_time_ID]),
                        std::chrono::month(month_data[right_time_ID]),
                        std::chrono::day(day_data[right_time_ID])
                    };
                    auto right_time_temp = std::chrono::sys_days{right_date_temp} + std::chrono::hours{hour_data[right_time_ID]} + std::chrono::minutes{minute_data[right_time_ID]} + std::chrono::seconds{second_data[right_time_ID]};

                    if(right_time_temp > sample_time_temp){
                        break;
                    }
                    right_time_ID += 1;
                }
                std::chrono::year_month_day right_date{
                    std::chrono::year(year_data[right_time_ID]),
                    std::chrono::month(month_data[right_time_ID]),
                    std::chrono::day(day_data[right_time_ID])
                };
                auto right_time = std::chrono::sys_days{right_date} + std::chrono::hours{hour_data[right_time_ID]} + std::chrono::minutes{minute_data[right_time_ID]} + std::chrono::seconds{second_data[right_time_ID]};

//                std::cout << left_time_ID << "\t" << right_time_ID << "\n";


                double interval_ratio = (double) (sample_time_temp - left_time).count();
                interval_ratio /= (double) (right_time - left_time).count();
                std::cout << interval_ratio << "\n";
            }
        }
};



