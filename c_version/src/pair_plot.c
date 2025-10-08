#include <pair_plot.h>
#include <utils.h>
#include <describe.h>

void divide_window(SDL_Renderer *renderer, int section_width, int section_height, int w_width, int w_height) {
	SDL_SetRenderDrawColor(renderer, 35, 35, 35, 255); // set color
	
	// vertical lines
	int i = 0;
	while (i <= w_width) {
		SDL_RenderLine(renderer, i, 0, i, w_height);
		i += section_width;
	}
	// horizontal lines
	i = 0;
	while (i <= w_height) {
		SDL_RenderLine(renderer, 0, i, w_width, i);
		i += section_height;
	}
}

void draw_pair_in_window(SDL_Renderer *renderer, t_data_frame ***df,
	int i, int j,
	int row, int col,
	int section_width, int section_height) {

	int min_i = find_min(df, i);
	int max_i = find_max(df, i);
	int ext_i = max_i - min_i; // estensione massima della colonna i

	int min_j = find_min(df, j);
	int max_j = find_max(df, j);
	int ext_j = max_j - min_j; // estensione massima della colonna j
	
	int start_x = col * section_width;
	int start_y = row * section_height;

	int px = 0;
	int x_off = 0;
	int py = 0;
	int y_off = 0;

	int index = 1;
	while (df[index]) {
		x_off = (int)(section_width / ext_i) * abs((int)(df[index][i]->d));
		y_off = (int)(section_height / ext_j) * abs((int)(df[index][j]->d));

		px = start_x + x_off;
		py = start_y + y_off;

		if (!strcmp(df[i][1]->s, "Gryffindor"))
			SDL_SetRenderDrawColor(renderer, 242, 255, 94, 255);
		else if (!strcmp(df[i][1]->s, "Hufflepuff"))
			SDL_SetRenderDrawColor(renderer, 255, 69, 66, 255);
		else if (!strcmp(df[i][1]->s, "Ravenclaw"))
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		else if (!strcmp(df[i][1]->s, "Slytherin"))
			SDL_SetRenderDrawColor(renderer, 88, 255, 66, 255);
		
		SDL_RenderPoint(renderer, px, py);
		index++;
	}
}

void pair_plot(t_data_frame ***df) {
	// open sdl window
	SDL_Window *window;
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not initialize SDL: %s", SDL_GetError());
		return ;
	}
	window = SDL_CreateWindow("Pair Plot", 720, 480, SDL_WINDOW_FULLSCREEN);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
	SDL_SyncWindow(window);

	// find width and height
	int w_height = 0;
	int w_width = 0;
	SDL_GetWindowSizeInPixels(window, &w_width, &w_height); // store window size in variables

	// count valid columns
	int i = 0, count = 0;
	while (df[0][i]) {
		if (is_valid_column(df, i))
			count++;
		i++;
	}
	int section_width = (int)(w_width / count);
	int section_height = (int)(w_height / count);

	// draw sections in the window
	divide_window(renderer, section_width, section_height, w_width, w_height);
	// we now have both w & h for the single visualization

	// cycle the data frame
	i = 0;
	int j = 0;
	int row = 0, col = 0;
	while (df[0][i]) {
		// per ogni colonna valida...
		if (is_valid_column(df, i)) {
			col = 0;
			// crea un ciclo contro tutte le altre valide
			j = 0;
			while (df[0][j]) {
				if (is_valid_column(df, j)) {
					draw_pair_in_window(renderer, df, 
						i, j,
						row, col,
						section_width, section_height);
					col++;
				}
				j++;
			}
			row++;
		}
		i++;
	}

	SDL_RenderPresent(renderer);

	SDL_Event event;
	bool running = true;
	while (running) {
		SDL_PollEvent(&event);
		switch (event.type) {
			case (SDL_EVENT_MOUSE_BUTTON_DOWN):
				SDL_DestroyWindow(window);
				SDL_Quit();
				running = false;
				break;
			
			default:
				break;
		}
	}
}

int main(int argc, char **argv) {
	if (argc != 2) {
		perror("ERROR: usage : ./pair_plot *dataset_train.csv*");
		exit(EXIT_FAILURE);
	}

	t_data_frame ***df = get_data_frame(argv[1]);
	if (!df) {
		perror("ERROR: usage : could not generate data frame");
		exit(EXIT_FAILURE);
	}
	pair_plot(df);
	free_data_frame(df);
	return 0;
}