/**
 * @file    fill.c
 * @brief   Turn on matrix from top-down (fill), then bottom-up (drain).
 * @author  Ashiln Saju
 * @date    2025-08-05
 * 
 * @details
 * This file fills the LED matrix from top to bottom and bottom to up in 
 * the LED matrix by lighting a single LED at a time.
 * 
 * @note
 * - Display: 8×8 LED Matrix
 * - Controller: Halo Ver 1.0
 */
#include "halo.h"

void fw_main(void)
{
    //USER CODE BEGINS
    WRITE_REGISTER(0x40000000, 0xFF);
    WRITE_REGISTER(0x40000400, 0xFF); 
    int i=0;
    int col;
    int frame;
    while (1)
    {
        if(i < 0xff)
        {
            i = (i <<  1) + 1;
            WRITE_REGISTER(0x40000008,i);
            for (frame = 0; frame < 20; frame++) 
            {
                col = 1;
                for (int j = 0;j < 8; j++)
                {
                    WRITE_REGISTER(0x40000408,col);
                    delay_us(10);
                    col = col << 1;
                }
                WRITE_REGISTER(0x40000408, 0xFF);
            }
        }
        else if(i >=0)
        {
            while(i >=0)
            {
                i = i >> 1;
                WRITE_REGISTER(0x40000008,i);
                for (frame = 0; frame < 20; frame++) 
                {
                    col = 1;
                    for (int j = 0;j < 8; j++)
                    {
                        WRITE_REGISTER(0x40000408,col);
                        delay_us(10);
                        col = col << 1;
                    }
                    WRITE_REGISTER(0x40000408, 0xFF);
                }
            }
        }
    }
}