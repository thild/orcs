#include <tuple>
#include <cmath>

using namespace std;

class btb_set_t {
    public:
        btb_line_t *lines = NULL;
        uint8_t depth = 0;
        
        // ====================================================================
        /// Methods
        // ====================================================================
        // hit, target_address, opcode_address
        uint64_t search (uint32_t index, uint64_t opcode_address) {
            return lines[index].search(opcode_address); 
        }

        void update (uint32_t index, uint64_t opcode_address, uint64_t target_address) {
			lines[index].update(opcode_address, target_address);
        }

        void allocate() {
            auto numlines = (uint16_t)pow (2, this->depth);
            this->lines = new btb_line_t[numlines];
        }
        
        btb_set_t(uint8_t depth) {
            this->depth = depth;        
        }

        ~btb_set_t() {
            if (this->lines) delete [] lines;
        }
};