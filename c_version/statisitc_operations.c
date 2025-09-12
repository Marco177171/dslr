#include "lib.h"

int array_len(double *array) {
	int c = 0;
	while (array[c])
		c++;
	return c;
}

int count(char ***matrix, int col)
{
	int i = 0, sum = 0;
	while (matrix[i]) {
		if (is_valid(matrix[i][col])
			&& is_number(matrix[i][col])) {
			sum++;
		}
		i++;
	}
	return sum;
}

double array_mean(char ***matrix, int col)
{
	int i = 0, sum = 0, count = 0;
	while (matrix[i]) {
		if (is_valid(matrix[i][col])
			&& is_number(matrix[i][col])) {
			sum += atof(matrix[i][col]);
			count++;
		}
		i++;
	}
	return sum / count;
}

double standard_deviation(char ***matrix, int col)
{
	int i = 0, count = 0;
	double mean = array_mean(matrix, col);
	// printf("after array mean\n");
	double sums_squared = 0;
	while (matrix[i]) {
		if (is_valid(matrix[i][col])
			&& is_number(matrix[i][col])) {
			double d = atof(matrix[i][col]);
			sums_squared += pow(d - mean, 2);
			count++;
		}
		i++;
	}
	return sqrt(sums_squared / (count + 1));
}

double find_max(char ***matrix, int col) {
	double max = 0, current = 0;
	for (int i = 0; matrix[i]; i++) {
		current = atof(matrix[i][col]);
		if (current > max)
			max = current;
	}
	return max;
}

double find_min(char ***matrix, int col) {
	double min = 0, current = 0;
	for (int i = 0; matrix[i]; i++) {
		current = atof(matrix[i][col]);
		if (current < min)
			min = current;
	}
	return min;
}

t_feature* get_statistics(char ***matrix)
{
	t_feature *results = NULL;
	for (int i = 0; matrix[1][i]; i++) {
		if (is_valid_column(matrix, i)) {
			t_feature *new = malloc(sizeof(t_feature));
			new->name = strdup(matrix[0][i]);
			new->stats.count = count(matrix, i);
			new->stats.std = standard_deviation(matrix, i);
			new->stats.mean = array_mean(matrix, i);
			new->stats.max = find_max(matrix, i);
			new->stats.min = find_min(matrix, i);
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