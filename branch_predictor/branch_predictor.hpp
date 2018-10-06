class branch_predictor_t { 
    protected:
        uint8_t *pht = NULL;    
        uint32_t half_history_width = 0;
        uint32_t int_history_width = 0;

    public: 
        uint8_t depth = 0;
        uint32_t history_width = 0;
        virtual bool predict(uint32_t index) = 0;
        virtual void update(uint32_t index, bool taken) = 0;

        void allocate(uint8_t depth, uint32_t history_width) {
            this->depth = depth;        
            this->history_width = history_width;        
            this->int_history_width = (uint16_t)pow (2, history_width);
            this->half_history_width =  int_history_width / 2;
            auto numpht = (uint16_t)pow (2, depth);
            this->pht = new uint8_t[numpht];
        }

        branch_predictor_t() {
        }

        virtual ~branch_predictor_t() {
            if (this->pht) delete [] pht;
        }        
}; 


