#include <data_frame.h>
#include <scatter_plot.h>
#include <describe.h>

void define_color(char *hogwarts_house, SDL_Renderer *renderer) {
	if (!strcmp(hogwarts_house, "Gryffindor"))
		SDL_SetRenderDrawColor(renderer, 242, 255, 94, 255);
	else if (!strcmp(hogwarts_house, "Hufflepuff"))
		SDL_SetRenderDrawColor(renderer, 255, 69, 66, 255);
	else if (!strcmp(hogwarts_house, "Ravenclaw"))
		SDL_SetRenderDrawColor(renderer, 102, 102, 102, 255);
	else if (!strcmp(hogwarts_house, "Slytherin"))
		SDL_SetRenderDrawColor(renderer, 88, 255, 66, 255);
}

// void print_current_dot(int color_number, int score_1, int score_2) {
// 	attron(COLOR_PAIR(color_number));
// 	mvaddch(2 + score_1, score_2, '+');
// 	attroff(COLOR_PAIR(color_number));
// }

void visualize_scatter_plot(t_data_frame*** df, int feat_1, int feat_2) {

	double min_1 = find_min(df, feat_1);
	double max_1 = find_max(df, feat_1);
	double f_one_extension = max_1 - min_1;

	double min_2 = find_min(df, feat_2);
	double max_2 = find_max(df, feat_2);
	double f_two_extension = max_2 - min_2;

	// double abs_min = min_1;ls
	// if (min_2 < min_1)
	// 	abs_min = min_2;
	
	// double abs_max = max_1;
	// if (max_2 > max_1)
	// 	abs_max = max_2;
	// int top_lines = 2;
	
	SDL_Window *window;
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not initialize SDL: %s", SDL_GetError());
        return ;
    }
	window = SDL_CreateWindow("Scatter Plot", 640, 480, SDL_WINDOW_FULLSCREEN);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
	SDL_SyncWindow(window);
	
	int w_height = 0;
	int w_width = 0;
	SDL_GetWindowSizeInPixels(window, &w_width, &w_height);

	double module_f_one = (w_width / f_one_extension); // orizzontale
	double module_f_two = (w_height / f_two_extension); // verticale
	
	printf("h %f w %f\n", module_f_one, module_f_two);
	printf("h %d w %d\n", w_width, w_height);

	// compute modulus:
	// draw initial grid
	SDL_SetRenderDrawColor(renderer, 35, 35, 35, 255);
	int i = 0;
	while (i <= w_height) {
		if (i % (int)module_f_one == 0)
			SDL_RenderLine(renderer, 0, i, w_width, i);
		i++;
	}
	
	i = 0;
	while (i <= w_width) {
		if (i % (int)module_f_two == 0)
			SDL_RenderLine(renderer, i, 0, i, w_height);
		i++;
	}

	i = 1;
	// int house_color = 0;
	while (df[i]) {
		define_color(df[i][1]->s, renderer);
		SDL_RenderPoint(renderer, (int)(module_f_one * df[i][feat_1]->d), (int)(module_f_two * df[i][feat_2]->d));
		i++;
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