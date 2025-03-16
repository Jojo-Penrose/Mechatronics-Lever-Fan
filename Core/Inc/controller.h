/**
 * @file controller.h
 * @author Jojo Penrose
 * @date 16 Mar 2025
 * @brief Closed-loop PID controller for the STM32.
 *
 * how do it do
 */
// controller.h
#ifndef INC_CONTROLLER_H_
#define INC_CONTROLLER_H_

#include "stm32l4xx_hal.h"

/**
 * @class control_obj
 * @brief A PID controller object to use with the STM32.
 *
 * how do it do
 *
 * @var enc_obj::ticks2deg
 * float -- Conversion factor from encoder ticks to angular position in degrees.
 * @var enc_obj::count
 * uint32_t -- Last polled timer count register.
 * @var enc_obj::angle
 * float -- Current angle in degrees.
 * @var enc_obj::Timer
 * TIM_HandleTypeDef * -- STM32 timer handle.
 */
class control_obj {
private:
    float               ticks2deg;
     int32_t            count;
    float               angle;
    TIM_HandleTypeDef * Timer;
public:
    control_obj (TIM_HandleTypeDef * htim, int ticksprev);
    
    void update ();
    float getAngle () { return angle; }
};

#endif /* INC_CONTROLLER_H_ */
