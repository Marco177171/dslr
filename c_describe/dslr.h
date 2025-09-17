#ifndef DSLR_H
#define DSLR_H

#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct s_statistics {
	int count;
	double std;
	double mean;
	double min;
	double twentyfive;
	double fifty;
	double seventyfive;
	double max;
} t_statistics;

typedef struct s_feature {
	char 				*name;
	t_statistics		stats;
	struct s_feature 	*next;
}	t_feature;

typedef enum {
	DOUBLE,
	STRING
}	e_type;

typedef struct s_data_frame {
	int		valid;
	e_type	type;
	union {
		double d;
		char *s;
	};
}	t_data_frame;

// memory_managers.c
void free_array(char** list);
void free_matrix(char*** matrix);
// pinters.c
void print_matrix_vertically(t_data_frame ***df);
void print_matrix_column_vertically(t_data_frame ***df, int col_index);
void print_matrix_row(t_data_frame **df);
// utilities
char** split(char *string, char delimiter);
t_data_frame **load_df(char *s, char limit_char);
char* substring(char *string, int start_index, int last_index);
// statistics
int array_len(double *array);
double array_mean(t_data_frame ***df, int col);
double standard_deviation(t_data_frame ***df, int col);
double find_max(t_data_frame ***df, int col);
double find_min(t_data_frame ***df, int col);
void sort_column(t_data_frame ***df, int col);
t_feature* get_statistics(t_data_frame ***df);
// check
int is_valid(char *s);
int is_number(char *s);
int is_valid_column(t_data_frame ***df, int col);
#endif
