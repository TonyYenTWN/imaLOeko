// STL
#include <iostream>

// Project specific
#include "src/db.h"
#include "src/market.h"
#include "src/schedule.h"
#include "src/ts.h"

int main(int argc, char* argv[]){
    // Read-write test
//    reader_class reader;

    // Time series test
    time_series_class time_series;
    time_series_tester_class time_series_tester;
//    time_series.read_timestamp("2024-06-30 13:54:00");
    auto ts_test = time_series_tester.create_ts_test(2);
    time_series.temporal_integration(ts_test, 60);

    // Database test
//    data_base_class data_base;
//    auto conn = data_base.conn_init();

    // Optimization test
//    market_class market;
//    market.test();
//    scheduler_class scheduler;
//    scheduler.LP_set(market);
//    scheduler.LP_solve(market);
}
