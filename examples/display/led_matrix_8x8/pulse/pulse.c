
/**
 * @file    pulse.c
 * @brief   Fill LEDs from center to edge, then back inward.
 * @author  Ashlin Saju
 * @date    2025-08-05
 * 
 * @details
 * This file Displays a led staring from the centre and moving outwards until it reaches the  
 * diagonal ends of the 8×8 LED matrix and returns to the centre.
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

    unsigned char vals[4] = {
        0x18,  
        0x24,  
        0x42,  
        0x81   
    };
    
    int j = 0;
    int count = 0;

    while (1)
    {
        for (count = 0; count < 100; count++)
        {
            for (int i = 0; i <= j; i++)
            {
                // Set row
                WRITE_REGISTER(0x40000008, vals[i]);     

                WRITE_REGISTER(0x40000408, ~vals[i]);  

                delay_us(10);

                WRITE_REGISTER(0x40000408, 0xFF);
            }
        }
        j++;
        if (j >= 3) 
            while( j >=0 )
            {
                for (count = 0; count < 100; count++)
                {
                    for (i = 0; i <= j; i++)
                    {
                        WRITE_REGISTER(0x40000008, vals[i]);     
                
                        WRITE_REGISTER(0x40000408, ~vals[i]);  
                
                        delay_us(10);

                        WRITE_REGISTER(0x40000408, 0xFF);
                    }
                } 
                j--;
            }
    }
}
