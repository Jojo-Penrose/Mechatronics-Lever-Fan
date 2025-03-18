/**
 * @file motor.cpp
 * @author Jojo Penrose
 * @date 15 Mar 2025
 * @brief Motor PWM control library for the STM32, ported from C to C++.
 *
 * Contains a motor class to encapsulate a single motor object requiring one
 * PWM channel and one GPIO for control. Contains methods useful to motor control.
 */
// motor.cpp
#include "motor.h"

/**
 * @brief   Motor constructor.
 *
 * @param   htim            Handle to timer.
 * @param   channel         Channel number of PWM output.
 * @param   GPIOx           Handle to GPIO port.
 * @param   GPIO_Pin        GPIO pin alias.
 */
motor_obj :: motor_obj (TIM_HandleTypeDef * htim, int channel, GPIO_TypeDef * GPIOx, uint16_t GPIO_Pin) {
    // Set motor controller PWM timer handle
    Timer = htim;
    
    // Grab ARR 
    ARR = Timer->Instance->ARR;
    
    // Select CCR for the PWM channel
    switch (channel) {
        case 1: CCR = &(Timer->Instance->CCR1); break;
        case 2: CCR = &(Timer->Instance->CCR2); break;
        case 3: CCR = &(Timer->Instance->CCR3); break;
        case 4: CCR = &(Timer->Instance->CCR4); break;
        // default: // Panic!
    }
    
    // Set GPIO direction things
    DIR_Port = GPIOx;
    DIR_Pin = GPIO_Pin;
    
    // Initialize w/ motor off
    duty = 0;
    effort = 0;
    en = 0;
    
    // Make it so: set registers to brake
    HAL_GPIO_WritePin(DIR_Port, DIR_Pin, GPIO_PIN_RESET);
    *CCR = 0;
}

/**
 * @brief   Set motor duty cycle.
 *          Accepts signed desired speed from -100<->100. Checks sign,
 *          updates direction pin, saturates duty cycle, updates PWM CCR. 
 *
 * @param   effort          Desired duty cycle, signed.
 */
 void motor_obj :: setEffort (int effort) {
    // Store that speed
    effort = effort;
    
    // Disabled motor check
    if (!en){ return; }

    // Speed saturation
    if (effort > 100){ effort = 100; }
    else if (effort < -100){ effort = -100; }
    
    // Set direction pin & fill duty
    // Sign flip for reverse
    if (effort < 0) { 
        duty = -effort; 
        HAL_GPIO_WritePin(DIR_Port, DIR_Pin, GPIO_PIN_SET);
    } else { 
        duty = effort;
        HAL_GPIO_WritePin(DIR_Port, DIR_Pin, GPIO_PIN_RESET);
    }
    
    // Update timer CCR
    *CCR = ARR * duty / 100;
}

/**
 * @brief   Disables motor.
 *          Disable 'en' boolean, force direction low, set CCR to 0.
 *
 */
void motor_obj :: disable () {
    en = 0;
    
    // Make it so: set registers to brake
    HAL_GPIO_WritePin(DIR_Port, DIR_Pin, GPIO_PIN_RESET);
    *CCR = 0;
}
    
/**
 * @brief   Enables motor.
 *          Enable 'en' boolean, force direction low, set CCR to last duty cycle.
 *
 */    
void motor_obj :: enable () {
    en = 1;
    
    // Make it so: set registers to run
    HAL_GPIO_WritePin(DIR_Port, DIR_Pin, GPIO_PIN_RESET);
    *CCR = ARR * duty / 100;
}