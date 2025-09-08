#ifndef LIB_H
#define LIB_H

#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct s_statistics {
	float count;
	float std;
	float min;
	float twentyfive;
	float fifty;
	float seventyfive;
	float max;
} t_statistics;

// memory_managers.c
void free_array(char** list);
void free_matrix(char*** matrix);
// pinters.c
void print_array_vertically(char **array);
void print_matrix_vertically(char ***matrix);
// utilities
char** split(char *string, char delimiter);
char* substring(char *string, int start_index, int last_index);
// statistics
int array_len(float array[]);
float array_mean(float array[]);
float standard_deviation(float array[]);
void matrix_along_col(char ***matrix, int row_index);

#endif
