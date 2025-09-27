#include <data_frame.h>
#include <describe.h>
#include <logreg_train.h>
#include <math.h>

double sigmoid(double z) {
    return 1.0 / (1.0 + exp(-z));
}

char* predict_student(t_data_frame **student_row_std, double **all_weights, int n_features) {
    char  *houses[] = {"Gryffindor", "Hufflepuff", "Ravenclaw", "Slytherin"};
    double max_prob = -1.0;
    char *predicted_house = NULL;

    for (int k = 0; k < NUM_HOUSES; k++) {
        double *weights_k = all_weights[k];
        
        double z = weights_k[0]; // bias
        for (int j = 0; j < n_features; j++) {
            z += weights_k[j + 1] * student_row_std[j + STARTING_COL]->d; 
        }

        double probability = sigmoid(z);
        
        if (probability > max_prob) {
            max_prob = probability;
            predicted_house = houses[k];
        }
    }

    return predicted_house;
}

void extract_data(t_train_data *data, t_data_frame ***weights_df, int n_features, double **weights) {
    for (int j = 0; j < n_features; j++) {
        data[j].mean = weights_df[1][j + 2]->d;
        data[j].standard_deviation = weights_df[2][j + 2]->d;
    }
    
    for (int j = 0; j < 4; j++) {
        weights[j] = malloc(sizeof(double) * n_features + 1);
        for (int i = 0; i <= n_features; i++) {
            weights[j][i] = weights_df[3 + j][i + 1]->d;
        }
    }
}

void normalize_students(t_data_frame*** df, int n_features, t_train_data *data) {

    for (int i = 0; i < n_features; i++) {
        double mean = data[i].mean;
        double sigma = data[i].standard_deviation;
        for (int k = 1; df[k]; k++) {
            if (!df[k][i + STARTING_COL]->valid) {
                df[k][i + STARTING_COL]->valid = 1;
                df[k][i + STARTING_COL]->type = DOUBLE;
                df[k][i + STARTING_COL]->d = mean;
            }
            if (sigma > 1e-6) { // don't divide by 0
                df[k][i + STARTING_COL]->d = (df[k][i + STARTING_COL]->d - mean) / sigma;
            }
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        perror("Wrong number of arguments. Usage: logreg_predict <data_set.csv> <weights.csv>");
        exit(EXIT_FAILURE);
    }
    int n_features = (ENDING_COL - STARTING_COL) + 1;
    t_data_frame*** test = get_data_frame(argv[1]);
    t_data_frame*** weights_df = get_data_frame(argv[2]);
    t_train_data *data = malloc(sizeof(t_train_data) * n_features);
    double **weights = malloc(sizeof(double *) * 4);

    extract_data(data, weights_df, n_features, weights);
    normalize_students(test, n_features, data);
    FILE *fp = fopen("houses.csv", "w");
    fprintf(fp, "Index,Hogwars House\n");

    for (int i = 1; test[i]; i++) {
        char *predicted_house = predict_student(test[i], weights, n_features);
        fprintf(fp, "%d,%s\n", i - 1, predicted_house);
    }

    free_data_frame(test);
    free_data_frame(weights_df);
    free(data);
}