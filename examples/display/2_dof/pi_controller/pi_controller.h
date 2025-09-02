#ifndef PI_CONTROLLER_H
#define PI_CONTROLLER_H

// Arm parameters
#define L1 10.0f
#define L2 10.0f

// PI controller gains
#define KP 1.0f
#define KI 0.5f

// Function prototypes
int computeIK(float x, float y, float* theta1_deg, float* theta2_deg);
unsigned int pi_control(float desired_angle_deg, unsigned int reg_addr, float* integral);
void fw_main(void);

#endif // PI_CONTROLLER_H
