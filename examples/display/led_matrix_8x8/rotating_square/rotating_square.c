/**
 * @file    rotating_square.c
 * @brief   Shows a square rotating through four positions.
 * @author  Fathima Risha Ryaaz
 * @date    2025-08-05
 *
 * @details
 * This file contains a function that generates a square that rotates in four positions using 2D arrays that represet each rotating position.
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

   
    unsigned char frames[4][8] = {
        { // Frame 0: 
            0b00000000,
            0b00111100,
            0b00100100,
            0b00100100,
            0b00100100,
            0b00100100,
            0b00111100,
            0b00000000
        },
        { // Frame 1: 
            0b00001000,
            0b00010100,
            0b00100010,
            0b01000001,
            0b01000001,
            0b00100010,
            0b00010100,
            0b00001000
        },
        { // Frame 2: 
            0b00010000,
            0b00101000,
            0b01000100,
            0b10000010,
            0b01000100,
            0b00101000,
            0b00010000,
            0b00000000
        },
        { // Frame 3: 
            0b00000000,
            0b00000000,
            0b11111110,
            0b10000010,
            0b10000010,
            0b11111110,
            0b00000000,
            0b00000000
        }
    };

    while (1)
    {
        for (int f = 0; f < 4; f++)  
        {
            for (int repeat = 0; repeat < 50; repeat++)  
            {
                for (int row = 0; row < 8; row++)
                {
                    WRITE_REGISTER(0x40000008, (1 << row));           
                    WRITE_REGISTER(0x40000408, ~frames[f][row]);      
                    delay_us(5);                                     
                    WRITE_REGISTER(0x40000408, 0xFF);                 
                }
            }
        }
    }
}
