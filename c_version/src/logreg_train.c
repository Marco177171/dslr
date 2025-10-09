#include <data_frame.h>
#include <describe.h>
#include <logreg_train.h>
#include <math.h>

const char *info = "\
Usage: logreg_train [FILE.csv]... [OPTIONS]...\n\
Perform a Logistic regression.\n\n\
Read the .csv file and perform a logistic regression on the dataset.\n\
Produce a file weights.csv, containing the result of the training, plus the mean and the\n\
standard deviation used to normalize the data.\n\n\
options:\n\
--gradient_descent=<gd> Select a valid gradient descent to train the model.\n\
                        Default: 'batch'.\n\
                        Possible value:\n\
                            - 'stochastic': fastest possible gradient descent.\n\
                                            Analyze only 1 single example.\n\
                            - 'mini_batch': same as normal batch but on limited values (64).\n\
                                            faster but less precise.\n\
                            - 'adam'      : significantly more complex.\n\
                                            Adam doesn't use a single fixed learning rate for all weights.\n\
                                            Instead, it dynamically calculates a unique learning rate for *every* individual weight.\n\
";

void store_weights(FILE *fp, const char *house_name, double *weights, int n_features) {
    fprintf(fp, "%s", house_name);

    for (int i = 0; i <= n_features; i++) {
        fprintf(fp, ",%.10f", weights[i]); 
    }
    fprintf(fp, "\n");
}

int df_length(t_data_frame*** df) {
    int i = 0;
    while (df[i]) i++;
    return i - 1;
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

void logreg_train(t_data_frame*** df, char *gd) {
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
        if (!strcmp(gd, "batch")) gradient_descent(weights, df, m, n_features, houses[k]);
        else if (!strcmp(gd, "mini_batch")) mini_batch_gradient_descent(weights, df, m, n_features, houses[k]);
        else if (!strcmp(gd, "adam")) adam_gradient_descent(weights, df, m, n_features, houses[k]);
        else {
            fprintf(stderr, "error: invalid gradient descent: %s\n", gd);
        }
        store_weights(fp, houses[k], weights, n_features);
    }
    if (fp) fclose(fp);
    free(weights);
    free(data);
}

int main(int argc, char *argv[]) {
    if (argc < 2 || argc > 3) {
        printf("%s", info);
        exit(EXIT_FAILURE);
    }

    t_data_frame*** df = get_data_frame(argv[1]);

    if (!df) {
        fprintf(stderr, "error: invalid file: %s\n", argv[1]);
        return(1);
    }

    char gradient_descent[64];
    strcpy(gradient_descent, "batch\0");

    if (argc == 3) {
        if (!strncmp(argv[2], "--gradient_descent=", 19)) {
            strcpy(gradient_descent, argv[2] + 19);
            if (strcmp(gradient_descent, "batch") &&
                strcmp(gradient_descent, "mini_batch") &&
                strcmp(gradient_descent, "adam")) {
                    fprintf(stderr, "error: invalid option: %s\nvalid options:\n\t- 'batch'\n\t- 'mini_batch'\n\t- 'adam'\n", argv[2]);        
                    return 1;
                }
        } else {
            fprintf(stderr, "error: invalid option: %s\n", argv[2]);
            return 1;
        }
    }

    logreg_train(df, gradient_descent);

    if (df) free_data_frame(df);
}
