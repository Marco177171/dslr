#include "dslr.h"

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
		if (df[i][col]->valid && df[i][col]->type == DOUBLE) {
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

double standard_deviation(t_data_frame ***df, int col)
{
	int i = 0, count = 0;
	double mean = array_mean(df, col);
	double sums_squared = 0;
	while (df[i]) {
		if (df[i][col]->valid && df[i][col]->type == DOUBLE) {
			double d = df[i][col]->d;
			sums_squared += pow(d - mean, 2);
			count++;
		}
		i++;
	}
	return sqrt(sums_squared / (count + 1));
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

void sort_column(t_data_frame ***df, int col) {
	// funziona ma é lento in culo
	t_data_frame *temp;
	for (int i = 1; df[i]; i++) {
		if (df[i][col] && df[i][col]->valid && df[i][col]->type == DOUBLE) {
			if (df[i + 1] && df[i + 1][col] && df[i + 1][col]->valid && df[i + 1][col]->type == DOUBLE) {
				if (df[i][col]->d > df[i + 1][col]->d) {
					temp = df[i][col];
					df[i][col] = df[i + 1][col];
					df[i + 1][col] = temp;
					i = 1;
				}
			}
		}
	}
}

int compare(const void *a, const void *b) {
	return (*(double *)a - *(double *)b);
}

void percentiles(t_data_frame ***df, int col, double *l, double *m, double *h)
{
	int i = 0;
	while (df[i]) i++;
	double *arr = malloc(sizeof(double) * (i - 1));

	for (int j = 1; df[j]; j++) {
		if (df[j][col]->valid) {
			arr[j] = df[j][col]->d;
		}
	}
	// sort_column(df, col);
	qsort(arr, i - 1, sizeof(double), compare);
	printf("%f %f %f\n",*l, *m, *h);
	for (int j = 0; j < i - 1; j++) {
		printf("%f\n", arr[j]);
	}
	free(arr);
}

t_feature* get_statistics(t_data_frame ***df)
{
	t_feature *results = NULL;
	for (int i = 0; df[1][i]; i++) {
		if (is_valid_column(df, i)) {
			t_feature *new = malloc(sizeof(t_feature));
			new->name = strdup(df[0][i]->s);
			new->stats.count = count(df, i);
			new->stats.std = standard_deviation(df, i);
			new->stats.mean = array_mean(df, i);
			new->stats.max = find_max(df, i);
			new->stats.min = find_min(df, i);
			percentiles(df, i, &new->stats.twentyfive, &new->stats.fifty, &new->stats.seventyfive);
			new->next = NULL;
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
	}
	return results;
}