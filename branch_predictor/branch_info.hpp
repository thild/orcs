// ============================================================================
// ============================================================================
class branch_info_t {
    public:
        /// TRACE Variables
        uint64_t cycle;
        bool miss;
        branch_t branch_type;

        // ====================================================================
        /// Methods
        // ====================================================================
        branch_info_t();
};