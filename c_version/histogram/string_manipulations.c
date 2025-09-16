#include "histogram.h"

char* substring(char *string, int begin, int end) {
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