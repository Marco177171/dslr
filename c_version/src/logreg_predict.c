#include "../include/utils.h"
#include "../include/data_frame.h"

double adherence_percentage(double model, double current_value) {
	return ((current_value * 100) / model);
}

int find_closest(double g, double h, double r, double s) {
	
	if (g < 0)
		g *= -1;
	if (h < 0)
		h *= -1;
	if (r < 0)
		r *= -1;
	if (s < 0)
		s *= -1;
	
	if (g < h && g < r && g < s)
		return 1;
	else if (h < g && h < r && h < s)
		return 2;
	else if (r < g && r < h && r < s)
		return 3;
	else if (s < g && s < h && s < r)
		return 3;
	return 0;
}

void assign_student(FILE *result, double gryffindor, double hufflepuff, double ravenclaw, double slytherin) {
	double g = 100 - gryffindor;
	double h = 100 - hufflepuff;
	double r = 100 - ravenclaw;
	double s = 100 - slytherin;
	fprintf(result, "-> ");

	switch (find_closest(g, h, r, s)) {
		case 1:
			fprintf(result, "Gryffindor\n");
			break;
		case 2:
			fprintf(result, "Hufflepuff\n");
			break;
		case 3:
			fprintf(result, "Ravenclaw\n");
			break;
		case 4:
			fprintf(result, "Slytherin\n");
			break;
		default:
			return ;
	}
	return ;
}

void predict(t_data_frame ***df, t_data_frame ***avg) {
	
	FILE *debug = fopen("./debug_log.csv", "w");
	FILE *result = fopen("./result.csv", "w");

	double gryiffindor_score = 0;
	double hufflepuff_score = 0;
	double ravenclaw_score = 0;
	double slytherin_score = 0;

	double adherence = 0;
	double overall_distance = 0, distance_percentage = 0;

	int df_i = 1, df_j = 1;
	int h_i = 0, h_j = 1;

	while (df[df_i]) {
		fprintf(result, "\n// STUDENT: %s %s\n", df[df_i][2]->s, df[df_i][3]->s);

		gryiffindor_score = 0;
		hufflepuff_score = 0;
		ravenclaw_score = 0;
		slytherin_score = 0;

		h_i = 1;
		while (h_i <= 4) {
			df_j = 1;
			h_j = 1;
			adherence = 0;
			overall_distance = 0;
			while (df[df_i][df_j] && avg[h_i][df_j]) {
				if (is_valid_column(df, df_j)) {
					// deviazione tra il punteggio dello studente e punteggio medio della casa.
					fprintf(debug, "h : %d | %s / %s\n", h_i, df[0][df_j]->s, avg[0][df_j]->s);
					fprintf(debug, "avg: %f | stundent: %f\n", avg[h_i][df_j]->d, df[df_i][df_j]->d);
					adherence = adherence_percentage(avg[h_i][df_j]->d, df[df_i][df_j]->d);
					distance_percentage = 100 - adherence;
					overall_distance += distance_percentage;
					fprintf(debug, "adherence: %f %%\n", adherence);
					fprintf(debug, "\n");
					h_j++;
				}
				df_j++;
			}
			overall_distance /= h_j;

			if (h_i == 1) {
				gryiffindor_score = overall_distance;
				fprintf(result, "Gryffondor:\t%f\n", gryiffindor_score);
			}
			else if (h_i == 2) {
				hufflepuff_score = overall_distance;
				fprintf(result, "Hufflepuff:\t%f\n", hufflepuff_score);
			}
			else if (h_i == 3) {
				ravenclaw_score = overall_distance;
				fprintf(result, "Ravenclaw:\t%f\n", ravenclaw_score);
			}
			else if (h_i == 4) {
				slytherin_score = overall_distance;
				fprintf(result, "Slytherin:\t%f\n", slytherin_score);
			}
			
			h_i++;
		}
		assign_student(result, gryiffindor_score, hufflepuff_score, ravenclaw_score, slytherin_score);
		df_i++;
	}
}

int main(int argc, char **argv) {
	
	if (argc != 3) {
		perror("ERROR: usage: ./logreg_predict *csv_to_complete* *averages_csv_file*");
		exit(EXIT_FAILURE);
	}
	
	t_data_frame ***df = get_data_frame(argv[1]);
	t_data_frame ***avg = get_data_frame(argv[2]);
	predict(df, avg);

	free_data_frame(df);
	free_data_frame(avg);
	return 0;
}
