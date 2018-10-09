#include <vector> 
// ============================================================================
// ============================================================================
class processor_t {
    private:    
		void print();
	
    
    public:
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

		vector<branch_unit_t*> branch_units;
		// ====================================================================
		/// Methods
		// ====================================================================
		processor_t();
	    void allocate();
	    void clock();
	    void statistics();
};
