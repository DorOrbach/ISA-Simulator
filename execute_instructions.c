#define _CRT_SECURE_NO_WARNINGS
#include "execute_instructions.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cores.h"
#include "bus_and_memory.h"
#include "alu_instr.h"
#include "print_outpu.h"
#include "stalls.h"


int check_if_halt(Inst* fetch) {
	char op[3];
	op[0] = fetch->inst[0];
	op[1] = fetch->inst[1];
	op[2] = '\0';
	if (strcmp(op, "14") == 0) {
		return 1;
	}
	return 0;
}

//save the current inst from imem-to be decoded:
void fetch(Core *core) {
	//send the last decoded inst to be decoded:
	strcpy(core->pipline->old_fetch_decode->inst, core->pipline->fetch_decode->inst);
	core->pipline->old_fetch_decode->is_empty= core->pipline->fetch_decode->is_empty;
	core->pipline->old_fetch_decode->pc = core->pipline->fetch_decode->pc;
	//save the new fetched inst:
	strcpy(core->pipline->fetch_decode->inst, core->imem[core->pc]);
	core->pipline->fetch_decode->is_empty = 0;
	core->pipline->fetch_decode->pc = core->pc;

	//check if halt:
	if (check_if_halt(core->pipline->fetch_decode) == 1) {
		core->is_halt_fetched = 1;
	}
}

void update_pc_branch(Core* core, int rd) {
	if (rd != 1) {
		core->pc = convert_hex_to_int(core->registers[rd]);
	}
	else {
		core->pc = core->pipline->decode_exec->imm_val;
	}
}

int branch_resolutoin(Core *core) {
	//beq
	int branch_condition = 0, rs= core->pipline->decode_exec->rs_index;
	int rt = core->pipline->decode_exec->rt_index, rd= core->pipline->decode_exec->rd_index;
	if (strcmp(core->pipline->decode_exec->op, "09") == 0) {
		branch_condition = check_equal(core, rs, rt);
		if (branch_condition == 1) {
			update_pc_branch(core, rd);
			return BRANCH;
		}
	}
	//bne
	if (strcmp(core->pipline->decode_exec->op, "0A") == 0) {
		branch_condition = check_equal(core, rs, rt);
		if (branch_condition == 0) {
			update_pc_branch(core, rd);
			return BRANCH;
		}
	}
	//blt
	if (strcmp(core->pipline->decode_exec->op, "0B") == 0) {
		branch_condition = check_less_or_greater_then(core, rs, rt);
		if (branch_condition == 1) {
			update_pc_branch(core, rd);
			return BRANCH;
		}
	}
	//bgt
	if (strcmp(core->pipline->decode_exec->op, "0C") == 0) {
		branch_condition = check_less_or_greater_then(core, rs, rt);
		if (branch_condition == 0) {
			update_pc_branch(core, rd);
			return BRANCH;
		}
	}
	//ble
	if (strcmp(core->pipline->decode_exec->op, "0D") == 0) {
		branch_condition = check_less_or_greater_then(core, rs, rt);
		if (branch_condition == 1 || branch_condition == -1) {
			update_pc_branch(core, rd);
			return BRANCH;
		}
	}
	//bge
	if (strcmp(core->pipline->decode_exec->op, "0E") == 0) {
		branch_condition = check_less_or_greater_then(core, rs, rt);
		if (branch_condition == 0 || branch_condition == -1) {
			update_pc_branch(core, rd);
			return BRANCH;
		}
	}
	//jal
	if (strcmp(core->pipline->decode_exec->op, "0F") == 0) {

	}
	return NO_BRANCH;
}

void promote_pipeline_reg(Inst* old, Inst* new) {
	strcpy(old->imm, new->imm);
	old->imm_val = new->imm_val;
	strcpy(old->inst, new->inst);
	old->is_empty = new->is_empty;
	old->is_jump = new->is_jump;
	old->pc = new->pc;
	strcpy(old->op, new->op);
	strcpy(old->rd, new->rd);
	strcpy(old->rs, new->rs);
	strcpy(old->rt, new->rt);
	old->rd_index = new->rd_index;
	old->rs_index = new->rs_index;
	old->rt_index = new->rt_index;
}

