#ifndef _GRADIENT_DESCENT_H__
#define _GRADIENT_DESCENT_H__

#include "datatypes.h"
#include "3dmath_helper.h"

typedef struct {
    float beta, zeta;
    //filter feedback gain
    float SEq_1, SEq_2, SEq_3, SEq_4; 
    // estimated orientation quaternion elements with initial conditions
    float b_x, b_z; 
    // reference direction of flux in earth frame
    float w_bx, w_by, w_bz; 
    // estimate gyroscope biases error
}gradient_descent_filter_t;

void gradient_descent_filter_init(
    gradient_descent_filter_t*,
    float gyro_error,            //unit in deg/s
    float gyro_error_drift);     //unit in deg/s/s

void gradient_descent_filter_set_drift(
    gradient_descent_filter_t*,
    float gyro_error,            //unit in deg/s
    float gyro_error_drift);

void gradient_descent_filter_set_earth_flux(
    gradient_descent_filter_t*,float hx,float hz);

void gradient_descent_filter_update(
    gradient_descent_filter_t *p,
    vec3f w,vec3f a,vec3f m,float dt);

quaternion gradient_descent_filter_q(gradient_descent_filter_t*);
vec3f gradient_descent_filter_gyro_bias(gradient_descent_filter_t*);

#endif