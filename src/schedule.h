// Market clearing and scheduling
#pragma once

// Included external codes
#include <alglib/optimization.h>

// Project specific
#include "src/market.h"

class scheduler_class{
    private:
        alglib::minlpstate Problem;

    public:
        void LP_set(market_class &market){
            // Set equality constraints

            // Set box constraints
        }
};
