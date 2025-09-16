/**
 * @file    hello.c
 * @brief   Scroll thw word ("HELLO") across the matrix using windowed font buffer.
 * @author  Ashiln Saju
 * @date    2025-08-05
 * 
 * @details
 * This file displays the word 'HELLO' moving from left to right 
 * 
 * 
 * @note
 * - Display: 8×8 LED Matrix
 * - Controller: Halo Ver 1.0
 */
#include "halo.h"

// 5 characters "HELLO" (each 6 cols wide, including spacing)
const unsigned char font[][6] = {
    // 'H'
    {0x7F, 0x08, 0x08, 0x08, 0x7F, 0x00},
    // 'E'
    {0x7F, 0x49, 0x49, 0x49, 0x41, 0x00},
    // 'L'
    {0x7F, 0x40, 0x40, 0x40, 0x40, 0x00},
    // 'L'
    {0x7F, 0x40, 0x40, 0x40, 0x40, 0x00},
    // 'O'
    {0x3E, 0x41, 0x41, 0x41, 0x3E, 0x00}
};

#define NUM_CHARS   5
#define FONT_WIDTH  6
#define MATRIX_COLS 8
#define MATRIX_ROWS 8

void fw_main(void)
{
    WRITE_REGISTER(0x40000000, 0xFF); // rows as output
    WRITE_REGISTER(0x40000400, 0xFF); // cols as output

    int total_width = NUM_CHARS * FONT_WIDTH + MATRIX_COLS; // include trailing blank
    int shift = 0;

    while (1)
    {
        for (shift = 0; shift < total_width; shift++) // scroll across full width
        {
            for (int frame = 0; frame < 40; frame++)  // hold each shift for visibility
            {
                for (int row = 0; row < MATRIX_ROWS; row++)
                {
                    unsigned char row_mask = (1 << row);
                    unsigned char col_data = 0xFF;

                    for (int col = 0; col < MATRIX_COLS; col++)
                    {
                        int font_col_index = shift + col;
                        if (font_col_index < NUM_CHARS * FONT_WIDTH)
                        {
                            // Pick correct character + column inside character
                            int char_index = font_col_index / FONT_WIDTH;
                            int col_index  = font_col_index % FONT_WIDTH;

                            if (font[char_index][col_index] & (1 << row))
                            {
                                col_data &= ~(1 << col); // LED ON
                            }
                        }
                    }

                    WRITE_REGISTER(0x40000408, col_data); // set columns
                    WRITE_REGISTER(0x40000008, row_mask); // select row
                    delay_us(1);                       // ~1ms for brightness
                    WRITE_REGISTER(0x40000408, 0xFF);     // clear
                }
            }
        }
    }
}

