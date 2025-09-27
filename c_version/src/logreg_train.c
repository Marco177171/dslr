#include <data_frame.h>
#include <describe.h>
#include <logreg_train.h>
#include <math.h>

#define LEARNING_RATE 0.01 
#define ITERATIONS 10000 
#define NUM_HOUSES 4
#define NUM_FEATURES 14 
#define STARTING_COL 6
#define ENDING_COL 18

void store_weights(FILE *fp, const char *house_name, double *weights, int n_features) {
    fprintf(fp, "%s", house_name);

    for (int i = 0; i <= n_features; i++) {
        fprintf(fp, ",%.10f", weights[i]); 
    }
    fprintf(fp, "\n");
}

double sigmoid(double z) {
    return 1.0 / (1.0 + exp(-z));
}

int df_length(t_data_frame*** df) {
    int i = 0;
    while (df[i]) i++;
    return i - 1;
}

void gradient_descent(double *weights, t_data_frame ***df, int m, int n_features, const char *house) {
    double *predictions = (double *)malloc(m * sizeof(double));
    double *errors = (double *)malloc(m * sizeof(double));
    double *gradients = (double *)malloc((n_features + 1) * sizeof(double));

    for (int iter = 0; iter < ITERATIONS; iter++) {
        // prevision (h(x))
        for (int i = 0; i < m; i++) {
            // scalar product (theta_T * x)
            double z = weights[0];
            for (int j = 0; j < n_features; j++) {
                z += weights[j + 1] * df[i + 1][j + STARTING_COL]->d;
            }
            predictions[i] = sigmoid(z);
            errors[i] = predictions[i] - (double)(!strcmp(house, df[i + 1][1]->s));
        }

        // theta 0
        gradients[0] = 0.0;
        for (int i = 0; i < m; i++) {
            gradients[0] += errors[i];
        }
        gradients[0] /= m;

        for (int j = 0; j < n_features; j++) {
            gradients[j + 1] = 0.0;
            for (int i = 0; i < m; i++) {
                gradients[j + 1] += errors[i] * df[i + 1][j + STARTING_COL]->d;
            }
            gradients[j + 1] /= m;
        }

        // update weights
        for (int j = 0; j <= n_features; j++) {
            weights[j] -= LEARNING_RATE * gradients[j];
        }
    }

    free(predictions);
    free(errors);
    free(gradients);
}

void normalize_data(t_data_frame*** df, int n_features, t_train_data *data) {

    for (int i = 0; i < n_features; i++) {
        double mean = array_mean(df, i + STARTING_COL);
        double sigma = standard_deviation(df, mean, i + STARTING_COL);
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
        data[i].mean = mean;
        data[i].standard_deviation = sigma;
    }
}

void logreg_train(t_data_frame*** df) {
    int m = df_length(df);
    int n_features = (ENDING_COL - STARTING_COL) + 1;
    const char  *houses[] = {"Gryffindor", "Hufflepuff", "Ravenclaw", "Slytherin"};
    
    // + 1 because weights[0] is the bias
    double *weights = malloc(sizeof(double) * (n_features + 1));
    if (!weights) {
        perror("Error: malloc");
        exit(EXIT_FAILURE);
    }
    FILE *fp = fopen("weights.csv", "w");
    if (!fp) {
        perror("Error: fopen");
        exit(EXIT_FAILURE);
    }
    fprintf(fp, "%s,%s,%s", "Category", "Bias", "Arithmancy,Astronomy,Herbology,Defense Against the Dark Arts,Divination,Muggle Studies,Ancient Runes,History of Magic,Transfiguration,Potions,Care of Magical Creatures,Charms,Flying\n");
    t_train_data *data = malloc(sizeof(t_train_data) * n_features);
    if (!data) {
        perror("Error: malloc");
        exit(EXIT_FAILURE);
    }
    normalize_data(df, n_features, data);
    fprintf(fp, "%s,%f", "mean", 0.0);
    for (int i = 0; i < n_features; i++) {
        fprintf(fp, ",%f", data[i].mean);
    }
    fprintf(fp, "\n");
    fprintf(fp, "%s,%f", "standard deviation", 1.0);
    for (int i = 0; i < n_features; i++) {
        fprintf(fp, ",%f", data[i].standard_deviation);
    }
    fprintf(fp, "\n");
    
    for (int k = 0; k < NUM_HOUSES; k++) {
        // clear weights for each house
        for (int i = 0; i <= n_features; i++) {
            weights[i] = 0.0;
        }
        gradient_descent(weights, df, m, n_features, houses[k]);
        store_weights(fp, houses[k], weights, n_features);
    }
    if (fp) fclose(fp);
    free(weights);
    free(data);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        perror("Wrong number of arguments. Usage: logre_train <file.csv>");
        exit(EXIT_FAILURE);
    }
    t_data_frame*** df = get_data_frame(argv[1]);

    logreg_train(df);

    if (df) free_data_frame(df);
}
