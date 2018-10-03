class branch_predictor_t { 
    public: 
        virtual bool is_taken(uint64_t opcode_address) = 0;
        virtual void update(uint64_t opcode_address, bool taken) = 0;
}; 


