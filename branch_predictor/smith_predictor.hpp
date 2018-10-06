#include <stdint.h>

class smith_predictor: public branch_predictor_t { 
    private: 

    public: 
     
        bool predict(uint32_t index) override {
            // auto history = this->pht[index];
            // auto ret = (history & ((1 << (this->history_width - 1)))) > 0;
            // return ret;
            auto ret = this->pht[index] >= this->half_history_width;
            return ret;
        }

        void update(uint32_t index, bool taken) override {
            if(taken) {
                if(this->pht[index] != this->int_history_width - 1)
                    this->pht[index]++;
            }
            else {
                if(this->pht[index] != 0)
                    this->pht[index]--;
            }
            // auto history = this->pht[index];
            // this->pht[index] = ((history << 1) | (int)taken) & ((1 << this->history_width) - 1);
        } 

        smith_predictor() : branch_predictor_t() {
        }

        ~smith_predictor() {
        }
}; 