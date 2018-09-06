#include "simulator.hpp"

// =====================================================================
branch_info_t::branch_info_t() {
    this->branch_type = BRANCH_UNCOND;
    this->cycle = 0;
    this->miss = false;
};


