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
			for(size_t i = 0; i <= 3; i++)
			{
				auto way = &this->ways[i];
				if (way->tag == opcode_address) {
					update_lru(i);
					return way->target_address;
				} 
			}
			return 0;
		}

		void update (uint64_t opcode_address, uint64_t target_address) {
			
			for(size_t i = 0; i <= 3; i++)
			{
				auto way = &this->ways[i];
				if (way->tag == opcode_address)
				{
					way->target_address = target_address;
					update_lru (i);
					return;
				}
			}

			for(size_t i = 0; i <= 3; i++)
			{
				auto way = &this->ways[i];
				if (way->lru_index == 0)
				{
					way->tag = opcode_address;
					way->target_address = target_address;
					update_lru (i);
					return;
				}
			}
		}

		void update_lru (size_t lru_index) {
			if (this->ways[lru_index].lru_index == 3) {
				return;
			}
			auto old = this->ways[lru_index].lru_index;
			for(size_t i = 0; i <= 3; i++)
			{
				if(this->ways[i].lru_index > old) {
					this->ways[i].lru_index--;
				}
			}
			this->ways[lru_index].lru_index = 3;
		}

		btb_line_t() {
            this->ways = new way_t[4];
			this->ways[0].lru_index = 0;
			this->ways[1].lru_index = 1;
			this->ways[2].lru_index = 2;
			this->ways[3].lru_index = 3;
		}

        ~btb_line_t() {
            if (this->ways) delete [] ways;
        }
};
