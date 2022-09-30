#define _CRT_SECURE_NO_WARNINGS
#include "stalls.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cores.h"

//
int check_if_branch(Inst* inst) {
	if (strcmp(inst->op, "09") == 0) {
		return 1;
	}
	if (strcmp(inst->op, "0A") == 0) {
		return 1;
	}
	if (strcmp(inst->op, "0B") == 0) {
		return 1;
	}
	if (strcmp(inst->op, "0C") == 0) {
		return 1;
	}
	if (strcmp(inst->op, "0D") == 0) {
		return 1;
	}
	if (strcmp(inst->op, "0E") == 0) {
		return 1;
	}
	if (strcmp(inst->op, "0f") == 0) {
		return 1;
	}
	return 0;
}

int check_if_reg_in_pipe(Core* core, int reg) {
	//check if in exec stage:
	if (reg == 0) {
		return 0;
	}
	if (reg == core->pipline->old_decode_exec->rd_index && core->pipline->old_decode_exec->is_empty!=1) {
		if (check_if_branch(core->pipline->old_decode_exec) == 1) {
			return 0;
		}
		return 1;
	}
	//check if in mem stage:
	if (reg == core->pipline->old_exec_mem->rd_index && core->pipline->old_exec_mem->is_empty!=1) {
		if (check_if_branch(core->pipline->old_exec_mem) == 1) {
			return 0;
		}
		return 1;
	}
	//check if in wb stage:
	if (reg == core->pipline->old_mem_wb->rd_index && core->pipline->old_mem_wb->is_empty!=1) {
		if (check_if_branch(core->pipline->old_mem_wb) == 1 || strcmp(core->pipline->old_mem_wb->op,"10")==0) {
			return 0;
		}
		return 1;
	}
	return 0;
}

int check_if_pipe_stall(Core *core) {
	int rd = core->pipline->decode_exec->rd_index;
	int rs = core->pipline->decode_exec->rs_index;
	int rt = core->pipline->decode_exec->rt_index;
	int is_rd_in_pipe = check_if_reg_in_pipe(core, rd);
	int is_rs_in_pipe = check_if_reg_in_pipe(core, rs);
	int is_rt_in_pipe = check_if_reg_in_pipe(core, rt);
	if (is_rs_in_pipe == 1 || is_rt_in_pipe == 1 || is_rd_in_pipe == 1) {
		return 1;
	}
	return 0;
}

void insert_stall_to_pipe(Inst* old_decode_exec) {
	strcpy(old_decode_exec->op, "---");
	old_decode_exec->imm_val = -1;
	old_decode_exec->pc = -1;
	old_decode_exec->rd_index = -1;
	old_decode_exec->rs_index = -1;
	old_decode_exec->rt_index = -1;
	//old_decode_exec->is_stall = 1;
}