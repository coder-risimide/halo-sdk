#include "halo.h"
void fw_main(void)
{
    WRITE_REGISTER(0x40000000, 0xFF); 
    WRITE_REGISTER(0x40000400, 0xFF); 
    unsigned char arrow[8] = {
        0b00010000,
        0b00111000,
        0b01111110,
        0b11111110,
        0b01111110,
        0b00111000,
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
                    unsigned char rotated = (current << 1) | (current >> 7);
                    arrow[row] = rotated;
                    WRITE_REGISTER(0x40000408, ~rotated);
                    delay_us(10);
                    WRITE_REGISTER(0x40000408, 0xFF); 
                }
            }
        }
    }
}