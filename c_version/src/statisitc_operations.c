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

static int compare_strings(const void *a, const void *b) {
	const char *a1 = *(char**)a;
    const char *a2 = *(char**)b;

    return strcmp(a1, a2);;
}

void free_statistics(t_feature *features)
{
	t_feature *to_del;
	while (features) {
		to_del = features;
		features = features->next;
		if (to_del->stats.percentiles->percentage) free(to_del->stats.percentiles->percentage);
		if (to_del->type == DATE) {
			for (int i = 0; i < to_del->stats.percentiles->len; i++) {
				free(to_del->stats.percentiles->s[i]);
			}
			free(to_del->stats.percentiles->s);
			free(to_del->stats.max.s);
			free(to_del->stats.min.s);
		} else if (to_del->type == STRING) {
			free(to_del->stats.top);
		}else {
			if (to_del->stats.percentiles->d) free(to_del->stats.percentiles->d);
		}
		if (to_del->stats.percentiles) free(to_del->stats.percentiles);
		free(to_del->name);
		free(to_del);
	}
}

static void date_percentile(t_data_frame ***df, int col, t_statistics *stats)
{
	t_percentile *percentile = stats->percentiles;
	int i = 0;
	int count = -1;
	while (df[i]) {
		if (df[i][col]->valid) count++;
		i++;
	}
	char **arr = calloc(count + 1, sizeof(char *));
	arr[count] = 0;
	int j = 0;
	for (int k = 0; k < count; k++) {
		if (df[k + 1][col]->valid) {
			arr[j++] = strdup(df[k + 1][col]->s);
		}
	}
	
	qsort(arr, count, sizeof(char *), compare_strings);
	
	stats->min.s = strdup(arr[0]);
	stats->max.s = strdup(arr[count - 1]);
	j = 0;
	for (int c = 0; c < i; c++) {
		if (c == (int)((double)i * percentile->percentage[j])) {
			percentile->s[j] = arr[c];
			j++;
		} else {
			free(arr[c]);
		}
	}
	// for (int k = 0; arr[k]; k++) free(arr[k]);
	free(arr);
}

static void num_percentile(t_data_frame ***df, int col, t_statistics *stats)
{
	t_percentile *percentile = stats->percentiles;
	int i = 0;
	int count = -1;
	while (df[i]) {
		if (df[i][col]->valid) count++;
		i++;
	}
	double *arr = calloc(count, sizeof(double));
	
	int j = 0;
	for (int k = 0; k < count; k++) {
		if (df[k + 1][col]->valid) {
			arr[j++] = df[k + 1][col]->d;
		}
	}

	qsort(arr, count, sizeof(double), compare);
	stats->min.d = arr[0];
	stats->max.d = arr[count - 1];
	j = 0;
	for (int c = 0; c < i; c++) {
		if (c == (int)((double)i * percentile->percentage[j])) {
			percentile->d[j] = arr[c];
			j++;
		}
	}
	free(arr);
}


t_percentile* percentiles(t_option *option, dtype type)
{
	t_percentile *percent_array = malloc(sizeof(t_percentile));

	if (option->arg) {
		char **n_options = option->arg;
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
		percent_array->percentage[0] = 0.25;
		percent_array->percentage[1] = 0.5;
		percent_array->percentage[2] = 0.75;
	}
	if (type == DOUBLE) percent_array->d = malloc(sizeof(double) * percent_array->len);
	else if (type == DATE) percent_array->s = malloc(sizeof(char *) * percent_array->len);
	else percent_array->s = NULL;
	return percent_array;
}

void percent_min_max(t_data_frame ***df, int col, t_option *option, dtype type, t_statistics *stats)
{
	stats->percentiles = percentiles(option, type);
	if (!stats->percentiles) return;
	if (type == DOUBLE) num_percentile(df, col, stats);
	else if (type == DATE) date_percentile(df, col, stats);
}

static t_feature* numeric_describe(t_data_frame ***df, int col, t_option options[static 3])
{
	t_feature *new = malloc(sizeof(t_feature));
	new->type = DOUBLE;
	new->name = strdup(df[0][col]->s);
	new->count = count(df, col);
	new->stats.mean = array_mean(df, col);
	new->stats.std = standard_deviation(df, new->stats.mean, col);
	percent_min_max(df, col, &options[PERCENTILE], DOUBLE, &new->stats);
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
	percent_min_max(df, col, &options[PERCENTILE], STRING, &new->stats);
	if (!new->stats.percentiles) {
		if (new->name) free(new->name);
		free(new);
		return NULL;
	}
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
	percent_min_max(df, col, &options[PERCENTILE], DATE, &new->stats);
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
			if(!options[INCLUDE].arg ||
				(options[INCLUDE].arg && include_option(options[INCLUDE].arg, "all")) ||
				(options[INCLUDE].arg && include_option(options[INCLUDE].arg, "numeric"))) 
				new = numeric_describe(df, i, options);
			break;
		}
		case STRING: {
			if((options[INCLUDE].arg && include_option(options[INCLUDE].arg, "all")) ||
			  (options[INCLUDE].arg && include_option(options[INCLUDE].arg, "string")))
				new = string_describe(df, i, options);
			break;
		}
		case DATE: {
			if((options[INCLUDE].arg && include_option(options[INCLUDE].arg, "all")) ||
				(options[INCLUDE].arg && include_option(options[INCLUDE].arg, "datetime")))
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