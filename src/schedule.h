// Market clearing and scheduling
#pragma once

// STL
#include <map>

// Included external source and headers
#include <alglib/optimization.h>

// Project specific
#include "src/market.h"

class scheduler_class{
    private:
        alglib::minlpstate Problem;
        struct variable_info{
            unsigned int number;
            std::map <std::string, unsigned int> ID;
            std::vector <std::string> name;
        };
        variable_info variable;


    public:
        scheduler_class(){
            // At each time interval, each market participant has 36 variables
            // self, lem, rer, cer of initial SOC are the first variables,
            // then for every t:
                // Variables 0 - 3: self, lem, rer, cer of aggregated supply
                // Variables 4 - 7: self, lem, rer, cer of aggregated demand
                // Variables 8 - 12: conv, res, demand, ch, dc
                // Variables 13 - 15: soc, soc_{free}, soc_{ref}
                // Variables 16 - 19: self, lem, rer, cer of RES generation
                // Variables 20 - 23: self, lem, rer, cer of default demand
                // Variables 24 - 27: self, lem, rer, cer of BESS charge
                // Variables 28 - 31: self, lem, rer, cer of BESS discharge
                // Variable 32 - 34: downgrading variables of stored electricity (self->lem, lem->rer, rer->cer)
                // Variables 35 - 38: self, lem, rer, cer of SOC
            this->variable.name = std::vector <std::string> (39);
            this->variable.name[0] = "supply_self";
            this->variable.name[1] = "supply_lem";
            this->variable.name[2] = "supply_rer";
            this->variable.name[3] = "supply_cer";
            this->variable.name[4] = "demand_self";
            this->variable.name[5] = "demand_lem";
            this->variable.name[6] = "demand_rer";
            this->variable.name[7] = "demand_cer";
            this->variable.name[8] = "conv";
            this->variable.name[9] = "res";
            this->variable.name[10] = "default_demand";
            this->variable.name[11] = "bess_ch";
            this->variable.name[12] = "bess_dc";
            this->variable.name[13] = "soc";
            this->variable.name[14] = "soc_free";
            this->variable.name[15] = "soc_ref";
            this->variable.name[16] = "res_self";
            this->variable.name[17] = "res_lem";
            this->variable.name[18] = "res_rer";
            this->variable.name[19] = "res_cer";
            this->variable.name[20] = "default_demand_self";
            this->variable.name[21] = "default_demand_lem";
            this->variable.name[22] = "default_demand_rer";
            this->variable.name[23] = "default_demand_cer";
            this->variable.name[24] = "bess_ch_self";
            this->variable.name[25] = "bess_ch_lem";
            this->variable.name[26] = "bess_ch_rer";
            this->variable.name[27] = "bess_ch_cer";
            this->variable.name[28] = "bess_dc_self";
            this->variable.name[29] = "bess_dc_lem";
            this->variable.name[30] = "bess_dc_rer";
            this->variable.name[31] = "bess_dc_cer";
            this->variable.name[32] = "bess_self_lem";
            this->variable.name[33] = "bess_lem_rer";
            this->variable.name[34] = "bess_rer_cer";
            this->variable.name[35] = "soc_self";
            this->variable.name[36] = "soc_lem";
            this->variable.name[37] = "soc_rer";
            this->variable.name[38] = "soc_cer";

            for(unsigned int var_iter = 0; var_iter < this->variable.name.size(); ++ var_iter){
                this->variable.ID[this->variable.name[var_iter]] = var_iter;
            }
        }

