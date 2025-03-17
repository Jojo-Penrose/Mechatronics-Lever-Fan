/**
 * @file encoder.h
 * @author Jojo Penrose
 * @date 16 Mar 2025
 * @brief Encoder library for the STM32, ported from C to C++.
 *
 * Contains an encoder class to encapsulate a single encoder object
 * using an STM32 timer set to Encoder mode.
 */
// encoder.h
#ifndef INC_ENCODER_H_
#define INC_ENCODER_H_

#include "stm32l4xx_hal.h"

/**
 * @class enc_obj
 * @brief An encoder object to use with the STM32.
 *
 * An encoder in this context uses a full timer for AB Encoder mode, polls
 * the timer count when updated, and returns calculated angle when asked.
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
class enc_obj {
private:
    float               ticks2deg;
    int32_t             count;
    float               angle;
    TIM_HandleTypeDef * Timer;
public:
    enc_obj (TIM_HandleTypeDef * htim, int ticksprev);
    
    float update ();
    float getAngle () { return angle; }
};

#endif /* INC_ENCODER_H_ */
