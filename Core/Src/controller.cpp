/**
 * @file controller.h
 * @author Jojo Penrose
 * @date 16 Mar 2025
 * @brief Closed-loop PID controller for the STM32.
 *
 * how do it do
 */
// controller.h
#include "controller.h"

/**
 * @brief   Controller constructor.
 *
 * @param   htim            Handle to timer.
 * @param   ticksprev       Number of counter ticks in one full revolution.
 */
control_obj::control_obj (TIM_HandleTypeDef * htim, int TicksPerRev) {
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
void control_obj::update () {
    // Pull current encoder count
    count = Timer->Instance->CNT;
    
    // Calculate angle
    angle = count*ticks2deg;
}