#ifndef ALU_INST_H
#define ALU_INST_H

#include "cores.h"

//Arithmetic and logic:
int convert_hex_to_int(char* hex);
int add(Core *core, int rs, int rt);
int sub(Core* core, int rs, int rt);
int mul(Core* core, int rs, int rt);
int and(Core* core, int rs, int rt);
int or (Core * core, int rs, int rt);
int xor (Core * core, int rs, int rt);
int sll(Core* core, int rs, int rt);
int sra(Core* core, int rs, int rt);
int add_for_rd(Core* core, int rs, int rt);
void save_res_in_reg(char *reg, int num);

//check branch conditions:
int check_equal(Core* core, int rs, int rt);
int check_less_or_greater_then(Core* core, int rs, int rt);

#endif // !ALU_INST_H
