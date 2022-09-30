#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "alu_instr.h"
#include "cores.h"
#include "bus_and_memory.h"

extern clock;

Bus* create_bus_struct() {
	Bus* bus = NULL;
	bus = (Bus*)malloc(sizeof(Bus));
	if (NULL == bus) {
		printf("memory allocation failed, exiting program\n");
		exit(1);
	}
	bus->bus_cmd = 0;
	bus->bus_origid = -1;
	bus->bus_shared = 0;
	bus->time_of_last_call = -1;
	bus->bus_add = 0;
	bus->last_served = -1;
}

MemCall** create_bus_queue() {
	MemCall** queue = (MemCall**)malloc(sizeof(MemCall));
	if (queue == NULL) {
		printf("memory allocation failed, exiting program\n");
		exit(1);
	}
	int i = 0;
	while (i < 4) {
		queue[i] = (MemCall*)malloc(sizeof(MemCall));
		if (queue[i] == NULL) {
			printf("memory allocation failed, exiting program\n");
			exit(1);
		}
		queue[i]->is_call = 0;
		queue[i]->now_serve = -1;
		i++;
	}
	
	return queue;
}

MemStat** init_mem_stat_arr() {
	MemStat** stat = (MemStat*)malloc(sizeof(MemStat)* TOTAL_BLOCK_NUM);
	if (stat == NULL) {
		printf("memory allocation failed. Exiting program\n");
		exit(1);
	}
	int i = 0;
	while (i < TOTAL_BLOCK_NUM) {
		stat[i] = (MemStat*)malloc(sizeof(MemStat));
		if (stat[i] == NULL) {
			printf("memory allocation failed. Exiting program\n");
			exit(1);
		}
		stat[i]->core_num = -1;
		stat[i]->mesi = -1;
		i++;
	}
	return stat;
}

int get_mem_add_offset(int mem_add) {
	int offset = mem_add % BLOCK_SIZE;
	return offset;
}

int find_block_location_in_cache(int add) {
	int block_num = add % TOTAL_BLOCK_NUM;
	return block_num;
}

int get_blocks_tag(int add) {
	char block[REGISTER_SIZE];
	sprintf(&block, "%08X", add);
	char tag[TSRAM_BITS];
	int i = 0;
	while (i < TSRAM_BITS-1) {
		tag[i] = block[i];
		i++;
	}
	tag[i] = '\0';
	int tag_int=convert_hex_to_int(tag);
	return tag_int;
}

int check_if_tag_is_in_cache(Core *core, int tag, int tag_saved_in_block, int block) {
	int is_tag_in_cache = 0;
	//check if block is empty in cache:
	if (core->TSRAM[block/BLOCK_SIZE]->MESI == 0) {
		return 0;
	}
	if (tag_saved_in_block == tag) {
		is_tag_in_cache = 1;
	}
	else {
		is_tag_in_cache = 0;
	}
	return is_tag_in_cache;
}



int find_next_core_for_bus(Bus* bus, MemCall** queue) {
	//find next in line:
	int next_in_line = bus->last_served;
	next_in_line++;
	int count = 0;
	while (count<CORE_NUM) {
		if (next_in_line == 4) {
			next_in_line = 0;
		}
		if (queue[next_in_line]->is_call == 1) {
			return next_in_line;
		}
		next_in_line++;
		count++;
	}
	return -1;
}

