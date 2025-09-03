/**
 * @file    point.c
 * @brief   Moves the effector to a particular point using PI logic to obtain feedback and adjust the PWM duty cycles for controlling of the effector. 
 *          
 * @author  Fathima Risha Ryaaz
 * @date    2025-09-02
 * 
 * @details
 * This file controls two servo motors mimicing a robotic arm, by generating PWM duty cycles corresponding to angles from 0° to 180° in incremental steps. 
 * We use PI logic here so that errors can be addressed by taking feedback and duty cycles are adjusted appropriately for required motion.
 * @note
 * - Controller: Halo Ver 1.0
 * - Servo range: 0° – 180°
 * - PWM period: 20 ms (50 Hz)
 */

#include "halo.h"
#include <stdio.h>
#include <math.h>
#define L1 10.0f
#define L2 10.0f
// PI gains (tune these!)
#define KP 1.0f
#define KI 0.5f

#define M_PI 3.14
// ---------- helpers ----------
static inline float clampf(float v, float min, float max) {
    if (v < min) return min;
    if (v > max) return max;
    return v;
}
// Convert radians :left_right_arrow: PWM pulse width (µs)
static inline unsigned int angle_to_duty_us(float angle_rad)
{
    if (angle_rad < 0) angle_rad = 0;
    if (angle_rad > M_PI) angle_rad = M_PI;
    return 1000 + (unsigned int)((angle_rad / M_PI) * 1000.0f);
}
static inline float duty_us_to_angle(unsigned int duty)
{
    return ((float)(duty - 1000) / 1000.0f) * M_PI;
}
// Inverse Kinematics (choose valid solution in [0,π])
int computeIK(float x, float y, float* theta1, float* theta2)
{
    float r2 = x * x + y * y;
    float r = sqrtf(r2);
    if (r > (L1 + L2) || r < fabsf(L1 - L2))
        return 0;
    float c2 = (r2 - L1 * L1 - L2 * L2) / (2.0f * L1 * L2);
    c2 = clampf(c2, -1.0f, 1.0f);
    float ang2_up = acosf(c2);
    float ang2_down = -acosf(c2);
    float k1_up = L1 + L2 * cosf(ang2_up);
    float k2_up = L2 * sinf(ang2_up);
    float k1_down = L1 + L2 * cosf(ang2_down);
    float k2_down = L2 * sinf(ang2_down);
    float beta = atan2f(y, x);
    float ang1_up = beta - atan2f(k2_up, k1_up);
    float ang1_down = beta - atan2f(k2_down, k1_down);
    if (ang1_up >= 0 && ang1_up <= M_PI && ang2_up >= 0 && ang2_up <= M_PI) {
        *theta1 = ang1_up;
        *theta2 = ang2_up;
        return 1;
    }
    else if (ang1_down >= 0 && ang1_down <= M_PI && ang2_down >= 0 && ang2_down <= M_PI) {
        *theta1 = ang1_down;
        *theta2 = ang2_down;
        return 1;
    }
    return 0;
}
// ---------- PI Controller for one joint ----------
unsigned int pi_control(float desired_angle, unsigned int reg_addr, float* integral)
{
    // Read current PWM from register

    float current_angle = READ_REGISTER(reg_addr) * M_PI/180;
    // Error
    float error = desired_angle - current_angle;
    // Integrate error
    *integral += error * 0.02f; // 20 ms loop
    // PI control
    float control = KP * error + KI * (*integral);
    // Convert back to PWM duty
    float desired_angle_new = current_angle + control;
    printf("%f ", desired_angle_new);
    return angle_to_duty_us(desired_angle_new);
}
// ---------- main ----------
void fw_main(void)
{
    // Setup PWM registers
    WRITE_REGISTER(0x40004000, 20000);
    WRITE_REGISTER(0x40004008, 0x01);
    WRITE_REGISTER(0x40004040, 20000);
    WRITE_REGISTER(0x40004048, 0x01);
    // Target position
    float x = -10.0f;
    float y = 0.0f;
    float t1_des, t2_des;
    float integral1 = 0, integral2 = 0;
    if (computeIK(x, y, &t1_des, &t2_des))
    {
        while (1) {
            unsigned int duty1 = pi_control(t1_des, 0x40001000, &integral1);
            unsigned int duty2 = pi_control(t2_des, 0x40001004, &integral2);
            printf("%d %d\n", duty1, duty2);
            WRITE_REGISTER(0x40004004, duty1);
            WRITE_REGISTER(0x40004044, duty2);
            delay_us(20); // 20 ms control loop
        }
    }
}
