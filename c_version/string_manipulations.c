#include "lib.h"

char* substring(char *string, int begin, int end) {
	
	// printf("SUBSTRING INTRO - %sfirst = %d | last = %d\n", string, first_index, last_index);
	if (begin > end) {
		perror("Last index can't be smaller than the first one\n");
		exit(1);
	}

	int len = end - begin;
	char* r = malloc(sizeof(char) * (len + 1));
	r[len] = '\0';

	int i = 0;
	while (i < len) {
		r[i] = string[begin];
		begin++;
		i++;
	}

	return r;
}