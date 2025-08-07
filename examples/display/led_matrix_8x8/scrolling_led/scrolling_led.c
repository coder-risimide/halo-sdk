/**
 * @file    scrolling_led.c
 * @brief   Single LED scrolls across the top row of the 8x8 LED matrix.
 * @author  Fathima Risha Ryaaz
 * @date    2025-08-05
 *
 * @details
 * This file contains a function that generates a scrolling LED pattern across the top row of the LED matrix.
 * @note
 * - Controller: Halo Ver 1.0 
 * - GPIOs used: GPIO0, GPIO1
 */

#include "scrolling_led.h"
void fw_main(void)
{
    WRITE_REGISTER(0x40000000, 0xFF);  
    WRITE_REGISTER(0x40000400, 0xFF);  

    while (1)
    {
        for (int col = 0; col < 8; col++)          {
            for (int repeat = 0; repeat < 30; repeat++)  
            {
                WRITE_REGISTER(0x40000008, 0b00000001);  

                unsigned char col_pattern = ~(1 << col); 
                WRITE_REGISTER(0x40000408, col_pattern); 
                delay_us(5);  

                WRITE_REGISTER(0x40000408, 0xFF);  
            }
        }
    }
}

