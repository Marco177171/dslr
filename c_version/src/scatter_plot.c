#include <data_frame.h>
#include <scatter_plot.h>
#include <describe.h>

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
	int h_1 = (int)(max_1 - min_1);

	double min_2 = find_min(df, feat_2);
	double max_2 = find_max(df, feat_2);
	int h_2 = (int)(max_2 - min_2);

	// double abs_min = min_1;
	// if (min_2 < min_1)
	// 	abs_min = min_2;

	// double abs_max = max_1;
	// if (max_2 > max_1)
	// 	abs_max = max_2;

	// int top_lines = 2;
	SDL_Window *window = SDL_CreateWindow("Scatter Plot", h_1, h_2, SDL_WINDOW_FULLSCREEN);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);

	int w_height;
	int w_width;
	SDL_GetWindowSize(window, &w_width, &w_height);

	// compute modulus:
	// draw initial grid
	SDL_SetRenderDrawColor(renderer, 35, 35, 35, 255);
	int i = 0;
	// while (i <= w_height) {
	// 	if (i % 10 == 0)
	// 		SDL_RenderLine(renderer, 0, i, w_width, i);
	// 	i++;
	// }
	while (i <= w_height) {
		SDL_RenderLine(renderer, 0, i, w_width, i); // devo accedere con l'&?
		i += 10;
	}
	i = 0;
	while (i <= w_width) {
		SDL_RenderLine(renderer, i, 0, i, w_height);
		i += 10;
	}
	SDL_RenderPresent(renderer);

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