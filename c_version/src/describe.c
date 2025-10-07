#include <describe.h>
#include <data_frame.h>
#include <utils.h>

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

static char** validate_options(char *param)
{
	if (param[0] != '[' || param[strlen(param) - 1] != ']') {
		fprintf(stderr, "error: invalid parameter: %s\n", param);
		return NULL;
	}
	param[strlen(param)] = 0;
	param++;
	return split(param, ',');
}

void free_options(t_option options[static 3])
{
	for (int i = 0; i < OPTIONS_COUNT; i++) {
		if (options[i].arg) {
			int j = 0;
			while (options[i].arg[j]) {
				free(options[i].arg[j]);
				j++;
			}
			free(options[i].arg);
		}
	}
}

int parse_options(t_option options[static 3], char **argv) {
	int i = 0;
	while (argv[i]) {
		if (argv[i][0] == '-') {
			if (argv[i][1] == '-') {
				if (!strncmp(argv[i], "--percentile=", 13)) {
					if ((options[PERCENTILE].arg = validate_options(argv[i] + 13)) == NULL) return 1;
				} else if (!strncmp(argv[i], "--include=", 10)) {
					if ((options[INCLUDE].arg = validate_options(argv[i] + 10)) == NULL) return 1;
				} else if (!strncmp(argv[i], "--exclude=", 10)) {
					if ((options[EXCLUDE].arg = validate_options(argv[i] + 10)) == NULL) return 1;
				} else {
					fprintf(stderr, "error: invalid option: %s\n", argv[i]);
					return 1;
				}
			} else {
				switch (argv[i][1])
				{
				case 'p': if ((options[PERCENTILE].arg = validate_options(argv[i] + 3)) == NULL) return 1; break;
				case 'i': if ((options[INCLUDE].arg = validate_options(argv[i] + 3)) == NULL) return 1; break;
				case 'e': if ((options[EXCLUDE].arg = validate_options(argv[i] + 3)) == NULL) return 1; break;
				default: fprintf(stderr, "error: invalid option: %s\n", argv[i]); return 1; break;
				}
			}
		}
		i++;
	}
	return 0;
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
		if (type == DATE) printf("min: %s\n", stats.min.s);
		else printf("min: %f\n", stats.min.d);
		print_percentiles(stats.percentiles, type);
		if (type == DATE) printf("max: %s\n", stats.max.s);
		else printf("max: %f\n", stats.max.d);
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

	t_option options[3] = {{.type=PERCENTILE}, {.type=INCLUDE}, {.type=EXCLUDE}};
	int err = parse_options(options, argv);
	if (err) {
		free_options(options);
		return 1;
	}

	t_data_frame ***df = get_data_frame(argv[1]);

	if(df) {
		t_feature *features = get_statistics(df, options);
		if (features) {
			t_feature *cpy = features;
			while (cpy) {
				if (options[INCLUDE].arg) {
					printf("\n---------------\n");
					printf("%s\n", cpy->name);
					printf("count: %d\n", cpy->count);
					if (include_option(options[INCLUDE].arg, "all")) {
						print_string(cpy->stats, cpy->type);
						print_numeric(cpy->stats, cpy->type);
					}
					else if (include_option(options[INCLUDE].arg, "numeric") && cpy->type == DOUBLE) print_numeric(cpy->stats, cpy->type);
					else if (include_option(options[INCLUDE].arg, "string") && cpy->type == STRING) {
						print_string(cpy->stats, cpy->type);
					} else if (include_option(options[INCLUDE].arg, "datetime") && cpy->type == DATE) {
						print_numeric(cpy->stats, cpy->type);
					}
				} else {
					printf("\n---------------\n");
					printf("%s\n", cpy->name);
					printf("count: %d\n", cpy->count);
					printf("mean: %f\n", cpy->stats.mean);
					printf("std: %f\n", cpy->stats.std);
					printf("min: %f\n", cpy->stats.min.d);
					print_percentiles(cpy->stats.percentiles, cpy->type);
					printf("max: %f\n", cpy->stats.max.d);
				}
				cpy = cpy->next;
			}
		}
		free_options(options);
		free_statistics(features);
		free_data_frame(df);
	}
	return 0;
}