#ifndef DESCRIBE_H
#define DESCRIBE_H

#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
# include "data_frame.h"

/*
 * For object data (e.g. strings or timestamps),
 * the result’s index will include count, unique, top, and freq.
 * The top is the most common value. The freq is the most common value’s frequency.
 * Timestamps also include the first and last items.
*/

typedef struct s_percentile {
	int 				len;
	double 				*percentage;
	union {
		char			**s;
		double 			*d;
	};
}	t_percentile;

typedef struct s_statistics {
	int					unique; // number of unique values. NaN for numbers
	char				*top;	// most common value. NaN for numbers
	int					freq; // most common value’s frequency. NaN for numbers
	double 				std;
	double 				mean;
	union {
		char			*s;
		double 			d;
	}	min;
	t_percentile		*percentiles;
	union {
		char			*s;
		double 			d;
	}	max;
} t_statistics;

typedef struct s_feature {
	char 				*name;
	dtype				type;
	int					count;
	t_statistics		stats;
	struct s_feature 	*next;
}	t_feature;

typedef enum {
	PERCENTILE,
	INCLUDE,
	EXCLUDE,
	OPTIONS_COUNT
}	option_type;

typedef struct s_option {	
	option_type			type;
	char				**arg;
}	t_option;

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
t_feature* get_statistics(t_data_frame ***df, t_option options[3]);

#endif
