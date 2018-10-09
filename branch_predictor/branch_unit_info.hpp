// ============================================================================
// ============================================================================
class branch_unit_info_t {
    public:
        branch_unit_t *branch_unit;
        tuple<uint64_t, uint64_t, bool, bool> result;
        string name;
        string description;

        branch_unit_info_t(string name, string description, branch_unit_t *branch_unit) {
            this->name = name;
            this->description = description;
            this->branch_unit = branch_unit;
        };        
};
