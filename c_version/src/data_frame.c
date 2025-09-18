#include "../include/describe.h"
#include "../include/utils.h"

void print_data_frame(t_data_frame*** df)
{
    int c = 0;
	while (df[c]) {
		for (int i = 0; df[c][i]; i++) {
			if (df[c][i]->type == STRING) {
				printf("%s|", df[c][i]->s);
			} else {
				printf("%f|", df[c][i]->d);
			}
		}
		printf("\n");
		c++;
	}
}

void free_data_frame(t_data_frame*** df)
{
	int c = 0;
	while (df[c]) {
		for (int i = 0; df[c][i]; i++) {
			if (df[c][i]->type == STRING) {
				free(df[c][i]->s);
			}
			free(df[c][i]);
		}
		free(df[c]);
		c++;
	}
	free(df);
}

t_data_frame*** get_data_frame(char *file)
{
    FILE* csv_file = fopen(file, "r");
	
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
	rewind(csv_file);
	t_data_frame ***df = malloc(sizeof(t_data_frame**) * (c + 1));
	df[c] = NULL;
	c = 0;
	while (fgets(buffer, 2048, csv_file)) {
		df[c] = load_df(buffer, ',');
		c++;
	}

	fclose(csv_file);
    return df;
}