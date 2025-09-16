#include "dslr.h"

void free_array(char** list) {
	int i = 0;
	
	while (list[i]) {
		free(list[i]);
		i++;
	}
	free(list);
}

void free_matrix(char*** matrix) {
	int i = 0;

	while (matrix[i]) {
		free_array(matrix[i]);
		i++;
	}
	
	free(matrix);
}

