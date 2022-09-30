#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "ReadInput.h"
#include "free_allocated_mem.h"
#include "cores.h"
#include "execute_instructions.h"
#include "bus_and_memory.h"
#include "print_outpu.h"

#define MAX_IMEM_ROWS 1049
#define MAX_LINE_IMEM 10
#define MAX_MEMIN_ROWS 1048576 //2 to the pwer of 20.

int clock = 0;

Core* core_0 = NULL;
Core* core_1 = NULL;
Core* core_2 = NULL;
Core* core_3 = NULL;


int main(int argc, char* argv[]) {
	char* imem0_path = argv[1], * imem1_path = argv[2], * imem2_path = argv[3], * imem3_path = argv[4];
	char* memin_path = argv[5];

	//delte old output files:
	delete_old_output_files();

	//create struct for all 4 cores:
	core_0 = create_core(imem0_path,0);
	core_1 = create_core(imem1_path,1);
	core_2 = create_core(imem2_path,2);
	core_3 = create_core(imem3_path,3);

	//store memin input file:
	char **memin = create_input_store_arr(MAX_MEMIN_ROWS);
	memin = read_input_file(memin_path, memin, MAX_MEMIN_ROWS);

	//create bus and enable bus access to cache mem of each core:
	Bus* bus = create_bus_struct();
	
	MemCall** bus_queue = create_bus_queue();
	simulate_clock_cycle(core_0, core_1, core_2, core_3, memin, bus, bus_queue);

	//print end of run output files
	print_regout(core_0);
	print_regout(core_1);
	print_regout(core_2);
	print_regout(core_3);
	print_memout(memin);
	print_dsram(core_0);
	print_dsram(core_1);
	print_dsram(core_2);
	print_dsram(core_3);
	print_tsram(core_0);
	print_tsram(core_1);
	print_tsram(core_2);
	print_tsram(core_3);
	
	//free_all_input_arrays(core_0, core_1, core_2, core_3, memin, bus, bus_queue);
	exit(0);;
}