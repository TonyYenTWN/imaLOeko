// STL
#include <iostream>

// Project specific
#include "src/db.h"
#include "src/market.h"
#include "src/schedule.h"
#include "src/ts.h"

int main(int argc, char* argv[]){
    time_series_class time_series;
//    time_series.get_past_days(7);
    auto ts_test = time_series.create_ts_test();

    // Database Test
//    data_base_class data_base;
//    auto conn = data_base.conn_init();

    // Optimization Test
//    market_class market;
//    market.test();
//    scheduler_class scheduler;
//    scheduler.LP_set(market);
//    scheduler.LP_solve(market);
}
