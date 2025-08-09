
/**
 * @file    letter.c
 * @brief   Displays a letter 'H' moving in 8×8 LED matrix.
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

void fw_main(void)
{
    WRITE_REGISTER(0x40000000, 0xFF);
    WRITE_REGISTER(0x40000400, 0xFF);  

    unsigned char pattern[8] = {
        0x81, 
        0x81, 
        0x81, 
        0xFF, 
        0x81, 
        0x81, 
        0x81, 
        0x81  
    };
    
    int shift=0;
    int frame;
    int row;
    while (1)
    {
        while (shift < 8)
        {
            for (frame = 0; frame < 50; frame++) 
            {
                for (row = 0;row < 8; row++)
                {
                    unsigned char row_mask = (1 << row);
        
                    unsigned char col_data = ~(pattern[row] << shift);
        
                    WRITE_REGISTER(0x40000008, row_mask); 
                    WRITE_REGISTER(0x40000408, col_data); 
                    delay_us(10);  
        
                    WRITE_REGISTER(0x40000408, 0xFF);
                        
                }
                WRITE_REGISTER(0x40000408, 0xFF);
            }
        WRITE_REGISTER(0x40000408, 0xFF);
        shift ++;
        }
        shift = 0;
    }
}
