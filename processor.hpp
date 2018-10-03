// ============================================================================
// ============================================================================
class processor_t {
    private:    
    
    
    public:
        btb_set_t *btb_set;
		uint64_t btb_result;
		uint64_t next_fetch_address;
		uint64_t previous_fetch_address;
		uint64_t misses;
		branch_unit_t *smith_predictor_1;
		branch_unit_t *smith_predictor_2;
		bool branching;

		// ====================================================================
		/// Methods
		// ====================================================================
		processor_t();
	    void allocate();
	    void clock();
	    void statistics();
};
