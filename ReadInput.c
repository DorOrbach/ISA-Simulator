#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "ReadInput.h"
#include <string.h>
#include <stdlib.h>

FILE *open_file(char* path_to_file) {
	FILE* f_ptr = NULL;
	f_ptr = fopen(path_to_file, "r");
	if (NULL == f_ptr) {
		printf("Input file does not exist, plz check file location and try again\n");
		exit(1);
	}
	return f_ptr;
}

char** create_input_store_arr(int max_lines) {
	char** arr = (char**)malloc(sizeof(char) * max_lines * MAX_LINE_IMEM);
	if (NULL == arr) {
		printf("memory allocation faild, exiting program\n");
		exit(1);
	}
	int i = 0;
	while (i < max_lines) {
		arr[i] = (char*)malloc(sizeof(char) * MAX_LINE_IMEM);
		if (arr[i] == NULL) {
			printf("memory allocation faild, exiting program\n");
			exit(1);
		}
		i++;
	}

	return arr;
}

char** read_input_file(char* path_to_file, char **input_arr, int max_rows_in_file) {
	FILE *f_ptr = open_file(path_to_file);
	char *buffer[MAX_LINE_IMEM];
	int i = 0;
	while (fgets(buffer, MAX_LINE_IMEM , f_ptr) != NULL) {
		strcpy(input_arr[i], buffer);
		i++;
	}
	if (i != max_rows_in_file) {
		while (i < max_rows_in_file) {
			strcpy(input_arr[i], "00000000\n");
			i++;
		}
		//input_arr size is 1049 but input file may be smaller then that, so NULL indicates end of content.
	}
	fclose(f_ptr);
	return input_arr;

}