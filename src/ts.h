// time series process and functions
#pragma once

// STL
#include <chrono>
//#include <iostream>

class time_series_class {
    private:

    public:
        std::map <std::string, unsigned int> read_timestamp(std::string timestamp){
            std::map <std::string, unsigned int> output;

            std::string date_delimiter = "-";
            std::string time_delimiter = ":";

            std::string year = timestamp.substr(0, timestamp.find(date_delimiter));
            output["year"] = (unsigned int) stoi(year);
            timestamp.erase(0, timestamp.find(date_delimiter) + date_delimiter.length());

            std::string month = timestamp.substr(0, timestamp.find(date_delimiter));
            output["month"] = (unsigned int) stoi(month);
            timestamp.erase(0, timestamp.find(date_delimiter) + date_delimiter.length());

            std::string day = timestamp.substr(0, timestamp.find(date_delimiter));
            output["day"] = (unsigned int) stoi(day);
            timestamp.erase(0, timestamp.find(date_delimiter) + date_delimiter.length());

            timestamp.erase(0, timestamp.find(" ") + date_delimiter.length());

            std::string hour = timestamp.substr(0, timestamp.find(time_delimiter));
            output["hour"] = (unsigned int) stoi(hour);
            timestamp.erase(0, timestamp.find(time_delimiter) + time_delimiter.length());

            std::string minute = timestamp.substr(0, timestamp.find(time_delimiter));
            output["minute"] = (unsigned int) stoi(minute);
            timestamp.erase(0, timestamp.find(time_delimiter) + time_delimiter.length());

            std::string second = timestamp.substr(0, timestamp.find(time_delimiter));
            output["second"] = (unsigned int) stoi(second);
            timestamp.erase(0, timestamp.find(time_delimiter) + time_delimiter.length());

//            std::cout << output["year"] << "-" << output["month"] << "-" << output["day"] << " ";
//            std::cout << output["hour"] << "-" << output["minute"] << "-" << output["second"] << "\n";

            return output;
        }

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

