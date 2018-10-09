class branch_predictor_t { 
    protected:

    public: 
        virtual bool predict(uint32_t index, uint32_t gbh) = 0;
        virtual void update(uint32_t index, uint32_t gbh, bool taken) = 0;

        branch_predictor_t() {
        }

        virtual ~branch_predictor_t() {
        }        
}; 


