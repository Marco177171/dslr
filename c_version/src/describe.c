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

void parse_options(t_option options[static 3], char **argv) {
	int i = 0;
	while (argv[i]) {
		if (argv[i][0] == '-') {
			if (argv[i][1] == '-') {
				if (!strncmp(argv[i], "--percentile=", 13)) options[0].arg = strdup(argv[i] + 13);
				else if (!strncmp(argv[i], "--include=", 10)) options[1].arg = strdup(argv[i] + 10);
				else if (!strncmp(argv[i], "--exclude=", 10)) options[2].arg = strdup(argv[i] + 10);
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

static void print_percentiles(t_percentile *percentiles, dtype type)
{
	if (type == DOUBLE) {
		for (int i = 0; i < percentiles->len; i++) {
			printf("%d%%: %f\n", (int)(percentiles->percentage[i] * 100), percentiles->d[i]);
		}
	} else if (type == DATE) {
		for (int i = 0; i < percentiles->len; i++) {
			printf("%d%%: %s\n", (int)(percentiles->percentage[i] * 100), percentiles->s[i]);
		}
	} else {
		for (int i = 0; i < percentiles->len; i++) {
			printf("%d%%: %s\n", (int)(percentiles->percentage[i] * 100), "NaN");
		}
	}
}

static void print_numeric(t_statistics stats, dtype type)
{
	if (type == DOUBLE || type == DATE) {
		printf("mean: %f\n", stats.mean);
		printf("std: %f\n", stats.std);
		printf("min: %f\n", stats.min);
		print_percentiles(stats.percentiles, type);
		printf("max: %f\n", stats.max);
	} else {
		printf("mean: %s\n", "NaN");
		printf("std: %s\n", "NaN");
		printf("min: %s\n", "NaN");
		print_percentiles(stats.percentiles, type);
		printf("max: %s\n", "NaN");
	}
}

static void print_string(t_statistics stats, dtype type)
{
	if (type == STRING) {
		printf("unique: %d\n", stats.unique);
		printf("top: %s\n", stats.top);
		printf("freq: %d\n", stats.freq);
	} else {
		printf("unique: %s\n", "NaN");
		printf("top: %s\n", "NaN");
		printf("freq: %s\n", "NaN");
	}
}

int main(int argc, char **argv) {
	
	if (argc < 2) {
		printf("%s", info);
		exit(1);
	}

	t_option options[3] = {{.type=percentile}, {.type=include}, {.type=exclude}};
	parse_options(options, argv);
	t_data_frame ***df = get_data_frame(argv[1]);

	if(df) {
		t_feature *features = get_statistics(df, options);
		if (features) {
			t_feature *cpy = features;
			while (cpy) {
				if (options[include].arg) {
					printf("\n---------------\n");
					printf("%s\n", cpy->name);
					printf("count: %d\n", cpy->count);
					if (!strcmp(options[include].arg, "all")) {
						print_string(cpy->stats, cpy->type);
						print_numeric(cpy->stats, cpy->type);
					}
					else if (!strcmp(options[include].arg, "numeric") && cpy->type == DOUBLE) print_numeric(cpy->stats, cpy->type);
					else if (!strcmp(options[include].arg, "string") && cpy->type == STRING) {
						print_string(cpy->stats, cpy->type);
					} else if (!strcmp(options[include].arg, "datetime") && cpy->type == DATE) {
						print_numeric(cpy->stats, cpy->type);
					}
				} else {
					printf("\n---------------\n");
					printf("%s\n", cpy->name);
					printf("count: %d\n", cpy->count);
					printf("mean: %f\n", cpy->stats.mean);
					printf("std: %f\n", cpy->stats.std);
					printf("min: %f\n", cpy->stats.min);
					print_percentiles(cpy->stats.percentiles, cpy->type);
					printf("max: %f\n", cpy->stats.max);
				}
				cpy = cpy->next;
			}
		}
		free_statistics(features);
		free_data_frame(df);
	}
	for (int i = 0; i < 3; i++) {
		if (options[i].arg) free(options[i].arg);
	}
	return 0;
}