#include <stdint.h>

class two_level_ghb_predictor_t: public branch_predictor_t { 
    private: 

    public: 

        bool predict(uint32_t index) override {
            // auto history = this->pht[index];
            // return (history & ((1 << (this->history_width - 1)))) > 0;
            auto ret = this->pht[index] >= this->half_history_width;
            return ret;
        }

        void update(uint32_t index, bool taken) override {
            // auto history = this->pht[index];
            // this->pht[index] = ((history << 1) | (int)taken) & ((1 << this->history_width) - 1);
            if(taken) {
                if(this->pht[index] != this->history_width - 1)
                    this->pht[index]++;
            }
            else {
                if(this->pht[index] != 0)
                    this->pht[index]--;
            }            
        } 

        two_level_ghb_predictor_t() : branch_predictor_t() {
        }
        
        ~two_level_ghb_predictor_t() {
        }
}; 