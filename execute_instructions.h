#ifndef EXECUTE_INSTRUCTIONS
#define	EXECUTE_INSTRUCTIONS
#include "bus_and_memory.h"
#include "cores.h"

#define HALT 14
#define BRANCH 1
#define NO_BRANCH 0

extern clock;

void simulate_clock_cycle(Core* core_0, Core* core_1, Core* core_2, Core* core_3, char** main_mem, Bus* bus, MemCall **queue);

#endif // !EXECUTE_INSTRUCTIONS

