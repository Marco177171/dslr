#include "histogram.h"

void print_array_vertically(char **array) {
	for (int i = 0; array[i]; i++)  {
		printf("%d:\t%s\n", i, array[i]);
		i++;
	}
}

void print_matrix_column_vertically(char ***matrix, int col_index) {
	for (int i = 0; matrix[i]; i++) {
		printf("%s\n", matrix[i][col_index]);
		i++;
	}
}

void print_matrix_vertically(char ***matrix) {
	for (int i = 0; matrix[i]; i++) {
        printf("Array at index %d:\n", i);
		print_array_vertically(matrix[i]);
		i++;
	}
}

void print_matrix_row(char **matrix)
{
	for (int i = 0; matrix[i]; i++) {
		printf("%s ", matrix[i]);
	}
}
