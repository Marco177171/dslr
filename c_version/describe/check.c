#include "dslr.h"

int is_valid(char *s)
{
	return s[0] != 0;
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

int is_valid_column(t_data_frame ***df, int col)
{
	for (int i = 1; df[i]; i++) {
		if (df[i][col]->valid && df[i][col]->type == STRING)
			return 0;
	}
	return 1;
}