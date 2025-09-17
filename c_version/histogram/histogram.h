#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

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
void print_array_vertically(char **array);
void print_matrix_vertically(char ***matrix);
void print_matrix_column_vertically(char ***matrix, int col_index);
void print_matrix_row(char **matrix);
// utilities
char** split(char *string, char delimiter);
t_data_frame **load_df(char *s, char limit_char);
char* substring(char *string, int start_index, int last_index);
// statistics
int array_len(double *array);
double array_mean(char ***matrix, int col);
double standard_deviation(char ***matrix, int col);
double find_max(char ***matrix, int col);
double find_min(char ***matrix, int col);
t_feature* get_statistics(char ***matrix);
// check
int is_valid(char *s);
int is_number(char *s);
int is_valid_column(char ***matrix, int col);
#endif
