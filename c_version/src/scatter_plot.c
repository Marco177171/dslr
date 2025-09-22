#include "../include/data_frame.h"
#include "../include/scatter_plot.h"
#include "../include/describe.h"

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

// void print_current_dot(int color_number, int score_1, int score_2) {
// 	attron(COLOR_PAIR(color_number));
// 	mvaddch(2 + score_1, score_2, '+');
// 	attroff(COLOR_PAIR(color_number));
// }

void visualize_scatter_plot(t_data_frame*** df, int feat_1, int feat_2) {

	double min_1 = find_min(df, feat_1);
	double max_1 = find_max(df, feat_1);
	// int h_1 = (int)(max_1 - min_1);

	double min_2 = find_min(df, feat_2);
	double max_2 = find_max(df, feat_2);
	// int h_2 = (int)(max_2 - min_2);
	
	// int top_lines = 2;
	SDL_Window *window = SDL_CreateWindow("Scatter Plot", 1080, 720, SDL_WINDOW_FULLSCREEN);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL, 0);

	// SDL_Color *gryffondor;
	// SDL_Color *hufflepuff;
	// SDL_Color *ravenclaw;
	// SDL_Color *slytherin;

	SDL_SetRenderDrawColor(renderer, 35, 35, 35, 255);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);

	// initscr(); // open visualization in terminal
	// start_color(); // set color capabilities on
	// init_pair(1, COLOR_YELLOW, COLOR_BLACK); // Gryffindor
	// init_pair(2, COLOR_RED, COLOR_BLACK); // Hufflepuff
	// init_pair(3, COLOR_WHITE, COLOR_BLACK); // Ravenclaw
	// init_pair(4, COLOR_GREEN, COLOR_BLACK); // Slytherin

	// addstr("Scatter Plot for:\n");
	// printw("%s | %s\n", df[0][feat_1]->s, df[0][feat_2]->s);

	// int i = 0;
	// int len = 0; // jump first line of the CSV file
	// int color_number = 0;

	// while (df[len]) len++;
	// // cycle among students
	// while (i < len) {
	// 	color_number = define_color(df[i][1]->s);
	// 	attron(COLOR_PAIR(color_number));
	// 	mvaddch(top_lines + (int)df[i][feat_1]->d + abs((int)min_1), (int)df[i][feat_2]->d, '+');
	// 	attroff(COLOR_PAIR(color_number));
	// 	i++;
	// }

	// refresh(); // update window content
	// getch(); // read a char from keyboard
	// endwin(); // close windows
	// clear(); // clear screen, back to terminal
	SDL_Event event;
	bool running = true;
	while (running) {
		SDL_PollEvent(&event);
		switch (event.type)
		{
			case (SDL_EVENT_MOUSE_BUTTON_DOWN):
				SDL_DestroyWindow(window);
				SDL_Quit();
				running = false;
				break;
			
			default:
				break;
		}
	}
	
	printf("1: MIN : %f | MAX : %f\n", min_1, max_1);
	printf("2: MIN : %f | MAX : %f\n", min_2, max_2);
}

int main(int argc, char **argv) {

	if (argc != 4) {
		perror("Wrong amount of arguments. Usage: ./scatter_plot *csv_file* *first_feature_index* *second_feature_index*\n");
		exit(EXIT_FAILURE);
	}

	t_data_frame*** df = get_data_frame(argv[1]);
	visualize_scatter_plot(df, atoi(argv[2]), atoi(argv[3]));
	free_data_frame(df);
}