        void LP_set(market_class &market){
            // -------------------------------------------------------------------------------
            // LP Solver initialization for copper plate market optimization
            // -------------------------------------------------------------------------------
            std::vector <std::string> keys;

            keys = std::vector <std::string> (2);
            keys[0] = "parameter";
            keys[1] = "num_interval";
            auto num_interval = std::any_cast <unsigned int> (*market.information.get_value_ptr(keys));
            unsigned int variable_num_single = this->variable.name.size();
            unsigned int variable_num = variable_num_single * num_interval + 4;
            variable_num *= market.participants.size();
            this->variable.number = variable_num;

            // -------------------------------------------------------------------------------
            // Set matrix for general constraints
            // -------------------------------------------------------------------------------
            // At each time interval the market has (3 + 21 * p) general constraints
            // Unless stated otherwise, they will be equality constraints
            // for every t:
                // Constraints 0 - 2: equality between summation of aggregated supply and demand for lem, rer, and cer markets
                // for every p:
                    // Constraints 0: equality between summation of physical and accounting components of supply
                    // Constraints 1: equality between summation of physical and accounting components of demand
                    // Constraint 2: g_{self} = d_{self}
                    // Constraint 3: d_{self} - default_demand - bess_ch <= 0
                    // Constraint 4: soc = soc_{free} + soc_{ref}
                    // Constraints 5 - 9: accounting components of res, demand, ch, dc, soc
                    // Constraints 10 - 13: physical components of self, lem, rer, cer of supply
                    // Constraints 14 - 17: physical components of self, lem, rer, cer of demand
                    // Constraints 18 - 21: dynamics of accounting components of soc
            unsigned int constraint_num_single = 22;
            unsigned int constraint_num = constraint_num_single * market.participants.size() + 3;
            constraint_num *= num_interval;
            std::vector <ptrdiff_t> non_zero_num(constraint_num);
            for(unsigned int tick = 0; tick < num_interval; ++ tick){
                unsigned int start_ID = (constraint_num_single * market.participants.size() + 3) * tick;
                non_zero_num[start_ID] = 2 * market.participants.size();
                non_zero_num[start_ID + 1] = 2 * market.participants.size();
                non_zero_num[start_ID + 2] = 2 * market.participants.size();

                for(unsigned int agent_iter = 0; agent_iter < market.participants.size(); ++ agent_iter){
                    unsigned int start_ID_temp = start_ID + 3 + constraint_num_single * agent_iter;
                    non_zero_num[start_ID_temp] = 7;
                    non_zero_num[start_ID_temp + 1] = 6;
                    non_zero_num[start_ID_temp + 2] = 2;
                    non_zero_num[start_ID_temp + 3] = 3;
                    non_zero_num[start_ID_temp + 4] = 3;
                    non_zero_num[start_ID_temp + 5] = 5;
                    non_zero_num[start_ID_temp + 6] = 5;
                    non_zero_num[start_ID_temp + 7] = 5;
                    non_zero_num[start_ID_temp + 8] = 5;
                    non_zero_num[start_ID_temp + 9] = 5;
                    non_zero_num[start_ID_temp + 10] = 3;
                    non_zero_num[start_ID_temp + 11] = 3;
                    non_zero_num[start_ID_temp + 12] = 3;
                    non_zero_num[start_ID_temp + 13] = 4;
                    non_zero_num[start_ID_temp + 14] = 3;
                    non_zero_num[start_ID_temp + 15] = 3;
                    non_zero_num[start_ID_temp + 16] = 3;
                    non_zero_num[start_ID_temp + 17] = 3;
                    non_zero_num[start_ID_temp + 18] = 5;
                    non_zero_num[start_ID_temp + 19] = 6;
                    non_zero_num[start_ID_temp + 20] = 6;
                    non_zero_num[start_ID_temp + 21] = 5;
                }
            }
            alglib::integer_1d_array row_sizes_general;
            row_sizes_general.setcontent(non_zero_num.size(), non_zero_num.data());

            std::vector <std::vector <double>> bound_general(2);
            bound_general[0] = std::vector <double> (constraint_num);
            bound_general[1] = std::vector <double> (constraint_num);
            alglib::sparsematrix constraint_general;
            alglib::sparsecreatecrs(constraint_num, variable_num, row_sizes_general, constraint_general);
            for(unsigned int tick = 0; tick < num_interval; ++ tick){
                unsigned int start_row_ID = (constraint_num_single * market.participants.size() + 3) * tick;
                unsigned int start_col_ID = variable_num_single * tick + 4;
                start_col_ID *= market.participants.size();

                // Market wide linking constraints
                // supply demand balancing of lem, cer, and rer
                for(unsigned int account_iter = 0; account_iter < 3; ++ account_iter){
                    unsigned int row_ID = start_row_ID + account_iter;
                    for(unsigned int agent_iter = 0; agent_iter < market.participants.size(); ++ agent_iter){
                        unsigned int supply_ID = start_col_ID + variable_num_single * agent_iter + 1 + account_iter;
                        unsigned int demand_ID = start_col_ID + variable_num_single * agent_iter + 5 + account_iter;
                        alglib::sparseset(constraint_general, row_ID, supply_ID, 1.);
                        alglib::sparseset(constraint_general, row_ID, demand_ID, -1.);
                    }

                    bound_general[0][row_ID] = 0.;
                    bound_general[1][row_ID] = 0.;
                }

                // Participant specific constraints
                for(unsigned int agent_iter = 0; agent_iter < market.participants.size(); ++ agent_iter){
                    unsigned int start_row_ID_temp = start_row_ID + 3 + constraint_num_single * agent_iter;
                    unsigned int constr_iter = 0;

                    // Physical equals accounting, supply and demand
                    for(unsigned int dir_iter = 0; dir_iter < 2; ++ dir_iter){
                        unsigned int row_ID = start_row_ID_temp + constr_iter;

                        // Accounting
                        for(unsigned int account_iter = 0; account_iter < 4; ++ account_iter){
                            unsigned int col_ID = start_col_ID + variable_num_single * agent_iter + account_iter + 4 * dir_iter;
                            alglib::sparseset(constraint_general, row_ID, col_ID, 1.);
                        }

                        // Physical
                        if(dir_iter == 0){
                            // Conv generation
                            unsigned int col_ID = start_col_ID + variable_num_single * agent_iter + this->variable.ID["conv"];
                            alglib::sparseset(constraint_general, row_ID, col_ID, -1.);

                            // RES generation
                            col_ID = start_col_ID + variable_num_single * agent_iter + this->variable.ID["res"];
                            alglib::sparseset(constraint_general, row_ID, col_ID, -1.);

                            // BESS discharge
                            col_ID = start_col_ID + variable_num_single * agent_iter + this->variable.ID["bess_dc"];
                            alglib::sparseset(constraint_general, row_ID, col_ID, -1.);
                        }
                        else{
                            // Default demand
                            unsigned int col_ID = start_col_ID + variable_num_single * agent_iter + this->variable.ID["default_demand"];
                            alglib::sparseset(constraint_general, row_ID, col_ID, -1.);

                            // BESS charge
                            col_ID = start_col_ID + variable_num_single * agent_iter + this->variable.ID["bess_ch"];
                            alglib::sparseset(constraint_general, row_ID, col_ID, -1.);
                        }

                        bound_general[0][row_ID] = 0.;
                        bound_general[1][row_ID] = 0.;
                        constr_iter += 1;
                    }

                    // g_{self} = d_{self}
                    {
                        unsigned int row_ID = start_row_ID_temp + constr_iter;
                        unsigned int col_ID;
                        std::string var_name;

                        var_name = "supply_self";
                        col_ID = start_col_ID + variable_num_single * agent_iter + this->variable.ID[var_name];
                        alglib::sparseset(constraint_general, row_ID, col_ID, 1.);

                        var_name = "demand_self";
                        col_ID = start_col_ID + variable_num_single * agent_iter + this->variable.ID[var_name];
                        alglib::sparseset(constraint_general, row_ID, col_ID, -1.);

                        bound_general[0][row_ID] = 0.;
                        bound_general[1][row_ID] = 0.;
                        constr_iter += 1;
                    }

                    // d_{self} - default_demand - bess_ch <= 0
                    {
                        unsigned int row_ID = start_row_ID_temp + constr_iter;
                        unsigned int col_ID;
                        std::string var_name;

                        var_name = "demand_self";
                        col_ID = start_col_ID + variable_num_single * agent_iter + this->variable.ID[var_name];
                        alglib::sparseset(constraint_general, row_ID, col_ID, 1.);

                        var_name = "default_demand";
                        col_ID = start_col_ID + variable_num_single * agent_iter + this->variable.ID[var_name];
                        alglib::sparseset(constraint_general, row_ID, col_ID, -1.);

                        var_name = "bess_ch";
                        col_ID = start_col_ID + variable_num_single * agent_iter + this->variable.ID[var_name];
                        alglib::sparseset(constraint_general, row_ID, col_ID, -1.);

                        bound_general[0][row_ID] = -std::numeric_limits<double>::infinity();
                        bound_general[1][row_ID] = 0.;
                        constr_iter += 1;
                    }

                    // soc = soc_{free} + soc_{ref}
                    {
                        unsigned int row_ID = start_row_ID_temp + constr_iter;
                        unsigned int col_ID;
                        std::string var_name;

                        var_name = "soc";
                        col_ID = start_col_ID + variable_num_single * agent_iter + this->variable.ID[var_name];
                        alglib::sparseset(constraint_general, row_ID, col_ID, 1.);

                        var_name = "soc_free";
                        col_ID = start_col_ID + variable_num_single * agent_iter + this->variable.ID[var_name];
                        alglib::sparseset(constraint_general, row_ID, col_ID, -1.);

                        var_name = "soc_ref";
                        col_ID = start_col_ID + variable_num_single * agent_iter + this->variable.ID[var_name];
                        alglib::sparseset(constraint_general, row_ID, col_ID, -1.);

                        bound_general[0][row_ID] = 0.;
                        bound_general[1][row_ID] = 0.;
                        constr_iter += 1;
                    }

                    // Accounting components of res, demand, ch, dc, soc
                    std::vector <std::string> account_components(4);
                    account_components[0] = "self";
                    account_components[1] = "lem";
                    account_components[2] = "rer";
                    account_components[3] = "cer";
                    {
                        std::vector <std::string> physical_components(5);
                        physical_components[0] = "res";
                        physical_components[1] = "default_demand";
                        physical_components[2] = "bess_ch";
                        physical_components[3] = "bess_dc";
                        physical_components[4] = "soc";

                        for(unsigned int physical_iter = 0; physical_iter < physical_components.size(); ++ physical_iter){
                            unsigned int row_ID = start_row_ID_temp + constr_iter;
                            unsigned int col_ID;

                            col_ID = start_col_ID + variable_num_single * agent_iter + this->variable.ID[physical_components[physical_iter]];
                            alglib::sparseset(constraint_general, row_ID, col_ID, 1.);

                            for(auto &account_iter : account_components){
                                std::string var_name_temp = physical_components[physical_iter] + "_" + account_iter;
                                col_ID = start_col_ID + variable_num_single * agent_iter + this->variable.ID[var_name_temp];
                                alglib::sparseset(constraint_general, row_ID, col_ID, -1.);
                            }

                            bound_general[0][row_ID] = 0.;
                            bound_general[1][row_ID] = 0.;
                            constr_iter += 1;
                        }
                    }

                    // Physical components of self, lem, rer, cer of supply
                    for(unsigned int account_iter = 0; account_iter < account_components.size(); ++ account_iter){
                        unsigned int row_ID = start_row_ID_temp + constr_iter;
                        unsigned int col_ID;
                        std::string var_name;

                        var_name = "supply_" + account_components[account_iter];
                        col_ID = start_col_ID + variable_num_single * agent_iter + this->variable.ID[var_name];
                        alglib::sparseset(constraint_general, row_ID, col_ID, 1.);

                        if(account_components[account_iter] == "cer"){
                            var_name = "conv";
                            col_ID = start_col_ID + variable_num_single * agent_iter + this->variable.ID[var_name];
                            alglib::sparseset(constraint_general, row_ID, col_ID, -1.);
                        }

                        var_name = "res_" + account_components[account_iter];
                        col_ID = start_col_ID + variable_num_single * agent_iter + this->variable.ID[var_name];
                        alglib::sparseset(constraint_general, row_ID, col_ID, -1.);

                        var_name = "bess_dc_" + account_components[account_iter];
                        col_ID = start_col_ID + variable_num_single * agent_iter + this->variable.ID[var_name];
                        alglib::sparseset(constraint_general, row_ID, col_ID, -1.);

                        bound_general[0][row_ID] = 0.;
                        bound_general[1][row_ID] = 0.;
                        constr_iter += 1;
                    }

                    // Physical components of self, lem, rer, cer of demand
                    for(unsigned int account_iter = 0; account_iter < account_components.size(); ++ account_iter){
                        unsigned int row_ID = start_row_ID_temp + constr_iter;
                        unsigned int col_ID;
                        std::string var_name;

                        var_name = "demand_" + account_components[account_iter];
                        col_ID = start_col_ID + variable_num_single * agent_iter + this->variable.ID[var_name];
                        alglib::sparseset(constraint_general, row_ID, col_ID, 1.);

                        var_name = "default_demand_" + account_components[account_iter];
                        col_ID = start_col_ID + variable_num_single * agent_iter + this->variable.ID[var_name];
                        alglib::sparseset(constraint_general, row_ID, col_ID, -1.);

                        var_name = "bess_ch_" + account_components[account_iter];
                        col_ID = start_col_ID + variable_num_single * agent_iter + this->variable.ID[var_name];
                        alglib::sparseset(constraint_general, row_ID, col_ID, -1.);

                        bound_general[0][row_ID] = 0.;
                        bound_general[1][row_ID] = 0.;
                        constr_iter += 1;
                    }

                    // Dynamics of accounting components of soc
                    for(unsigned int account_iter = 0; account_iter < account_components.size(); ++ account_iter){
                        unsigned int row_ID = start_row_ID_temp + constr_iter;
                        unsigned int col_ID;
                        std::string var_name;

                        keys = std::vector <std::string> (3);
                        keys[0] = "parameter";
                        keys[1] = "bess";
                        keys[2] = "efficiency";
                        auto efficiency = std::any_cast <double> (*market.participants[agent_iter].get_value_ptr(keys));

                        var_name = "soc_" + account_components[account_iter];
                        col_ID = start_col_ID + variable_num_single * agent_iter + this->variable.ID[var_name];
                        col_ID -= variable_num_single;
                        alglib::sparseset(constraint_general, row_ID, col_ID, 1.);

                        var_name = "bess_ch_" + account_components[account_iter];
                        col_ID = start_col_ID + variable_num_single * agent_iter + this->variable.ID[var_name];
                        alglib::sparseset(constraint_general, row_ID, col_ID, efficiency);

                        var_name = "bess_dc_" + account_components[account_iter];
                        col_ID = start_col_ID + variable_num_single * agent_iter + this->variable.ID[var_name];
                        alglib::sparseset(constraint_general, row_ID, col_ID, -1. / efficiency);

                        if(account_iter > 0){
                            var_name = "bess_" + account_components[account_iter - 1] + "_" + account_components[account_iter];
                            col_ID = start_col_ID + variable_num_single * agent_iter + this->variable.ID[var_name];
                            alglib::sparseset(constraint_general, row_ID, col_ID, 1.);
                        }

                        if(account_iter < account_components.size() - 1){
                            var_name = "bess_" + account_components[account_iter] + "_" + account_components[account_iter + 1];
                            col_ID = start_col_ID + variable_num_single * agent_iter + this->variable.ID[var_name];
                            alglib::sparseset(constraint_general, row_ID, col_ID, -1.);
                        }

                        var_name = "soc_" + account_components[account_iter];
                        col_ID = start_col_ID + variable_num_single * agent_iter + this->variable.ID[var_name];
                        alglib::sparseset(constraint_general, row_ID, col_ID, -1.);

                        bound_general[0][row_ID] = 0.;
                        bound_general[1][row_ID] = 0.;
                        constr_iter += 1;
                    }
                }
            }

            // -------------------------------------------------------------------------------
            // Set boundary for equality constraints
            // -------------------------------------------------------------------------------
            alglib::real_1d_array lb_general;
            alglib::real_1d_array ub_general;
            lb_general.setcontent(bound_general[0].size(), bound_general[0].data());
            ub_general.setcontent(bound_general[1].size(), bound_general[1].data());

            // -------------------------------------------------------------------------------
            // Set scale of variables
            // -------------------------------------------------------------------------------



            // -------------------------------------------------------------------------------
            // Set the LP problem object
            // -------------------------------------------------------------------------------
            alglib::minlpcreate(variable_num, this->Problem);
            alglib::minlpsetlc2(this->Problem, constraint_general, lb_general, ub_general, constraint_num);
            alglib::minlpsetalgodss(this->Problem, 0.);
        }

