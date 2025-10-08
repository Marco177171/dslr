#ifndef LOGREG_TRAIN_H
#define LOGREG_TRAIN_H

#define LEARNING_RATE 0.01 
#define ITERATIONS 10000 
#define NUM_HOUSES 4
#define NUM_FEATURES 14 
#define STARTING_COL 6
#define ENDING_COL 18

// mini batch gradient descent
#define BATCH_SIZE 64

// Adam hyperparameters
#define BETA1 0.9       // decay rate for the first moment (momentum)
#define BETA2 0.999     // decay rate for the second moment (RMSprop)
#define EPSILON 1e-8    // small constant for numerical stability

typedef struct s_train_data {
    double mean;
    double standard_deviation;
} t_train_data;

// double sigmoid(double z);

void gradient_descent(double *weights, t_data_frame ***df, int m, int n_features, const char *house);
void stochastic_gradient_descent(double *weights, t_data_frame ***df, int m, int n_features, const char *house);
void mini_batch_gradient_descent(double *weights, t_data_frame ***df, int m, int n_features, const char *house);
void adam_gradient_descent(double *weights, t_data_frame ***df, int m, int n_features, const char *house);

#endif
