#define _CRT_SECURE_NO_WARNINGS
#include "alu_instr.h"
#include <stdio.h>
#include "execute_instructions.h"
#include <string.h>
#include <stdlib.h>
#include "cores.h"


int convert_hex_to_int(char* hex) {
	unsigned long num = 0;
	num = strtol(hex, NULL, 16);
	return num;
}

void save_res_in_reg(char *reg, int num) {
	char hex_res[MAX_LINE_IMEM];
	sprintf(hex_res, "%08X", num);
	strcpy(reg, hex_res);
}

int add(Core *core, int rs, int rt) {
	int val_reg_rs = 0;
	int val_reg_rt = 0;
	//check if one of registers is the imm reg:
	if (rs == 1) {
		val_reg_rs = convert_hex_to_int(core->pipline->old_decode_exec->imm);
		val_reg_rt = convert_hex_to_int(core->registers[rt]);
	}
	else if (rt == 1) {
		val_reg_rt = convert_hex_to_int(core->pipline->old_decode_exec->imm);
		val_reg_rs = convert_hex_to_int(core->registers[rs]);
	}
	else {
		val_reg_rs = convert_hex_to_int(core->registers[rs]);
		val_reg_rt = convert_hex_to_int(core->registers[rt]);
	}
	int add_res = val_reg_rs + val_reg_rt;
	return add_res;
}

int sub(Core* core, int rs, int rt) {
	int val_reg_rs = 0;
	int val_reg_rt = 0;
	//check if one of registers is the imm reg:
	if (rs == 1) {
		val_reg_rs = convert_hex_to_int(core->pipline->old_decode_exec->imm);
		val_reg_rt = convert_hex_to_int(core->registers[rt]);
	}
	else if (rt == 1) {
		val_reg_rt = convert_hex_to_int(core->pipline->old_decode_exec->imm);
		val_reg_rs = convert_hex_to_int(core->registers[rs]);
	}
	else {
		val_reg_rs = convert_hex_to_int(core->registers[rs]);
		val_reg_rt = convert_hex_to_int(core->registers[rt]);
	}
	int sub_res = val_reg_rs - val_reg_rt;
	return sub_res;
}

int mul(Core* core, int rs, int rt) {
	int val_reg_rs = 0;
	int val_reg_rt = 0;
	//check if one of registers is the imm reg:
	if (rs == 1) {
		val_reg_rs = convert_hex_to_int(core->pipline->old_decode_exec->imm);
		val_reg_rt = convert_hex_to_int(core->registers[rt]);
	}
	else if (rt == 1) {
		val_reg_rt = convert_hex_to_int(core->pipline->old_decode_exec->imm);
		val_reg_rs = convert_hex_to_int(core->registers[rs]);
	}
	else {
		val_reg_rs = convert_hex_to_int(core->registers[rs]);
		val_reg_rt = convert_hex_to_int(core->registers[rt]);
	}
	int mul_res = val_reg_rs * val_reg_rt;
	return mul_res;
}

int and(Core* core, int rs, int rt) {
	int val_reg_rs = 0;
	int val_reg_rt = 0;
	//check if one of registers is the imm reg:
	if (rs == 1) {
		val_reg_rs = convert_hex_to_int(core->pipline->old_decode_exec->imm);
		val_reg_rt = convert_hex_to_int(core->registers[rt]);
	}
	else if (rt == 1) {
		val_reg_rt = convert_hex_to_int(core->pipline->old_decode_exec->imm);
		val_reg_rs = convert_hex_to_int(core->registers[rs]);
	}
	else {
		val_reg_rs = convert_hex_to_int(core->registers[rs]);
		val_reg_rt = convert_hex_to_int(core->registers[rt]);
	}
	int and_res = val_reg_rs & val_reg_rt;
	return and_res;
}

int or (Core* core, int rs, int rt) {
	int val_reg_rs = 0;
	int val_reg_rt = 0;
	//check if one of registers is the imm reg:
	if (rs == 1) {
		val_reg_rs = convert_hex_to_int(core->pipline->old_decode_exec->imm);
		val_reg_rt = convert_hex_to_int(core->registers[rt]);
	}
	else if (rt == 1) {
		val_reg_rt = convert_hex_to_int(core->pipline->old_decode_exec->imm);
		val_reg_rs = convert_hex_to_int(core->registers[rs]);
	}
	else {
		val_reg_rs = convert_hex_to_int(core->registers[rs]);
		val_reg_rt = convert_hex_to_int(core->registers[rt]);
	}
	int or_res = val_reg_rs | val_reg_rt;
	return or_res;
}

