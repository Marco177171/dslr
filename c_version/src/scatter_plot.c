#include <data_frame.h>
#include <scatter_plot.h>
#include <describe.h>

void define_color(char *hogwarts_house, SDL_Renderer *renderer) {
	if (!strcmp(hogwarts_house, "Gryffindor"))
		SDL_SetRenderDrawColor(renderer, 242, 255, 94, 255);
	else if (!strcmp(hogwarts_house, "Hufflepuff"))
		SDL_SetRenderDrawColor(renderer, 255, 69, 66, 255);
	else if (!strcmp(hogwarts_house, "Ravenclaw"))
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	else if (!strcmp(hogwarts_house, "Slytherin"))
		SDL_SetRenderDrawColor(renderer, 88, 255, 66, 255);
}

void draw_origin(SDL_Renderer *renderer, 
	int f1_origin, int f2_origin,
	int w_width, int w_height) {
	
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderLine(renderer, f1_origin, 0, f1_origin, w_height); // x axis
	SDL_RenderLine(renderer, 0, f2_origin, w_width, f2_origin); // y axis
}

void draw_points(SDL_Renderer *renderer, t_data_frame ***df, 
	int feat_1, int feat_2, 
	double f_one_unit, double f_two_unit, 
	int f_one_origin, int f_two_origin)
	{
	int i = 0;
	while (df[i]) {
		define_color(df[i][1]->s, renderer);
		SDL_RenderPoint(renderer, 
			f_one_origin + (int)(df[i][feat_1]->d * f_one_unit), 
			f_two_origin - (int)(df[i][feat_2]->d * f_two_unit));
		i++;
	}
}

void visualize_scatter_plot(t_data_frame*** df, int feat_1, int feat_2) {

	double min_1 = find_min(df, feat_1);
	double max_1 = find_max(df, feat_1);
	double ext_one = max_1 - min_1;

	double min_2 = find_min(df, feat_2);
	double max_2 = find_max(df, feat_2);
	double ext_two = max_2 - min_2;
	
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
	SDL_GetWindowSizeInPixels(window, &w_width, &w_height); // store window size in variables

	double f_one_unit = (w_width / ext_one); // horizontal unit
	double f_two_unit = (w_height / ext_two); // vertical unit

	int f1_origin = abs((int)(min_1 * f_one_unit));
	int f2_origin = (int)(max_2 * f_two_unit);
	
	printf("h %f w %f\n", f_one_unit, f_two_unit);
	printf("h %d w %d\n", w_width, w_height);

	draw_origin(renderer, f1_origin, f2_origin, w_width, w_height);
	draw_points(renderer, df, feat_1, feat_2, f_one_unit, f_two_unit, f1_origin, f2_origin);
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