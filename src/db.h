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
};
