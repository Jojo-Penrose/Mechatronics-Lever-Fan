/**
 * @file motor.h
 * @author Jojo Penrose
 * @date 15 Mar 2025
 * @brief Motor PWM control library for the STM32, ported from C to C++.
 *
 * Contains a motor class to encapsulate a single motor object requiring one
 * PWM channel and one GPIO for control. Contains methods useful to motor control.
 */
// motor.h
#ifndef INC_MOTOR_H_
#define INC_MOTOR_H_

#include "stm32l4xx_hal.h"
#include "BasePlant.h"

/**
 * @class motor_obj
 * @brief A PWM-driven motor object to use with the STM32.
 *
 * A motor in this context uses one channel of a PWM-enabled timer and one 
 * GPIO pin that is preconfigured at instantiation. 
 *
 * @var motor_obj::effort
 * int -- Signed duty cycle w/ direction, -100<->100.
 * @var motor_obj::duty
 * uint32_t -- Duty cycle, 0<->100.
 * @var motor_obj::en
 * bool -- 1 enabled, 0 disabled.
 * @var motor_obj::Timer
 * TIM_HandleTypeDef * -- STM32 timer handle.
 * @var motor_obj::ARR
 * uint32_t -- Value of timer auto-reload register.
 * @var motor_obj::CCR
 * volatile uint32_t * -- Pointer to timer capture/compare register. CCR is 32 bits wide.
 * @var motor_obj::DIR_Port
 * GPIO_TypeDef * -- STM32 GPIO port handle.
 * @var motor_obj::DIR_Pin
 * uint16_t * -- Alias for direction control GPIO pin.
 */
class motor_obj : public BasePlant {
private:
    int                 effort;
    uint32_t            duty;
    bool                en;
    TIM_HandleTypeDef * Timer;
    uint32_t            ARR;
    volatile uint32_t * CCR;
    GPIO_TypeDef *      DIR_Port;
    uint16_t            DIR_Pin;
public:
    motor_obj (TIM_HandleTypeDef * htim, int channel, GPIO_TypeDef * GPIOx, uint16_t GPIO_Pin);
    
    void RunPlant (int effort) override { setEffort(effort); }
    void setEffort (int effort);
    void disable ();
    void enable ();
    
    int getEffort () { return effort; }
    bool getEnable () { return en; }
};

#endif /* INC_MOTOR_H_ */
