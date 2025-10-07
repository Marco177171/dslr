#include <data_frame.h>
#include <histogram.h>
#include <describe.h>
#include <stdlib.h>

void draw_grid_from_origin(SDL_Renderer *renderer, 
	int origin_x, int origin_y,
	double f1_unit, double f2_unit,
	int w_width, int w_height) {
		
	(void)f2_unit;
	(void) origin_y;
	// draw vertical lines
	int i = origin_x;
	SDL_SetRenderDrawColor(renderer, 35, 35, 35, 255);

	int p = 1;
	char value[16];

	while (i <= w_width) {
		sprintf(value, "%d", p);
		SDL_RenderDebugText(renderer, i + 10, 30, value);
		SDL_RenderLine(renderer, i, 0, i, w_height); // x negative axis
		i += f1_unit;
		p *= 10;
	}
	i = origin_x;
	p = 1;
	while (i >= 0) {
		sprintf(value, "%d", p);
		SDL_RenderDebugText(renderer, i + 10, 30, value);
	    SDL_RenderLine(renderer, i, 0, i, w_height); // x positive axis
		i -= f1_unit;
		p *= 10;
	}

    // // draw horizontal lines
	// i = origin_y + f2_unit;
	// while (i <= w_height) {
	// 	SDL_RenderLine(renderer, 0, i, w_width, i); // y negative axis
	// 	i += f2_unit;
	// }
	// i = origin_y - f2_unit;
	// while (i >= 0) {
	// 	SDL_RenderLine(renderer, 0, i, w_width, i); // y positive axis
	// 	i -= f2_unit;
	// }

    // draw origin
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    // SDL_RenderLine(renderer, 0, origin_y, w_width, origin_y); // x axis
	SDL_RenderLine(renderer, origin_x, 0, origin_x, w_height); // y axis
}

void draw_students_at_score(SDL_Renderer *renderer, 
	double width_unit, double height_unit,
	int min,
	int score, int w_width, int w_height,
	int gryffindor, int hufflepuff, int ravenclaw, int slytherin) {
	
	(void) height_unit;
	(void) w_width;
	int current_score = ((score - min) * (int)(width_unit));
	// print gryffindor line
	SDL_SetRenderDrawColor(renderer, 242, 255, 94, 255);
	int house_width = width_unit / 4;
	int i = 0;
	while (i < house_width) {
		SDL_RenderLine(renderer, 
			current_score + i, w_height, 
			current_score + i, w_height - gryffindor);
		i++;
	}
	// print hufflepuff line
	SDL_SetRenderDrawColor(renderer, 255, 69, 66, 255);
	i = 0;
	while (i < house_width) {
		SDL_RenderLine(renderer, 
			current_score + i + house_width, w_height, 
			current_score + i + house_width, w_height - hufflepuff);
		i++;
	}
	// print ravenclaw line
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	i = 0;
	while (i < house_width) {
		SDL_RenderLine(renderer, 
			current_score + i + (house_width * 2), w_height, 
			current_score + i + (house_width * 2), w_height - ravenclaw);
		i++;
	}
	// print slytherin line
	SDL_SetRenderDrawColor(renderer, 88, 255, 66, 255);
	i = 0;
	while (i < house_width) {
		SDL_RenderLine(renderer, 
			score * (int)width_unit + i + (house_width * 3), w_height, 
			score * (int)width_unit + i + (house_width * 3), w_height - slytherin);
		i++;
	}
}

void visualize_data(t_data_frame*** df, int col) {
	
	double min = find_min(df, col);
	double max = find_max(df, col);
	double ext = max - min;
	
	SDL_Window *window;
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not initialize SDL: %s", SDL_GetError());
		return ;
	}
	window = SDL_CreateWindow("Histogram", 720, 480, SDL_WINDOW_FULLSCREEN);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
	SDL_SyncWindow(window);

	int w_height = 0;
	int w_width = 0;
	SDL_GetWindowSizeInPixels(window, &w_width, &w_height); // store window size in variables

	int i = 0;
	while (df[i]) i++;
	int width_unit = (int)(w_width / ext);
	int height_unit = (int)(w_height / ext);

	int origin_x = abs((int)(min * width_unit));
	int origin_y = max * height_unit;

	draw_grid_from_origin(renderer, 
		origin_x, origin_y, 
		width_unit, height_unit, 
		w_width, w_height);
	
	SDL_RenderDebugText(renderer, 10, 10, df[0][col]->s);

	int gryffindor = 0;
	int hufflepuff = 0;
	int ravenclaw = 0;
	int slytherin = 0;

	int score = (int)min;
	while (score <= (int)max) {
		gryffindor = 0;
		hufflepuff = 0;
		ravenclaw = 0;
		slytherin = 0;

		i = 0;
		while (df[i]) {
			if ((int)df[i][col]->d == (int)score) {
				if (!strcmp(df[i][1]->s, "Gryffindor"))
					gryffindor++;
				else if (!strcmp(df[i][1]->s, "Hufflepuff"))
					hufflepuff++;
				else if (!strcmp(df[i][1]->s, "Ravenclaw"))
					ravenclaw++;
				else if (!strcmp(df[i][1]->s, "Slytherin"))
					slytherin++;
			}
			i++;
		}
		draw_students_at_score(renderer, 
			width_unit, height_unit, 
			min,
			score, w_width, w_height, 
			gryffindor, hufflepuff, ravenclaw, slytherin);
		score++;
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
