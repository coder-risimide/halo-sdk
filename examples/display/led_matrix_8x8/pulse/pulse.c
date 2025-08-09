
/**
 * @file    left_arrow.c
 * @brief   Displays a led staring from the centre and moving outwards until it reaches the diagonal ends of the 8×8 LED matrix and returns to the centre.
 * @author  Ashlin Saju
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

#include "halo.h"

void fw_main(void)
{
    // USER CODE BEGINS
    WRITE_REGISTER(0x40000000, 0xFF);
    WRITE_REGISTER(0x40000400, 0xFF);  

    unsigned char vals[4] = {
        0x18,  
        0x24,  
        0x42,  
        0x81   
    };
    
    int i = 0;
    int j = 0;
    int direction = 0;
    int count = 0;
    // USER CODE ENDS
    while (1)
    {
        if(direction == 0)
        {
            while(count <= 1000)
            {
                for(i = 0;i<=j;i++)
                {
                    WRITE_REGISTER(0x40000008, vals[i]);     
                    WRITE_REGISTER(0x40000408, ~vals[i]);  
                    delay_us(10);
                    WRITE_REGISTER(0x40000400, 0XFF);
                }
                delay_us(1000);
                WRITE_REGISTER(0x40000400, 0XFF);
                count ++;
            }
            j++;
        }
        else
        {
            while(count <= 1000)
            {
                for(i = 0;i<=j;i++)
                {
                    WRITE_REGISTER(0x40000008, vals[i]);     
                    WRITE_REGISTER(0x40000408, ~vals[i]);  
                    delay_us(10);
                    WRITE_REGISTER(0x40000400, 0XFF);
                }
                delay_us(1000);
                WRITE_REGISTER(0x40000400, 0XFF);
                count ++;
            } 
            j--;
        }
        count = 0;
        if(j == 3 || j == 0)
        {
            direction = ~direction;
        }
        delay_us(10);
    }
}
