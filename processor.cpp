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
			auto result = this->btb_set->search(new_instruction.opcode_address);
			if(get<0>(result) == true) {
				printf("yeye");
			}
		}
	}

};

// =====================================================================
void processor_t::statistics() {
	ORCS_PRINTF("######################################################\n");
	ORCS_PRINTF("processor_t\n");

};
