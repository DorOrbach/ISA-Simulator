#ifndef PRINT_OUTPUT_H
#define PRINT_OUTPUT_H
#include "cores.h"

#define REG_NUM 16

void print_core_trace(Core* core);
void print_regout(Core* core);
void print_memout(char** mem);
void print_dsram(Core* core);
void delete_old_output_files();
void print_tsram(Core* core);

#endif // !PRINT_OUTPUT_H

