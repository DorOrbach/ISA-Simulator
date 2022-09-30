#include <stdio.h>
#include "free_allocated_mem.h"
#include "cores.h"
#include "bus_and_memory.h"

void free_2D_arr(char** arr,int arr_size) {
	int i = arr_size-1;
	while (i > -1) {
		if (arr[i] != NULL) {
			free(arr[i]);
		}
		i--;
	}
	//free(arr);
}

void free_pipeline(Core* core) {
	free(core->pipline->exec_mem);
	free(core->pipline->mem_wb);
	//free(core->pipline->decode_exec);
	free(core->pipline->old_exec_mem);
	free(core->pipline->old_mem_wb);
	free(core->pipline->old_decode_exec);
}

void free_all_input_arrays(Core *core_0, Core* core_1, Core* core_2, Core* core_3, char **memin, Bus *bus , MemCall *queue) {
	free_2D_arr(core_0->imem, MAX_IMEM_ROWS);
	free_2D_arr(core_1->imem, MAX_IMEM_ROWS);
	free_2D_arr(core_2->imem, MAX_IMEM_ROWS);
	free_2D_arr(core_3->imem, MAX_IMEM_ROWS);
	free_2D_arr(core_0->registers, REGISTERS_NUM);
	free_2D_arr(core_1->registers, REGISTERS_NUM);
	free_2D_arr(core_2->registers, REGISTERS_NUM);
	free_2D_arr(core_3->registers, REGISTERS_NUM);
	free_2D_arr(memin, MAX_MEMIN_ROWS);
	free_pipeline(core_0);
	free_pipeline(core_1);
	free_pipeline(core_2);
	free_pipeline(core_3);
	//free_2D_arr(queue, CORE_NUM);
	free(bus);
}