// Optimizer
#pragma once

// STL
#include <map>
#include <variant>
#include <vector>

class market_information_class{
    private:
        std::map <std::string, std::variant <unsigned int, double>> parameter;
        std::map <std::string, std::vector <double>> prediction;

    public:
        market_information_class(){
            this->parameter["num_interval"] = (unsigned int) 0;
            this->parameter["price_inflex_demand"] = (double) 3000.;

            std::vector <double> vec_double;
            this->prediction["time_length"] = vec_double;
            this->prediction["electricity_price"] = vec_double;
        }

        // Test the program
        void test(){
            // Insert time length (in hours)
            // 12 5-min time intervals + 23 1-hour intervals
            std::vector <double> time_length;
            unsigned int num_short_range = 12;
            unsigned int num_long_range = 23;
            this->parameter["num_interval"] = num_short_range + num_long_range;
            time_length.reserve(num_short_range + num_long_range);
            for(unsigned int short_iter = 0; short_iter < num_short_range; ++ short_iter){
                time_length.push_back(1. / 12.);
            }
            for(unsigned int short_iter = 0; short_iter < num_short_range; ++ short_iter){
                time_length.push_back(1.);
            }
            this->prediction["time_length"] = time_length;

            // Insert electricity prices
            std::vector <double> electricity_price;
        }

        std::variant <unsigned int, double> get_parameter_values(std::string key){
            return this->parameter[key];
        }

       std::vector <double> get_prediction_values(std::string key){
            return this->prediction[key];
        }
};

class market_participant_class{
    private:
        std::map <std::string, std::variant <unsigned int, double>> parameter;
        std::map <std::string, std::vector <double>> prediction;

    public:
        // Constructor function
        // participant type: 0 = cer; 1 = rer; 2 = ordinary prosumer; 3 = res prosumer; 4 = local prosumer
        market_participant_class(unsigned int participant_type){
            this->parameter["type"] = (unsigned int) participant_type;
            this->parameter["res_premium"] = (double) 0.;
            this->parameter["lem_premium"] = (double) 0.;
            this->parameter["self_premium"] = (double) 0.;

            std::vector <double> vec_double;
            this->prediction["default_demand"] = vec_double;
            this->prediction["conv_generation"] = vec_double;
            this->prediction["res_generation"] = vec_double;
        }

        // Test the program
        void test(unsigned int num_time, market_information_class &information){
            auto participant_type = std::get <unsigned int> (this->parameter["type"]);
            auto time_length = information.get_prediction_values("time_length");

            // Insert default demand for prosumers
            if(participant_type >= 2){
                std::vector <double> default_demand;
                default_demand.reserve(num_time);

                for(unsigned int tick = 0; tick < num_time; ++ tick){
                    double demand_temp = 1.;
                    demand_temp *= time_length[tick];
                    default_demand.push_back(demand_temp);
                }
            }
            // Insert default demand for retailers
            else{
                // Non-RES retailer
                if(participant_type == 0){
                    std::vector <double> conv_generation;
                    conv_generation.reserve(num_time);

                    for(unsigned int tick = 0; tick < num_time; ++ tick){
                        double conv_temp = 1.;
                        conv_temp *= time_length[tick];
                        conv_generation.push_back(conv_temp);
                    }
                }
                // RES retailer
                else{
                    std::vector <double> res_generation;
                    res_generation.reserve(num_time);

                    for(unsigned int tick = 0; tick < num_time; ++ tick){
                        double res_temp = .5;
                        res_temp *= time_length[tick];
                        res_generation.push_back(res_temp);
                    }
                }
            }
        }
};

class market_class{
    public:
        market_information_class information;
        std::vector <market_participant_class> participants;

        void test(){
            this->information.test();

            market_participant_class consumer(2);
            market_participant_class res(1);
            market_participant_class conv(0);
            std::vector <market_participant_class> participants;
            participants.reserve(3);
            participants.push_back(consumer);
            participants.push_back(res);
            participants.push_back(conv);
            this->participants = participants;
            this->participants[0].test(std::get <unsigned int> (this->information.get_parameter_values("num_interval")), this->information);
            this->participants[1].test(std::get <unsigned int> (this->information.get_parameter_values("num_interval")), this->information);
        }
};
