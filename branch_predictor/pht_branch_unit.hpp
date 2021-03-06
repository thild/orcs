#include <tuple>
#include <cmath>

using namespace std;

class pht_branch_unit_t : public branch_unit_t {
    private:
    
        uint32_t calculate_pht_index(uint64_t opcode_address) {
            auto indexer = opcode_address;
            if(use_gbh)  {
                indexer = gbh;
            }
            else if (gshare)
            {
                indexer ^= gbh;
            }
            return indexer & ((1 << pht_depth) - 1);
        }

    public:

        uint8_t pht_depth = 0;
        uint8_t pht_history_width = 0;

        void next_fetch_address(uint64_t opcode_address) override {
            auto btb_result = calculate_btb_result(opcode_address);
            
            auto prediction = false;
            if (this->branch_predictor != NULL) {
                auto index = calculate_pht_index(opcode_address);
                prediction = this->branch_predictor->predict(index, gbh);
            }
            
            this->fetch_result = make_tuple(opcode_address, get<0>(btb_result), get<1>(btb_result), prediction);
        }

        void update (uint64_t opcode_address_taken) override {
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
                this->branch_predictor->update(index, gbh, taken);
                if(direction_hit) this->direction_hits++;
            }
        }

        pht_branch_unit_t(uint8_t btb_depth, 
                          uint8_t pht_depth, 
                          uint8_t pht_history_width, 
                          bool use_gbh, 
                          bool gshare, 
                          uint8_t gbh_width, 
                          branch_predictor_t *branch_predictor) : branch_unit_t(btb_depth, use_gbh, gbh_width, gshare, branch_predictor) {
            if (use_gbh)
            {
                this->pht_depth = gbh_width;
            }
            this->pht_depth = pht_depth;
            this->pht_history_width = pht_history_width;
        }

        ~pht_branch_unit_t() {
        }
};