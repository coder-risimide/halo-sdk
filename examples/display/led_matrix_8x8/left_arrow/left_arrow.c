/**
 * @file    left_arrow.c
 * @brief   Displays a left-pointing arrow animation on an 8×8 LED matrix.
 * @author  Adithya Balak
 * @date    2025-08-05
 * 
 * @details
 * This file contains the LED pattern data and logic to display a 
 * left-pointing arrow that animate by shifting.
 * 
 * @note
 * - Display: 8×8 LED Matrix
 * - Controller: Halo Ver 1.0
 */

 #include "left_arrow.h"

void left_arrow_animation(void)
{
    WRITE_REGISTER(0x40000000, 0xFF); 
    WRITE_REGISTER(0x40000400, 0xFF); 
    unsigned char arrow[8] = {
        0b00010000,
        0b00011000,
        0b01111100,
        0b11111100,
        0b01111100,
        0b00011000,
        0b00010000,
        0b00000000
    };

    while (1)
    {
        for (int repeat = 0; repeat < 1; repeat++)
        {
            for (int frame = 0; frame < 50; frame++) 
            {
                for (int row = 0; row < 8; row++)
                {
                    WRITE_REGISTER(0x40000008, (1 << row)); 
                    unsigned char current = arrow[row];
                    unsigned char rotated = (current >> 1) | (current << 7);
                    arrow[row] = rotated;
                    WRITE_REGISTER(0x40000408, ~rotated);
                    delay_us(10);
                    WRITE_REGISTER(0x40000408, 0xFF); 
                }
            }
        }
    }
}
