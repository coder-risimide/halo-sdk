
/**
 * @file    checker_fill.c
 * @brief   Generates a checkerboard pattern fill animation on the LED matrix.
 * @author  Adithya Balak
 * @date    2025-08-05
 * 
 * @details
 * This file creates an alternating pattern (checkerboard style) across the 8×8 LED matrix.
 * 
 * @note
 * - Display: 8×8 LED Matrix
 * - Controller: Halo Ver 1.0
 */

 #include "checker_fill.h"

void checker_fill_pattern_gen(void)
{
    WRITE_REGISTER(0x40000000, 0xFF);
    WRITE_REGISTER(0x40000400, 0xFF);

    unsigned char pattern1[8];
    unsigned char pattern2[8];

    for (int row = 0; row < 8; row++)
    {
        unsigned char p1 = 0, p2 = 0;
        for (int col = 0; col < 8; col++)
        {
            if ((row + col) % 2 == 0)
                p1 |= (1 << (7 - col));
            else
                p2 |= (1 << (7 - col));
        }
        pattern1[row] = p1;
        pattern2[row] = p2;
    }

    unsigned char* current = pattern1;

    while (1)
    {
        for (int frame = 0; frame < 100; frame++)
        {
            for (int row = 0; row < 8; row++)
            {
                WRITE_REGISTER(0x40000008, (1 << row));
                WRITE_REGISTER(0x40000408, ~(current[row]));
                delay_us(10);
                WRITE_REGISTER(0x40000408, 0xFF);
            }
        }

        current = (current == pattern1) ? pattern2 : pattern1;
    }
}
