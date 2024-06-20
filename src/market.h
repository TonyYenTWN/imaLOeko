// Market information and participants
#pragma once

// STL
#include <any>
#include <map>
#include <variant>
#include <vector>

#ifndef DATAFRAME
#define DATAFRAME

typedef std::map <std::string, double> double_df;
typedef std::map <std::string, std::vector <double>> double_vec_df;
typedef std::map <std::string, std::any> dataset;

#endif

class basic_data_class{
    protected:
        dataset data;

    public:
        auto get_value_ptr(std::vector <std::string> keys){
            std::vector <dataset::iterator> data_iter(keys.size());

            for (data_iter[0] = this->data.begin(); data_iter[0] != this->data.end(); ++ data_iter[0]){
                if(data_iter[0]->first == keys[0]){
                    break;
                }
            }

            for(unsigned int level_iter = 1; level_iter < keys.size(); ++ level_iter){
                dataset* data_temp = std::any_cast <dataset> (&data_iter[level_iter - 1]->second);

                for(data_iter[level_iter] = data_temp->begin(); data_iter[level_iter] != data_temp->end(); ++ data_iter[level_iter]){
                    if(data_iter[level_iter]->first == keys[level_iter]){
                        break;
                    }
                }
            }

            auto result = &data_iter[keys.size() - 1]->second;
            return result;
        }

        template <typename T>
        auto get_vector_value(std::vector <std::string> keys, unsigned int ID){
            std::vector <T> vec = std::any_cast <std::vector <T>> (*this->get_value_ptr(keys));
            auto result = vec[ID];

            return result;
        }

        void update_value(std::vector <std::string> keys, auto value){
            auto value_ptr = this->get_value_ptr(keys);
            *value_ptr = value;
        }

        template <typename T>
        void update_vector_value(std::vector <std::string> keys, unsigned int ID, T value){
            auto vec_ptr = std::any_cast <std::vector <T>> (this->get_value_ptr(keys));
            (*vec_ptr)[ID] = value;
        }
};

class market_information_class: public basic_data_class{
    public:
        market_information_class(){
            // Set parameters
            dataset parameter;
            parameter["num_interval"] = (unsigned int) 0;
            parameter["price_inflex_demand"] = (double) 3000.;
            this->data["parameter"] = parameter;

            // Set prediction
            dataset prediction;
            std::vector <double> vec_double;
            prediction["time_length"] = vec_double;
            prediction["electricity_price"] = vec_double;
            this->data["prediction"] = prediction;
        }

        // Test the program
        void test(){
            std::vector <std::string> keys;

            // Insert time length (in hours)
            // 12 5-min time intervals + 23 1-hour intervals
            unsigned int num_short_range = 12;
            unsigned int num_long_range = 23;
            keys = std::vector <std::string> (2);
            keys[0] = "parameter";
            keys[1] = "num_interval";
            this->update_value(keys, num_short_range + num_long_range);

            std::vector <double> time_length;
            time_length.reserve(num_short_range + num_long_range);
            for(unsigned int short_iter = 0; short_iter < num_short_range; ++ short_iter){
                time_length.push_back(1. / 12.);
            }
            for(unsigned int long_iter = 0; long_iter < num_long_range; ++ long_iter){
                time_length.push_back(1.);
            }
            keys = std::vector <std::string> (2);
            keys[0] = "prediction";
            keys[1] = "time_length";
            this->update_value(keys, time_length);

            // Insert electricity prices
            std::vector <double> electricity_price;
            electricity_price.reserve(time_length.size());
            for(unsigned int tick = 0; tick < time_length.size(); ++ tick){
                electricity_price.push_back(10.);
            }
            keys = std::vector <std::string> (2);
            keys[0] = "prediction";
            keys[1] = "electricity_price";
            this->update_value(keys, electricity_price);
        }
};

class market_participant_class: public basic_data_class{
    public:
        // Constructor function
        market_participant_class(unsigned int participant_type){
            std::vector <double> vec_double;

            // Set parameters
            dataset parameter;

            // Participant type: 0 = cer; 1 = rer; 2 = ordinary prosumer; 3 = res prosumer; 4 = local prosumer
            parameter["type"] = (unsigned int) participant_type;

            // Premium of different electricity sources
            dataset premium;
            premium["res"] = 0.;
            premium["lem"] = 0.;
            premium["self"] = 0.;
            parameter["premium"] = premium;

            // Technical parameters of BESS
            dataset bess_par;
            {
                // Capability
                bess_par["energy"] = (double) 0.;
                bess_par["capacity"] = (double) 0.;
                bess_par["ref_ratio"] = (double) 0.;
                bess_par["efficiency"] = (double) 1.;
                bess_par["ref_coeff"] = vec_double;

                // Initial soc
                dataset initial_soc;
                initial_soc["self"] = (double) 0.;
                initial_soc["lem"] = (double) 0.;
                initial_soc["rer"] = (double) 0.;
                initial_soc["cer"] = (double) 0.;
                bess_par["initial_soc"] = initial_soc;
            }
            parameter["bess"] = bess_par;
            this->data["parameter"] = parameter;

            // Prediction
            dataset prediction;
            prediction["default_demand"] = vec_double;
            prediction["res_generation"] = vec_double;
            prediction["conv_generation"] = vec_double;
            this->data["prediction"] = prediction;

            // Schedule and actual operation for optimization
            // Operation of BESS
            dataset operation;

            dataset accounting;
            accounting["self"] = vec_double;
            accounting["lem"] = vec_double;
            accounting["rer"] = vec_double;
            accounting["cer"] = vec_double;

            dataset bess_var;
            {
                bess_var["ch"] = accounting;
                bess_var["dc"] = accounting;
            }
            operation["bess"] = bess_var;

            // Operation of RES
            operation["res_generation"] = accounting;

            // Operation of demand
            operation["default_demand"] = accounting;

            // Operation of non-RES
            operation["conv_generation"] = vec_double;

            this->data["schedule"] = operation;
            this->data["actual"] = operation;
        }

