void fw_main(void)
{
    WRITE_REGISTER(0x40000000, 0xFF); 
    WRITE_REGISTER(0x40000400, 0xFF); 
    unsigned char cross_on[8] = {
        0b00011000,
        0b00011000,
        0b00011000,
        0b11111111,
        0b11111111,
        0b00011000,
        0b00011000,
        0b00011000
    };

    unsigned char cross_off[8] = {
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000
    };

    while (1)
    {
        for (int blink = 0; blink < 2; blink++)  
        {
            unsigned char* current_frame = (blink == 0) ? cross_on : cross_off;

            for (int repeat = 0; repeat < 50; repeat++) 
            {
                for (int row = 0; row < 8; row++)
                {
                    WRITE_REGISTER(0x40000008, (1 << row)); 
                    WRITE_REGISTER(0x40000408, ~current_frame[row]); 
                    delay_us(5); 

                    WRITE_REGISTER(0x40000408, 0xFF); 
                }
            }
        }
    }
}
