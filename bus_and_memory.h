#ifndef BUS_AND_MEMORY_H
#define BUS_AND_MEMORY_H

#include <stdio.h>
#include <string.h>
#include "cores.h"

#define MAX_MEMIN_ROWS 1048576 //2 to the pwer of 20.

#define ADD_LEN 6 // 5 hexa bits and 1 extra for '\0'
#define DATA_LEN 9 // 8 for hexa bits and extra 1 for '\0'

#define BLOCK_SIZE 4
#define TOTAL_BLOCK_NUM 64 // 256 words divided by 4 words per block.

#define BUS_FREE 1
#define BUS_BUSY 0
#define MEM_CALL_TIME 20 // 16 cycles for bus and 4 more to get the block.

#define INVALID 0
#define SHARED 1
#define EXCLUSIVE 2
#define MODIFIED 3 

#define BUS_ANSWERED 1
#define BUS_BUSY 0
#define BUS_RD 1
#define BUS_RDX 2
#define FLUSH 3

#define DATA_IN_MEM 1
#define DATA_NOT_IN_MEM 0

#define CORE_NUM 4

typedef struct MemCall {
	int is_call;
	int now_serve;
	int mem_add;
	int call_type;
}MemCall;

typedef struct Bus {
	int bus_origid; // 0,1,2,3 or 4.
	int bus_cmd; // 0, 1, 2 or 3
	int bus_add;
	char bus_data[DATA_LEN];
	int bus_shared;
	int time_of_last_call;
	int finish_time;
	int last_served;
}Bus;

typedef struct MemStat {
	int core_num;
	int mesi;
}MemStat;

Bus* create_bus_struct();
MemCall** create_bus_queue();
void memory_call_read(Core* core, char** main_mem, Bus* bus, MemCall** queue);
void free_bus(Bus* bus);
int find_block_location_in_cache(int add);
int transfer_block_from_main_mem(Core* core, Bus* bus, MemCall** queue, char** main_mem);
void bus_accept_next_call(Bus* bus, MemCall** queue);
int get_mem_add_offset(int mem_add);
void memory_call_write(Core* core, char** main_mem, Bus* bus, MemCall** queue);
MemStat** init_mem_stat_arr();

#endif // !BUS_H
