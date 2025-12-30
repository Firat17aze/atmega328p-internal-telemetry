#ifndef KALMAN_H
#define KALMAN_H

#include <stdint.h>

typedef struct {
    float estimate;
    float error_covariance;
    float process_noise;
    float measurement_noise;
    float kalman_gain;
} KalmanState;

void Kalman_Init(KalmanState* k, float initial_estimate, float initial_error,
                 float process_noise, float measurement_noise);

float Kalman_Update(KalmanState* k, float measurement);

void Kalman_Reset(KalmanState* k, float new_estimate);

#endif

