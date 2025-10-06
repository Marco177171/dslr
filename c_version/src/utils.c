#include <utils.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

int date_to_ms(char *s)
{
	struct tm t;

	t.tm_year = atoi(substring(s, 0, 4));
	t.tm_mon = atoi(substring(s, 5, 7));
	t.tm_mday = atoi(substring(s, 8, 10));
	t.tm_hour = 0;
	t.tm_min = 0;
	t.tm_sec = 0;

	return mktime(&t) * 1000;
}

int is_valid(char *s)
{
	return s[0] != 0;
}

int is_date(char *s) {
	if (strlen(s) != 10) return 0;
	if (s[4] != '-' || s[7] != '-') return 0;
	return (isdigit(s[0]) && isdigit(s[1]) && isdigit(s[2]) && isdigit(s[3]) &&
			isdigit(s[5]) && isdigit(s[6]) && isdigit(s[8]) && isdigit(s[9]));
}

int is_number(char *s)
{
	int discriminant = 0;
	for (int i = 0; s[i]; i++) {
		if ((s[i] >= '0' && s[i] <= '9') || s[i] == '.' || s[i] == '-') {
			if (s[i] == '-') discriminant++;
			if (discriminant > 1) return 0;
			continue;
		} else {
			return 0;
		}
	}
	return s != NULL;
}

dtype get_column_type(t_data_frame ***df, int col)
{
	dtype t = df[1][col]->type;
	
	for (int i = 1; df[i]; i++) {
		if (df[i][col]->valid && df[i][col]->type != t)
			return OBJECT;
	}
	return t;
}

int is_valid_column(t_data_frame ***df, int col)
{
	for (int i = 1; df[i]; i++) {
		if (df[i][col]->valid && df[i][col]->type == STRING)
			return 0;
	}
	return 1;
}

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

char **split(char *s, char limit_char) {
	char **r;
	int i = 0, limit_count = 0;

	while (s[i]) {
		if (s[i] == limit_char)
			limit_count++;
		i++;
	}
	limit_count++;
	r = calloc(limit_count + 1, sizeof(char*));
	if (!r) {
		perror("Malloc error for results list\n");
		exit(1);
	}
	r[limit_count] = NULL;
	
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
	t_data_frame **r = NULL;
	int i = 0, limit_count = 0;

	while (s[i]) {
		if (s[i] == limit_char)
			limit_count++;
		i++;
	}
	limit_count++;
	r = calloc(limit_count + 1, sizeof(t_data_frame*));
	if (!r) {
		perror("Malloc error for results list\n");
		return NULL;
	}
	r[limit_count] = NULL;
	
	int start = 0;
	int index = 0;
	i = 0;
	while (s[i]) {
		if (s[i] == limit_char) {
			s[i] = 0;
			r[index] = calloc(1, sizeof(t_data_frame));
			if (is_valid(&s[start])) {
				r[index]->valid = 1;
				if (is_number(&s[start])) {
					r[index]->type = DOUBLE;
					r[index]->d = atof(&s[start]);
				} else if (is_date(&s[start])) {
					r[index]->type = DATE;
					r[index]->s = substring(s, start, i);
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
	r[index] = calloc(1, sizeof(t_data_frame));
	if (is_valid(&s[start])) {
		r[index]->valid = 1;
		if (is_number(&s[start])) {
			r[index]->type = DOUBLE;
			r[index]->d = atof(&s[start]);
		} else if (is_date(&s[start])) {
			r[index]->type = DATE;
			r[index]->s = substring(s, start, i);
		} else {
			r[index]->type = STRING;
			r[index]->s = substring(s, start, i);
		}
	}
	return r;
}