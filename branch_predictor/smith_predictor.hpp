#include <stdint.h>

class smith_predictor: public branch_predictor_t { 
    private: 

    public: 
     
        uint8_t depth = 0;
        uint8_t width = 0;
        uint8_t *lines = NULL;

        bool is_taken(uint64_t opcode_address) override {
			uint32_t index = opcode_address & 0x2FF;
            auto taken = (lines[index] & ((1 << (this->width - 1)))) > 0;
            return taken;
        }

        void update(uint64_t opcode_address, bool taken) override {
            uint32_t index = opcode_address & 0x2FF;
            auto rotation = ((2 << 1) | (int)taken) & ((1 << this->width) - 1);
            lines[index] = rotation;
        } 


        smith_predictor(uint8_t depth, uint8_t width) {
            this->depth = depth;        
            this->width = width;        
            auto numlines = (uint16_t)pow (2, depth);
            this->lines = new uint8_t[numlines];
        }

        ~smith_predictor() {
            if (this->lines) delete [] lines;
        }
}; 