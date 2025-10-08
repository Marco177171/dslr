#include <describe.h>
#include <utils.h>

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
	if (!df) return;
	while (df[c]) {
		for (int i = 0; df[c][i]; i++) {
			if ((df[c][i]->valid && df[c][i]->type == STRING)
				|| (df[c][i]->valid && df[c][i]->type == DATE)) {
				free(df[c][i]->s);
			}
			if (df[c][i]) free(df[c][i]);
		}
		if (df[c]) free(df[c]);
		c++;
	}
	free(df);
}

t_data_frame*** get_data_frame(char *file)
{
    FILE* csv_file = fopen(file, "r");
	
	if (!csv_file) {
		perror("File not open. Quitting");
		exit(1);
	}

	char buffer[2048];
	int c = 0;

	while (fgets(buffer, 2048, csv_file))
		c++;

	rewind(csv_file);
	t_data_frame ***df = calloc(c + 1, sizeof(t_data_frame**));
	if (!df) {
		return NULL;
	}
	df[c] = NULL;
	c = 0;
	while (fgets(buffer, 2048, csv_file)) {
		df[c] = load_df(buffer, ',');
		c++;
	}

	fclose(csv_file);
    return df;
}