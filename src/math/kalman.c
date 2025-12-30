#include "kalman.h"
#include <stddef.h>

void Kalman_Init(KalmanState* k, float initial_estimate, float initial_error,
                 float process_noise, float measurement_noise)
{
    if (k == NULL) {
        return;
    }
    
    k->estimate = initial_estimate;
    k->error_covariance = initial_error;
    k->process_noise = process_noise;
    k->measurement_noise = measurement_noise;
    k->kalman_gain = 0.0f;
}

float Kalman_Update(KalmanState* k, float measurement)
{
    if (k == NULL) {
        return 0.0f;
    }
    
    float predicted_error = k->error_covariance + k->process_noise;
    
    k->kalman_gain = predicted_error / (predicted_error + k->measurement_noise);
    
    float innovation = measurement - k->estimate;
    k->estimate = k->estimate + (k->kalman_gain * innovation);
    
    k->error_covariance = (1.0f - k->kalman_gain) * predicted_error;
    
    return k->estimate;
}

void Kalman_Reset(KalmanState* k, float new_estimate)
{
    if (k == NULL) {
        return;
    }
    
    k->estimate = new_estimate;
    k->error_covariance = k->measurement_noise;
    k->kalman_gain = 0.0f;
}

