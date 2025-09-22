#ifndef UTILS_H
#define UTILS_H

#include <data_frame.h>
#include <stdlib.h>
#include <stdio.h>

char* substring(char *string, int begin, int end);
char **split(char *s, char limit_char);
t_data_frame **load_df(char *s, char limit_char);

int is_valid(char *s);
int is_number(char *s);
int is_valid_column(t_data_frame ***df, int col);

#endif