/**
 * @file    2dof_square.c
 * @brief   Generates a square with two servo motors. 
 *          
 * @author  Fathima Risha Ryaaz
 * @date    2025-08-23
 * 
 * @details
 * This file controls two servo motors mimicing a robotic arm, to trace a square with the effectors. 
 * The square is defined by four corner coordinates and the program interpolates between each pair of corners in small steps, making the arm move along each side.
 * @note
 * - Controller: Halo Ver 1.0
 * - Servo range: 0° – 180°
 * - PWM period: 20 ms (50 Hz)
 */

#include "halo.h"
#include <stdio.h>
#include <math.h>

#define PI 3.141592653589793
#define DEG_TO_RAD(deg) ((deg) * PI / 180.0)
#define RAD_TO_DEG(rad) ((rad) * 180.0 / PI)

// Arm link lengths (in cm)
#define L1 10.0
#define L2 10.0

unsigned int angle_to_duty_us(unsigned int angle) {
    if (angle > 180) angle = 180;
    return 1000 + (angle * 1000) / 180;
}

void inverse_kinematics(float x, float y, float* theta1, float* theta2) {
    float d = (x * x + y * y - L1 * L1 - L2 * L2) / (2 * L1 * L2);
    if (d > 1.0) d = 1.0;
    if (d < -1.0) d = -1.0;

    float t2 = acos(d);
    float t1 = atan2(y, x) - atan2(L2 * sin(t2), L1 + L2 * cos(t2));

    *theta1 = RAD_TO_DEG(t1);
    *theta2 = RAD_TO_DEG(t2);
}

void move_to(float x, float y) {
    float theta1, theta2;
    inverse_kinematics(x, y, &theta1, &theta2);

    if (theta1 < 0) theta1 = 0;
    if (theta1 > 180) theta1 = 180;
    if (theta2 < 0) theta2 = 0;
    if (theta2 > 180) theta2 = 180;

    unsigned int duty0 = angle_to_duty_us((unsigned int)theta1);
    unsigned int duty1 = angle_to_duty_us((unsigned int)theta2);

    WRITE_REGISTER(0x40004004, duty0); // pwm0_duty
    WRITE_REGISTER(0x40004044, duty1); // pwm1_duty

    delay_us(200); // smooth motion
}

void fw_main(void) {
    WRITE_REGISTER(0x40004000, 20000); // pwm0_period
    WRITE_REGISTER(0x40004040, 20000); // pwm1_period
    WRITE_REGISTER(0x40004008, 0x01);  // pwm0_ctrl
    WRITE_REGISTER(0x40004048, 0x01);  // pwm1_ctrl

    // Square coordinates (10x10 cm, counterclockwise)
    float square[4][2] = {
        {5.0, 5.0},   // top-left
        {15.0, 5.0},  // top-right
        {15.0, -5.0}, // bottom-right
        {5.0, -5.0}   // bottom-left
    };

    while (1) {
        for (int i = 0; i < 4; ++i) {
            float x1 = square[i][0];
            float y1 = square[i][1];
            float x2 = square[(i + 1) % 4][0];
            float y2 = square[(i + 1) % 4][1];

            int segments = 20;
            for (int j = 0; j <= segments; ++j) {
                float t = (float)j / segments;
                float x = x1 + t * (x2 - x1);
                float y = y1 + t * (y2 - y1);
                move_to(x, y);
            }
        }
    }
}
