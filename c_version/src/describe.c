#include <describe.h>
#include <data_frame.h>

const char *info = "\
Usage: predict [OPTIONS]... [FILE.csv]...\n\
Generate descriptive statistics.\n\
Descriptive statistics include those that summarize the central tendency, dispersion and shape of a dataset's distribution, excluding NaN values.\n\
\n\
Analyzes both numeric and object series, as well as DataFrame column sets of mixed data types.\n\
\n\
options:\n\
-p --percentile=[] The percentiles to include in the output. All should fall between 0 and 1.\n\
                   The default is [.25, .5, .75], which returns the 25th, 50th, and 75th percentiles.\n\n\
-i --include=[]    A white list of data types to include in the result.\n\
                   - 'all': All columns of the input will be included in the output.\n\
                   - a list-like of dtypes: limits the result to the provided data types.\n\
                   Valid values are: 'numeric', 'datetime', 'string'.\n\n\
-e --exclude=[]    A black list of data types to omit from the result.\n\
                   - a list-like of dtypes: limits the result to the provided data types.\n\
                   Valid values are: 'numeric', 'datetime', 'string'.\n";

int is_datetime(char *s) {
	// 2001-09-21
	if (strlen(s) != 10) return 0;
	if (s[4] != '-' || s[7] != '-') return 0;
	return (isdigit(s[0]) && isdigit(s[1]) && isdigit(s[2]) && isdigit(s[3]) &&
			isdigit(s[5]) && isdigit(s[6]) && isdigit(s[8]) && isdigit(s[9]));
}

void parse_options(t_options options[static 3], char **argv) {
	int i = 0;
	while (argv[i]) {
		if (argv[i][0] == '-') {
			if (argv[i][1] == '-') {
				if (!strncmp(argv[i], "--percentile=", 13)) options[0].arg = strdup(argv[i] + 13);
				else if (!strncmp(argv[i], "--include=", 10)) options[0].arg = strdup(argv[i] + 10);
				else if (!strncmp(argv[i], "--exclude=", 10)) options[0].arg = strdup(argv[i] + 10);
				else {
					fprintf(stderr, "error: invalid option: %s\n", argv[i]);
					exit(1);
				}
			} else {
				switch (argv[i][1])
				{
				case 'p': options[0].arg = strdup(argv[i] + 3); break;
				case 'i': options[1].arg = strdup(argv[i] + 3); break;
				case 'e': options[2].arg = strdup(argv[i] + 3); break;
				default: fprintf(stderr, "error: invalid option: %s\n", argv[i]); exit(1);
				}
			}
		}
		i++;
	}
}

int main(int argc, char **argv) {
	
	if (argc != 2) {
		printf("%s", info);
		exit(1);
	}

	t_options options[3] = {{.type=percentile}, {.type=include}, {.type=exclude}};
	parse_options(options, argv);
	t_data_frame ***df = get_data_frame(argv[1]);

	t_feature *features = get_statistics(df);
	t_feature *cpy = features;
	while (cpy) {
		printf("\n---------------\n");
		printf("%s\n", cpy->name);
		printf("count: %d\n", cpy->stats.count);
		printf("mean: %f\n", cpy->stats.mean);
		printf("std: %f\n", cpy->stats.std);
		printf("min: %f\n", cpy->stats.min);
		printf("25%%: %f\n", cpy->stats.twentyfive);
		printf("50%%: %f\n", cpy->stats.fifty);
		printf("75%%: %f\n", cpy->stats.seventyfive);
		printf("max: %f\n", cpy->stats.max);
		
		cpy = cpy->next;
	}
	free_statistics(features);
	free_data_frame(df);
	return 0;
}