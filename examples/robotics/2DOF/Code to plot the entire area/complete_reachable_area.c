#include "halo.h"
#include"complete_reachable_area.h"


unsigned int angle_to_duty_us(unsigned int angle)
{

    if (angle > 180)
        angle = 180;

    return 1000 + (angle * 1000) / 180; 
}

void fw_main(void)
{
    WRITE_REGISTER(0x40004000, 20000); 
    WRITE_REGISTER(0x40004008, 0x01); 

    // Motor 2 (Joint 1)
    WRITE_REGISTER(0x40004040, 20000); 
    WRITE_REGISTER(0x40004048, 0x01);  

    while (1)
    {
 
        for (int angle0 = 0; angle0 <= 180; angle0 += 5)
        {
            unsigned int duty0 = angle_to_duty_us(angle0);
            WRITE_REGISTER(0x40004004, duty0); 

         
            for (int angle1 = 0; angle1 <= 180; angle1 += 5)
            {
                unsigned int duty1 = angle_to_duty_us(angle1);
                WRITE_REGISTER(0x40004044, duty1); 

                delay_us(20); 
            }
        }

   
        for (int angle0 = 180; angle0 >= 0; angle0 -= 5)
        {
            unsigned int duty0 = angle_to_duty_us(angle0);
            WRITE_REGISTER(0x40004004, duty0); 

            for (int angle1 = 180; angle1 >= 0; angle1 -= 5)
            {
                unsigned int duty1 = angle_to_duty_us(angle1);
                WRITE_REGISTER(0x40004044, duty1); 

                delay_us(20);
            }
        }
    }
}

