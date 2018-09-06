#include <tuple>

using namespace std;

class way_t {
    public:
		uint64_t target_address;
        uint64_t tag;
		uint64_t lru_index;
		uint64_t access_counter;		

		
		// ====================================================================
		/// Methods
		// ====================================================================
		way_t() {
        };

        ~way_t() {
        };
};

class btb_line_t {
    private:    
    
    
    public:
		way_t ways[4];

		// ====================================================================
		/// Methods
		// ====================================================================

		tuple<bool, uint64_t> cache (uint64_t opcode_address) {
			auto result = make_tuple(false, -1);
			size_t lru_index = -1;
			for(size_t i = 0; i < 3; i++)
			{
				auto way = ways[i];
				if (way.tag == opcode_address) {
					result = make_tuple(true, way.target_address);
					lru_index = i;
					break;
				} 
			}
			update_lru (lru_index);
			return result;
		}

		void update_lru (size_t lru_index) {
			// -1 for miss
			for(size_t i = 0; i < 3; i++)
			{
				auto way = ways[i];
				if (i == lru_index) {
					way.lru_index = 0;
				}
				else {
					way.lru_index++;
				}
			}
		}

		btb_line_t() {
        };

        ~btb_line_t() {
            //if (this->ways) delete [] ways;
        };
};
