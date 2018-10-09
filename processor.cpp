#include <iostream> 
#include "simulator.hpp"
#include <tuple>

using namespace std;

// =====================================================================
processor_t::processor_t() {
};

// =====================================================================
void processor_t::allocate() {
	this->smith_predictor_1 = new pht_branch_unit_t(10, 8, 1, false, false, 8, new smith_predictor_t(8, 1));
	this->smith_predictor_gbh_1 = new pht_branch_unit_t(10, 8, 1, true, false, 8, new smith_predictor_t(8, 1));
	this->smith_predictor_gbh_xor_1 = new pht_branch_unit_t(10, 10, 2, false, true, 10, new smith_predictor_t(8, 2));
	
	// branch_unit_t* bu = new pht_branch_unit_t(10, 0, 0, false, false, 0, NULL);
	// bu->name = "BTB Only";
	// bu->description = "10 bits";
	// this->branch_units.push_back(bu);

	branch_unit_t* bu = new nbbp_branch_unit_t(10, false, 8, new nbbp_predictor_t(8, 8));
	bu->name = "NBBP";
	bu->description = "NBBP branch unit";
	this->branch_units.push_back(bu);

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
		if (this->branching) {
			for (auto bu = branch_units.begin(); bu != branch_units.end(); bu++) {
				(*bu)->update(new_instruction.opcode_address);
			}
			this->branching = false;
		}
		if(new_instruction.opcode_operation == instruction_operation_t::INSTRUCTION_OPERATION_BRANCH) {
			this->num_branches++;
			this->branching = true;
		    for (auto bu = branch_units.begin(); bu != branch_units.end(); bu++) {
				(*bu)->next_fetch_address(new_instruction.opcode_address);
			}
		}
	}

};

// =====================================================================
void processor_t::statistics() {
	print();
};

void processor_t::print() {
    for (auto it = branch_units.begin(); it != branch_units.end(); it++) {
		ORCS_PRINTF("------------------------------------------------------\n");
		cout << (*it)->name << "\n" << (*it)->description << "\n";
		ORCS_PRINTF("------------------------------------------------------\n");
		ORCS_PRINTF("fetchs: %ld\n", orcs_engine.trace_reader->fetch_instructions);
		ORCS_PRINTF("branchs: %ld\n", this->num_branches);
		ORCS_PRINTF("btb hits: %ld\n", (*it)->btb_hits);
		ORCS_PRINTF("direction hits: %ld\n", (*it)->direction_hits);
		ORCS_PRINTF("btb hitrate: %lf\n", ((double)(*it)->btb_hits / this->num_branches) * 100);
		ORCS_PRINTF("direction hitrate: %lf\n", ((double)(*it)->direction_hits / this->num_branches) * 100);
	}
}
