void fw_main(void)
{
    WRITE_REGISTER(0x40000000, 0xFF); 
    WRITE_REGISTER(0x40000400, 0xFF); 

    const unsigned char sine_y[16] = {3, 4, 5, 4, 3, 2, 1, 2, 3, 4, 5, 4, 3, 2, 1, 2};

    int offset = 0;

    while (1)
    {
        for (int frame = 0; frame < 50; frame++)
        {
            for (int row = 0; row < 8; row++)
            {
                WRITE_REGISTER(0x40000008, (1 << row));
                unsigned char col_pattern = 0xFF;

                for (int col = 0; col < 8; col++)
                {
                    int index = (offset + col) % 16;
                    if (row == sine_y[index])
                        col_pattern &= ~(1 << (7 - col));
                }

                WRITE_REGISTER(0x40000408, col_pattern);
                delay_us(1);
                WRITE_REGISTER(0x40000408, 0xFF);
            }
        }

        offset = (offset + 1) % 16;
    }
}
