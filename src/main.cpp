// STL
#include <iostream>

// Project specific
#include "src/db.h"
#include "src/market.h"
#include "src/optimizer.h"
#include "src/ts.h"

int main(int argc, char* argv[]){
    data_base_tester_class data_base_tester;
    reader_class reader;

    // Database test
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

    // Optimization Test
    market_class market;
    market.test();
}
