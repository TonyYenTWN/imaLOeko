// STL
#include <iostream>

// Project specific
#include "src/db.h"
#include "src/market.h"
#include "src/schedule.h"
#include "src/ts.h"

int main(int argc, char* argv[]){
    // Optimization Test
    market_class market;
    market.test();
    scheduler_class scheduler;
    scheduler.LP_set(market);
}