void bus_accept_next_call(Bus *bus, MemCall **queue) {
	if (bus->bus_cmd != 0) {//bus is busy
		return;
	}
	int next_in_line = find_next_core_for_bus(bus, queue);
	if (next_in_line == -1) {
		return;
	}
	bus->bus_origid = next_in_line;
	bus->bus_cmd = queue[next_in_line]->call_type;
	bus->bus_add = queue[next_in_line]->mem_add;
	bus->time_of_last_call = clock;
	bus->finish_time = bus->time_of_last_call + MEM_CALL_TIME;
	queue[next_in_line]->now_serve = 1;

	FILE* ptr = NULL;
	ptr = fopen("bustrace.txt", "a");
	if (NULL == ptr) {
		printf("error while opening output file, exiting program\n");
		exit(1);
	}
	char bus_add[9];
	sprintf(bus_add, "%08X", bus->bus_add);
	fprintf(ptr, "%d %d %d %s %s %d\n", clock, bus->bus_origid, BUS_RD, bus_add, "00000000", 0);
	fclose(ptr);

}

void load_word(Core* core, int cache_address) {
	int offset = get_mem_add_offset(cache_address);
	int rd = core->pipline->old_exec_mem->rd_index;
	strcpy(core->registers[rd], core->DSRAM[cache_address]);
}

void memory_call_read(Core* core, char** main_mem, Bus* bus, MemCall **queue) {
	//check if data is already in cache:
	int cache_address = find_block_location_in_cache(core->pipline->ALU_result_old);
	int tag = get_blocks_tag(core->pipline->ALU_result_old);
	int current_tag_on_cache = get_blocks_tag(convert_hex_to_int(core->DSRAM[cache_address]));
	int is_block_in_cache = check_if_tag_is_in_cache(core, tag, current_tag_on_cache,cache_address);

	//need to access main memory via bus:
	if (is_block_in_cache == 0) {
		//enter bus_queue:
		queue[core->core_num]->is_call = 1;
		queue[core->core_num]->now_serve = 0;
		queue[core->core_num]->mem_add = core->pipline->ALU_result_old;
		queue[core->core_num]->call_type = BUS_RD;
	}
	//block in cache:
	else {
		load_word(core, cache_address);
	}

}

void free_bus(Bus* bus) {
	bus->bus_origid = 0;
	bus->bus_cmd = 0;
}


