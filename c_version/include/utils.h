#ifndef UTILS_H
#define UTILS_H

#include <data_frame.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char* substring(char *string, int begin, int end);
char **split(char *s, char limit_char);
t_data_frame **load_df(char *s, char limit_char);

int include_option(char **arr, char *opt);
int is_valid(char *s);
int is_number(char *s);
int is_date(char *s);
int is_valid_column(t_data_frame ***df, int col);
dtype get_column_type(t_data_frame ***df, int col);
size_t date_to_ms(char *s);

#endif