#include <describe.h>
#include <data_frame.h>
#include <logreg_train.h>
#include <math.h>

static double sigmoid(double z) {
    return 1.0 / (1.0 + exp(-z));
}

void stochastic_gradient_descent(double *weights, t_data_frame ***df, int m, int n_features, const char *house) {
    for (int iter = 0; iter < ITERATIONS; iter++) {
        for (int i = 0; i < m; i++) {
            double prediction;
            double error;
            double z;

            // prediction for the current example (i)
            z = weights[0];
            
            for (int j = 0; j < n_features; j++) {
                z += weights[j + 1] * df[i + 1][j + STARTING_COL]->d;
            }
            prediction = sigmoid(z);

            // 1.0 if the house matches, 0.0 otherwise.
            error = prediction - (double)(!strcmp(house, df[i + 1][1]->s));
            
            // update weights immediately
            weights[0] -= LEARNING_RATE * error;

            // update weights immediately
            for (int j = 0; j < n_features; j++) {
                double gradient_j = error * df[i + 1][j + STARTING_COL]->d;
                
                weights[j + 1] -= LEARNING_RATE * gradient_j;
            }
        }
    }
}
