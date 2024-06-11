// Market clearing and scheduling
#pragma once

// STL
#include <map>

// Included external source and headers
#include <alglib/optimization.h>
//#include <Eigen/Dense>
//#include <Eigen/Sparse>

// Project specific
#include "src/market.h"

//#ifndef EIGEN_TYPES
//#define EIGEN_TYPES
//
//namespace Eigen{
//    typedef Eigen::Matrix<ptrdiff_t, Eigen::Dynamic, 1> VectorXpd;
////    typedef Eigen::Triplet <double> TripletXd;
//}
//
//#endif

class scheduler_class{
    private:
        alglib::minlpstate Problem;
        std::map <std::string, unsigned int> variable_ID;
        std::vector <std::string> variable_name;

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
                // Variables 32 - 35: self, lem, rer, cer of SOC
            this->variable_name = std::vector <std::string> (36);
            this->variable_name[0] = "supply_self";
            this->variable_name[1] = "supply_lem";
            this->variable_name[2] = "supply_rer";
            this->variable_name[3] = "supply_cer";
            this->variable_name[4] = "demand_self";
            this->variable_name[5] = "demand_lem";
            this->variable_name[6] = "demand_rer";
            this->variable_name[7] = "demand_cer";
            this->variable_name[8] = "conv";
            this->variable_name[9] = "res";
            this->variable_name[10] = "default_demand";
            this->variable_name[11] = "bess_ch";
            this->variable_name[12] = "bess_dc";
            this->variable_name[13] = "soc";
            this->variable_name[14] = "soc_free";
            this->variable_name[15] = "soc_ref";
            this->variable_name[16] = "res_self";
            this->variable_name[17] = "res_lem";
            this->variable_name[18] = "res_rer";
            this->variable_name[19] = "res_cer";
            this->variable_name[20] = "default_demand_self";
            this->variable_name[21] = "default_demand_lem";
            this->variable_name[22] = "default_demand_rer";
            this->variable_name[23] = "default_demand_cer";
            this->variable_name[24] = "bess_ch_self";
            this->variable_name[25] = "bess_ch_lem";
            this->variable_name[26] = "bess_ch_rer";
            this->variable_name[27] = "bess_ch_cer";
            this->variable_name[28] = "bess_dc_self";
            this->variable_name[29] = "bess_dc_lem";
            this->variable_name[30] = "bess_dc_rer";
            this->variable_name[31] = "bess_dc_cer";
            this->variable_name[32] = "soc_self";
            this->variable_name[33] = "soc_lem";
            this->variable_name[34] = "soc_rer";
            this->variable_name[35] = "soc_cer";

