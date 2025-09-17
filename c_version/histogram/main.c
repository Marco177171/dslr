#include "histogram.h"

// COLORS = {
// 	'GRYFFINDOR': 'YELLOW',
// 	'HUFFLEPUFF': 'RED',
// 	'RAVENCLAW': 'GRAY',
// 	'SLYTHERIN': 'GREEN',
// }

int house_offset(char *hogwarts_house) {
	if (!strcmp(hogwarts_house, "Gryffindor"))
		return (0);
	else if (!strcmp(hogwarts_house, "Hufflepuff"))
		return (1);
	else if (!strcmp(hogwarts_house, "Ravenclaw"))
		return (2);
	else if (!strcmp(hogwarts_house, "Slytherin"))
		return (3);
	else
		return(COLOR_WHITE);
}

int define_color(char *hogwarts_house) {
	if (!strcmp(hogwarts_house, "Gryffindor"))
		return (1);
	else if (!strcmp(hogwarts_house, "Hufflepuff"))
		return (2);
	else if (!strcmp(hogwarts_house, "Ravenclaw"))
		return (3);
	else if (!strcmp(hogwarts_house, "Slytherin"))
		return (4);
	else
		return(COLOR_WHITE);
}

void visualize_data(char ***matrix, int col, int matrix_len) {
	double min = find_min(matrix, col);
	double max = find_max(matrix, col);

	int height = (int)(max - min);
	int top_lines = 1;

	
	initscr(); // open visualization in terminal
	start_color(); // set color capabilities on
	init_pair(1, COLOR_BLACK, COLOR_YELLOW);
	init_pair(2, COLOR_BLACK, COLOR_RED);
	init_pair(3, COLOR_BLACK, COLOR_BLACK);
	init_pair(4, COLOR_BLACK, COLOR_GREEN);
	addstr("HISTOGRAM\n");
	// printw("%s\n", matrix[0][1]);

	int i = 0, j = 0, value = 0, counter = 0, color_number = 0;
	while (i < height) {
		j = 0;
		counter = 0;
		while (j < matrix_len) {
			value = atoi(matrix[j][col]) + abs((int)min);
			// add char at the end of line at 'i'
			if (value == i) {
				color_number = define_color(matrix[j][1]);
				attron(COLOR_PAIR(color_number));
				mvhline(i + top_lines, counter, '|', value);
				attroff(COLOR_PAIR(color_number));
				counter++;
			}
			j++;
		}
		i++;
	}
	refresh(); // update window content
	getch(); // read a char from keyboard
	endwin(); // close windows
	clear(); // clear screen, back to terminal
	
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
	
	visualize_data(matrix, atoi(argv[2]), c);
	free_matrix(matrix);

	fclose(csv_file);
	return 0;
}