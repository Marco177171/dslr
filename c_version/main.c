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

	char buffer[1024];
	int stopper = 0, c = 0;

	while (fgets(buffer, 1024, csv_file))
		c++;

	printf("%d lines counted in csv file\n\n", c);

	char* title_string;
	char** rows_array = malloc(sizeof(char*) * (c + 1));
	rows_array[c] = NULL;

	rewind(csv_file);
	int i = 0;
	while (fgets(buffer, 1024, csv_file)) {
		rows_array[i] = strdup(buffer);
		memset(buffer, 0, 1024);
		i++;
	}

	char ***matrix = malloc(sizeof(char**) * (i + 1));
	matrix[i] = NULL;
	i = 0;
	int j = 0;
	while (rows_array[i]) {
		matrix[i] = split(rows_array[i], ',');
		i++;
	}

	print_matrix_vertically(matrix);

	free_array(rows_array);
	free_matrix(matrix);
	fclose(csv_file);
	return 0;
}