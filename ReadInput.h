#ifndef READ_INPUT
#define READ_INPUT

#define MAX_LINE_IMEM 10
#define MAX_IMEM_ROWS 1049

char** read_input_file(char* path_to_file, char** input_arr, int max_rows_in_file);
char** create_input_store_arr(int max_lines);

#endif // !READ_INPUT
