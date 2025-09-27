#ifndef DESCRIBE_H
#define DESCRIBE_H

#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

# include "data_frame.h"

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

// memory_managers.c
void free_statistics(t_feature *features);
// pinters.c
void print_matrix_vertically(t_data_frame ***df);
void print_matrix_column_vertically(t_data_frame ***df, int col_index);
void print_matrix_row(t_data_frame **df);
// statistics
int array_len(double *array);
double array_mean(t_data_frame ***df, int col);
double standard_deviation(t_data_frame ***df, double mean, int col);
double find_max(t_data_frame ***df, int col);
double find_min(t_data_frame ***df, int col);
void sort_column(t_data_frame ***df, int col);
t_feature* get_statistics(t_data_frame ***df);

#endif
