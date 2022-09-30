#ifndef STALLS_H
#define STALLS_H
#include "cores.h"

#define RD 2
#define RS 3
#define RT 4

int check_if_pipe_stall(Core* core);
void insert_stall_to_pipe(Inst* old_decode_exec);

#endif 
