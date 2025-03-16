/**
 * @file encoder.cpp
 * @author Jojo Penrose
 * @date 16 Mar 2025
 * @brief Encoder library for the STM32, ported from C to C++.
 *
 * Contains an encoder class to encapsulate a single encoder object
 * using an STM32 timer set to Encoder mode.
 */
// encoder.cpp
#include "encoder.h"

/**
 * @brief   Encoder constructor.
 *
 * @param   htim            Handle to timer.
 * @param   ticksprev       Number of counter ticks in one full revolution.
 */
enc_obj::enc_obj (TIM_HandleTypeDef * htim, int TicksPerRev) {
    // Set encoder timer handle
    Timer = htim;
    
    // Grab first count 
    count = Timer->Instance->CNT;
    
    // Store ticks2deg
    ticks2deg = 360.0 / static_cast< float >(TicksPerRev);
    
    // Calculate first angle
    angle = count*ticks2deg;
}

/**
 * @brief   Update angular position register.
 *          Pull encoder counts, calculate degrees.
 *
 */
void enc_obj::update () {
    // Pull current encoder count
    count = Timer->Instance->CNT;
    
    // Calculate angle
    angle = count*ticks2deg;
}