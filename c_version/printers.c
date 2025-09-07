#include "lib.h"

void print_array_vertically(char **array) {
	int i = 0;
	while (array[i]) {
		printf("%d:\t%s\n", i, array[i]);
		i++;
	}
}

void print_matrix_vertically(char ***matrix) {
	int i = 0;
	while (matrix[i]) {
        printf("Array at index %d:\n", i);
		print_array_vertically(matrix[i]);
		i++;
	}
}