void print_bus_transfer_from_mem(Bus* bus, MemCall** queue, char** main_mem, int main_mem_add, int offset) {
	FILE* ptr = NULL;
	ptr = fopen("bustrace.txt", "a");
	if (NULL == ptr) {
		printf("Error while opening output file,  exiting program\n");
		exit(1);
	}
	int print_clock = clock-3 , init_add=main_mem_add;
	char add[9];
	if (offset == 0) {
		sprintf(add, "%08X", main_mem_add);
		main_mem[main_mem_add][8] = '\0';
		main_mem[main_mem_add+1][8] = '\0';
		main_mem[main_mem_add+2][8] = '\0';
		main_mem[main_mem_add + 3][8] = '\0';
		fprintf(ptr, "%d %d %d %s %s %d\n", print_clock, 4, FLUSH, add, main_mem[init_add], 0);
		print_clock++;
		main_mem_add++;
		sprintf(add, "%08X", main_mem_add);
		fprintf(ptr, "%d %d %d %s %s %d\n", print_clock, 4, FLUSH, add, main_mem[init_add + 1], 0);
		print_clock++;
		main_mem_add++;
		sprintf(add, "%08X", main_mem_add);
		fprintf(ptr, "%d %d %d %s %s %d\n", print_clock, 4, FLUSH, add, main_mem[init_add + 2], 0);
		print_clock++;
		main_mem_add++;
		sprintf(add, "%08X", main_mem_add);
		fprintf(ptr, "%d %d %d %s %s %d\n", print_clock, 4, FLUSH, add, main_mem[init_add + 3], 0);
	}
	if (offset == 1) {
		sprintf(add, "%08X", main_mem_add);
		main_mem[main_mem_add - 1][8] = '\0';
		main_mem[main_mem_add][8] = '\0';
		main_mem[main_mem_add+1][8] = '\0';
		main_mem[main_mem_add + 2][8] = '\0';
		main_mem[main_mem_add][8] = '\0';
		fprintf(ptr, "%d %d %d %s %s %d\n", print_clock, 4, FLUSH, add, main_mem[init_add -1], 0);
		print_clock++;
		main_mem_add++;
		sprintf(add, "%08X", main_mem_add);
		fprintf(ptr, "%d %d %d %s %s %d\n", print_clock, 4, FLUSH, add, main_mem[init_add], 0);
		print_clock++;
		main_mem_add++;
		sprintf(add, "%08X", main_mem_add);
		fprintf(ptr, "%d %d %d %s %s %d\n", print_clock, 4, FLUSH, add, main_mem[init_add + 1], 0);
		print_clock++;
		main_mem_add++;
		sprintf(add, "%08X", main_mem_add);
		fprintf(ptr, "%d %d %d %s %s %d\n", print_clock, 4, FLUSH, add, main_mem[init_add + 2], 0);
	}
	if (offset == 2) {
		sprintf(add, "%08X", main_mem_add);
		main_mem[main_mem_add - 2][8] = '\0';
		main_mem[main_mem_add - 1][8] = '\0';
		main_mem[main_mem_add][8] = '\0';
		main_mem[main_mem_add+1][8] = '\0';
		fprintf(ptr, "%d %d %d %s %s %d\n", print_clock, 4, FLUSH, add, main_mem[init_add -2], 0);
		print_clock++;
		main_mem_add++;
		sprintf(add, "%08X", main_mem_add);
		fprintf(ptr, "%d %d %d %s %s %d\n", print_clock, 4, FLUSH, add, main_mem[init_add - 1], 0);
		print_clock++;
		main_mem_add++;
		sprintf(add, "%08X", main_mem_add);
		fprintf(ptr, "%d %d %d %s %s %d\n", print_clock, 4, FLUSH, add, main_mem[init_add], 0);
		print_clock++;
		main_mem_add++;
		sprintf(add, "%08X", main_mem_add);
		fprintf(ptr, "%d %d %d %s %s %d\n", print_clock, 4, FLUSH, add, main_mem[init_add + 1], 0);
	}
	if (offset == 3) {
		sprintf(add, "%08X", main_mem_add);
		main_mem[main_mem_add-3][8] = '\0';
		main_mem[main_mem_add-2][8] = '\0';
		main_mem[main_mem_add-1][8] = '\0';
		main_mem[main_mem_add][8] = '\0';
		fprintf(ptr, "%d %d %d %s %s %d\n", print_clock, 4, FLUSH, add, main_mem[init_add -3], 0);
		print_clock++;
		main_mem_add++;
		sprintf(add, "%08X", main_mem_add);
		fprintf(ptr, "%d %d %d %s %s %d\n", print_clock, 4, FLUSH, add, main_mem[init_add - 2], 0);
		print_clock++;
		main_mem_add++;
		sprintf(add, "%08X", main_mem_add);
		fprintf(ptr, "%d %d %d %s %s %d\n", print_clock, 4, FLUSH, add, main_mem[init_add - 1], 0);
		print_clock++;
		main_mem_add++;
		sprintf(add, "%08X", main_mem_add);
		fprintf(ptr, "%d %d %d %s %s %d\n", print_clock, 4, FLUSH, add, main_mem[init_add], 0);
	}
	fclose(ptr);
}

int search_in_other_cache(Core* core1, Core* core2, Core* core3, int block, int tag) {
	if (core1->TSRAM[block]->tag == tag) {
		return core1->core_num;
	}
	if (core2->TSRAM[block]->tag == tag) {
		return core2->core_num;
	}
	if (core3->TSRAM[block]->tag == tag) {
		return core3->core_num;
	}
	return -1;
}

