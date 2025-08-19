#include "halo.h"
#include <stdio.h>
#include <math.h>

#define L1 10.0f
#define L2 10.0f

static inline float clampf(float v, float min, float max) {
    if (v < min) return min;
    if (v > max) return max;
    return v;
}

// conversion
unsigned int angle_to_duty_us(float angle)
{
    if (angle < 0) angle = 0;
    if (angle > 180) angle = 180;
    return 1000 + (unsigned int)((angle * 1000) / 180.0f);
}

// IK
int computeIK(float x, float y, float *theta1_deg, float *theta2_deg, int elbowUp)
{
    float r2 = x*x + y*y;
    float r  = sqrtf(r2);
   
    if (r > (L1 + L2) || r < fabsf(L1 - L2))
        return 0;
    
    // theta2
    float c2 = (r2 - L1*L1 - L2*L2) / (2.0f * L1 * L2);
    c2 = clampf(c2, -1.0f, 1.0f);
    float ang2 = acosf(c2);
    if (elbowUp) ang2 = -ang2;

    // theta1 
    float s2 = sinf(ang2);
    float k1 = L1 + L2 * c2;
    float k2 = L2 * s2;

    float beta  = atan2f(y, x);
    float alpha = atan2f(k2, k1);
    float ang1  = beta - alpha;

    // convert to degrees
    *theta1_deg = ang1 * 180.0f / M_PI;
    *theta2_deg = ang2 * 180.0f / M_PI;

    return 1;
}


void move_to(float x, float y, int elbowUp)
{
    float t1_deg, t2_deg;
    if (computeIK(x, y, &t1_deg, &t2_deg, elbowUp))
    {
        unsigned int duty1 = angle_to_duty_us(t1_deg);
        unsigned int duty2 = angle_to_duty_us(t2_deg);

        WRITE_REGISTER(0x40004004, duty1); 
        WRITE_REGISTER(0x40004044, duty2); 
    }
    else
    {
        //ignore
    }
}


void move_line(float x1, float y1, float x2, float y2, int steps, int elbowUp)
{
    for (int i = 0; i <= steps; i++) {
        float t = (float)i / (float)steps;
        float x = x1 + t * (x2 - x1);
        float y = y1 + t * (y2 - y1);
        move_to(x, y, elbowUp);
        delay_us(20); 
    }
}

void fw_main(void)
{

    WRITE_REGISTER(0x40004000, 20000); 
    WRITE_REGISTER(0x40004008, 0x01);  
    WRITE_REGISTER(0x40004040, 20000); 
    WRITE_REGISTER(0x40004048, 0x01);  

    
    move_line(-15.0f, 0.0f, 15.0f, 15.0f, 50, 0);

    while (1) {
        delay_us(20); 
    }
}