#include "histogram.h"

void visualize_data(char ***matrix, int col) {
	double min = find_min(matrix, col);
	double max = find_max(matrix, col);

	// double *scores = malloc(sizeof(double*) * max - min);

	initscr(); // open visualization in terminal
	addstr("HISTOGRAM\n");
	addstr("Press any key to exit...\n");
	refresh(); // update window content
	getch(); // read a char from keyboard
	endwin(); // close windows
	clear(); // clear screen, back to terminal
	
	int i = 0;
	while (matrix[i]) {
		printf("%s\t%s\n", matrix[i][1], matrix[i][col]);
		hline('.', 42);
		i++;
	}
	printf("MIN : %f | MAX : %f\n", min, max);
}

int main(int argc, char **argv) {
	
	if (argc != 3) {
		perror("Wrong amount of arguments. Usage: ./histogram *csv_file* *column_index*\n");
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
	char ***matrix = malloc(sizeof(char**) * (c + 1));
	matrix[c] = NULL;
	
	rewind(csv_file);
	c = 0;
	while (fgets(buffer, 2048, csv_file)) {
		matrix[c] = split(buffer, ',');
		c++;
	}
	
	visualize_data(matrix, atoi(argv[2]));
	free_matrix(matrix);

	fclose(csv_file);
	return 0;
}