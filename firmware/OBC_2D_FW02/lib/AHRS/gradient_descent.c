#include "math.h"
#include "gradient_descent.h"

// System constants
#define gyroMeasError 3.14159265358979 * (gyro_error / 180.0f) // gyroscope measurement error in rad/s (shown as 5 deg/s)
#define gyroMeasDrift 3.14159265358979 * (gyro_error_drift / 180.0f) // gyroscope measurement error in rad/s/s (shown as 0.2f deg/s/s)

void gradient_descent_filter_init(
    gradient_descent_filter_t *p,
    float gyro_error,
    float gyro_error_drift)
{
    p->beta = sqrt(3.0f / 4.0f) * gyroMeasError; // compute beta
    p->zeta = sqrt(3.0f / 4.0f) * gyroMeasDrift; // compute zeta
    p->b_x = 1.0;p->b_z = 0.0;
    p->w_bx = p->w_by = p->w_bz = 0.0;
    p->SEq_1 = 1;p->SEq_2 = 0;p->SEq_3 = 0;p->SEq_4 = 0;
}

void gradient_descent_filter_set_drift(
    gradient_descent_filter_t *p,
    float gyro_error,            //unit in deg/s
    float gyro_error_drift)
{
    p->beta = sqrt(3.0f / 4.0f) * gyroMeasError; // compute beta
    p->zeta = sqrt(3.0f / 4.0f) * gyroMeasDrift; // compute zeta
}

void gradient_descent_filter_set_earth_flux(
    gradient_descent_filter_t *p,float hx,float hz)
{
    p->b_x = hx;
    p->b_z = hz;
}

quaternion gradient_descent_filter_q(gradient_descent_filter_t *p)
{
    quaternion q;
    q.w = p->SEq_1;
    q.x = p->SEq_2;
    q.y = p->SEq_3;
    q.z = p->SEq_4;
    return q;
}

vec3f gradient_descent_filter_gyro_bias(gradient_descent_filter_t *p)
{
    vec3f w_b;
    w_b.x = p->w_bx;w_b.y = p->w_by;w_b.z = p->w_bz;
    return w_b;
}