//decode the inst saved in "old_fetch_decode" and save it to "decode_exec"
void decode(Core* core) {
	//check if there is an inst to decode or pipeline empty:
	if (core->pipline->old_fetch_decode->is_empty == 1) {
		core->pc++;
		return ;
	}

	//push the last decoded data, to execute stage:
	core->pc++;
	promote_pipeline_reg (core->pipline->old_decode_exec , core->pipline->decode_exec);
	//decode the current data:
	core->pipline->decode_exec->is_empty = core->pipline->old_fetch_decode->is_empty;
	// get inst pc:
	core->pipline->decode_exec->pc = core->pipline->old_fetch_decode->pc;
	//save the inst:
	strcpy(core->pipline->decode_exec->inst, core->pipline->old_fetch_decode->inst);
	//opcode:
	core->pipline->decode_exec->op[0] = core->pipline->old_fetch_decode->inst[0];
	core->pipline->decode_exec->op[1] = core->pipline->old_fetch_decode->inst[1];
	core->pipline->decode_exec->op[2] = '\0';
	//rd:
	core->pipline->decode_exec->rd[0] = core->pipline->old_fetch_decode->inst[2];
	core->pipline->decode_exec->rd[1] = '\0';
	core->pipline->decode_exec->rd_index = convert_hex_to_int(core->pipline->decode_exec->rd);
	//rs:
	core->pipline->decode_exec->rs[0] = core->pipline->old_fetch_decode->inst[3];
	core->pipline->decode_exec->rs[1] = '\0';
	core->pipline->decode_exec->rs_index = convert_hex_to_int(core->pipline->decode_exec->rs);
	//rt:
	core->pipline->decode_exec->rt[0] = core->pipline->old_fetch_decode->inst[4];
	core->pipline->decode_exec->rs[1] = '\0';
	core->pipline->decode_exec->rt_index = convert_hex_to_int(core->pipline->decode_exec->rt);
	//imm:
	core->pipline->decode_exec->imm[0] = core->pipline->old_fetch_decode->inst[5];
	core->pipline->decode_exec->imm[1] = core->pipline->old_fetch_decode->inst[6];
	core->pipline->decode_exec->imm[2] = core->pipline->old_fetch_decode->inst[7];
	core->pipline->decode_exec->imm[3] = '\0';
	core->pipline->decode_exec->imm_val = convert_hex_to_int(core->pipline->decode_exec->imm);
	//sign extension:
	sprintf(core->pipline->decode_exec->imm, "%08X", core->pipline->decode_exec->imm_val);

	//finished decoding - now branch resolution:
	int is_branch = branch_resolutoin(core);
	if (is_branch == 1) {
		core->pipline->decode_exec->is_jump = 1;
	}
	else {
		core->pipline->decode_exec->is_jump = 0;
	}
}


//takes the instruction from old_decode_exec and calc the alu result
//save the data to exec_mem
void execute(Core* core) {
	//check if pipeline empty:
	if (core->pipline->old_decode_exec->is_empty == 1) {
		return;
	}
	
//push the last executed command to exec_mem register:
promote_pipeline_reg(core->pipline->old_exec_mem, core->pipline->exec_mem);
promote_pipeline_reg(core->pipline->exec_mem, core->pipline->old_decode_exec);
//check if it is a stall bubble:
if (strcmp(core->pipline->old_decode_exec->op, "---") == 0) {
	return;
}
int rd = core->pipline->old_decode_exec->rd_index;
int rs = core->pipline->old_decode_exec->rs_index;
int rt = core->pipline->old_decode_exec->rt_index;
int imm_val = core->pipline->old_decode_exec->imm_val;
core->pipline->ALU_result_old = core->pipline->ALU_result_new;
if (strcmp(core->pipline->old_decode_exec->op, "00") == 0) {//add
	core->pipline->ALU_result_new = add(core, rs, rt);
}
if (strcmp(core->pipline->old_decode_exec->op, "01") == 0) {//sub
	core->pipline->ALU_result_new = sub(core, rs, rt);
}
if (strcmp(core->pipline->old_decode_exec->op, "02") == 0) {//and
	core->pipline->ALU_result_new = and (core, rs, rt);
}
if (strcmp(core->pipline->old_decode_exec->op, "03") == 0) {//or
	core->pipline->ALU_result_new = or (core, rs, rt);
}
if (strcmp(core->pipline->old_decode_exec->op, "04") == 0) {//xor
	core->pipline->ALU_result_new = xor (core, rs, rt);
}
if (strcmp(core->pipline->old_decode_exec->op, "05") == 0) {//mul
	core->pipline->ALU_result_new = mul(core, rs, rt);
}
if (strcmp(core->pipline->old_decode_exec->op, "06") == 0) {//sll
	core->pipline->ALU_result_new = sll(core, rs, rt);
}
if (strcmp(core->pipline->old_decode_exec->op, "07") == 0) {//sra
	core->pipline->ALU_result_new = sra(core, rs, rt);
}
if (strcmp(core->pipline->old_decode_exec->op, "08") == 0) {//srl

}
if (strcmp(core->pipline->old_decode_exec->op, "10") == 0) {//lw
	core->pipline->ALU_result_new = add(core, rs, rt);
}
if (strcmp(core->pipline->old_decode_exec->op, "11") == 0) {//sw
	core->pipline->ALU_result_new = add(core, rs, rt);
}

}

