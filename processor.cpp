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
	this->btb_only = new branch_unit_t(10, 0, 0, false, false, 8, NULL);
	this->smith_predictor_1 = new branch_unit_t(10, 8, 1, false, false, 8, new smith_predictor());
	this->smith_predictor_gbh_1 = new branch_unit_t(10, 8, 1, true, false, 8, new smith_predictor());
	this->smith_predictor_gbh_xor_1 = new branch_unit_t(10, 10, 2, false, true, 10, new smith_predictor());
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
			auto r = this->btb_only_result;
			this->btb_only->update(r, new_instruction.opcode_address);
			r = this->smith_predictor_1_result;
			this->smith_predictor_1->update(r, new_instruction.opcode_address);
			r = this->smith_predictor_gbh_1_result;
			this->smith_predictor_gbh_1->update(r, new_instruction.opcode_address);
			r = this->smith_predictor_gbh_xor_1_result;
			this->smith_predictor_gbh_xor_1->update(r, new_instruction.opcode_address);
			// ORCS_PRINTF("target: %ld\n", get<1>(r));
			// ORCS_PRINTF("taken : %ld\n", new_instruction.opcode_address);
			// ORCS_PRINTF("dhits : %ld\n", this->smith_predictor_1->direction_hits);
			this->branching = false;
		}
		if(new_instruction.opcode_operation == instruction_operation_t::INSTRUCTION_OPERATION_BRANCH) {
			this->num_branches++;
			this->branching = true;
			this->btb_only_result = this->btb_only->next_fetch_address(new_instruction.opcode_address);
			this->smith_predictor_1_result = this->smith_predictor_1->next_fetch_address(new_instruction.opcode_address);
			this->smith_predictor_gbh_1_result = this->smith_predictor_gbh_1->next_fetch_address(new_instruction.opcode_address);
			this->smith_predictor_gbh_xor_1_result = this->smith_predictor_gbh_xor_1->next_fetch_address(new_instruction.opcode_address);
			// ORCS_PRINTF("fetch : %ld\n", new_instruction.opcode_address);
		}
	}

};

// =====================================================================
void processor_t::statistics() {
	ORCS_PRINTF("######################################################\n");
	ORCS_PRINTF("processor_t\n");
	ORCS_PRINTF("\n\n");
	ORCS_PRINTF("------------------------------------------------------\n");
	ORCS_PRINTF("btb_only\n");
	ORCS_PRINTF("------------------------------------------------------\n");
	this->print(this->btb_only, orcs_engine.trace_reader->fetch_instructions, this->num_branches);
	ORCS_PRINTF("\n");
	ORCS_PRINTF("------------------------------------------------------\n");
	ORCS_PRINTF("smith_predictor_1\n");
	ORCS_PRINTF("------------------------------------------------------\n");
	this->print(this->smith_predictor_1, orcs_engine.trace_reader->fetch_instructions, this->num_branches);
	ORCS_PRINTF("\n");
	ORCS_PRINTF("------------------------------------------------------\n");
	ORCS_PRINTF("smith_predictor_gbh_1\n");
	ORCS_PRINTF("------------------------------------------------------\n");
	this->print(this->smith_predictor_gbh_1, orcs_engine.trace_reader->fetch_instructions, this->num_branches);
	ORCS_PRINTF("\n");
	ORCS_PRINTF("------------------------------------------------------\n");
	ORCS_PRINTF("smith_predictor_gbh_xor_1\n");
	ORCS_PRINTF("------------------------------------------------------\n");
	this->print(this->smith_predictor_gbh_xor_1, orcs_engine.trace_reader->fetch_instructions, this->num_branches);
};

void processor_t::print(branch_unit_t *branch_unit, uint64_t fetch_instructions, uint64_t num_branches) {
	ORCS_PRINTF("fetchs: %ld\n", fetch_instructions);
	ORCS_PRINTF("branchs: %ld\n", num_branches);
	ORCS_PRINTF("btb hits: %ld\n", branch_unit->btb_hits);
	ORCS_PRINTF("direction hits: %ld\n", branch_unit->direction_hits);
	ORCS_PRINTF("btb hitrate: %lf\n", ((double)branch_unit->btb_hits / num_branches) * 100);
	ORCS_PRINTF("direction hitrate: %lf\n", ((double)branch_unit->direction_hits / num_branches) * 100);
}
