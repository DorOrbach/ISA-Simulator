#ifndef FREE_ALLOCATED_MEM
#define FREE_ALLOCATED_MEM
#include "cores.h"
#include "bus_and_memory.h"

#define MAX_IMEM_ROWS 1049
#define MAX_MEMIN_ROWS 1048576 //2 to the pwer of 20.

void free_all_input_arrays(Core* core_0, Core* core_1, Core* core_2, Core* core_3, char** memin, Bus* bus, MemCall *queue);
#endif // !FREE_ALLOCATED_MEM