        void LP_solve(market_class &market){
            // -------------------------------------------------------------------------------
            // Solve the LP for copper plate market optimization
            // -------------------------------------------------------------------------------
            std::vector <std::string> keys;

            keys = std::vector <std::string> (2);
            keys[0] = "parameter";
            keys[1] = "num_interval";
            auto num_interval = std::any_cast <unsigned int> (*market.information.get_value_ptr(keys));

            unsigned int variable_num = this->variable.number;
            unsigned int variable_num_single = this->variable.name.size();

            // -------------------------------------------------------------------------------
            // Set boundary for box constraints and obj values
            // -------------------------------------------------------------------------------
            std::vector <std::vector <double>> bound_box(2);
            bound_box[0] = std::vector <double> (variable_num);
            bound_box[1] = std::vector <double> (variable_num);
            std::vector <double> obj_vec(variable_num);

            // Set default values for vectors
            // Default box constraint = non-negativity
            // Default objective value = 0
            for(unsigned int var_iter = 0; var_iter < variable_num; ++ var_iter){
                bound_box[0][var_iter] = 0.;
                bound_box[1][var_iter] = std::numeric_limits<double>::infinity();
                obj_vec[var_iter] = 0.;
            }

            // Boundaries for initial SOC
            for(unsigned int agent_iter = 0; agent_iter < market.participants.size(); ++ agent_iter){
                keys = std::vector <std::string> (3);
                keys[0] = "parameter";
                keys[1] = "bess";
                keys[2] = "initial_soc";
                auto initial_soc = std::any_cast <dataset> (*market.participants[agent_iter].get_value_ptr(keys));

                std::vector <std::string> account_components(4);
                account_components[0] = "self";
                account_components[1] = "lem";
                account_components[2] = "rer";
                account_components[3] = "cer";

                for(unsigned int account_iter = 0; account_iter < account_components.size(); ++ account_iter){
                    unsigned int var_ID = 4 * agent_iter + account_iter;
                    bound_box[0][var_ID] = std::any_cast <double> (initial_soc[account_components[account_iter]]);
                    bound_box[1][var_ID] = std::any_cast <double> (initial_soc[account_components[account_iter]]);
                }
            }

            for(unsigned int tick = 0; tick < num_interval; ++ tick){
                for(unsigned int agent_iter = 0; agent_iter < market.participants.size(); ++ agent_iter){
                    keys = std::vector <std::string> (2);
                    keys[0] = "parameter";
                    keys[1] = "type";
                    auto type_ID = std::any_cast <unsigned int> (*market.participants[agent_iter].get_value_ptr(keys));

                    // Indices of variables
                    unsigned int start_var_ID = variable_num_single * tick + 4;
                    start_var_ID *= market.participants.size();

                    // Box constraints and obj values of physical constraints
                    {
                        unsigned int var_ID;
                        std::string var_name;

                        keys = std::vector <std::string> (3);
                        keys[0] = "parameter";
                        keys[1] = "bess";
                        keys[2] = "capacity";
                        auto capacity = std::any_cast <double> (*market.participants[agent_iter].get_value_ptr(keys));
                        keys[2] = "energy";
                        auto energy = std::any_cast <double> (*market.participants[agent_iter].get_value_ptr(keys));
                        keys[2] = "efficiency";
                        auto efficiency = std::any_cast <double> (*market.participants[agent_iter].get_value_ptr(keys));

                        keys = std::vector <std::string> (2);
                        keys[0] = "prediction";
                        keys[1] = "time_length";
                        auto time_length_temp = market.information.get_vector_value <double> (keys, tick);
                        keys[1] = "electricity_price";
                        auto electricity_price_temp = market.information.get_vector_value <double> (keys, tick);
                        keys[1] = "conv_generation";
                        auto conv_generation_temp = market.participants[agent_iter].get_vector_value <double> (keys, tick);
                        keys[1] = "res_generation";
                        auto res_generation_temp = market.participants[agent_iter].get_vector_value <double> (keys, tick);
                        keys[1] = "default_demand";
                        auto default_demand_temp = market.participants[agent_iter].get_vector_value <double> (keys, tick);

                        var_name = "conv";
                        var_ID = start_var_ID + variable_num_single * agent_iter + this->variable.ID[var_name];
                        bound_box[1][var_ID] = conv_generation_temp;
                        obj_vec[var_ID] = electricity_price_temp;

                        var_name = "res";
                        var_ID = start_var_ID + variable_num_single * agent_iter + this->variable.ID[var_name];
                        bound_box[1][var_ID] = res_generation_temp;

                        var_name = "default_demand";
                        var_ID = start_var_ID + variable_num_single * agent_iter + this->variable.ID[var_name];
                        bound_box[1][var_ID] = default_demand_temp;

                        var_name = "bess_ch";
                        var_ID = start_var_ID + variable_num_single * agent_iter + this->variable.ID[var_name];
                        bound_box[1][var_ID] = capacity / efficiency * time_length_temp;

                        var_name = "bess_dc";
                        var_ID = start_var_ID + variable_num_single * agent_iter + this->variable.ID[var_name];
                        bound_box[1][var_ID] = capacity * efficiency * time_length_temp;

                        var_name = "soc";
                        var_ID = start_var_ID + variable_num_single * agent_iter + this->variable.ID[var_name];
                        bound_box[1][var_ID] = energy;
                    }

                    // Box constraints and objective values of soc_{ref}
                    {
                        unsigned int var_ID;
                        std::string var_name;

                        keys = std::vector <std::string> (3);
                        keys[0] = "parameter";
                        keys[1] = "bess";
                        keys[2] = "energy";
                        auto energy = std::any_cast <double> (*market.participants[agent_iter].get_value_ptr(keys));
                        keys[2] = "ref_ratio";
                        auto ref_ratio = std::any_cast <double> (*market.participants[agent_iter].get_value_ptr(keys));
                        keys[2] = "ref_coeff";
                        auto ref_coeff_temp = market.participants[agent_iter].get_vector_value <double> (keys, tick);

                        var_name = "soc_ref";
                        var_ID = start_var_ID + variable_num_single * agent_iter + this->variable.ID[var_name];
                        bound_box[1][var_ID] = ref_ratio * energy;
                        obj_vec[var_ID] = ref_coeff_temp;
                    }

                    // Objective values of various accounting components
                    {
                        unsigned int var_ID;
                        std::string var_name;

                        keys = std::vector <std::string> (2);
                        keys[0] = "parameter";
                        keys[1] = "price_inflex_demand";
                        auto price_inflex_demand = std::any_cast <double> (*market.information.get_value_ptr(keys));

                        keys = std::vector <std::string> (2);
                        keys[0] = "prediction";
                        keys[1] = "electricity_price";
                        auto electricity_price_temp = market.information.get_vector_value <double> (keys, tick);

                        keys = std::vector <std::string> (3);
                        keys[0] = "parameter";
                        keys[1] = "premium";
                        keys[2] = "res";
                        auto res_premium = std::any_cast <double> (*market.participants[agent_iter].get_value_ptr(keys));
                        keys[2] = "lem";
                        auto lem_premium = std::any_cast <double> (*market.participants[agent_iter].get_value_ptr(keys));
                        keys[2] = "self";
                        auto self_premium = std::any_cast <double> (*market.participants[agent_iter].get_value_ptr(keys));

                        var_name = "res_self";
                        var_ID = start_var_ID + variable_num_single * agent_iter + this->variable.ID[var_name];
                        // Retailers don't self-prosume electricity
                        // Marginal cost of res production from prosumers is 0
                        if(type_ID < 2){
                            bound_box[1][var_ID] = 0.;
                            obj_vec[var_ID] = electricity_price_temp + res_premium;
                        }

                        var_name = "res_lem";
                        var_ID = start_var_ID + variable_num_single * agent_iter + this->variable.ID[var_name];
                        // CERs don't sell renewable energy
                        // RERs participation in lem is allocated to res_rer
                        // Marginal cost of res production from prosumers is 0
                        if(type_ID < 2){
                            bound_box[1][var_ID] = 0.;
                            obj_vec[var_ID] = electricity_price_temp + res_premium;
                        }

                        var_name = "res_rer";
                        var_ID = start_var_ID + variable_num_single * agent_iter + this->variable.ID[var_name];
                        // CERs don't sell renewable energy
                        // Marginal cost of res production from prosumers is 0
                        if(type_ID < 2){
                            if(type_ID == 0){
                                bound_box[1][var_ID] = 0;
                            }
                            obj_vec[var_ID] = electricity_price_temp + res_premium;
                        }

                        var_name = "res_cer";
                        var_ID = start_var_ID + variable_num_single * agent_iter + this->variable.ID[var_name];
                        // CERs don't sell renewable energy
                        // RERs don't sell renewable energy to normal market
                        // Marginal cost of res production from prosumers is 0
                        if(type_ID < 2){
                            bound_box[1][var_ID] = 0.;
                            obj_vec[var_ID] = electricity_price_temp + res_premium;
                        }

                        var_name = "default_demand_self";
                        var_ID = start_var_ID + variable_num_single * agent_iter + this->variable.ID[var_name];
                        // Retailers don't self-prosume electricity
                        // Only prosumers have marginal utility for using electricity
                        if(type_ID < 2){
                            bound_box[1][var_ID] = 0.;
                        }
                        else{
                            obj_vec[var_ID] = price_inflex_demand + res_premium + lem_premium + self_premium;
                            obj_vec[var_ID] *= -1;
                        }

                        var_name = "default_demand_lem";
                        var_ID = start_var_ID + variable_num_single * agent_iter + this->variable.ID[var_name];
                        // Retailers participate in _rer or _cer
                        // Only prosumers have marginal utility for using electricity
                        if(type_ID < 2){
                            bound_box[1][var_ID] = 0.;
                        }
                        else{
                            obj_vec[var_ID] = price_inflex_demand + res_premium + lem_premium;
                            obj_vec[var_ID] *= -1;
                        }

                        var_name = "default_demand_rer";
                        var_ID = start_var_ID + variable_num_single * agent_iter + this->variable.ID[var_name];
                        // CERs don't participate in _rer
                        // marginal utility of retailers = price of selling electricity to consumers outside the lem
                        if(type_ID < 2){
                            if(type_ID == 0){
                                bound_box[1][var_ID] = 0.;
                            }
                            obj_vec[var_ID] = electricity_price_temp + res_premium;
                            obj_vec[var_ID] *= -1;
                        }
                        else{
                            obj_vec[var_ID] = price_inflex_demand + res_premium;
                            obj_vec[var_ID] *= -1;
                        }

                        var_name = "default_demand_cer";
                        var_ID = start_var_ID + variable_num_single * agent_iter + this->variable.ID[var_name];
                        // RERs don't participate in _cer
                        // marginal utility of retailers = price of selling electricity to consumers outside the lem
                        if(type_ID < 2){
                            if(type_ID == 1){
                                bound_box[1][var_ID] = 0;
                            }
                            obj_vec[var_ID] = electricity_price_temp + res_premium;
                            obj_vec[var_ID] *= -1;
                        }
                        else{
                            obj_vec[var_ID] = price_inflex_demand;
                            obj_vec[var_ID] *= -1;
                        }

                        std::vector <std::string> bess_components(3);
                        bess_components[0] = "bess_ch";
                        bess_components[1] = "bess_dc";
                        bess_components[2] = "soc";
                        for(unsigned int component_iter = 0; component_iter < bess_components.size(); ++ component_iter){
                            var_name = bess_components[component_iter] + "_self";
                            var_ID = start_var_ID + variable_num_single * agent_iter + this->variable.ID[var_name];
                            // Retailers don't self-prosume electricity
                            if(type_ID < 2){
                                bound_box[1][var_ID] = 0.;
                            }

                            var_name = bess_components[component_iter] + "_lem";
                            var_ID = start_var_ID + variable_num_single * agent_iter + this->variable.ID[var_name];
                            // Retailers participate in _rer or _cer
                            if(type_ID < 2){
                                bound_box[1][var_ID] = 0.;
                            }

                            var_name = bess_components[component_iter] + "_rer";
                            var_ID = start_var_ID + variable_num_single * agent_iter + this->variable.ID[var_name];
                            // CERs don't participate in _rer
                            if(type_ID == 0){
                                bound_box[1][var_ID] = 0.;
                            }

                            var_name = bess_components[component_iter] + "_cer";
                            var_ID = start_var_ID + variable_num_single * agent_iter + this->variable.ID[var_name];
                            // RERs don't participate in _rer
                            if(type_ID == 1){
                                bound_box[1][var_ID] = 0.;
                            }
                        }
                    }
                }
            }
            alglib::real_1d_array lb_box;
            alglib::real_1d_array ub_box;
            lb_box.setcontent(bound_box[0].size(), bound_box[0].data());
            ub_box.setcontent(bound_box[1].size(), bound_box[1].data());
            alglib::minlpsetbc(this->Problem, lb_box, ub_box);

            // -------------------------------------------------------------------------------
            // Set objective coefficients of variables
            // -------------------------------------------------------------------------------
            alglib::real_1d_array obj_coeff;
            obj_coeff.setcontent(obj_vec.size(), obj_vec.data());
            alglib::minlpsetcost(this->Problem, obj_coeff);

            // -------------------------------------------------------------------------------
            // Solve the problem
            // -------------------------------------------------------------------------------
            alglib::minlpoptimize(this->Problem);

            // -------------------------------------------------------------------------------
            // Write the results
            // -------------------------------------------------------------------------------
            alglib::real_1d_array sol;
            alglib::minlpreport rep;
            minlpresults(this->Problem, sol, rep);
            for(unsigned int tick = 0; tick < num_interval; ++ tick){
                for(unsigned int agent_iter = 0; agent_iter < market.participants.size(); ++ agent_iter){
                    std::vector <std::string> account_components(4);
                    account_components[0] = "self";
                    account_components[1] = "lem";
                    account_components[2] = "rer";
                    account_components[3] = "cer";

                    // Indices of variables
                    unsigned int start_var_ID = variable_num_single * tick + 4;
                    start_var_ID *= market.participants.size();

                    // Store schedule for default demand
                    {
                        unsigned int var_ID;
                        std::string var_name;
                        double store_value;

                        keys = std::vector <std::string> (3);
                        keys[0] = "schedule";
                        keys[1] = "default_demand";
                        for(unsigned int account_iter = 0; account_iter < account_components.size(); ++ account_iter){
                            keys[2] = account_components[account_iter];
                            var_name = keys[1] + "_" + keys[2];
                            var_ID = start_var_ID + variable_num_single * agent_iter + this->variable.ID[var_name];
                            store_value = sol[var_ID];
                            market.participants[agent_iter].update_vector_value <double> (keys, tick, store_value);
                            std::cout << agent_iter << "\t";
                            std::cout << var_name << "\t";
                            std::cout << tick << "\t";
                            std::cout << store_value << "\t";
                            std::cout << "\n";
                        }
                    }
                }
            }

        }
};
