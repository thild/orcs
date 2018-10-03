#include <tuple>

using namespace std;

class way_t {
    public:
		uint64_t target_address = 0;
        uint64_t tag = 0;
		uint64_t lru_index = 0;
		uint64_t access_counter = 0;		

		
		// ====================================================================
		/// Methods
		// ====================================================================
		way_t() {
			this->target_address = 0;
        	this->tag = 0;
			this->lru_index = 0;
			this->access_counter = 0;			
        }

        ~way_t() {
        }
};

class btb_line_t {
    private:    
    
    
    public:
		way_t *ways;

		// ====================================================================
		/// Methods
		// ====================================================================

		uint64_t search (uint64_t opcode_address) {
			auto result = 0;
			//int lru_index = -1;
			for(size_t i = 0; i < 3; i++)
			{
				auto way = this->ways[i];
				if (way.tag == opcode_address) {
					result = way.target_address;
					return result;
					//lru_index = i;
				} 
			}
			//update_lru (lru_index);
			return result;
		}

		void update (uint64_t opcode_address, uint64_t target_address) {
			this->ways[0].tag = opcode_address;
			this->ways[0].target_address = target_address;
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
            this->ways = new way_t[4];
		}

        ~btb_line_t() {
            if (this->ways) delete [] ways;
        }
};
