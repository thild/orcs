#include <tuple>
#include <cmath>

using namespace std;

class branch_unit_t {
    private:
    
        branch_predictor_t *branch_predictor;
        btb_set_t *btb_set;


    public:
        uint64_t misses = 0;

        uint64_t next_fetch_address(uint64_t opcode_address) {
            auto is_taken = this->branch_predictor->is_taken(opcode_address);
            auto target = this->btb_set->search(opcode_address);
            return is_taken && target != 0 ? target : 0;
        }

        void update (uint64_t opcode_address, uint64_t target_address, bool taken) {
			if (!taken) misses++;
            this->branch_predictor->update(opcode_address, taken);
            this->btb_set->update(opcode_address, target_address);
        }

        branch_unit_t(uint8_t btb_depth, branch_predictor_t *branch_predictor) {
            this->btb_set = new btb_set_t(btb_depth);
            this->btb_set->allocate();
            this->branch_predictor = branch_predictor;
        }

        ~branch_unit_t() {
        }
};