void memory(Core* core, char** main_mem, Bus* bus, MemCall** queue) {
	if (core->pipline->old_exec_mem->is_empty == 1) {
		return;
	}
	promote_pipeline_reg(core->pipline->old_mem_wb, core->pipline->mem_wb);
	promote_pipeline_reg(core->pipline->mem_wb, core->pipline->old_exec_mem);
	//check for stalls:
	if (strcmp(core->pipline->old_exec_mem->op, "---") == 0) {
		return;
	}
	if (strcmp(core->pipline->old_exec_mem->op, "10") == 0) {//lw
		memory_call_read(core, main_mem, bus, queue);
	}
	else if (strcmp(core->pipline->old_exec_mem->op, "11") == 0) {//sw
		memory_call_write(core, main_mem, bus, queue);
	}
}

int write_back(Core* core) {
	if (core->pipline->old_mem_wb->is_empty == 1 || core->pipline->old_mem_wb->is_jump == 1) {
		return 0;
	}
	if (strcmp(core->pipline->old_mem_wb->op, "---") == 0) {
		return 0;
	}
	if (strcmp(core->pipline->old_mem_wb->op, "14") == 0) {
		//promote_pipeline_reg(core->pipline->old_mem_wb, core->pipline->mem_wb);
		return HALT;
	}
	//sw does not require write back:
	if (strcmp(core->pipline->old_mem_wb->op, "11") == 0) {
		//promote_pipeline_reg(core->pipline->old_mem_wb, core->pipline->mem_wb);
		return;
	}
	//if lw then copy information from DSRAM(cache) to reg:
	if (strcmp(core->pipline->old_mem_wb->op, "10") == 0) {
		int cache_add = find_block_location_in_cache(core->pipline->ALU_result_old);
		int cache_offset = get_mem_add_offset(core->pipline->ALU_result_old);
		strcpy(core->registers[core->pipline->old_mem_wb->rd_index], core->DSRAM[cache_add]);

	}
	else {//any other arithmetic cammand:
		char math_result[MAX_LINE_IMEM];
		int res = add_for_rd(core, core->pipline->old_mem_wb->rs_index, core->pipline->old_mem_wb->rt_index);
		sprintf(math_result, "%08X", res);
		strcpy(core->registers[core->pipline->old_mem_wb->rd_index], math_result);
	}
	//promote_pipeline_reg(core->pipline->old_mem_wb, core->pipline->mem_wb);
	return 0;
}

