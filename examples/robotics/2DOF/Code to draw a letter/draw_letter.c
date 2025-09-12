#include "halo.h"
#include <stdio.h>
#include <math.h>

// -------------------- Robot Parameters --------------------
#define L1 10.0f
#define L2 10.0f

// Servo pulse limits
#define SERVO_MIN_US 1000
#define SERVO_MAX_US 2000
#define SERVO_RANGE_DEG 180.0f

// -------------------- Helpers --------------------
static inline float clampf(float v, float min, float max) {
    return (v < min) ? min : (v > max) ? max : v;
}

static inline unsigned int angle_to_duty_us(float angle_deg)
{
    float a = clampf(angle_deg, 0.0f, SERVO_RANGE_DEG);
    return SERVO_MIN_US + (unsigned int)((a * (SERVO_MAX_US - SERVO_MIN_US)) / SERVO_RANGE_DEG);
}

// -------------------- Inverse Kinematics --------------------
int computeIK(float x, float y, float *theta1_deg, float *theta2_deg, int elbowUp)
{
    float r2 = x*x + y*y;
    float r  = sqrtf(r2);

    if (r > (L1 + L2) || r < fabsf(L1 - L2))
        return 0;

    float c2 = (r2 - L1*L1 - L2*L2) / (2.0f * L1 * L2);
    c2 = clampf(c2, -1.0f, 1.0f);

    float ang2 = acosf(c2);
    if (elbowUp) ang2 = -ang2;

    float s2 = sinf(ang2);
    float k1 = L1 + L2 * c2;
    float k2 = L2 * s2;

    float beta  = atan2f(y, x);
    float alpha = atan2f(k2, k1);
    float ang1  = beta - alpha;

    *theta1_deg = ang1 * 180.0f / M_PI;
    *theta2_deg = ang2 * 180.0f / M_PI;
    return 1;
}

// -------------------- Motion --------------------
void move_to(float x, float y, int elbowUp)
{
    float t1, t2;
    if (computeIK(x, y, &t1, &t2, elbowUp)) {
        WRITE_REGISTER(0x40004004, angle_to_duty_us(t1));
        WRITE_REGISTER(0x40004044, angle_to_duty_us(t2));
    }
}

void move_line(float x1, float y1, float x2, float y2, int steps, int elbowUp)
{
    for (int i = 0; i <= steps; i++) {
        float t = (float)i / (float)steps;
        float x = x1 + t * (x2 - x1);
        float y = y1 + t * (y2 - y1);
        move_to(x, y, elbowUp);
        delay_us(100);
    }
}

// -------------------- Symbol Drawing --------------------
// Example: draw letter 'L'
void draw_letter_L(void)
{
    // Start point (move without drawing if you have pen-up control)
    move_to(-5.0f, 10.0f, 0);  
    
    // Vertical stroke
    move_line(-5.0f, 10.0f, -5.0f, 5.0f, 30, 0);
    // Horizontal base
    move_line(-5.0f, 5.0f, 0.0f, 5.0f, 30, 0);
}

// Example: draw a square symbol
void draw_n(void)
{
    // Start point (move without drawing if you have pen-up control)
    // Start point (move without drawing if you have pen-up control)
    move_to(0.0f, 10.0f, 0);  
    
    // Vertical stroke
    move_line(0.0f, 10.0f, 0.0f, 5.0f, 30, 0);
    // Horizontal base
    move_line(0.0f, 5.0f, -5.0f, 10.0f, 30, 0);
    move_line(-5.0f, 10.0f, -5.0f, -5.0f, 30, 0);
    
    //Reverse
    //move_line(-5.0f, -5.0f, -5.0f, 10.0f, 30, 0);
    //move_line(0.0f, 5.0f, -5.0f, 10.0f, 30, 0);
    //move_line(-5.0f, 10.0f, 0.0f, 5.0f, 30, 0);
    //move_line(0.0f, 5.0f, 0.0f, 10.0f, 30, 0);
    //move_to(20.0f, 0.0f, 0);  
}

// -------------------- Main --------------------
void fw_main(void)
{
    // Setup PWM
    WRITE_REGISTER(0x40004000, 20000); 
    WRITE_REGISTER(0x40004008, 0x01);  
    WRITE_REGISTER(0x40004040, 20000); 
    WRITE_REGISTER(0x40004048, 0x01);  

    // Draw something
    draw_n();
    // draw_square();

    while (1) {
        delay_us(100);
    }
}

