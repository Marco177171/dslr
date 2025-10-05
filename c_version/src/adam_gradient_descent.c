#include <data_frame.h>
#include <describe.h>
#include <logreg_train.h>
#include <math.h>

static double sigmoid(double z) {
    return 1.0 / (1.0 + exp(-z));
}

void adam_gradient_descent(double *weights, t_data_frame ***df, int m, int n_features, const char *house) {
    
    int num_weights = n_features + 1;
    double *gradients = (double *)calloc(num_weights, sizeof(double));
    double *moment_vector = (double *)calloc(num_weights, sizeof(double));
    double *RMSprop = (double *)calloc(num_weights, sizeof(double));

    if (gradients == NULL || moment_vector == NULL || RMSprop == NULL) {
        free(gradients);
        free(moment_vector);
        free(RMSprop);
        return;
    }
    
    int num_batches = (m + BATCH_SIZE - 1) / BATCH_SIZE;
    long total_step_count = 0;

    for (int iter = 0; iter < ITERATIONS; iter++) {
        for (int b = 0; b < num_batches; b++) {
            total_step_count++;
            
            int batch_start = b * BATCH_SIZE;
            int batch_end = batch_start + BATCH_SIZE < m ? batch_start + BATCH_SIZE : m;
            int current_batch_size = batch_end - batch_start;
            
            // initialize gradients for the current batch to zero
            memset(gradients, 0, num_weights * sizeof(double));

            // compute the SUM of gradients over the current mini-batch
            for (int i = batch_start; i < batch_end; i++) {
                double z = weights[0];
                for (int j = 0; j < n_features; j++) {
                    z += weights[j + 1] * df[i + 1][j + STARTING_COL]->d;
                }
                
                double prediction = sigmoid(z);
                double target = (double)(!strcmp(house, df[i + 1][1]->s));
                double error = prediction - target;
                
                gradients[0] += error;
                
                for (int j = 0; j < n_features; j++) {
                    gradients[j + 1] += error * df[i + 1][j + STARTING_COL]->d;
                }
            }
            
            // perform Adam update using the calculated gradients
            
            // pre-calculate bias correction factors
            double beta1_power_t = pow(BETA1, total_step_count);
            double beta2_power_t = pow(BETA2, total_step_count);
            
            double m_hat_scaling = 1.0 / (1.0 - beta1_power_t);
            double v_hat_scaling = 1.0 / (1.0 - beta2_power_t);
            
            // scale the sum of gradients to get the average batch gradient (momentum)
            double current_gradient[num_weights]; 
            for (int j = 0; j < num_weights; j++) {
                current_gradient[j] = gradients[j] / current_batch_size;
            }

            for (int j = 0; j < num_weights; j++) {
                double g_t = current_gradient[j];
                
                // a) update biased first moment estimate (momentum)
                moment_vector[j] = BETA1 * moment_vector[j] + (1.0 - BETA1) * g_t;
                
                // b) update biased second moment estimate (RMSprop)
                RMSprop[j] = BETA2 * RMSprop[j] + (1.0 - BETA2) * (g_t * g_t);
                
                // c) compute bias-corrected first and second moment estimates
                double m_hat = moment_vector[j] * m_hat_scaling;
                double v_hat = RMSprop[j] * v_hat_scaling;
                
                // d) update weights
                weights[j] -= LEARNING_RATE * m_hat / (sqrt(v_hat) + EPSILON);
            }
        }
    }

    free(gradients);
    free(moment_vector);
    free(RMSprop);
}
