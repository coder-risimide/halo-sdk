/**
 * @file    flower_pattern.c
 * @brief   Generates a flower trajectory for a 2-DOF robotic arm.
 *          Uses inverse kinematics to convert Cartesian coordinates into
 *          servo PWM signals and traces the predefined path.
 * 
 * @author  Adithya
 * @date    2025-08-28
 * 
 * @details
 * - Arm model: 2-link planar arm with lengths L1 and L2
 * - Path: Precomputed flower coordinates
 * - Inverse kinematics: Computes joint angles from (x,y)
 * - PWM: Outputs duty cycles to control servos
 * 
 * @note
 * - Controller: Halo Ver 1.0
 * - Servo range: 0° – 180°
 * - PWM period: 20 ms (50 Hz)
 */

#include "flower_pattern.h"
#include "halo.h"
#include <stdio.h>
#include <math.h>

#define L1 10.0f
#define L2 10.0f

// ----------------------------
// Generated flower coordinates
// ----------------------------
float flower_coords[][2] = {
    { -9.16, -3.40 },
    { -9.23, -3.51 },
    { -9.31, -3.62 },
    { -9.38, -3.73 },
    { -9.44, -3.84 },
    { -9.51, -3.95 },
    { -9.57, -4.06 },
    { -9.66, -4.16 },
    { -9.73, -4.27 },
    { -9.79, -4.38 },
    //.............
    //Add rest of the coordinates
  
};

#define NUM_POINTS (sizeof(flower_coords)/sizeof(flower_coords[0]))


// ----------------------------
// Clamp helper
// ----------------------------
static inline float clampf(float v, float min, float max) {
    if (v < min) return min;
    if (v > max) return max;
    return v;
}

// ----------------------------
// Convert angle [0–180°] → servo pulse
// ----------------------------
unsigned int angle_to_duty_us(float angle) {
    return 1000 + (unsigned int)((clampf(angle, 0.0f, 180.0f) * 1000.0f) / 180.0f);
}

// ----------------------------
// Inverse kinematics
// ----------------------------
int computeIK(float x, float y, float *theta1_deg, float *theta2_deg) {
    float r2 = x*x + y*y;
    float r  = sqrtf(r2);
    if (r > (L1 + L2) || r < fabsf(L1 - L2))
        return 0; // unreachable

    float c2 = (r2 - L1*L1 - L2*L2) / (2.0f * L1 * L2);
    c2 = clampf(c2, -1.0f, 1.0f);

    float ang2 = acosf(c2);         // elbow-down
    float s2   = sinf(ang2);
    float k1   = L1 + L2 * c2;
    float k2   = L2 * s2;
    float ang1 = atan2f(y, x) - atan2f(k2, k1);

    *theta1_deg = ang1 * 180.0f / M_PI;
    *theta2_deg = ang2 * 180.0f / M_PI;

    return (*theta1_deg >= 0 && *theta1_deg <= 180 &&
            *theta2_deg >= 0 && *theta2_deg <= 180);
}

// ----------------------------
// Move arm to (x,y)
// ----------------------------
void move_to(float x, float y) {
    float t1, t2;
    if (!computeIK(x, y, &t1, &t2))
        return; // skip if unreachable

    unsigned int duty1 = angle_to_duty_us(t1);
    unsigned int duty2 = angle_to_duty_us(t2);

    WRITE_REGISTER(0x40004004, duty1); // shoulder
    WRITE_REGISTER(0x40004044, duty2); // elbow
}

// ----------------------------
// Main firmware
// ----------------------------
void fw_main(void) {
    // Setup PWM channels
    WRITE_REGISTER(0x40004000, 20000);
    WRITE_REGISTER(0x40004008, 0x01);
    WRITE_REGISTER(0x40004040, 20000);
    WRITE_REGISTER(0x40004048, 0x01);

    // Loop through path forever
    while (1) {
        for (int i = 0; i < NUM_POINTS; i++) {
            // Apply any extra offset if needed
            float x = flower_coords[i][0]*0.5f+3.0f;
            float y = flower_coords[i][1]*0.5f+10.0f;

            move_to(x, y);
            delay_us(5); // adjust drawing speed (5 ms per point here)
        }
    }
}

