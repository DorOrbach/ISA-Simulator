#ifndef CORES
#define CORES

#define MAX_IMEM_ROWS 1049
#define MAX_LINE_IMEM 10
#define NO_MEM_CALL -1

#define REGISTERS_NUM 16
#define REGISTER_SIZE 9 // 32 bits is 8 bits in hexa - so 9 slots for '\0'

#define OPCOED_LEN 3
#define $REG_LEN 2
#define IMM_LEN 9

#define TSRAM_ROWS 64
#define TSRAM_BITS 4 //12 bits is 3 hexa number + extra slot for '\0'

#define DSRAM_ROWS 256
#define DSRAM_BITS 10 // 32 bits is 8 bits in hexa.

typedef struct TSRAM {
	int MESI;
	char* TAG[13];
	int tag;
}TSRAM;

typedef struct Inst {
	int pc;
	int is_empty;
	int is_jump;

	char op[OPCOED_LEN];
	char rd[$REG_LEN];
	char rs[$REG_LEN];
	char rt[$REG_LEN];
	char imm[IMM_LEN];
	char inst[MAX_LINE_IMEM];

	int rd_index;
	int rs_index;
	int rt_index;
	int imm_val;
}Inst;

typedef struct Pipe {
	Inst* fetch_decode;
	Inst* old_fetch_decode;
	Inst* decode_exec;
	Inst* old_decode_exec;
	Inst* exec_mem;
	Inst* old_exec_mem;
	Inst* mem_wb;
	Inst* old_mem_wb;
	int ALU_result_old;
	int ALU_result_new;
	
}Pipe;

typedef struct Core {
	int core_num;
	int pc;
	char** imem;
	char** registers;
	char DSRAM[DSRAM_ROWS][DSRAM_BITS];
	TSRAM TSRAM[TSRAM_ROWS][TSRAM_BITS];
	Pipe* pipline;
	
	int is_stall;
	int is_halt_fetched;
}Core;

Core* create_core(char** imem_path, int core_num);

#endif // !CORES