        // Test the program
        void test(market_information_class &information){
            std::vector <std::string> keys;

            keys = std::vector <std::string> (2);
            keys[0] = "parameter";
            keys[1] = "num_interval";
            auto num_interval = std::any_cast <unsigned int> (*information.get_value_ptr(keys));

            keys = std::vector <std::string> (2);
            keys[0] = "prediction";
            keys[1] = "time_length";
            auto time_length = std::any_cast <std::vector <double>> (*information.get_value_ptr(keys));

            keys = std::vector <std::string> (2);
            keys[0] = "parameter";
            keys[1] = "type";
            auto participant_type = std::any_cast <unsigned int> (*this->get_value_ptr(keys));

            std::vector <double> default_demand;
            default_demand.reserve(num_interval);
            std::vector <double> conv_generation;
            conv_generation.reserve(num_interval);
            std::vector <double> res_generation;
            res_generation.reserve(num_interval);
            std::vector <double> ref_coeff;
            ref_coeff.reserve(num_interval);

            // Insert default demand for prosumers
            if(participant_type >= 2){
                for(unsigned int tick = 0; tick < num_interval; ++ tick){
                    double demand_temp = 2.;
                    double res_temp = 0.;
                    double conv_temp = 0.;
                    double ref_coeff_temp = 0.;

                    demand_temp *= time_length[tick];
                    default_demand.push_back(demand_temp);
                    conv_temp *= time_length[tick];
                    conv_generation.push_back(conv_temp);
                    res_temp *= time_length[tick];
                    res_generation.push_back(res_temp);
                    ref_coeff.push_back(ref_coeff_temp);
                }
            }
            // Insert default demand for retailers
            else{
                // Non-RES retailer
                if(participant_type == 0){
                    for(unsigned int tick = 0; tick < num_interval; ++ tick){
                        double demand_temp = 0.;
                        double res_temp = 0.;
                        double conv_temp = 1.;
                        double ref_coeff_temp = 0.;

                        demand_temp *= time_length[tick];
                        default_demand.push_back(demand_temp);
                        conv_temp *= time_length[tick];
                        conv_generation.push_back(conv_temp);
                        res_temp *= time_length[tick];
                        res_generation.push_back(res_temp);
                        ref_coeff.push_back(ref_coeff_temp);
                    }
                }
                // RES retailer
                else{
                    for(unsigned int tick = 0; tick < num_interval; ++ tick){
                        double demand_temp = 0.;
                        double res_temp = .5;
                        double conv_temp = 0.;
                        double ref_coeff_temp = 0.;

                        demand_temp *= time_length[tick];
                        default_demand.push_back(demand_temp);
                        conv_temp *= time_length[tick];
                        conv_generation.push_back(conv_temp);
                        res_temp *= time_length[tick];
                        res_generation.push_back(res_temp);
                        ref_coeff.push_back(ref_coeff_temp);
                    }
                }
            }

            // Store parameter time series
            keys = std::vector <std::string> (3);
            keys[0] = "parameter";
            keys[1] = "bess";
            keys[2] = "ref_coeff";
            this->update_value(keys, ref_coeff);

            // Higher cost for res retailer
            if(participant_type == 1){
                keys = std::vector <std::string> (3);
                keys[0] = "parameter";
                keys[1] = "premium";
                keys[2] = "res";
                this->update_value(keys, 1.);
            }

            // Store prediction time series
            keys = std::vector <std::string> (2);
            keys[0] = "prediction";
            keys[1] = "default_demand";
            this->update_value(keys, default_demand);
            keys[1] = "res_generation";
            this->update_value(keys, res_generation);
            keys[1] = "conv_generation";
            this->update_value(keys, conv_generation);

            // Initialize schedule and actual
            std::vector <double> vec_double (num_interval);

            dataset accounting;
            accounting["self"] = vec_double;
            accounting["lem"] = vec_double;
            accounting["rer"] = vec_double;
            accounting["cer"] = vec_double;

            dataset operation;
            operation["default_demand"] = accounting;

            this->data["schedule"] = operation;
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
            for(unsigned int agent_iter = 0; agent_iter < this->participants.size(); ++ agent_iter){
                this->participants[agent_iter].test(this->information);
            }
        }
};