int check_if_imem_empty(Core* core) {
	if (strcmp(core->imem[0], "00000000\n") == 0) {
		return 1;
	}
	return 0;
}
void simulate_clock_cycle(Core* core_0, Core* core_1, Core* core_2, Core* core_3, char** main_mem, Bus* bus, MemCall **queue) {
	//core_0
	int is_done = 0, is_stall_0 = 0, is_stall_1 = 0, is_stall_2 = 0, is_stall_3 = 0;
	int is_halt_0 = 0, is_halt_1 = 0, is_halt_2 = 0, is_halt_3 = 0;
	if (check_if_imem_empty(core_0) == 1) {
		is_halt_0 = HALT;
	}
	if (check_if_imem_empty(core_1) == 1) {
		is_halt_1 = HALT;
	}
	if (check_if_imem_empty(core_2) == 1) {
		is_halt_2 = HALT;
	}
	if (check_if_imem_empty(core_3) == 1) {
		is_halt_3 = HALT;
	}
	while (1) {
		//core_0
		if (is_halt_0 != HALT) {
			if (queue[0]->is_call == 0) {
				if (is_stall_0 == 0) {
					core_0->is_stall = 0;
					fetch(core_0);
					decode(core_0);
				}
				else {
					core_0->is_stall = 1;
					insert_stall_to_pipe(core_0->pipline->old_decode_exec);
				}
				//after decoding - check for stalls
				is_stall_0 = check_if_pipe_stall(core_0);
				execute(core_0);
				memory(core_0, main_mem, bus, queue);
				if (queue[0]->is_call == 0) {
					is_halt_0 = write_back(core_0);
				}
			}
			else {//need to wait for memory to be writen to cache from main memory
				is_done = transfer_block_from_main_mem(core_0, bus, queue, main_mem);
				if (is_done == 1) {
					is_halt_0 = write_back(core_0);
					promote_pipeline_reg(core_0->pipline->old_mem_wb, core_0->pipline->mem_wb);
				}
			}
			print_core_trace(core_0);
		}
		
		////core_1:
		if (is_halt_1 != HALT) {
			if (queue[1]->is_call == 0) {
				if (is_stall_1 == 0) {
					fetch(core_1);
					decode(core_1);
					
				}
				else {
					insert_stall_to_pipe(core_1->pipline->old_decode_exec);
				}
				//after decoding - check for stalls
				is_stall_1 = check_if_pipe_stall(core_1);
				execute(core_1);
				memory(core_1, main_mem, bus, queue);
				if (queue[1]->is_call == 0) {
					is_halt_1 = write_back(core_1);
				}
			}
			else {//need to wait for memory to be writen to cache from main memory
				is_done = transfer_block_from_main_mem(core_1, bus, queue, main_mem);
				if (is_done == 1) {
					promote_pipeline_reg(core_1->pipline->old_mem_wb, core_1->pipline->mem_wb);
					is_halt_1 = write_back(core_1);
				}
			}
			print_core_trace(core_1);
		}
		//core2
		if (is_halt_2 != HALT) {
			if (queue[2]->is_call == 0) {
				if (is_stall_2 == 0) {
					fetch(core_2);
					decode(core_2);
				}
				else {
					
					insert_stall_to_pipe(core_2->pipline->old_decode_exec);
				}
				//after decoding - check for stalls
				is_stall_2 = check_if_pipe_stall(core_2);
				execute(core_2);
				memory(core_2, main_mem, bus, queue);
				if (queue[2]->is_call == 0) {
					is_halt_2 = write_back(core_2);
				}
			}
			else {//need to wait for memory to be writen to cache from main memory
				is_done = transfer_block_from_main_mem(core_2, bus, queue, main_mem);
				if (is_done == 1) {
					promote_pipeline_reg(core_2->pipline->old_mem_wb, core_2->pipline->mem_wb);
					is_halt_2 = write_back(core_2);
				}
			}
			print_core_trace(core_2);
		}
		//core3
		if (is_halt_3 != HALT) {
			if (queue[3]->is_call == 0) {
				if (is_stall_3 == 0) {
					fetch(core_3);
					decode(core_3);
				}
				else {
					insert_stall_to_pipe(core_3->pipline->old_decode_exec);
				}
				//after decoding - check for stalls
				is_stall_3 = check_if_pipe_stall(core_3);
				execute(core_3);
				memory(core_3, main_mem, bus, queue);
				if (queue[3]->is_call == 0) {
					is_halt_3 = write_back(core_3);
				}
			}
			else {//need to wait for memory to be writen to cache from main memory
				is_done = transfer_block_from_main_mem(core_3, bus, queue, main_mem);
				if (is_done == 1) {
					promote_pipeline_reg(core_3->pipline->old_mem_wb, core_3->pipline->mem_wb);
					is_halt_3 = write_back(core_3);
				}
			}
			print_core_trace(core_3);
		}
		
		if (is_halt_0 == HALT && is_halt_1 == HALT && is_halt_2 == HALT && is_halt_3 == HALT) {
			return;
		}
		bus_accept_next_call(bus, queue);
		clock++;
	}
	

}