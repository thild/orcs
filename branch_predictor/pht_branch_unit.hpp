#include <tuple>
#include <cmath>

using namespace std;

class branch_unit_t {
    private:
    
        branch_predictor_t *branch_predictor;
        btb_set_t *btb_set;

        void update_gbh(bool taken) {
            this->gbh = ((this->gbh << 1) | (int)taken) & ((1 << gbh_width) - 1);
        }

        uint32_t calculate_btb_index(uint64_t opcode_address) {
            return opcode_address & ((1 << btb_depth) - 1);
        }

        uint32_t calculate_pht_index(uint64_t opcode_address) {
            auto indexer = opcode_address;
            if(use_gbh)  {
                indexer = gbh;
            }
            else if (xor_gbh)
            {
                indexer ^= gbh;
            }
            return indexer & ((1 << pht_depth) - 1);
        }

    public:
        uint64_t misses = 0;
        uint64_t btb_hits = 0;
        uint64_t direction_hits = 0;
        uint32_t gbh = 0;
        uint8_t gbh_width = 0;

        bool use_gbh = false;
        bool xor_gbh = false;

        uint8_t btb_depth = 0;

        uint8_t pht_depth = 0;
        uint8_t pht_history_width = 0;
        uint8_t *pht = NULL;    



        tuple<uint64_t, uint64_t, bool, bool> next_fetch_address(uint64_t opcode_address) {
		 	uint32_t index = calculate_btb_index(opcode_address);
            auto target = this->btb_set->search(index, opcode_address);
            auto btb_hit = target != 0;
            if (btb_hit) this->btb_hits++;
            
            auto prediction = false;
            if (this->branch_predictor != NULL) {
                index = calculate_pht_index(opcode_address);
                prediction = this->branch_predictor->predict(index);
            }
            
            return make_tuple(opcode_address, target, btb_hit, prediction);
        }

        void update (tuple<uint64_t /* opcode_address */, 
                           uint64_t /* target */, 
                           bool /* btb_hit */, 
                           bool /* prediction */> fetch_result, 
                           uint64_t opcode_address_taken) {
            auto opcode_address = get<0>(fetch_result);
            auto target_address = get<1>(fetch_result);
            auto direction_prediction = get<3>(fetch_result);
            auto index = calculate_btb_index(opcode_address);
            this->btb_set->update(index, opcode_address, opcode_address_taken);
            
            if (this->branch_predictor != NULL) {
                auto taken = target_address == opcode_address_taken;
                auto direction_hit = direction_prediction == taken;
    		 	index = calculate_pht_index(opcode_address);
                update_gbh(taken);
                this->branch_predictor->update(index, taken);
                if(direction_hit) this->direction_hits++;
            }
        }

        branch_unit_t(uint8_t btb_depth, uint8_t pht_depth, uint8_t pht_history_width, bool use_gbh, bool xor_gbh, uint8_t gbh_width, branch_predictor_t *branch_predictor) {
            this->btb_depth = btb_depth;
            this->pht_depth = pht_depth;
            this->gbh_width = gbh_width;
            this->btb_set = new btb_set_t(btb_depth);
            this->use_gbh = use_gbh;
            this->xor_gbh = xor_gbh;
            this->btb_set->allocate();
            if(branch_predictor != NULL) {
                this->branch_predictor = branch_predictor;
                this->branch_predictor->allocate(pht_depth, pht_history_width);
            }
        }

        ~branch_unit_t() {
            if (this->branch_predictor) delete branch_predictor;
        }
};