int xor (Core * core, int rs, int rt) {
	int val_reg_rs = 0;
	int val_reg_rt = 0;
	//check if one of registers is the imm reg:
	if (rs == 1) {
		val_reg_rs = convert_hex_to_int(core->pipline->old_decode_exec->imm);
		val_reg_rt = convert_hex_to_int(core->registers[rt]);
	}
	else if (rt == 1) {
		val_reg_rt = convert_hex_to_int(core->pipline->old_decode_exec->imm);
		val_reg_rs = convert_hex_to_int(core->registers[rs]);
	}
	else {
		val_reg_rs = convert_hex_to_int(core->registers[rs]);
		val_reg_rt = convert_hex_to_int(core->registers[rt]);
	}
	int xor_res = val_reg_rs ^ val_reg_rt;
	return xor_res;
}

int sll (Core* core, int rs, int rt) {
	int val_reg_rs = 0;
	int val_reg_rt = 0;
	//check if one of registers is the imm reg:
	if (rs == 1) {
		val_reg_rs = convert_hex_to_int(core->pipline->old_decode_exec->imm);
		val_reg_rt = convert_hex_to_int(core->registers[rt]);
	}
	else if (rt == 1) {
		val_reg_rt = convert_hex_to_int(core->pipline->old_decode_exec->imm);
		val_reg_rs = convert_hex_to_int(core->registers[rs]);
	}
	else {
		val_reg_rs = convert_hex_to_int(core->registers[rs]);
		val_reg_rt = convert_hex_to_int(core->registers[rt]);
	}
	int sll_res = val_reg_rs << val_reg_rt;
	return sll_res;
}

int sra(Core* core, int rs, int rt) {
	int val_reg_rs = 0;
	int val_reg_rt = 0;
	//check if one of registers is the imm reg:
	if (rs == 1) {
		val_reg_rs = convert_hex_to_int(core->pipline->old_decode_exec->imm);
		val_reg_rt = convert_hex_to_int(core->registers[rt]);
	}
	else if (rt == 1) {
		val_reg_rt = convert_hex_to_int(core->pipline->old_decode_exec->imm);
		val_reg_rs = convert_hex_to_int(core->registers[rs]);
	}
	else {
		val_reg_rs = convert_hex_to_int(core->registers[rs]);
		val_reg_rt = convert_hex_to_int(core->registers[rt]);
	}
	int sra_res = val_reg_rs >> val_reg_rt;
	return sra_res;
}

//logic checks for branch operations:
int check_equal(Core* core, int rs, int rt) {
	int val_reg_rs = 0;
	int val_reg_rt = 0;
	//check if one of registers is the imm reg:
	if (rs == 1) {
		val_reg_rs = convert_hex_to_int(core->pipline->old_decode_exec->imm);
		val_reg_rt = convert_hex_to_int(core->registers[rt]);
	}
	else if (rt == 1) {
		val_reg_rt = convert_hex_to_int(core->pipline->old_decode_exec->imm);
		val_reg_rs = convert_hex_to_int(core->registers[rs]);
	}
	else {
		val_reg_rs = convert_hex_to_int(core->registers[rs]);
		val_reg_rt = convert_hex_to_int(core->registers[rt]);
	}
	if (val_reg_rs == val_reg_rt) {
		return 1;
	}
	return 0;
}

int check_less_or_greater_then(Core* core, int rs, int rt) {
	int val_reg_rs = 0;
	int val_reg_rt = 0;
	//check if one of registers is the imm reg:
	if (rs == 1) {
		val_reg_rs = convert_hex_to_int(core->pipline->decode_exec->imm);
		val_reg_rt = convert_hex_to_int(core->registers[rt]);
	}
	else if (rt == 1) {
		val_reg_rt = convert_hex_to_int(core->pipline->decode_exec->imm);
		val_reg_rs = convert_hex_to_int(core->registers[rs]);
	}
	else {
		val_reg_rs = convert_hex_to_int(core->registers[rs]);
		val_reg_rt = convert_hex_to_int(core->registers[rt]);
	}
	if (val_reg_rs < val_reg_rt) {
		return 1;//blt
	}
	if (val_reg_rs > val_reg_rt) {
		return 0;//bgt
	}
	if(val_reg_rs == val_reg_rt){
		return -1;//option to be also equal.
	}
}

int add_for_rd(Core* core, int rs, int rt) {
	int val_reg_rs = 0;
	int val_reg_rt = 0;
	//check if one of registers is the imm reg:
	if (rs == 1) {
		val_reg_rs = convert_hex_to_int(core->pipline->old_mem_wb->imm);
		val_reg_rt = convert_hex_to_int(core->registers[rt]);
	}
	else if (rt == 1) {
		val_reg_rt = convert_hex_to_int(core->pipline->old_mem_wb->imm);
		val_reg_rs = convert_hex_to_int(core->registers[rs]);
	}
	else {
		val_reg_rs = convert_hex_to_int(core->registers[rs]);
		val_reg_rt = convert_hex_to_int(core->registers[rt]);
	}
	int add_res = val_reg_rs + val_reg_rt;
	return add_res;
}