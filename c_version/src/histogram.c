#include "../include/data_frame.h"
#include "../include/histogram.h"
#include "../include/describe.h"

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

void print_current_line(int top_lines, int start, int color_number, int len) {
	attron(COLOR_PAIR(color_number));
	move(top_lines + start + color_number - 1, 0);
	hline('|', len);
	attroff(COLOR_PAIR(color_number));
}

void visualize_data(t_data_frame*** df, int col) {
	double min = find_min(df, col);
	double max = find_max(df, col);

	int height = (int)(max - min);
	int top_lines = 2;

	initscr(); // open visualization in terminal
	start_color(); // set color capabilities on
	init_pair(1, COLOR_YELLOW, COLOR_BLACK); // Gryffindor
	init_pair(2, COLOR_RED, COLOR_BLACK); // Hufflepuff
	init_pair(3, COLOR_WHITE, COLOR_BLACK); // Ravenclaw
	init_pair(4, COLOR_GREEN, COLOR_BLACK); // Slytherin

	addstr("Histogram for:\n");
	printw("%s\n", df[0][col]->s);

	int i = 0, j = 0, value = 0, counter = 0, color_number = 0;
	int len = 0;
	int start = 0;
	int gryffindor = 0, hufflepuff = 0, ravenclaw = 0, slytherin = 0;
	
	while (df[len]) len++;
	while (i < height) {
		gryffindor = 0;
		hufflepuff = 0;
		ravenclaw = 0;
		slytherin = 0;

		j = 0;
		start = i * 4;
		while (j < len) {
			value = (int)df[j][col]->d + abs((int)min);
			if (value == i) {
				color_number = define_color(df[j][1]->s);
				
				if (color_number == 1)
					gryffindor++;
				else if (color_number == 2)
					hufflepuff++;
				else if (color_number == 3)
					ravenclaw++;
				else if (color_number == 4)
					slytherin++;
				
				counter++;
			}
			j++;
		}
		print_current_line(top_lines, start, 1, gryffindor);
		print_current_line(top_lines, start, 2, hufflepuff);
		print_current_line(top_lines, start, 3, ravenclaw);
		print_current_line(top_lines, start, 4, slytherin);
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
		exit(EXIT_FAILURE);
	}

	t_data_frame*** df = get_data_frame(argv[1]);

	visualize_data(df, atoi(argv[2]));

	free_data_frame(df);
}
