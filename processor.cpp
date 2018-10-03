#include "simulator.hpp"
#include <tuple>

using namespace std;

// =====================================================================
processor_t::processor_t() {
};

// =====================================================================
void processor_t::allocate() {
	this->btb_set = new btb_set_t(10);
	this->btb_set->allocate();
	this->smith_predictor_1 = new branch_unit_t(10, new smith_predictor(10, 1));
	this->smith_predictor_2 = new branch_unit_t(10, new smith_predictor(10, 2));
};

// miss latency
//	- cond 1-8 cycles rand 
//	- incond 4 cycles

// GBHR (Global Branch History Register)
// 	- 8 bits
//	- iniciado com zeros
//	 PHT (Pattern History Table) 1 bit predictor

// =====================================================================
void processor_t::clock() {
	/// Get the next instruction from the trace
	opcode_package_t new_instruction;
	if (!orcs_engine.trace_reader->trace_fetch(&new_instruction)) {
		/// If EOF
		orcs_engine.simulator_alive = false;
	}
	else {
		if(new_instruction.opcode_operation == instruction_operation_t::INSTRUCTION_OPERATION_BRANCH) {
			this->branching = true;
			this->next_fetch_address = this->smith_predictor_1->next_fetch_address(new_instruction.opcode_address);
			this->previous_fetch_address = new_instruction.opcode_address;
			return;
		}
		if (this->branching) {
			if(this->next_fetch_address == 0 || this->next_fetch_address == new_instruction.opcode_address) {
				// printf("hit %lx\n", new_instruction.opcode_address);
				this->smith_predictor_1->update(this->previous_fetch_address, new_instruction.opcode_address, true);
				// printf("hit %lx\n", this->next_fetch_address);
			}
			else {
				// miss
				this->smith_predictor_1->update(this->previous_fetch_address, new_instruction.opcode_address, false);
				// printf("miss %lx - %lx\n", this->previous_fetch_address, new_instruction.opcode_address);
			}
			this->branching = false;
		}
	}

};

// =====================================================================
void processor_t::statistics() {
	ORCS_PRINTF("######################################################\n");
	ORCS_PRINTF("processor_t\n");
	ORCS_PRINTF("this->misses: %ld\n", this->misses);
	ORCS_PRINTF("misses: %ld\n", this->smith_predictor_1->misses);
	ORCS_PRINTF("fetchs: %ld\n", orcs_engine.trace_reader->fetch_instructions);
	ORCS_PRINTF("rate: %lf\n", (1 - (double)this->smith_predictor_1->misses / orcs_engine.trace_reader->fetch_instructions) * 100);
};
