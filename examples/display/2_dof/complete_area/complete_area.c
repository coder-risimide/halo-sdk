/**
 * @file    complete_area.c
 * @brief   Generates a complete area sweep for two servo motors using PWM signals. 
 *          Both servos iterate through their 0°–180° range in steps.
 * @author  Adithya
 * @date    2025-08-17
 * 
 * @details
 * This file controls two servo motors by generating PWM duty cycles corresponding 
 * to angles from 0° to 180° in incremental steps. The servos sweep through all 
 * combinations of positions to cover the complete area of motion.
 * 
 * @note
 * - Controller: Halo Ver 1.0
 * - Servo range: 0° – 180°
 * - PWM period: 20 ms (50 Hz)
 */

#include "complete_area.h"
#include "halo.h"

// Helper: convert angle to duty cycle in microseconds
static unsigned int angle_to_duty_us(unsigned int angle)
{
    if (angle > 180)
        angle = 180;

    return 1000 + (angle * 1000) / 180;  // 1000–2000 µs pulse width
}

void complete_area_pattern_gen(void)
{
    // Configure PWM for Motor 1 (Joint 0)
    WRITE_REGISTER(0x40004000, 20000);  // 20 ms period
    WRITE_REGISTER(0x40004008, 0x01);   // Enable

    // Configure PWM for Motor 2 (Joint 1)
    WRITE_REGISTER(0x40004040, 20000);  // 20 ms period
    WRITE_REGISTER(0x40004048, 0x01);   // Enable

    while (1)
    {
        for (int angle0 = 0; angle0 <= 180; angle0 += 5)
        {
            unsigned int duty0 = angle_to_duty_us(angle0);
            WRITE_REGISTER(0x40004004, duty0);  // Motor 1

            for (int angle1 = 0; angle1 <= 180; angle1 += 5)
            {
                unsigned int duty1 = angle_to_duty_us(angle1);
                WRITE_REGISTER(0x40004044, duty1);  // Motor 2

                delay_us(20);
            }
        }
    }
}
