/**
 * @file    reachable_area.c
 * @brief   Generates the complete reachable area for two servo motors using PWM logic. 
 *          
 * @author  Fathima Risha Ryaaz
 * @date    2025-08-23
 * 
 * @details
 * This file controls two servo motors mimicing a robotic arm, by generating PWM duty cycles corresponding 
 * to angles from 0° to 180° in incremental steps. The servos sweep through all 
 * combinations of positions to cover the complete area of motion.
 * 
 * @note
 * - Controller: Halo Ver 1.0
 * - Servo range: 0° – 180°
 * - PWM period: 20 ms (50 Hz)
 */

#include "halo.h"

unsigned int angle_duty_us(unsigned int angle)
{
    if (angle > 180)
        angle = 180;
    return 1000 + (angle * 1000) / 180; 
}

void reachable_area(void)
{
    //motor 1
    WRITE_REGISTER(0x40004000, 20000); 
    WRITE_REGISTER(0x40004008, 0x01); 

    //motor 2
    WRITE_REGISTER(0x40004040, 20000); 
    WRITE_REGISTER(0x40004048, 0x01);  

    while (1)
    {
        for (int angle0 = 0; angle0 <= 180; angle0 += 5)
        {
            unsigned int duty0 = angle_duty_us(angle0);
            WRITE_REGISTER(0x40004004, duty0); 
         
            for (int angle1 = 0; angle1 <= 180; angle1 += 5)
            {
                unsigned int duty1 = angle_duty_us(angle1);
                WRITE_REGISTER(0x40004044, duty1); 

                delay_us(20); 
            }
        }
    }
}

void fw_main(void)
{
    reachable_area();
}