// Function to compute one filter iteration
void gradient_descent_filter_update(
    gradient_descent_filter_t *p,vec3f w,vec3f a,vec3f m,float deltat)
{
    // local system variables
    float norm; // vector norm
    float SEqDot_omega_1, SEqDot_omega_2, SEqDot_omega_3, SEqDot_omega_4; // quaternion rate from gyroscopes elements
    float f_1, f_2, f_3, f_4, f_5, f_6; // objective function elements
    float J_11or24, J_12or23, J_13or22, J_14or21, J_32, J_33, // objective function Jacobian elements
    J_41, J_42, J_43, J_44, J_51, J_52, J_53, J_54, J_61, J_62, J_63, J_64; //
    float SEqHatDot_1, SEqHatDot_2, SEqHatDot_3, SEqHatDot_4; // estimated direction of the gyroscope error
    float w_err_x, w_err_y, w_err_z; // estimated direction of the gyroscope error (angular)
    float h_x, h_y, h_z; // computed flux in the earth frame
    
    // axulirary variables to avoid reapeated calcualtions
    float halfSEq_1 = 0.5f * p->SEq_1;
    float halfSEq_2 = 0.5f * p->SEq_2;
    float halfSEq_3 = 0.5f * p->SEq_3;
    float halfSEq_4 = 0.5f * p->SEq_4;
    float twoSEq_1 = 2.0f * p->SEq_1;
    float twoSEq_2 = 2.0f * p->SEq_2;
    float twoSEq_3 = 2.0f * p->SEq_3;
    float twoSEq_4 = 2.0f * p->SEq_4;
    float twob_x = 2.0f * p->b_x;
    float twob_z = 2.0f * p->b_z;
    float twob_xSEq_1 = 2.0f * p->b_x * p->SEq_1;
    float twob_xSEq_2 = 2.0f * p->b_x * p->SEq_2;
    float twob_xSEq_3 = 2.0f * p->b_x * p->SEq_3;
    float twob_xSEq_4 = 2.0f * p->b_x * p->SEq_4;
    float twob_zSEq_1 = 2.0f * p->b_z * p->SEq_1;
    float twob_zSEq_2 = 2.0f * p->b_z * p->SEq_2;
    float twob_zSEq_3 = 2.0f * p->b_z * p->SEq_3;
    float twob_zSEq_4 = 2.0f * p->b_z * p->SEq_4;
    float SEq_1SEq_2;
    float SEq_1SEq_3 = p->SEq_1 * p->SEq_3;
    float SEq_1SEq_4;
    float SEq_2SEq_3;
    float SEq_2SEq_4 = p->SEq_2 * p->SEq_4;
    float SEq_3SEq_4;
    float twom_x = 2.0f * m.x;
    float twom_y = 2.0f * m.y;
    float twom_z = 2.0f * m.z;
    
    // normalise the accelerometer measurement
    norm = sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
    a.x /= norm;
    a.y /= norm;
    a.z /= norm;
    // normalise the magnetometer measurement
    norm = sqrt(m.x * m.x + m.y * m.y + m.z * m.z);
    m.x /= norm;
    m.y /= norm;
    m.z /= norm;
    // compute the objective function and Jacobian
    f_1 = twoSEq_2 * p->SEq_4 - twoSEq_1 * p->SEq_3 - a.x;
    f_2 = twoSEq_1 * p->SEq_2 + twoSEq_3 * p->SEq_4 - a.y;
    f_3 = 1.0f - twoSEq_2 * p->SEq_2 - twoSEq_3 * p->SEq_3 - a.z;
    f_4 = twob_x * (0.5f - p->SEq_3 * p->SEq_3 - p->SEq_4 * p->SEq_4) + twob_z * (SEq_2SEq_4 - SEq_1SEq_3) - m.x;
    f_5 = twob_x * (p->SEq_2 * p->SEq_3 - p->SEq_1 * p->SEq_4) + twob_z * (p->SEq_1 * p->SEq_2 + p->SEq_3 * p->SEq_4) - m.y;
    f_6 = twob_x * (SEq_1SEq_3 + SEq_2SEq_4) + twob_z * (0.5f - p->SEq_2 * p->SEq_2 - p->SEq_3 * p->SEq_3) - m.z;
    J_11or24 = twoSEq_3; // J_11 negated in matrix multiplication
    J_12or23 = 2.0f * p->SEq_4;
    J_13or22 = twoSEq_1; // J_12 negated in matrix multiplication
    J_14or21 = twoSEq_2;
    J_32 = 2.0f * J_14or21; // negated in matrix multiplication
    J_33 = 2.0f * J_11or24; // negated in matrix multiplication
    J_41 = twob_zSEq_3; // negated in matrix multiplication
    J_42 = twob_zSEq_4;
    J_43 = 2.0f * twob_xSEq_3 + twob_zSEq_1; // negated in matrix multiplication
    J_44 = 2.0f * twob_xSEq_4 - twob_zSEq_2; // negated in matrix multiplication
    J_51 = twob_xSEq_4 - twob_zSEq_2; // negated in matrix multiplication
    J_52 = twob_xSEq_3 + twob_zSEq_1;
    J_53 = twob_xSEq_2 + twob_zSEq_4;
    J_54 = twob_xSEq_1 - twob_zSEq_3; // negated in matrix multiplication
    J_61 = twob_xSEq_3;
    J_62 = twob_xSEq_4 - 2.0f * twob_zSEq_2;
    J_63 = twob_xSEq_1 - 2.0f * twob_zSEq_3;
    J_64 = twob_xSEq_2;
    // compute the gradient (matrix multiplication)
    SEqHatDot_1 = J_14or21 * f_2 - J_11or24 * f_1 - J_41 * f_4 - J_51 * f_5 + J_61 * f_6;
    SEqHatDot_2 = J_12or23 * f_1 + J_13or22 * f_2 - J_32 * f_3 + J_42 * f_4 + J_52 * f_5 + J_62 * f_6;
    SEqHatDot_3 = J_12or23 * f_2 - J_33 * f_3 - J_13or22 * f_1 - J_43 * f_4 + J_53 * f_5 + J_63 * f_6;
    SEqHatDot_4 = J_14or21 * f_1 + J_11or24 * f_2 - J_44 * f_4 - J_54 * f_5 + J_64 * f_6;
    // normalise the gradient to estimate direction of the gyroscope error
    norm = sqrt(SEqHatDot_1 * SEqHatDot_1 + SEqHatDot_2 * SEqHatDot_2 + SEqHatDot_3 * SEqHatDot_3 + SEqHatDot_4 * SEqHatDot_4);
    SEqHatDot_1 = SEqHatDot_1 / norm;
    SEqHatDot_2 = SEqHatDot_2 / norm;
    SEqHatDot_3 = SEqHatDot_3 / norm;
    SEqHatDot_4 = SEqHatDot_4 / norm;
    // compute angular estimated direction of the gyroscope error
    w_err_x = twoSEq_1 * SEqHatDot_2 - twoSEq_2 * SEqHatDot_1 - twoSEq_3 * SEqHatDot_4 + twoSEq_4 * SEqHatDot_3;
    w_err_y = twoSEq_1 * SEqHatDot_3 + twoSEq_2 * SEqHatDot_4 - twoSEq_3 * SEqHatDot_1 - twoSEq_4 * SEqHatDot_2;
    w_err_z = twoSEq_1 * SEqHatDot_4 - twoSEq_2 * SEqHatDot_3 + twoSEq_3 * SEqHatDot_2 - twoSEq_4 * SEqHatDot_1;
    // compute and remove the gyroscope baises
    p->w_bx += w_err_x * deltat * p->zeta;
    p->w_by += w_err_y * deltat * p->zeta;
    p->w_bz += w_err_z * deltat * p->zeta;
    w.x -= p->w_bx;
    w.y -= p->w_by;
    w.z -= p->w_bz;
    // compute the quaternion rate measured by gyroscopes
    SEqDot_omega_1 = -halfSEq_2 * w.x - halfSEq_3 * w.y - halfSEq_4 * w.z;
    SEqDot_omega_2 = halfSEq_1 * w.x + halfSEq_3 * w.z - halfSEq_4 * w.y;
    SEqDot_omega_3 = halfSEq_1 * w.y - halfSEq_2 * w.z + halfSEq_4 * w.x;
    SEqDot_omega_4 = halfSEq_1 * w.z + halfSEq_2 * w.y - halfSEq_3 * w.x;
    // compute then integrate the estimated quaternion rate
    p->SEq_1 += (SEqDot_omega_1 - (p->beta * SEqHatDot_1)) * deltat;
    p->SEq_2 += (SEqDot_omega_2 - (p->beta * SEqHatDot_2)) * deltat;
    p->SEq_3 += (SEqDot_omega_3 - (p->beta * SEqHatDot_3)) * deltat;
    p->SEq_4 += (SEqDot_omega_4 - (p->beta * SEqHatDot_4)) * deltat;
    // normalise quaternion
    norm = sqrt(p->SEq_1 * p->SEq_1 + p->SEq_2 * p->SEq_2 + p->SEq_3 * p->SEq_3 + p->SEq_4 * p->SEq_4);
    p->SEq_1 /= norm;
    p->SEq_2 /= norm;
    p->SEq_3 /= norm;
    p->SEq_4 /= norm;
    // compute flux in the earth frame
    SEq_1SEq_2 = p->SEq_1 * p->SEq_2; // recompute axulirary variables
    SEq_1SEq_3 = p->SEq_1 * p->SEq_3;
    SEq_1SEq_4 = p->SEq_1 * p->SEq_4;
    SEq_3SEq_4 = p->SEq_3 * p->SEq_4;
    SEq_2SEq_3 = p->SEq_2 * p->SEq_3;
    SEq_2SEq_4 = p->SEq_2 * p->SEq_4;
    h_x = twom_x * (0.5f - p->SEq_3 * p->SEq_3 - p->SEq_4 * p->SEq_4) + twom_y * (SEq_2SEq_3 - SEq_1SEq_4) + twom_z * (SEq_2SEq_4 + SEq_1SEq_3);
    h_y = twom_x * (SEq_2SEq_3 + SEq_1SEq_4) + twom_y * (0.5f - p->SEq_2 * p->SEq_2 - p->SEq_4 * p->SEq_4) + twom_z * (SEq_3SEq_4 - SEq_1SEq_2);
    h_z = twom_x * (SEq_2SEq_4 - SEq_1SEq_3) + twom_y * (SEq_3SEq_4 + SEq_1SEq_2) + twom_z * (0.5f - p->SEq_2 * p->SEq_2 - p->SEq_3 * p->SEq_3);
    // normalise the flux vector to have only components in the x and z
    p->b_x = sqrt((h_x * h_x) + (h_y * h_y));
    p->b_z = h_z;
}

