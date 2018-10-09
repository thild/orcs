#include <stdint.h>

class pht_predictor_t: public branch_predictor_t { 
    private: 

    protected:
        uint32_t half_history_width = 0;
        uint32_t int_history_width = 0;
        uint8_t *pht = NULL;    

    public: 

        uint8_t depth = 0;
        uint32_t history_width = 0;

        pht_predictor_t(uint8_t depth, uint32_t history_width) : branch_predictor_t() {
            auto numpht = (uint16_t)pow (2, depth);
            this->pht = new uint8_t[numpht];
            this->depth = depth;        
            this->history_width = history_width;        
            this->int_history_width = (uint16_t)pow (2, history_width);
            this->half_history_width =  int_history_width / 2;
        }

        virtual ~pht_predictor_t() {
            if (this->pht) delete [] pht;
        }        
}; 