int transfer_block_from_main_mem(Core* core, Bus* bus, MemCall** queue,char **main_mem) {	
	if (clock < bus->finish_time || queue[core->core_num]->now_serve==0) {
		return 0;
	}
	if (strcmp(core->pipline->old_mem_wb->op, "---") == 0) {
		return 1;
	}
	int main_mem_add = core->pipline->ALU_result_old;
	int tag = get_blocks_tag(main_mem_add);
	int offset = get_mem_add_offset(main_mem_add);
	int block_index_cache = find_block_location_in_cache(main_mem_add);
	//int search_other_cache = search_in_other_cache(core1, core2, core3, block_index_cache, tag);

	bus->bus_cmd = FLUSH;

	if (offset == 0) {
		strcpy(core->DSRAM[block_index_cache], main_mem[main_mem_add]);
		strcpy(core->DSRAM[block_index_cache+1], main_mem[main_mem_add+1]);
		strcpy(core->DSRAM[block_index_cache + 2], main_mem[main_mem_add + 2]);
		strcpy(core->DSRAM[block_index_cache + 3], main_mem[main_mem_add + 3]);
	}
	if (offset == 1) {
		strcpy(core->DSRAM[block_index_cache-1], main_mem[main_mem_add-1]);
		strcpy(core->DSRAM[block_index_cache], main_mem[main_mem_add]);
		strcpy(core->DSRAM[block_index_cache + 1], main_mem[main_mem_add + 1]);
		strcpy(core->DSRAM[block_index_cache + 2], main_mem[main_mem_add + 2]);
	}
	if (offset == 2) {
		strcpy(core->DSRAM[block_index_cache-2], main_mem[main_mem_add - 2]);
		strcpy(core->DSRAM[block_index_cache - 1], main_mem[main_mem_add-1]);
		strcpy(core->DSRAM[block_index_cache ], main_mem[main_mem_add]);
		strcpy(core->DSRAM[block_index_cache + 1], main_mem[main_mem_add + 1]);
	}
	if (offset == 3) {
		strcpy(core->DSRAM[block_index_cache-3], main_mem[main_mem_add - 3]);
		strcpy(core->DSRAM[block_index_cache - 2], main_mem[main_mem_add - 2]);
		strcpy(core->DSRAM[block_index_cache - 1], main_mem[main_mem_add - 1]);
		strcpy(core->DSRAM[block_index_cache], main_mem[main_mem_add]);
	}
	//print to output bustrace.txt:
	print_bus_transfer_from_mem(bus, queue, main_mem, main_mem_add, offset);
	core->TSRAM[block_index_cache/BLOCK_SIZE]->MESI = SHARED;
	core->TSRAM[block_index_cache/BLOCK_SIZE]->tag = tag;
	sprintf(core->TSRAM[block_index_cache/BLOCK_SIZE]->TAG, "%03X", tag);
	free_bus(bus);
	queue[core->core_num]->is_call = 0;
	queue[core->core_num]->now_serve = 0;

	if (strcmp(core->pipline->old_mem_wb->op, "11") == 0) {
		memory_call_write(core, main_mem, bus, queue);
	}
	return 1;
}

void memory_call_write(Core* core, char** main_mem, Bus* bus, MemCall** queue) {
	int cache_address = find_block_location_in_cache(core->pipline->ALU_result_old);
	int tag = get_blocks_tag(core->pipline->ALU_result_old);
	int current_tag_on_cache = get_blocks_tag(convert_hex_to_int(core->DSRAM[cache_address]));
	int is_block_in_cache = check_if_tag_is_in_cache(core, tag, current_tag_on_cache, cache_address);
	//data in cache - change it and chagne mesi to M
	if (is_block_in_cache == 1) {
		strcpy(core->DSRAM[cache_address], core->registers[core->pipline->old_exec_mem->rd_index]);
		core->TSRAM[cache_address]->MESI = MODIFIED;
		//
		strcpy(main_mem[core->pipline->ALU_result_new], core->registers[core->pipline->old_exec_mem->rd_index]);
		//
	}
	else {//data was not in cache so need to get a main memory read request:
		memory_call_read(core, main_mem, bus, queue);
	}

}