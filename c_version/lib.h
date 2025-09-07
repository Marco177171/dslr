#ifndef LIB_H
#define LIB_H

#include <unistd.h>
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

char**  split(char *string, char delimiter);
char*   substring(char *string, int start_index, int last_index);

#endif
