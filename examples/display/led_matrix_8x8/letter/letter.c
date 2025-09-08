
/**
 * @file    letter.c
 * @brief   Scroll a single letter from left to right using font buffer.
 * @author  Ashiln Saju
 * @date    2025-08-05
 * 
 * @details
 * This file displays the letter 'H' moving from left to right  
 * in the 8x8 LED matrix.
 * 
 * @note
 * - Display: 8×8 LED Matrix
 * - Controller: Halo Ver 1.0
 */

#include "halo.h"  

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
    
    while (1)
    {
        for(int shift=0;shift<8;shift++)
        {
            for (int frame = 0; frame < 50; frame++) 
            {   
                for (int row = 0;row < 8; row++)
                {
                    unsigned char row_mask = (1 << row);
        
                    unsigned char col_data = ~( (pattern[row] << shift) & 0xFF );
        
                    WRITE_REGISTER(0x40000008, row_mask); 
                    WRITE_REGISTER(0x40000408, col_data); 
                    delay_us(10);  
                    WRITE_REGISTER(0x40000408, 0XFF);                       
                }
            }
        }
    }
}

