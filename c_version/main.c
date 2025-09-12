#include "lib.h"

int main(int argc, char **argv) {
	
	if (argc != 2) {
		perror("Wrong amount of arguments\n");
		exit(1);
	}

	FILE* csv_file = fopen(argv[1], "r");
	
	if (csv_file)
		printf("CSV File open\n\n");

	else {
		perror("File not open. Quitting");
		exit(1);
	}

	char buffer[2048];
	int c = 0;

	while (fgets(buffer, 2048, csv_file))
		c++;

	printf("%d lines counted in csv file\n\n", c);
	rewind(csv_file);
	char ***matrix = malloc(sizeof(char**) * (c + 1));
	matrix[c] = NULL;
	c = 0;
	while (fgets(buffer, 2048, csv_file)) {
		matrix[c] = split(buffer, ',');
		c++;
	}

	for (int i = 0; matrix[i]; i++) {
		for (int j = 0; matrix[i][j]; j++) {
			printf("%s,", matrix[i][j]);
		}
		printf("\n");
	}

	// matrix_along_col(matrix, 3);
	free_matrix(matrix);

	fclose(csv_file);
	return 0;
}