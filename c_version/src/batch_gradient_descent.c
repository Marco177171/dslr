#include <data_frame.h>
#include <describe.h>
#include <logreg_train.h>
#include <math.h>

static double sigmoid(double z) {
    return 1.0 / (1.0 + exp(-z));
}

void gradient_descent(double *weights, t_data_frame ***df, int m, int n_features, const char *house) {
    double *predictions = (double *)malloc(m * sizeof(double));
    double *errors = (double *)malloc(m * sizeof(double));
    double *gradients = (double *)malloc((n_features + 1) * sizeof(double));

    if (gradients == NULL || predictions == NULL || errors == NULL) {
        free(gradients);
        free(predictions);
        free(errors);
        return;
    }

    for (int iter = 0; iter < ITERATIONS; iter++) {
        // prevision (h(x))
        for (int i = 0; i < m; i++) {
            // scalar product (theta_T * x)
            double z = weights[0];
            for (int j = 0; j < n_features; j++) {
                z += weights[j + 1] * df[i + 1][j + STARTING_COL]->d;
            }
            predictions[i] = sigmoid(z);
            // 1.0 if the house matches, 0.0 otherwise.
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

