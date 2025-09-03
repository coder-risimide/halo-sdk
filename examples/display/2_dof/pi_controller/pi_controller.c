/**
 * @file    pi_controller.c
 * @brief   Inverse Kinematics + PI Control for 2-DOF arm using PWM servos.
 * @author  Adithya
 * @date    2025-09-02
 *
 * @details
 * - Computes joint angles from target (x, y) using IK.
 * - Converts angles to PWM duty cycles for two servo motors.
 * - Uses a PI controller in the angle (degree) domain.
 * - Runs a continuous loop to position the arm at target coordinates.
 *
 * @note
 * - Controller: Halo Ver 1.0
 * - Servo range: 0° – 180°
 * - PWM period: 20 ms (50 Hz)
 */

#include "pi_controller.h"
#include "halo.h"
#include <stdio.h>
#include <math.h>

// ---------- Helpers ----------

// Clamp float value between min and max
static inline float clampf(float v, float min, float max)
{
    if (v < min) return min;
    if (v > max) return max;
    return v;
}

// Normalize angle to [0, 360)
static inline float normalize360(float ang_deg)
{
    while (ang_deg < 0)       ang_deg += 360.0f;
    while (ang_deg >= 360.0f) ang_deg -= 360.0f;
    return ang_deg;
}

// Convert degrees → PWM duty cycle in microseconds
static inline unsigned int angle_deg_to_duty_us(float angle_deg)
{
    if (angle_deg < 0) angle_deg = 0;
    if (angle_deg > 180.0f) angle_deg = 180.0f;

    return 1000 + (unsigned int)((angle_deg / 180.0f) * 1000.0f); // 1000–2000 µs
}

// ---------- Inverse Kinematics ----------

int computeIK(float x, float y, float* theta1_deg, float* theta2_deg)
{
    float r2 = x * x + y * y;
    float r = sqrtf(r2);

    if (r > (L1 + L2) || r < fabsf(L1 - L2))
        return 0; // Target unreachable

    float c2 = (r2 - L1 * L1 - L2 * L2) / (2.0f * L1 * L2);
    c2 = clampf(c2, -1.0f, 1.0f);

    float ang2_up   = acosf(c2);
    float ang2_down = -acosf(c2);

    float k1_up   = L1 + L2 * cosf(ang2_up);
    float k2_up   = L2 * sinf(ang2_up);
    float k1_down = L1 + L2 * cosf(ang2_down);
    float k2_down = L2 * sinf(ang2_down);

    float beta = atan2f(y, x);

    float ang1_up   = beta - atan2f(k2_up,   k1_up);
    float ang1_down = beta - atan2f(k2_down, k1_down);

    // Convert to degrees
    float ang1_up_deg   = normalize360(ang1_up   * 180.0f / M_PI);
    float ang2_up_deg   = normalize360(ang2_up   * 180.0f / M_PI);
    float ang1_down_deg = normalize360(ang1_down * 180.0f / M_PI);
    float ang2_down_deg = normalize360(ang2_down * 180.0f / M_PI);

    // Check if valid servo range
    if (ang1_up_deg   >= 0 && ang1_up_deg   <= 180 &&
        ang2_up_deg   >= 0 && ang2_up_deg   <= 180)
    {
        *theta1_deg = ang1_up_deg;
        *theta2_deg = ang2_up_deg;
        return 1;
    }
    else if (ang1_down_deg >= 0 && ang1_down_deg <= 180 &&
             ang2_down_deg >= 0 && ang2_down_deg <= 180)
    {
        *theta1_deg = ang1_down_deg;
        *theta2_deg = ang2_down_deg;
        return 1;
    }

    return 0;
}

// ---------- PI Control ----------

unsigned int pi_control(float desired_angle_deg, unsigned int reg_addr, float* integral)
{
    float current_angle_deg = READ_REGISTER(reg_addr);
    float error = desired_angle_deg - current_angle_deg;

    *integral += error * 0.02f; // 20 ms loop
    *integral = clampf(*integral, -50.0f, 50.0f);

    float control = KP * error + KI * (*integral);
    float new_angle_deg = current_angle_deg + control;

    if (new_angle_deg < 0) new_angle_deg = 0;
    if (new_angle_deg > 180.0f) new_angle_deg = 180.0f;

    return angle_deg_to_duty_us(new_angle_deg);
}

// ---------- Main ----------

void fw_main(void)
{
    // Configure PWM for Motor 1
    WRITE_REGISTER(0x40004000, 20000);
    WRITE_REGISTER(0x40004008, 0x01);

    // Configure PWM for Motor 2
    WRITE_REGISTER(0x40004040, 20000);
    WRITE_REGISTER(0x40004048, 0x01);

    float x = -10.0f;
    float y = -5.0f;
    float t1_des_deg, t2_des_deg;
    float integral1 = 0, integral2 = 0;

    if (computeIK(x, y, &t1_des_deg, &t2_des_deg))
    {
        while (1)
        {
            unsigned int duty1 = pi_control(t1_des_deg, 0x40001000, &integral1);
            unsigned int duty2 = pi_control(t2_des_deg, 0x40001004, &integral2);

            WRITE_REGISTER(0x40004004, duty1);
            WRITE_REGISTER(0x40004044, duty2);

            delay_us(20000); // 20 ms loop
        }
    }
}

