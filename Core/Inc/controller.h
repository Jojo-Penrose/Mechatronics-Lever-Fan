/**
 * @file controller.h
 * @author Jojo Penrose
 * @date 16 Mar 2025
 * @brief Closed-loop PID controller for the STM32.
 *
 * Controller class needs three things:
 *  1) A timer set to count ticks as unsigned microseconds
 *  2) A plant object with an "apply output" method that accepts numerical effort
 *  3) A sensor object with a "return input" method that provides numerical measurement
 * Uses BasePlant and BaseSensor base classes. See those headers for details.
 */
// controller.h
#ifndef INC_CONTROLLER_H_
#define INC_CONTROLLER_H_

#include "stm32l4xx_hal.h"
#include "BasePlant.h"
#include "BaseSensor.h"

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
 * @var control_obj::gainScaler
 * float -- If the units of the sensor and plant are order of magnitude different,
 *          use this to multiply gains by so gains set in main aren't huge/tiny.
 *          ex: Proportional gain (actual) = Kp * gainScaler
 * @var enc_obj::setpoint
 * float -- Current controller target value.
 * @var enc_obj::sensorVal
 * float -- Last polled sensor value.
 * @var enc_obj::error
 * float -- Current plant error.
 * @var enc_obj::reimsum
 * float -- Plant error Reimann sum, for integral control.
 * @var enc_obj::errlast
 * float -- Last plant error, for derivative control.
 * @var enc_obj::EffP
 * float -- Effort due to proportional gain.
 * @var enc_obj::EffI
 * float -- Effort due to integral gain.
 * @var enc_obj::EffD
 * float -- Effort due to derivative gain.
 * @var enc_obj::tLast
 * uint32_t -- Last time in MICROSECONDS pulled from Timer.
 * @var enc_obj::Dt
 * float -- Delta time in SECONDS relative to tLast. Used in float effort calcs
 * @var enc_obj::Timer
 * TIM_HandleTypeDef * -- STM32 timer handle, for timebase counts in us.
 * @var enc_obj::Plant
 * BasePlant * -- Pointer to plant object.
 * @var enc_obj::Sensor
 * BaseSensor * -- Pointer to sensor object.
 */
class control_obj {
private:
    float                   Kp = 0.0, Ki = 0.0, Kd = 0.0, gainScaler = 1.0;
    float                   setpoint = 0.0;
    float                   sensorVal = 0.0, error = 0.0, reimsum = 0.0, errlast = 0.0;
    float                   EffP, EffI, EffD;
    uint32_t                tLast;
    float                   Dt;
    TIM_HandleTypeDef *     Timer;
    BasePlant *             Plant;
    BaseSensor *            Sensor;
    
public:
    control_obj (TIM_HandleTypeDef * htim, BasePlant * Plant_, BaseSensor * Sensor_);

    void setGains (float Kp_, float Ki_, float Kd_, float scaler_);
    void setPoint (float setpoint_);
    void runPlant (int val);
    void readSensor ();
    void getDt ();
    float retSensor () { return sensorVal; }
    void runControl ();
};

#endif /* INC_CONTROLLER_H_ */