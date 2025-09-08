#include "lib.h"

int array_len(float array[]) {
	int c = 0;
	while (array[c])
		c++;
	return c;
}

float array_mean(float array[]) {
	int i = 0, sum = 0;
	while (array[i]) {
		sum += array[i];
		i++;
	}
	return sum / i;
}

float standard_deviation(float array[]) {
	int i = 0;
	float mean = array_mean(array);
	float sums_squared = 0;
	while (array[i]) {
		sums_squared += pow(array[i] - mean, 2);
		i++;
	}
	float std = sqrt(sums_squared / (i + 1));
	return std;
}

void matrix_along_col(char ***matrix, int col_index) {
	int i = 0;
	while (matrix[i]) {
		printf("%s\n", matrix[i][col_index]);
		i++;
	}
}