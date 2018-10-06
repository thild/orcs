// ============================================================================
// ============================================================================
class processor_t {
    private:    
		void print(branch_unit_t *branch_unit, uint64_t fetch_instructions, uint64_t num_branches);
	
    
    public:
        btb_set_t *btb_set;
		uint64_t btb_result;
		uint64_t num_branches = 0;
		branch_unit_t *smith_predictor_1;
		branch_unit_t *smith_predictor_gbh_xor_1;
		branch_unit_t *smith_predictor_gbh_1;
		branch_unit_t *btb_only;
		tuple<uint64_t, uint64_t, bool, bool> smith_predictor_1_result;
		tuple<uint64_t, uint64_t, bool, bool> smith_predictor_gbh_xor_1_result;
		tuple<uint64_t, uint64_t, bool, bool> smith_predictor_gbh_1_result;
		tuple<uint64_t, uint64_t, bool, bool> btb_only_result;
		bool branching;

		// ====================================================================
		/// Methods
		// ====================================================================
		processor_t();
	    void allocate();
	    void clock();
	    void statistics();
};
