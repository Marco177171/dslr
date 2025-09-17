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
	int index = 0;
	i = 0;
	while (s[i]) {
		if (s[i] == limit_char) {
			r[index] = substring(s, start, i);
			index++;
			start = i + 1;
		}
		i++;
	}
	r[index] = substring(s, start, i - 1);
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
	int index = 0;
	i = 0;
	while (s[i]) {
		if (s[i] == limit_char) {
			s[i] = 0;
			r[index] = malloc(sizeof(t_data_frame));
			if (is_valid(&s[start])) {
				r[index]->valid = 1;
				if (is_number(&s[start])) {
					r[index]->type = DOUBLE;
					r[index]->d = atof(&s[start]);
				} else {
					r[index]->type = STRING;
					r[index]->s = substring(s, start, i);
				}
			}
			index++;
			start = i + 1;
		}
		i++;
	}

	if (s[i -1] == '\n') s[i - 1] = 0;
	r[index] = malloc(sizeof(t_data_frame));
	if (is_valid(&s[start])) {
		r[index]->valid = 1;
		if (is_number(&s[start])) {
			r[index]->type = DOUBLE;
			r[index]->d = atof(&s[start]);
		} else {
			r[index]->type = STRING;
			r[index]->s = substring(s, start, i);
		}
	}
	// printf("before return\n");
	return r;
}