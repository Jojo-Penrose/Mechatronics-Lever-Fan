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
template <class PlantClass, class SensorClass> 
class control_obj {
private:
    float                   Kp, Ki, Kd;
    float                   setpoint;
    float                   sensorVal;
    TIM_HandleTypeDef *     Timer;
    PlantClass *            Plant;
    void (PlantClass::*PlantFunc)(int);
    SensorClass *           Sensor;
    float (SensorClass::*SensorFunc)();
    
public:
    control_obj (TIM_HandleTypeDef * htim, 
        PlantClass * PlantObj, void (PlantClass::*PlantFunc)(int), 
        SensorClass * SensorObj, float (SensorClass::*SensorFunc)())
        : Timer(htim), Plant(PlantObj), PlantFunc(PlantFunc), 
                       Sensor(SensorObj), SensorFunc(SensorFunc) {}

    void runPlant (int val);
    void readSensor();
    float retSensor() { return sensorVal; }
};

/**
 * @brief   Send controller output to the plant.
 *
 * @param   val             Effort value to send to plant.
 */
template <class PlantClass, class SensorClass>
void control_obj<PlantClass, SensorClass>::runPlant (int val) {
    // Construct pointer to plant member function, pass val
    (*Plant.*PlantFunc)(val);
}

/**
 * @brief   Read input from the sensor.
 *
 */
template <class PlantClass, class SensorClass>
void control_obj<PlantClass, SensorClass>::readSensor () {
    // Construct pointer to sensor member function, load sensorVal
    sensorVal = static_cast<float>((*Sensor.*SensorFunc)());
}

#endif /* INC_CONTROLLER_H_ */