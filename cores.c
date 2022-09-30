#define _CRT_SECURE_NO_WARNINGS
#include "cores.h"
#include "ReadInput.h"
#include <stdio.h>

char** create_registers_arr() {
	char** registers = NULL;
	registers = (char**)malloc(sizeof(char) * REGISTERS_NUM * REGISTER_SIZE);
	if (NULL == registers) {
		printf("memory allocation failed, exiting program\n");
		exit(1);
	}
	int i = 0;
	while (i < REGISTERS_NUM) {
		registers[i] = (char*)malloc(sizeof(char) * REGISTER_SIZE);
		if (registers[i] == NULL) {
			printf("memory allocation failed, exiting program\n");
			exit(1);
		}
		//init all registers to zero at start of run:
		strcpy(registers[i], "00000000");
		i++;
	}
	return registers;
}

void INIT_DSRAM_TO_ZEORS(Core *core) {
	int i = 0;
	while (i < DSRAM_ROWS) {
		strcpy(core->DSRAM[i], "00000000");
		i++;
	}
}

void INIT_TSRAM(Core* core) {
	int i = 0;
	while (i < TSRAM_ROWS) {
		core->TSRAM[i]->MESI = 0;
		core->TSRAM[i]->tag = 0;
		strcpy(core->TSRAM[i]->TAG, "000");
		i++;
	}
}



//set the pipeline registers to empty values.

void fill_empty_pipe(Inst* inst) {
	strcpy(inst->op, "---");
	inst->is_jump = 0;
	inst->is_empty = 1;
}
void set_pipeline_to_zeros(Pipe* pipe) {
	//allocate memory for inst:
	pipe->fetch_decode = (Inst*)malloc(sizeof(Inst));
	pipe->old_fetch_decode = (Inst*)malloc(sizeof(Inst));
	pipe->decode_exec = (Inst*)malloc(sizeof(Inst));
	pipe->old_decode_exec = (Inst*)malloc(sizeof(Inst));
	pipe->exec_mem = (Inst*)malloc(sizeof(Inst));
	pipe->old_exec_mem = (Inst*)malloc(sizeof(Inst));
	pipe->mem_wb = (Inst*)malloc(sizeof(Inst));
	pipe->old_mem_wb = (Inst*)malloc(sizeof(Inst));

	fill_empty_pipe(pipe->fetch_decode);
	fill_empty_pipe(pipe->old_fetch_decode);
	fill_empty_pipe(pipe->decode_exec);
	fill_empty_pipe(pipe->old_decode_exec);
	fill_empty_pipe(pipe->exec_mem);
	fill_empty_pipe(pipe->old_exec_mem);
	fill_empty_pipe(pipe->mem_wb);	
	fill_empty_pipe(pipe->old_mem_wb);
}

void INIT_pipeline(Core *core) {
	core->pipline = (Pipe*)malloc(sizeof(Pipe));
	if (NULL == core->pipline) {
		printf("memory allocation failed, exiting program\n");
		exit(1);
	}
	set_pipeline_to_zeros(core->pipline);
}

Core* create_core(char** imem_path, int core_num) {
	char** imem = create_input_store_arr(MAX_IMEM_ROWS);
	imem = read_input_file(imem_path, imem, MAX_IMEM_ROWS);
	char** registers = create_registers_arr();
	Core* core = NULL;
	core = (Core*)malloc(sizeof(Core)+ REGISTERS_NUM * REGISTER_SIZE+ MAX_LINE_IMEM* MAX_IMEM_ROWS+ DSRAM_ROWS* DSRAM_BITS);
	if (NULL == core) {
		printf("memory allocation failed, exiting program\n");
		exit(1);
	}
	core->core_num = core_num;
	core->pc = 0;
	core->imem = imem;
	core->registers = registers;	
	INIT_DSRAM_TO_ZEORS(core);
	INIT_TSRAM(core);
	INIT_pipeline(core);
	core->is_halt_fetched = 0;
	core->is_stall = 0;
	return core;
}