        dataset temporal_integration(dataset &data, unsigned int res_min){
            dataset output;

            auto year_data = std::any_cast <std::vector <unsigned int>> (data["year"]);
            auto month_data = std::any_cast <std::vector <unsigned int>> (data["month"]);
            auto day_data = std::any_cast <std::vector <unsigned int>> (data["day"]);
            auto hour_data = std::any_cast <std::vector <unsigned int>> (data["hour"]);
            auto minute_data = std::any_cast <std::vector <unsigned int>> (data["minute"]);
            auto second_data = std::any_cast <std::vector <unsigned int>> (data["second"]);
            auto value_data = std::any_cast <std::vector <double>> (data["value"]);

            std::chrono::year_month_day first_date{
                std::chrono::year(year_data[0]),
                std::chrono::month(month_data[0]),
                std::chrono::day(day_data[0])
            };

            std::chrono::year_month_day last_date{
                std::chrono::year(year_data[year_data.size() - 1]),
                std::chrono::month(month_data[month_data.size() - 1]),
                std::chrono::day(day_data[day_data.size() - 1])
            };

            auto num_day = (std::chrono::sys_days{last_date} - std::chrono::sys_days{first_date}).count();
            num_day -= 1;
            unsigned int num_sample_day = 1440 / res_min;
            unsigned int num_sample = num_sample_day * num_day;

            std::vector <unsigned int> year_sample;
            std::vector <unsigned int> month_sample;
            std::vector <unsigned int> day_sample;
            std::vector <unsigned int> hour_sample;
            std::vector <unsigned int> minute_sample;
            std::vector <unsigned int> second_sample;
            std::vector <double> value_sample;
            year_sample.reserve(num_sample);
            month_sample.reserve(num_sample);
            day_sample.reserve(num_sample);
            hour_sample.reserve(num_sample);
            minute_sample.reserve(num_sample);
            second_sample.reserve(num_sample);
            value_sample.reserve(num_sample);

            // Create sampling time intervals
            for(unsigned int day_iter = 0; day_iter < num_day; ++ day_iter){
                for(unsigned int tick = 0; tick < num_sample_day; ++ tick){
                    auto sample_time_temp  = std::chrono::sys_days{first_date} + std::chrono::days{day_iter + 1} + std::chrono::minutes{tick * res_min};
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
            }

            unsigned int left_time_ID = 0;
            unsigned int right_time_ID = 0;
            for(unsigned int tick = 0; tick < num_sample; ++ tick){
                std::chrono::year_month_day sample_date_temp{
                    std::chrono::year(year_sample[tick]),
                    std::chrono::month(month_sample[tick]),
                    std::chrono::day(day_sample[tick])
                };
                auto sample_time_left_temp = std::chrono::sys_days{sample_date_temp} + std::chrono::hours{hour_sample[tick]} + std::chrono::minutes{minute_sample[tick]} + std::chrono::seconds{second_sample[tick]};
                auto sample_time_right_temp = sample_time_left_temp + std::chrono::minutes{res_min};

                while(true){
                    std::chrono::year_month_day left_date_temp{
                        std::chrono::year(year_data[left_time_ID]),
                        std::chrono::month(month_data[left_time_ID]),
                        std::chrono::day(day_data[left_time_ID])
                    };
                    auto left_time_temp = std::chrono::sys_days{left_date_temp} + std::chrono::hours{hour_data[left_time_ID]} + std::chrono::minutes{minute_data[left_time_ID]} + std::chrono::seconds{second_data[left_time_ID]};

                    if(left_time_temp > sample_time_left_temp){
                        left_time_ID -= 1;
                        break;
                    }
                    left_time_ID += 1;
                }

                while(true){
                    std::chrono::year_month_day right_date_temp{
                        std::chrono::year(year_data[right_time_ID]),
                        std::chrono::month(month_data[right_time_ID]),
                        std::chrono::day(day_data[right_time_ID])
                    };
                    auto right_time_temp = std::chrono::sys_days{right_date_temp} + std::chrono::hours{hour_data[right_time_ID]} + std::chrono::minutes{minute_data[right_time_ID]} + std::chrono::seconds{second_data[right_time_ID]};

                    if(right_time_temp > sample_time_right_temp){
                        break;
                    }
                    right_time_ID += 1;
                }

                double value_temp = 0.;
                for(unsigned int gap_iter = left_time_ID; gap_iter < right_time_ID; ++ gap_iter){
                    std::chrono::year_month_day left_date_temp{
                        std::chrono::year(year_data[gap_iter]),
                        std::chrono::month(month_data[gap_iter]),
                        std::chrono::day(day_data[gap_iter])
                    };
                    auto left_time_temp = std::chrono::sys_days{left_date_temp} + std::chrono::hours{hour_data[gap_iter]} + std::chrono::minutes{minute_data[gap_iter]} + std::chrono::seconds{second_data[gap_iter]};

                    std::chrono::year_month_day right_date_temp{
                        std::chrono::year(year_data[gap_iter + 1]),
                        std::chrono::month(month_data[gap_iter + 1]),
                        std::chrono::day(day_data[gap_iter + 1])
                    };
                    auto right_time_temp = std::chrono::sys_days{right_date_temp} + std::chrono::hours{hour_data[gap_iter + 1]} + std::chrono::minutes{minute_data[gap_iter + 1]} + std::chrono::seconds{second_data[gap_iter + 1]};

                    auto left_boundary_temp = std::max(left_time_temp, sample_time_left_temp);
                    auto right_boundary_temp = std::min(right_time_temp, sample_time_right_temp);
                    auto interval_duration = (double) (right_boundary_temp - left_boundary_temp).count();
                    auto gap_duration = (double) (right_time_temp - left_time_temp).count();
                    auto start_time_temp = (double) (left_boundary_temp - left_time_temp).count();

                    double interval_ratio = start_time_temp;
                    interval_ratio += .5 * interval_duration;
                    interval_ratio /= gap_duration;
                    double value_sample_temp = (1. - interval_ratio) * value_data[gap_iter];
                    value_sample_temp += interval_ratio * value_data[gap_iter + 1];
                    value_temp += value_sample_temp * interval_duration / 3600.;
                }

                value_sample.push_back(value_temp);

//                std::cout << year_sample[tick];
//                std::cout << "-";
//                std::cout << month_sample[tick];
//                std::cout << "-";
//                std::cout << day_sample[tick];
//                std::cout << "\t";
//                std::cout << hour_sample[tick];
//                std::cout << ":";
//                std::cout << minute_sample[tick];
//                std::cout << ":";
//                std::cout << second_sample[tick];
//                std::cout << "\t";
//                std::cout << value_sample[tick];
//                std::cout << "\n";
            }

            // Store output time series data
            output["year"] = year_sample;
            output["month"] = month_sample;
            output["day"] = day_sample;
            output["hour"] = hour_sample;
            output["minute"] = minute_sample;
            output["second"] = second_sample;
            output["value"] = value_sample;

            return output;
        }

        void periodic_profile(dataset &data, unsigned int num_period){
            auto year_data = std::any_cast <std::vector <unsigned int>> (data["year"]);
            auto month_data = std::any_cast <std::vector <unsigned int>> (data["month"]);
            auto day_data = std::any_cast <std::vector <unsigned int>> (data["day"]);
            auto hour_data = std::any_cast <std::vector <unsigned int>> (data["hour"]);
            auto minute_data = std::any_cast <std::vector <unsigned int>> (data["minute"]);
            auto second_data = std::any_cast <std::vector <unsigned int>> (data["second"]);
            auto value_data = std::any_cast <std::vector <double>> (data["value"]);

            unsigned int num_interval_period = value_data.size() / num_period;
        }
};

class time_series_tester_class: public time_series_class{
    public:
        dataset create_ts_test(unsigned int num_day){
            dataset output;

            auto days_range = get_past_days(num_day, -4, 0);
            unsigned int num_interval = 2 + 48 * num_day;

            std::vector <unsigned int> year;
            std::vector <unsigned int> month;
            std::vector <unsigned int> day;
            std::vector <unsigned int> hour;
            std::vector <unsigned int> minute;
            std::vector <unsigned int> second;
            std::vector <double> value;
            year.reserve(num_interval);
            month.reserve(num_interval);
            day.reserve(num_interval);
            hour.reserve(num_interval);
            minute.reserve(num_interval);
            second.reserve(num_interval);
            value.reserve(num_interval);

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
            for(unsigned int day_iter = 0; day_iter < num_day; ++ day_iter){
                for(unsigned int tick = 0; tick < 48; ++ tick){
                    auto time_temp = days_range[0] + std::chrono::days{day_iter} + std::chrono::minutes{15 + 30 * tick};
                    std::chrono::year_month_day date_temp {days_range[0] + std::chrono::days{day_iter}};
                    int year_temp {date_temp.year()};
                    int month_temp = unsigned{date_temp.month()};
                    int day_temp = unsigned{date_temp.day()};
                    year.push_back(year_temp);
                    month.push_back(month_temp);
                    day.push_back(day_temp);

                    std::chrono::hh_mm_ss clock_temp {time_temp - days_range[0] - std::chrono::days{day_iter}};
                    hour.push_back(clock_temp.hours().count());
                    minute.push_back(clock_temp.minutes().count());
                    second.push_back(clock_temp.seconds().count());
                    value.push_back((double) (value.size() % 2 == 0) + .5 * day_iter);

//                    std::cout << year_temp;
//                    std::cout << "-";
//                    std::cout << month_temp;
//                    std::cout << "-";
//                    std::cout << day_temp;
//                    std::cout << "\t";
//                    std::cout << hour[hour.size() - 1];
//                    std::cout << ":";
//                    std::cout << minute[minute.size() - 1];
//                    std::cout << ":";
//                    std::cout << second[second.size() - 1];
//                    std::cout << "\t";
//                    std::cout << value[value.size() - 1];
//                    std::cout << "\n";
                }
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
                value.push_back((double) (value.size() % 2 == 0) + .5 * (num_day - 1));
            }

            // Store output time series data
            output["year"] = year;
            output["month"] = month;
            output["day"] = day;
            output["hour"] = hour;
            output["minute"] = minute;
            output["second"] = second;
            output["value"] = value;

            return output;
        }
};
