#ifndef FLOWER_PATTERN_H
#define FLOWER_PATTERN_H

/**
 * @brief Starts the flower pattern trajectory for a 2-DOF robotic arm.
 * 
 * This function loops infinitely, tracing the precomputed flower
 * path by computing inverse kinematics and generating PWM signals
 * for the servos.
 */
void flower_pattern_gen(void);

#endif // FLOWER_PATTERN_H


