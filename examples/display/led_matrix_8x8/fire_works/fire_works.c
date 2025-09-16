/**
 * @file    fire_works.c
 * @brief   Displays a firework-style animation on the LED matrix.
 * @author  Adithya Balak
 * @date    2025-08-05
 * 
 * @details
 * This file simulates a simple firework pattern, where points on the matrix 
 * light up and spread outward to mimic an explosion effect.
 * 
 * @note
 * - Display: 8×8 LED Matrix
 * - Controller: Halo Ver 1.0
 */

 #include "fire_works.h"

void fire_works_animation(void)
{
    WRITE_REGISTER(0x40000000, 0xFF);
    WRITE_REGISTER(0x40000400, 0xFF);

    const unsigned char explosion[5][8] = {
        {
            0b00000000,
            0b00000000,
            0b00000000,
            0b00001000,
            0b00010000,
            0b00000000,
            0b00000000,
            0b00000000
        },
        {
            0b00000000,
            0b00001000,
            0b00001000,
            0b00111110,
            0b00010000,
            0b00010000,
            0b00000000,
            0b00000000
        },
        {
            0b00001000,
            0b00011100,
            0b00111110,
            0b01111111,
            0b01111111,
            0b00111110,
            0b00011100,
            0b00001000
        },
        {
            0b00111100,
            0b01111110,
            0b11111111,
            0b11111111,
            0b11111111,
            0b11111111,
            0b01111110,
            0b00111100
        },
        {
            0b00000000,
            0b00000000,
            0b00000000,
            0b00000000,
            0b00000000,
            0b00000000,
            0b00000000,
            0b00000000
        }
    };

    while (1)
    {
        for (int f = 0; f < 5; f++)
        {
            for (int repeat = 0; repeat < 60; repeat++)
            {
                for (int row = 0; row < 8; row++)
                {
                    WRITE_REGISTER(0x40000008, (1 << row));
                    WRITE_REGISTER(0x40000408, ~(explosion[f][row]));
                    delay_us(10);
                    WRITE_REGISTER(0x40000408, 0xFF);
                }
            }
        }
    }
}

void fw_main(void)
{
	fire_works_animation();
}


