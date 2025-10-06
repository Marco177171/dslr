#include <describe.h>
#include <utils.h>

int array_len(double *array) {
	int c = 0;
	while (array[c])
		c++;
	return c;
}

int count(t_data_frame ***df, int col)
{
	int i = 0, sum = 0;
	while (df[i]) {
		if (df[i][col]->valid) {
			sum++;
		}
		i++;
	}
	return sum;
}

double array_mean(t_data_frame ***df, int col)
{
	int i = 0;
	double sum = 0, count = 0;
	while (df[i]) {
		if (df[i][col]->valid && df[i][col]->type == DOUBLE) {
			sum += df[i][col]->d;
			count++;
		}
		i++;
	}
	return sum / count;
}

double standard_deviation(t_data_frame ***df, double mean, int col)
{
	int i = 0, count = 0;
	double sums_squared = 0;
	while (df[i]) {
		if (df[i][col]->valid && df[i][col]->type == DOUBLE) {
			double d = df[i][col]->d;
			sums_squared += pow(d - mean, 2);
			count++;
		}
		i++;
	}
	return sqrt(sums_squared / count);
}

double find_max(t_data_frame ***df, int col) {
	double max = 0, current = 0;
	(*((unsigned long*)&max))= ~(1LL<<52);
	for (int i = 1; df[i]; i++) {
		if (df[i][col]->valid && df[i][col]->type == DOUBLE) {
			current = df[i][col]->d;
			if (current > max) {
				max = current;
			}
		}
	}
	return max;
}

double find_min(t_data_frame ***df, int col) {
	double min = 0, current = 0;
	for (int i = 1; df[i]; i++) {
		if (df[i][col]->valid && df[i][col]->type == DOUBLE) {
			current = df[i][col]->d;
			if (current < min)
				min = current;
		}
	}
	return min;
}

static int compare(const void *a, const void *b) {
	return (*(double *)a - *(double *)b);
}

void free_statistics(t_feature *features)
{
	t_feature *to_del;
	while (features) {
		to_del = features;
		features = features->next;
		if (to_del->stats.percentiles->percentage) free(to_del->stats.percentiles->percentage);
		if (to_del->stats.percentiles->d) free(to_del->stats.percentiles->d);
		if (to_del->stats.percentiles) free(to_del->stats.percentiles);
		free(to_del->name);
		free(to_del);
	}
}

static void date_percentile(t_data_frame ***df, int col, t_percentile *percentile)
{
	int i = 0;
	while (df[i])
		i++;
	int *arr = malloc((i - 1) * sizeof(int));

	for (int j = 1; df[j]; j++) {
		arr[j] = date_to_ms(df[j][col]->s);
	}
	qsort(arr, i - 1, sizeof(double), compare);
	int j = 0;
	for (int c = 0; c < i; c++) {
		if (c == (int)((double)i * percentile->percentage[j])) {
			percentile->s[j] = strdup(df[c][col]->s);
			j++;
		}
	}
	// free(arr);
	printf("qsort\n");
}

static void num_percentile(t_data_frame ***df, int col, t_percentile *percentile)
{
	int i = 0;
	while (df[i])
		i++;
	double *arr = malloc((i - 1) * sizeof(double));

	for (int j = 1; df[j]; j++) {
		if (df[j][col]->valid) {
			arr[j] = df[j][col]->d;
		}
	}
	qsort(arr, i - 1, sizeof(double), compare);
	int j = 0;
	for (int c = 0; c < i; c++) {
		if (c == (int)((double)i * percentile->percentage[j])) {
			percentile->d[j] = arr[c];
			j++;
		}
	}
	free(arr);
}

