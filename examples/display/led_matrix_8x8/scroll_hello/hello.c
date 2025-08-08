/**
 * @file    hello.c
 * @brief   Displays the word 'HELLO' in the LED matrix.
 * @author  Ashiln Saju
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
#include <halo.h>  


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

#define NUM_CHARS 5
#define FONT_WIDTH 6  

unsigned char screen[NUM_CHARS * FONT_WIDTH];

void fill_buffer() {
    int i, j;
    for (i = 0; i < NUM_CHARS; i++) {
        for (j = 0; j < FONT_WIDTH; j++) {
            screen[i * FONT_WIDTH + j] = font[i][j];
        }
    }
}

void hello(void)
{
    WRITE_REGISTER(0x40000000, 0xFF);
    WRITE_REGISTER(0x40000400, 0xFF);

    fill_buffer();

    int scroll_width = NUM_CHARS * FONT_WIDTH;
    int shift;

    while (1)
    {
        for (shift = 0; shift < scroll_width ; shift++)  
        {
            for (int frame = 0; frame < 10; frame++) 
            {
                for (int row = 0; row < 8; row++)  
                {
                    unsigned char row_mask = (1 << row);  
                    unsigned char col_data = 0xFF;

                    for (int col = 0; col < 8; col++)
                    {
                        if (screen[shift + col] & (1 << row))
                        {
                            col_data &= ~(1 << col); 
                        }
                    }
                    WRITE_REGISTER(0x40000408, col_data);
                    WRITE_REGISTER(0x40000008, row_mask);
                    delay_us(10);
                    WRITE_REGISTER(0x40000408, 0XFF);
                }
            }
        }
    }
}
