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
#include <functional>

/**
 * @class control_obj
 * @brief A PID controller object to use with the STM32.
 *
 * how do it do
 *
 * @var control_obj::Kp
 * float -- Proportional control gain.
 * @var control_obj::Ki
 * float -- Integral control gain.
 * @var control_obj::Kd
 * float -- Derivative control gain.
 * @var enc_obj::setpoint
 * float -- Current controller target value.
 * @var enc_obj::Timer
 * TIM_HandleTypeDef * -- STM32 timer handle, for timebase counts in us.
 * @var enc_obj::Plant
 * PlantClass * -- Pointer to plant object.
 * @var enc_obj::Timer
 * void (PlantClass::*PlantFunc)(int) -- Pointer to Plant.(output function).
 */
template <typename PlantClass> 
class control_obj {
private:
    float                   Kp, Ki, Kd;
    float                   setpoint;
    float                   sensorVal;
    TIM_HandleTypeDef *     Timer;
    PlantClass *            Plant;
    void (PlantClass::*PlantFunc)(int);
    
public:
    control_obj (TIM_HandleTypeDef * htim, 
        PlantClass * PlantObj, void (PlantClass::*PlantFunc)(int))
        : Timer(htim), Plant(PlantObj), PlantFunc(PlantFunc) {}

    void runPlant (int val);
    void readSensor();
};

/**
 * @brief   Send controller output to the plant.
 *
 * @param   val             Effort value to send to plant.
 */
template <typename PlantClass>
void control_obj<PlantClass>::runPlant (int val) {
    // Construct pointer to plant member function, pass val
    (*Plant.*PlantFunc)(val);
}

/**
 * @brief   Read input from the sensor.
 *
 */
template <typename PlantClass>
void control_obj<PlantClass>::readSensor () {
    // Load sensorVal
}

#endif /* INC_CONTROLLER_H_ */