t_percentile* percentiles(t_data_frame ***df, int col, t_option *option, dtype type)
{
	t_percentile *percent_array = malloc(sizeof(t_percentile));
	if (option->arg) {
		char *param = option->arg;
		if (param[0] != '[' || param[strlen(param) - 1] != ']') {
			fprintf(stderr, "error: invalid parameter: %s\n", option->arg);
			return NULL;
		}
		param[strlen(param)] = 0;
		param++;
		char **n_options = split(param, ',');
		if (!n_options) {
			free(percent_array);
			return NULL;
		}
		percent_array->len = 0;
		while (n_options[percent_array->len]) percent_array->len++;
		percent_array->percentage = malloc(sizeof(double) * percent_array->len);
		for (int i = 0; i < percent_array->len; i++) {
			double d = atof(n_options[i]);
			if (d <= 0 || d >= 1) {
				fprintf(stderr, "error: percentile invalid range: %s. Must be between 0 and 1\n", n_options[i]);
				free(percent_array->percentage);
				free(percent_array->d);
				free(percent_array);
				free(n_options);
				return NULL;
			}
			percent_array->percentage[i] = d;
		}
		free(n_options);
	} else {
		percent_array->len = 3;
		percent_array->percentage = malloc(sizeof(double) * percent_array->len);
		percent_array->d = malloc(sizeof(char **) * percent_array->len);
		percent_array->percentage[0] = 0.25;
		percent_array->percentage[1] = 0.5;
		percent_array->percentage[2] = 0.75;
	}
	if (type == DOUBLE) {
		percent_array->d = malloc(sizeof(char **) * percent_array->len);
		num_percentile(df, col, percent_array);
	} else if (type == DATE) {
		percent_array->d = malloc(sizeof(char **) * percent_array->len);
		date_percentile(df, col, percent_array);
	} else {
		percent_array->d = NULL;
	}
	return percent_array;
}

static t_feature* numeric_describe(t_data_frame ***df, int col, t_option options[static 3])
{
	t_feature *new = malloc(sizeof(t_feature));
	new->type = DOUBLE;
	new->name = strdup(df[0][col]->s);
	new->count = count(df, col);
	new->stats.mean = array_mean(df, col);
	new->stats.std = standard_deviation(df, new->stats.mean, col);
	new->stats.max = find_max(df, col);
	new->stats.min = find_min(df, col);
	new->stats.percentiles = percentiles(df, col, &options[0], DOUBLE);
	if (!new->stats.percentiles) {
		if (new->name) free(new->name);
		free(new);
		return NULL;
	}
	new->next = NULL;
	return new;
}


static t_feature* string_describe(t_data_frame ***df, int col, t_option options[static 3])
{
	t_feature *new = calloc(1, sizeof(t_feature));
	new->type = STRING;
	new->name = strdup(df[0][col]->s);
	new->count = count(df, col);
	char *current_top = df[1][col]->s;
	int current_freq;
	for (int i = 1; df[i]; i++) {
		current_freq = 0;
		int is_unique = 1;
		for (int j = 1; df[j]; j++) {
			int cmp = strcmp(df[i][col]->s, df[j][col]->s);
			if (j != i && cmp == 0) {
				is_unique = 0;
				current_freq++;
			}
		}
		if (current_freq > new->stats.freq) {
			current_top = df[i][col]->s;
			new->stats.freq = current_freq;
		}
		new->stats.unique += is_unique;
	}
	new->stats.top = strdup(current_top);
	new->stats.percentiles = percentiles(df, col, &options[0], STRING);
	new->next = NULL;
	return new;
}

static t_feature* date_describe(t_data_frame ***df, int col, t_option options[static 3])
{
	t_feature *new = malloc(sizeof(t_feature));
	new->type = DATE;
	new->name = strdup(df[0][col]->s);
	new->count = count(df, col);
	new->stats.mean = array_mean(df, col);
	new->stats.std = standard_deviation(df, new->stats.mean, col);
	new->stats.max = find_max(df, col);
	new->stats.min = find_min(df, col);
	new->stats.percentiles = percentiles(df, col, &options[0], DATE);
	if (!new->stats.percentiles) {
		if (new->name) free(new->name);
		free(new);
		return NULL;
	}
	new->next = NULL;
	return new;
}

t_feature* get_statistics(t_data_frame ***df, t_option options[static 3])
{
	t_feature *results = NULL;
	
	for (int i = 1; df[1][i]; i++) {
		t_feature *new = NULL;
		switch (get_column_type(df, i))
		{
		case DOUBLE: {
			if(!options[include].arg ||
				(options[include].arg && !strcmp(options[include].arg, "all")) ||
				(options[include].arg && !strcmp(options[include].arg, "numeric"))) 
				new = numeric_describe(df, i, options);
			break;
		}
		case STRING: {
			if((options[include].arg && !strcmp(options[include].arg, "all")) ||
			  (options[include].arg && !strcmp(options[include].arg, "string")))
				new = string_describe(df, i, options);
			break;
		}
		case DATE: {
			if((options[include].arg && !strcmp(options[include].arg, "all")) ||
				(options[include].arg && !strcmp(options[include].arg, "datetime")))
				new = date_describe(df, i, options);
			break;
		}
		case OBJECT: break;
		}
		if (!results) {
			results = new;
		} else {
			t_feature *copy = results;
			while (copy->next) {
				copy = copy->next;
			}
			copy->next = new;
		}
	}
	return results;
}