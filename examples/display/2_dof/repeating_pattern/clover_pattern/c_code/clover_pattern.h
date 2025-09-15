#ifndef CLOVER_PATTERN_H
#define CLOVER_PATTERN_H

/**
 * @brief Starts the clover pattern trajectory for a 2-DOF robotic arm.
 * 
 * This function loops infinitely, tracing the precomputed clover
 * path by computing inverse kinematics and generating PWM signals
 * for the servos.
 */
void clover_pattern_gen(void);

#endif // CLOVER_PATTERN_H


