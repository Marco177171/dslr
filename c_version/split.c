#include "lib.h"

char **split(char *s, char limit_char) {
	char **r;
	int i = 0, limit_count = 0;

	while (s[i]) {
		if (s[i] == limit_char)
			limit_count++;
		i++;
	}

	// printf("found %d commas in : %s\n", limit_count, s);

	r = malloc(sizeof(char*) * limit_count + 1);
	r[limit_count] = NULL;
	if (!r) {
		perror("Malloc error for results list\n");
		exit(1);
	}

	int start = 0;
	int r_index = 0;
	i = 0;
	while (s[i]) {
		if (s[i] == limit_char) {
			r[r_index] = substring(s, start, i);
			r_index++;
			i++;
			start = i;
		}
		i++;
	}
	return r;
}