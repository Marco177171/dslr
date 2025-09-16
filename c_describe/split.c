#include "dslr.h"

char **split(char *s, char limit_char) {
	char **r;
	int i = 0, limit_count = 0;

	while (s[i]) {
		if (s[i] == limit_char)
			limit_count++;
		i++;
	}
	limit_count++;
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
			start = i + 1;
		}
		i++;
	}
	r[r_index] = substring(s, start, i - 1);
	return r;
}

t_data_frame **load_df(char *s, char limit_char) {
	t_data_frame **r;
	int i = 0, limit_count = 0;

	while (s[i]) {
		if (s[i] == limit_char)
			limit_count++;
		i++;
	}
	limit_count++;
	r = malloc(sizeof(t_data_frame*) * limit_count + 1);
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
			s[i] = 0;
			printf("i: %d\n", i);
			if (is_number(&s[start])) {
				r[r_index]->type = DOUBLE;
				r[r_index]->d = atof(&s[start]);
			} else {
				printf("s: %s\n", &s[start]);
				r[r_index]->type = STRING;
				r[r_index]->s = substring(s, start, i);
			}
			r_index++;
			start = i + 1;
		}
		i++;
	}
	s[i] = 0;
	if (is_number(&s[start])) {
		r[r_index]->type = DOUBLE;
		r[r_index]->d = atof(&s[start]);
	} else {
		r[r_index]->type = STRING;
		r[r_index]->s = substring(s, start, i - 1);
	}
	return r;
}