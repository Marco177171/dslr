#include <describe.h>
#include <data_frame.h>

int main(int argc, char **argv) {
	
	if (argc != 2) {
		perror("Wrong amount of arguments\n");
		exit(1);
	}

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