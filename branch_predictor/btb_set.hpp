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
        tuple<bool, uint64_t> search (uint64_t opcode_address) {
			uint32_t index = opcode_address & 0x2FF;
			return lines[index].cache(opcode_address);
        }

        void allocate() {
            this->lines = new btb_line_t[(uint16_t)pow (depth, 2)];
        };

        btb_set_t(uint8_t depth) {
            this->depth = depth;        
        };

        ~btb_set_t() {
            if (this->lines) delete [] lines;
        };
};