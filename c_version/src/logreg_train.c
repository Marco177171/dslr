#include "../include/utils.h"
#include "../include/data_frame.h"

typedef struct s_features {
	char *name;
	int count;
	double total;
	double avg;
} t_features;

typedef struct s_student {
	char *house_name;
	t_features **features;
} t_student;

void print_student_data(t_student *student) {
	// printf("in print student data\n");
	int i = 0;
	printf("----- Average %s:\n", student->house_name);
	while (student->features[i]) {
		printf("%s : %f\n", student->features[i]->name, student->features[i]->avg);
		i++;
	}
}

t_features **init_features(int count) {
	t_features **features = (t_features**)malloc(sizeof(t_features*) * (count + 1));
	if (!features) {
		perror("MALLOC ERROR: cound not allocate features");
		exit(EXIT_FAILURE);
	}
	features[count] = NULL;
	return features;
}

t_student *init_student(char *house_name) {
	t_student *student = (t_student*)malloc(sizeof(t_student));
	if (!student) {
		perror("MALLOC ERROR: cound not allocate student");
		exit(EXIT_FAILURE);
	}
	student->house_name = strdup(house_name);
	student->features = NULL;
	return student;
}

void free_features(t_features **features) {
	// printf("in free features\n");
	int i = 0;
	while (features[i]) {
		if (features[i]->name)
			free(features[i]->name);
		free(features[i]);
		i++;
	}
	free(features);
}

void free_student(t_student *student) {
	// printf("in free student\n");
	if (student->features)
		free_features(student->features);
	if (student->house_name)
		free(student->house_name);
	free(student);
}

void allocate_student_features(t_student *student, t_data_frame ***df) {
	// printf("in allocate student features\n");
	int i = 1, count = 0;
	
	while (df[0][i]) {
		if (is_valid_column(df, i))
			count++;
		i++;
	}
	student->features = init_features(count);
	
	int j = 1;
	i = 1, count = 0;
	while (df[i]) {
		while (df[i][j]) {
			if (is_valid_column(df, j)) {
				student->features[count] = malloc(sizeof(t_features));
				student->features[count]->name = strdup(df[0][j]->s);
				student->features[count]->total = 0;
				student->features[count]->count = 0;
				student->features[count]->avg = 0;
				// printf("allocated %s\n", student->features[count]->name);
				count++;
			}
			j++;
		}
		i++;
	}
	student->features[count] = NULL;
}

void update_student_feature(t_student *student, char *feature_name, double score) {
	// printf("in update feature\n");
	int i = 0;
	while (student->features[i]) {
		if (!strcmp(student->features[i]->name, feature_name)) {
			student->features[i]->total += score;
			student->features[i]->count++;
			student->features[i]->avg = student->features[i]->total / student->features[i]->count;
		}
		i++;
	}
}

void perfect_student(t_student *student, t_data_frame ***df) {
	// printf("in pefrect student\n");
	allocate_student_features(student, df);
	int i = 0, j = 0;
	while (df[i]) {
		j = 0;
		while (df[i][j]) {
			if (is_valid_column(df, j) && !strcmp(df[i][1]->s, student->house_name))
				update_student_feature(student, df[0][j]->s, df[i][j]->d);
			j++;
		}
		i++;
	}
}

void print_to_file(t_student *student, FILE *csv) {
    int i = 0;
    
    while (student->features[i]) {
        fprintf(csv, "%f", student->features[i]->avg);
        if (student->features[i+1])
            fprintf(csv, ",");
        i++;
    }
    fprintf(csv, "\n");
}

void print_header_to_file(t_data_frame ***df, FILE *csv) {
    int i = 0;

    while (df[0][i]) {
        fprintf(csv, "%s", df[0][i]->s);
        if (df[0][i+1])
            fprintf(csv, ",");
        i++;
    }
    fprintf(csv, "\n");
}

void train(t_data_frame ***df) {
    
    FILE *csv = fopen("perfect_students.csv", "w");
    if (!csv) {
        perror("Could not create or open csv file");
        exit(EXIT_FAILURE);
    }

	t_student *gryffindor = init_student("Gryffindor");
	t_student *hufflepuff = init_student("Hufflepuff");
	t_student *ravenclaw = init_student("Ravenclaw");
	t_student *slytherin = init_student("Slytherin");

	perfect_student(gryffindor, df);
	perfect_student(hufflepuff, df);
	perfect_student(ravenclaw, df);
	perfect_student(slytherin, df);

	print_student_data(gryffindor);
	print_student_data(hufflepuff);
	print_student_data(ravenclaw);
	print_student_data(slytherin);

    print_header_to_file(df, csv);
    print_to_file(gryffindor, csv);
    print_to_file(hufflepuff, csv);
    print_to_file(ravenclaw, csv);
    print_to_file(slytherin, csv);

    fclose(csv);

	free_student(gryffindor);
	free_student(hufflepuff);
	free_student(ravenclaw);
	free_student(slytherin);
}

int main(int argc, char **argv) {
	
	if (argc != 2) {
		perror("ERROR: usage: ./logreg_train *csv_file*");
		exit(EXIT_FAILURE);
	}

	t_data_frame ***df = get_data_frame(argv[1]);
	train(df);
	free_data_frame(df);
	return 0;
}
