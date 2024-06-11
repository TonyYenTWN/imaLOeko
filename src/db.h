// database operations
#pragma once

// STL
#include <vector>

// Extension library
#include <pqxx/pqxx>

// Project specific
#include "src/rw.h"

// general db class
class data_base_class {
    public:
        void create_ts_table(pqxx::work trade, std::string table_name, std::vector <std::string> col_name){

        }
};

class data_base_tester_class: public data_base_class{
    private:
        void create_table_test(pqxx::work &trade){
            reader_class reader;

            // Create sql statement
            std::string filename = "input/create_table_test.sql";
            std::string sql_string = reader.read_one_line(filename);
            const char* sql_char = sql_string.c_str();

            // Execute sql query
            trade.exec(sql_char);
        }

        void drop_table_test(pqxx::work &trade){
            reader_class reader;

            // Create sql statement
            std::string filename = "input/drop_table_test.sql";
            std::string sql_string = reader.read_one_line(filename);
            const char* sql_char = sql_string.c_str();

            // Execute sql query
            trade.exec(sql_char);
        }

    public:
        void initialize_table_test(pqxx::work &trade, bool refresh = false){
            // Drop table if refresh is true
            if(refresh){
                this->drop_table_test(trade);
            }

            this->create_table_test(trade);
        }

        bool test(){
            data_base_tester_class data_base_tester;
            reader_class reader;

            try{
                std::string db_par = reader.read_one_line(".env");
                pqxx::connection conn(db_par);

                // Check if database can be connected
                if (conn.is_open()) {
                    std::cout << "Opened database successfully: " << conn.dbname() << std::endl;
                }
                else {
                    std::cout << "Can't open database" << std::endl;
                    return 1;
                }

                // Create a transactional object
                pqxx::work trade(conn);

                data_base_tester.initialize_table_test(trade, true);

                // Commit the sql commands
                trade.commit();
                std::cout << "Table created successfully" << std::endl;

                // Close the connection
                conn.close();
            }
            catch (const std::exception &e) {
                std::cerr << e.what() << std::endl;
                return 1;
            }

            return 0;
        }
};