            for(unsigned int var_iter = 0; var_iter < this->variable_name.size(); ++ var_iter){
                this->variable_ID[this->variable_name[var_iter]] = var_iter;
            }
        }

        void LP_set(market_class &market){
            // -------------------------------------------------------------------------------
            // LP Solver initialization for copper plate market optimization
            // -------------------------------------------------------------------------------
            unsigned int num_interval = std::get <unsigned int> (market.information.get_parameter_values("num_interval"));
            unsigned int variable_num_single = this->variable_name.size();
            unsigned int variable_num = variable_num_single * market.participants.size();
            variable_num *= num_interval;
            variable_num += 4 * market.participants.size();

            // -------------------------------------------------------------------------------
            // Set matrix for general equality constraints
            // -------------------------------------------------------------------------------
            // At each time interval the market has (3 + 21 * p) equality constraints
            // for every t:
                // Constraints 0 - 2: equality between summation of aggregated supply and demand for lem, rer, and cer markets
                // for every p:
                    // Constraints 0: equality between summation of physical and accounting components of supply
                    // Constraints 1: equality between summation of physical and accounting components of demand
                    // Constraint 2: g_{self} = d_{self}
                    // Constraint 3: soc = soc_{free} + soc_{ref}
                    // Constraints 4 - 8: accounting components of res, demand, ch, dc, soc
                    // Constraints 9 - 12: physical components of self, lem, rer, cer of supply
                    // Constraints 13 - 16: physical components of self, lem, rer, cer of demand
                    // Constraints 17 - 20: dynamics of accounting components of soc
            unsigned int constraint_num_single = 21;
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
                    non_zero_num[start_ID_temp + 4] = 5;
                    non_zero_num[start_ID_temp + 5] = 5;
                    non_zero_num[start_ID_temp + 6] = 5;
                    non_zero_num[start_ID_temp + 7] = 5;
                    non_zero_num[start_ID_temp + 8] = 5;
                    non_zero_num[start_ID_temp + 9] = 3;
                    non_zero_num[start_ID_temp + 10] = 3;
                    non_zero_num[start_ID_temp + 11] = 3;
                    non_zero_num[start_ID_temp + 12] = 4;
                    non_zero_num[start_ID_temp + 13] = 3;
                    non_zero_num[start_ID_temp + 14] = 3;
                    non_zero_num[start_ID_temp + 15] = 3;
                    non_zero_num[start_ID_temp + 16] = 3;
                    non_zero_num[start_ID_temp + 17] = 3 + (tick > 0);
                    non_zero_num[start_ID_temp + 18] = 3 + (tick > 0);
                    non_zero_num[start_ID_temp + 19] = 3 + (tick > 0);
                    non_zero_num[start_ID_temp + 20] = 3 + (tick > 0);
                }
            }
            alglib::integer_1d_array row_sizes_general;
            row_sizes_general.setcontent(non_zero_num.size(), non_zero_num.data());
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
                        std::cout << row_ID << "\t" << supply_ID << "\n";
                        std::cout << row_ID << "\t" << demand_ID << "\n";
                        alglib::sparseset(constraint_general, row_ID, supply_ID, 1.);
                        alglib::sparseset(constraint_general, row_ID, demand_ID, -1.);
                    }
                }

                // Participant specific constraints
                for(unsigned int agent_iter = 0; agent_iter < market.participants.size(); ++ agent_iter){
                    unsigned int start_row_ID_temp = start_row_ID + 3 + constraint_num_single * agent_iter;

                    // Physical equals accounting, supply and demand
                    for(unsigned int dir_iter = 0; dir_iter < 2; ++ dir_iter){
                        unsigned int row_ID = start_row_ID_temp + dir_iter;

                        // Accounting
                        for(unsigned int account_iter = 0; account_iter < 4; ++ account_iter){
                            unsigned int col_ID = start_col_ID + variable_num_single * agent_iter + account_iter + 4 * dir_iter;
                            std::cout << row_ID << "\t" << col_ID << "\n";
                            alglib::sparseset(constraint_general, row_ID, col_ID, 1.);
                        }

                        // Physical
                        if(dir_iter == 0){
                            // Conv generation
                            unsigned int col_ID = start_col_ID + variable_num_single * agent_iter + this->variable_ID["conv"];
                            alglib::sparseset(constraint_general, row_ID, col_ID, -1.);
                            std::cout << row_ID << "\t" << col_ID << "\n";

                            // RES generation
                            col_ID = start_col_ID + variable_num_single * agent_iter + this->variable_ID["res"];
                            alglib::sparseset(constraint_general, row_ID, col_ID, -1.);
                            std::cout << row_ID << "\t" << col_ID << "\n";

                            // BESS discharge
                            col_ID = start_col_ID + variable_num_single * agent_iter + this->variable_ID["bess_dc"];
                            alglib::sparseset(constraint_general, row_ID, col_ID, -1.);
                            std::cout << row_ID << "\t" << col_ID << "\n";
                        }
                        else{
                            // Default demand
                            unsigned int col_ID = start_col_ID + variable_num_single * agent_iter + this->variable_ID["default_demand"];
                            alglib::sparseset(constraint_general, row_ID, col_ID, -1.);
                            std::cout << row_ID << "\t" << col_ID << "\n";

                            // BESS charge
                            col_ID = start_col_ID + variable_num_single * agent_iter + this->variable_ID["bess_ch"];
                            alglib::sparseset(constraint_general, row_ID, col_ID, -1.);
                            std::cout << row_ID << "\t" << col_ID << "\n";
                        }
                    }

                    // g_{self} = d_{self}
                    {
                        unsigned int row_ID = start_row_ID_temp + 2;
                        unsigned int col_ID;
                        std::string var_name;

                        var_name = "supply_self";
                        col_ID = start_col_ID + variable_num_single * agent_iter + this->variable_ID[var_name];
                        alglib::sparseset(constraint_general, row_ID, col_ID, 1.);
                        std::cout << row_ID << "\t" << col_ID << "\n";

                        var_name = "demand_self";
                        col_ID = start_col_ID + variable_num_single * agent_iter + this->variable_ID[var_name];
                        alglib::sparseset(constraint_general, row_ID, col_ID, -1.);
                        std::cout << row_ID << "\t" << col_ID << "\n";
                    }

                    // soc = soc_{free} + soc_{ref}
                    {
                        unsigned int row_ID = start_row_ID_temp + 3;
                        unsigned int col_ID;
                        std::string var_name;

                        var_name = "soc";
                        col_ID = start_col_ID + variable_num_single * agent_iter + this->variable_ID[var_name];
                        alglib::sparseset(constraint_general, row_ID, col_ID, 1.);
                        std::cout << row_ID << "\t" << col_ID << "\n";

                        var_name = "soc_free";
                        col_ID = start_col_ID + variable_num_single * agent_iter + this->variable_ID[var_name];
                        alglib::sparseset(constraint_general, row_ID, col_ID, -1.);
                        std::cout << row_ID << "\t" << col_ID << "\n";

                        var_name = "soc_ref";
                        col_ID = start_col_ID + variable_num_single * agent_iter + this->variable_ID[var_name];
                        alglib::sparseset(constraint_general, row_ID, col_ID, -1.);
                        std::cout << row_ID << "\t" << col_ID << "\n";
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
                            unsigned int row_ID = start_row_ID_temp + 4 + physical_iter;
                            unsigned int col_ID;

                            col_ID = start_col_ID + variable_num_single * agent_iter + this->variable_ID[physical_components[physical_iter]];
                            alglib::sparseset(constraint_general, row_ID, col_ID, 1.);
                            std::cout << row_ID << "\t" << col_ID << "\n";

                            for(auto &account_iter : account_components){
                                std::string var_name_temp = physical_components[physical_iter] + "_" + account_iter;
                                col_ID = start_col_ID + variable_num_single * agent_iter + this->variable_ID[var_name_temp];
                                alglib::sparseset(constraint_general, row_ID, col_ID, -1.);
                                std::cout << row_ID << "\t" << col_ID << "\n";
                            }
                        }
                    }

                    // Physical components of self, lem, rer, cer of supply
                    for(unsigned int account_iter = 0; account_iter < account_components.size(); ++ account_iter){
                        unsigned int row_ID = start_row_ID_temp + 9 + account_iter;
                        unsigned int col_ID;
                        std::string var_name;

                        var_name = "supply_" + account_components[account_iter];
                        col_ID = start_col_ID + variable_num_single * agent_iter + this->variable_ID[var_name];
                        alglib::sparseset(constraint_general, row_ID, col_ID, 1.);
                        std::cout << row_ID << "\t" << col_ID << "\n";

                        if(account_components[account_iter] == "cer"){
                            var_name = "conv";
                            col_ID = start_col_ID + variable_num_single * agent_iter + this->variable_ID[var_name];
                            alglib::sparseset(constraint_general, row_ID, col_ID, -1.);
                            std::cout << row_ID << "\t" << col_ID << "\n";
                        }

                        var_name = "res_" + account_components[account_iter];
                        col_ID = start_col_ID + variable_num_single * agent_iter + this->variable_ID[var_name];
                        alglib::sparseset(constraint_general, row_ID, col_ID, -1.);
                        std::cout << row_ID << "\t" << col_ID << "\n";

                        var_name = "bess_dc_" + account_components[account_iter];
                        col_ID = start_col_ID + variable_num_single * agent_iter + this->variable_ID[var_name];
                        alglib::sparseset(constraint_general, row_ID, col_ID, -1.);
                        std::cout << row_ID << "\t" << col_ID << "\n";
                    }

                    // Physical components of self, lem, rer, cer of demand
                    for(unsigned int account_iter = 0; account_iter < account_components.size(); ++ account_iter){
                        unsigned int row_ID = start_row_ID_temp + 13 + account_iter;
                        unsigned int col_ID;
                        std::string var_name;

                        var_name = "demand_" + account_components[account_iter];
                        col_ID = start_col_ID + variable_num_single * agent_iter + this->variable_ID[var_name];
                        alglib::sparseset(constraint_general, row_ID, col_ID, 1.);
                        std::cout << row_ID << "\t" << col_ID << "\n";

                        var_name = "default_demand_" + account_components[account_iter];
                        col_ID = start_col_ID + variable_num_single * agent_iter + this->variable_ID[var_name];
                        alglib::sparseset(constraint_general, row_ID, col_ID, -1.);
                        std::cout << row_ID << "\t" << col_ID << "\n";

                        var_name = "bess_ch_" + account_components[account_iter];
                        col_ID = start_col_ID + variable_num_single * agent_iter + this->variable_ID[var_name];
                        alglib::sparseset(constraint_general, row_ID, col_ID, -1.);
                        std::cout << row_ID << "\t" << col_ID << "\n";
                    }

                    // Dynamics of accounting components of soc
                    for(unsigned int account_iter = 0; account_iter < account_components.size(); ++ account_iter){
                        unsigned int row_ID = start_row_ID_temp + 17 + account_iter;
                        unsigned int col_ID;
                        std::string var_name;
                        auto parameter = market.participants[agent_iter].get_parameters();
                        auto bess_param = std::get <std::map <std::string, std::variant <double, double_df>>> (parameter["bess"]);
                        auto efficiency = std::get <double> (bess_param["efficiency"]);

                        if(tick > 0){
                            var_name = "soc_" + account_components[account_iter];
                            col_ID = start_col_ID + variable_num_single * agent_iter + this->variable_ID[var_name];
                            col_ID -= variable_num_single;
                            alglib::sparseset(constraint_general, row_ID, col_ID, 1.);
                            std::cout << row_ID << "\t" << col_ID << "\n";
                        }

                        var_name = "bess_ch_" + account_components[account_iter];
                        col_ID = start_col_ID + variable_num_single * agent_iter + this->variable_ID[var_name];
                        alglib::sparseset(constraint_general, row_ID, col_ID, efficiency);
                        std::cout << row_ID << "\t" << col_ID << "\n";

                        var_name = "bess_dc_" + account_components[account_iter];
                        col_ID = start_col_ID + variable_num_single * agent_iter + this->variable_ID[var_name];
                        alglib::sparseset(constraint_general, row_ID, col_ID, -1. / efficiency);
                        std::cout << row_ID << "\t" << col_ID << "\n";

                        var_name = "soc_" + account_components[account_iter];
                        col_ID = start_col_ID + variable_num_single * agent_iter + this->variable_ID[var_name];
                        alglib::sparseset(constraint_general, row_ID, col_ID, -1.);
                        std::cout << row_ID << "\t" << col_ID << "\n";
                    }
                }
            }

            // -------------------------------------------------------------------------------
            // Set boundary for equality constraints
            // -------------------------------------------------------------------------------
            std::vector <double> bound_general(constraint_num);
            for(ptrdiff_t constr_iter = 0; constr_iter < constraint_num; ++ constr_iter){
                bound_general[constr_iter] = 0.;
            }
            alglib::real_1d_array lb_general;
            alglib::real_1d_array ub_general;
            lb_general.setcontent(bound_general.size(), bound_general.data());
            ub_general.setcontent(bound_general.size(), bound_general.data());

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
            // Set boundary for box constraints
            // -------------------------------------------------------------------------------
            std::vector <std::vector <double>> bound_box(2);
            bound_box[0] = std::vector <double> (variable_num);
            bound_box[1] = std::vector <double> (variable_num);
            for(ptrdiff_t var_iter = 0; var_iter < variable_num; ++ var_iter){
                bound_box[0][var_iter] = 0.;
                bound_box[1][var_iter] = 0.;
            }
            alglib::real_1d_array lb_box;
            alglib::real_1d_array ub_box;
            lb_general.setcontent(bound_box[0].size(), bound_box[0].data());
            ub_general.setcontent(bound_box[1].size(), bound_box[1].data());

            // -------------------------------------------------------------------------------
            // Set objective coefficients of variables
            // -------------------------------------------------------------------------------

    }
};
