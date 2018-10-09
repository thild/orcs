#include <tuple>
#include <cmath>

using namespace std;

class branch_unit_t {
    protected:
    
        branch_predictor_t *branch_predictor;
        btb_set_t *btb_set;

        void update_gbh(bool taken) {
            this->gbh = ((this->gbh << 1) | (int)taken) & ((1 << gbh_width) - 1);
        }

        uint32_t calculate_btb_index(uint64_t opcode_address) {
            return opcode_address & ((1 << btb_depth) - 1);
        }

        tuple<uint64_t, bool> calculate_btb_result(uint64_t opcode_address) {
		 	uint32_t index = calculate_btb_index(opcode_address);
            auto target = this->btb_set->search(index, opcode_address);
            auto btb_hit = target != 0;
            if (btb_hit) this->btb_hits++;
            return make_tuple(target, btb_hit);
        }

    public:
        uint8_t gbh_width = 0;
        bool use_gbh = false;
        bool gshare = false;
        uint8_t btb_depth = 0;

        uint64_t btb_hits = 0;
        uint64_t misses = 0;
        uint64_t direction_hits = 0;

        uint32_t gbh = 0;

        tuple<uint64_t /* opcode_address */, 
              uint64_t /* target */, 
              bool /* btb_hit */, 
              bool /* prediction */> fetch_result;
        string name;
        string description;

        virtual void next_fetch_address(uint64_t opcode_address) = 0;

        virtual void update (uint64_t opcode_address_taken) = 0;

        branch_unit_t(uint8_t btb_depth, bool use_gbh, uint8_t gbh_width, bool gshare, branch_predictor_t *branch_predictor) {
            this->btb_depth = btb_depth;
            this->gbh_width = gbh_width;
            this->btb_set = new btb_set_t(btb_depth);
            this->use_gbh = use_gbh;
            this->gshare = gshare;
            this->btb_set->allocate();
            this->branch_predictor = branch_predictor;
        }

        ~branch_unit_t() {
        }
};