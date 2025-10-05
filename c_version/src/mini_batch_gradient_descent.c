#include <data_frame.h>
#include <describe.h>
#include <logreg_train.h>
#include <math.h>

static double sigmoid(double z) {
    return 1.0 / (1.0 + exp(-z));
}

void mini_batch_gradient_descent(double *weights, t_data_frame ***df, int m, int n_features, const char *house) {
    double *gradients = (double *)malloc((n_features + 1) * sizeof(double));

    if (gradients == NULL) {
        return;
    }
    
    // total number of batches to process in one epoch
    int num_batches = (m + BATCH_SIZE - 1) / BATCH_SIZE;

    for (int iter = 0; iter < ITERATIONS; iter++) {
        for (int b = 0; b < num_batches; b++) {
            int batch_start = b * BATCH_SIZE;
            int batch_end = batch_start + BATCH_SIZE < m ? batch_start + BATCH_SIZE : m;
            int current_batch_size = batch_end - batch_start;
            
            // iInitialize gradients for the current batch to zero
            memset(gradients, 0, (n_features + 1) * sizeof(double));

            // iterate through all examples in the current mini-batch
            for (int i = batch_start; i < batch_end; i++) {
                double prediction;
                double error;
                double z;
                
                z = weights[0];
                for (int j = 0; j < n_features; j++) {
                    z += weights[j + 1] * df[i + 1][j + STARTING_COL]->d;
                }
                
                // calculate prediction and error for the single example
                prediction = sigmoid(z);
                // target is 1.0 if the house matches, 0.0 otherwise.
                double target = (double)(!strcmp(house, df[i + 1][1]->s));
                error = prediction - target;
                
                gradients[0] += error;
                
                for (int j = 0; j < n_features; j++) {
                    gradients[j + 1] += error * df[i + 1][j + STARTING_COL]->d;
                }
            }

            // update weights
            double scaling_factor = LEARNING_RATE / current_batch_size;
            for (int j = 0; j <= n_features; j++) {
                weights[j] -= scaling_factor * gradients[j];
            }
        }
    }

    free(gradients);
}
