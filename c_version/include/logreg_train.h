#ifndef LOGREG_TRAIN_H
#define LOGREG_TRAIN_H

#define LEARNING_RATE 0.01 
#define ITERATIONS 10000 
#define NUM_HOUSES 4
#define NUM_FEATURES 14 
#define STARTING_COL 6
#define ENDING_COL 18

typedef struct s_train_data {
    double mean;
    double standard_deviation;
} t_train_data;

// double sigmoid(double z);

#endif
