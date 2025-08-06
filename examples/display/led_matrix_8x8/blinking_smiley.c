/**
 * @file    blinking_smiley.c
 * @brief   Displays a smiley face pattern that blinks every second.
 * @author  Fathima Risha Ryaaz
 * @date    2025-08-05
 *
 * @details
 * This file contains a function that generates a smiley that blinks on and off using two 8-bit unsigned integer arrays.
 *
 * @note
 * - Controller: Halo Ver 1.0 
 * - GPIOs used: GPIO0, GPIO1
 */

#include "halo.h"
void fw_main(void)
{
    WRITE_REGISTER(0x40000000, 0xFF); 
    WRITE_REGISTER(0x40000400, 0xFF); 

    
    unsigned char smiley_on[8] = {
        0b00000000,        
        0b00100100,     
        0b00100100,   
        0b00000000,
        0b00000000,           
        0b00100100,     
        0b00011000,     
        0b00000000           
    };

    // All LEDs OFF
    unsigned char smiley_off[8] = {
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000
    };

    while (1)
    {
        for (int blink = 0; blink < 2; blink++)  // ON then OFF
        {
            unsigned char* current_frame = (blink == 0) ? smiley_on : smiley_off;

            for (int repeat = 0; repeat < 50; repeat++)  
            {
                for (int row = 0; row < 8; row++)
                {
                    WRITE_REGISTER(0x40000008, (1 << row));                
                    WRITE_REGISTER(0x40000408, ~current_frame[row]);       
                    delay_us(5);                                          
                    WRITE_REGISTER(0x40000408, 0xFF);                    
                }
            }
        }
    }
}
