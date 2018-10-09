#include <stdint.h>

class smith_predictor_t: public pht_predictor_t { 
    private: 
    
    protected:
        bool predict(uint32_t index, uint32_t gbh) override {
            (void)gbh;
            auto ret = this->pht[index] >= this->half_history_width;
            return ret;
        }

        void update(uint32_t index, uint32_t gbh, bool taken) override {
            (void)gbh;
            if(taken) {
                if(this->pht[index] != this->int_history_width - 1)
                    this->pht[index]++;
            }
            else {
                if(this->pht[index] != 0)
                    this->pht[index]--;
            }
        } 

    public: 

        uint8_t depth = 0;
        uint32_t history_width = 0;


        smith_predictor_t(uint8_t depth, uint32_t history_width) : pht_predictor_t(depth, history_width) {
        }

        virtual ~smith_predictor_t() {
        }        
}; 
