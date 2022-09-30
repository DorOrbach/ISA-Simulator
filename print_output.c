#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "print_outpu.h"
#include "cores.h"
#include <string.h>
#include <stdlib.h>

#define PATH_LEN 50
#define MAX_MEMIN_ROWS 1048576
#define DSRAM_ROWS 256

extern clock;

void delete_old_output_files() {
	FILE* ptr = NULL;
	ptr = fopen("memout.txt", "w");
	fclose(ptr);
	ptr= fopen("core0trace.txt", "w");
	fclose(ptr);
	ptr = fopen("core1trace.txt", "w");
	fclose(ptr);
	ptr = fopen("core2trace.txt", "w");
	fclose(ptr);
	ptr = fopen("core3trace.txt", "w");
	fclose(ptr);
	ptr = fopen("regout0.txt", "w");
	fclose(ptr);
	ptr = fopen("regout1.txt", "w");
	fclose(ptr);
	ptr = fopen("regout2.txt", "w");
	fclose(ptr);
	ptr = fopen("regout3.txt", "w");
	fclose(ptr);
	ptr = fopen("dsram0.txt", "w");
	fclose(ptr);
	ptr = fopen("dsram1.txt", "w");
	fclose(ptr);
	ptr = fopen("dsram2.txt", "w");
	fclose(ptr);
	ptr = fopen("dsram3.txt", "w");
	fclose(ptr);
	ptr = fopen("tsram0.txt", "w");
	fclose(ptr);
	ptr = fopen("tsram1.txt", "w");
	fclose(ptr);
	ptr = fopen("tsram2.txt", "w");
	fclose(ptr);
	ptr = fopen("tsram3.txt", "w");
	fclose(ptr);
	ptr = fopen("bustrace.txt", "w");
	fclose(ptr);
}

void print_core_trace(Core* core) {
	FILE* ptr = NULL;
	char path[PATH_LEN];
	sprintf(path, "core" "%d" "trace.txt", core->core_num);
	ptr = fopen(path, "a");
	if (ptr == NULL) {
		printf("Error: unable to open output file\n");
		exit(1);
	}
	//print cycle:
	fprintf(ptr, "%d ", clock);

	//print fetch op:
	if (core->pipline->fetch_decode->is_empty == 1 || strcmp(core->pipline->decode_exec->op, "14") == 0) {
		fprintf(ptr, "--- ");
	}
	else {
		fprintf(ptr, "%03X ", core->pipline->fetch_decode->pc);
	}
	
	//print decode op:
	if (core->pipline->decode_exec->is_empty == 1 || strcmp(core->pipline->exec_mem->op, "14") == 0) {
		fprintf(ptr, "--- ");
	}
	else {
		fprintf(ptr, "%03X ", core->pipline->decode_exec->pc);
	}

	//print exec op:
	if (core->pipline->exec_mem->is_empty == 1 || strcmp(core->pipline->exec_mem->op,"---")==0 || strcmp(core->pipline->mem_wb->op, "14") == 0) {
		fprintf(ptr, "--- ");
	}
	else {
		fprintf(ptr, "%03X ", core->pipline->exec_mem->pc);
	}
	
	//print mem op:
	if (core->pipline->mem_wb->is_empty == 1 || strcmp(core->pipline->mem_wb->op, "---") == 0 || strcmp(core->pipline->old_mem_wb->op, "14") == 0) {
		fprintf(ptr, "--- ");
	}
	else {
		fprintf(ptr, "%03X ", core->pipline->mem_wb->pc);
	}
	
	//print wb op:
	if (core->pipline->old_mem_wb->is_empty == 1 || strcmp(core->pipline->old_mem_wb->op, "---")==0) {
		fprintf(ptr, "--- ");
	}
	else {
		fprintf(ptr, "%03X ", core->pipline->old_mem_wb->pc);
	}
	
	//print registers:
	int i = 2;
	while (i < REG_NUM) {
		core->registers[i][8] = '\0';
		fprintf(ptr, "%s ", core->registers[i]);
		i++;
	}
	fprintf(ptr, "\n");
	fclose(ptr);
}

void print_regout(Core* core) {
	FILE* ptr = NULL;
	char path[PATH_LEN];
	sprintf(path, "regout" "%d" ".txt", core->core_num);
	ptr = fopen(path, "a");
	if (NULL == ptr) {
		printf("Error: unable to open output file\n");
		exit(1);
	}

	int i = 2;
	while (i < REG_NUM) {
		fprintf(ptr, "%s\n", core->registers[i]);
		i++;
	}

	fclose(ptr);
}

void print_memout(char** mem) {
	FILE* ptr = NULL;
	ptr = fopen("memout.txt", "w");
	if (NULL == ptr) {
		printf("Error: unable to open output file\n");
		exit(1);
	}
	int i = 0;
	while (i < MAX_MEMIN_ROWS) {
		mem[i][8] = '\n';
		fprintf(ptr, "%s", mem[i]);
		i++;
	}
	fclose(ptr);
}

void print_dsram(Core* core) {
	FILE* ptr = NULL;
	char path[PATH_LEN];
	sprintf(path, "dsram" "%d" ".txt", core->core_num);
	ptr = fopen(path, "a");
	if (NULL == ptr) {
		printf("Error: unable to open output file\n");
		exit(1);
	}

	int i = 0;
	while (i < DSRAM_ROWS) {
		core->DSRAM[i][8] = '\0';
		fprintf(ptr, "%s\n", core->DSRAM[i]);
		i++;
	}
	fclose(ptr);
}

void print_tsram(Core* core) {
	char mesi[6];
	char tag[4];
	char path[PATH_LEN];
	int i = 0;
	FILE* ptr = NULL;
	sprintf(path, "tsram""%d.txt", core->core_num);
	ptr = fopen(path, "w");
	if (ptr == NULL) {
		printf("Error while opening output file, exiting program\n");
		exit(1);
	}

	while (i < TSRAM_ROWS) {
		sprintf(mesi, "%05X", core->TSRAM[i]->MESI);
		sprintf(tag, "%03X", core->TSRAM[i]->tag);
		fprintf(ptr, "%s%s\n", mesi, tag);
		i++;
	}
	fclose(ptr);
}