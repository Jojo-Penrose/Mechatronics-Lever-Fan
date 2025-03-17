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
 * Uses class templates for Plant and Sensor. Instantiated with pointers to 
 * object and class method for each.
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
 * @var control_obj::gainScaler
 * float -- If the units of the sensor and plant are order of magnitude different,
 *          use this to multiply gains by so gains set in main aren't huge/tiny.
 * @var enc_obj::setpoint
 * float -- Current controller target value.
 * @var enc_obj::sensorVal
 * float -- Last polled sensor value.
 * @var enc_obj::error
 * float -- Current plant error.
 * @var enc_obj::reimsum
 * float -- Plant error Reimann sum.
 * @var enc_obj::errlast
 * float -- Last plant error.
 * @var enc_obj::EffP
 * float -- Effort due to proportional gain.
 * @var enc_obj::EffI
 * float -- Effort due to integral gain.
 * @var enc_obj::EffD
 * float -- Effort due to derivative gain.
 * @var enc_obj::tLast
 * uint32_t -- Last time in US pulled from Timer.
 * @var enc_obj::Dt
 * float -- Delta time in SECONDS relative to tLast. Used in float effort calcs
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
    float                   Kp = 0.0, Ki = 0.0, Kd = 0.0, gainScaler = 1.0;
    float                   setpoint = 0.0;
    float                   sensorVal = 0.0, error = 0.0, reimsum = 0.0, errlast = 0.0;
    float                   EffP, EffI, EffD;
    uint32_t                tLast;
    float                   Dt;
    TIM_HandleTypeDef *     Timer;
    PlantClass *            Plant;
    void (PlantClass::*PlantFunc)(int);
    SensorClass *           Sensor;
    float (SensorClass::*SensorFunc)();
    
public:
    /**
     * @brief   Controller constructor.
     *
     * @param   htim            Handle to timer.
     * @param   PlantObj        Pointer to Plant object.
     * @param   PlantFunc       Pointer to Plant class' output function.
     * @param   SensorObj       Pointer to Sensor object.
     * @param   SensorFunc      Pointer to Sensor class' output function.
     */
    control_obj (TIM_HandleTypeDef * htim, 
        PlantClass * PlantObj, void (PlantClass::*PlantFunc_)(int), 
        SensorClass * SensorObj, float (SensorClass::*SensorFunc_)())
        : Timer(htim), Plant(PlantObj), PlantFunc(PlantFunc_), 
                       Sensor(SensorObj), SensorFunc(SensorFunc_)
        {
            // Grab current time
            tLast = Timer->Instance->CNT;
            
            // Read sensor
            readSensor();
        }

    void setGains (float Kp_, float Ki_, float Kd_, float scaler_);
    void setPoint (float setpoint_);
    void runPlant (int val);
    void readSensor();
    void getDt();
    float retSensor() { return sensorVal; }
    void runControl();
};

/**
 * @brief   Set controller gains.
 *
 * @param   Kp_     Proportional gain.
 * @param   Ki_     Integral gain.
 * @param   Kd_     Derivative gain.
 * @param   scaler_ Sensor scaler.
 */
template <class PlantClass, class SensorClass>
void control_obj <PlantClass, SensorClass> :: setGains (float Kp_, float Ki_, float Kd_, float scaler_) {
    // Update gains
    Kp = Kp_;
    Ki = Ki_;
    Kd = Kd_;
    gainScaler = scaler_;
}

/**
 * @brief   Set controller setpoint.
 *
 * @param   setpoint    Desired target sensor value.
 */
template <class PlantClass, class SensorClass>
void control_obj <PlantClass, SensorClass> :: setPoint (float setpoint_) {
    // Update setpoint
    setpoint = setpoint_;
}

/**
 * @brief   Send controller output to the plant.
 *
 * @param   val     Effort value to send to plant.
 */
template <class PlantClass, class SensorClass>
void control_obj <PlantClass, SensorClass> :: runPlant (int val) {
    // Construct pointer to plant member function, pass val
    (*Plant.*PlantFunc)(val);
}

/**
 * @brief   Read input from the sensor.
 *
 */
template <class PlantClass, class SensorClass>
void control_obj <PlantClass, SensorClass> :: readSensor () {
    // Construct pointer to sensor member function, load sensorVal
    sensorVal = static_cast< float >( (*Sensor.*SensorFunc)() );
}

/**
 * @brief   Get current time from Timer.
 *
 */
template <class PlantClass, class SensorClass>
void control_obj <PlantClass, SensorClass> :: getDt () {
    // Calculate Dt
    Dt = static_cast< float >(Timer->Instance->CNT - tLast)*1E-06;
    // Construct pointer to timer count register, update tLast
    tLast = Timer->Instance->CNT;
}

/**
 * @brief   Run the closed-loop controller.
 *
 * Calculate gains using high school calculus.
 * Proportional effort = Kp*error
 * Integral effort = Ki*sum(error * Dt) (Reimann sum)
 * Derivative effort = Kd*(error - last error)/Dt (baby's first derivative)
 */
template <class PlantClass, class SensorClass>
void control_obj <PlantClass, SensorClass> :: runControl () {
    // Get current sensor state
    readSensor();
    
    // Get Dt
    getDt();
    
    // Calculate plant error and Reimann sum for integral control
    error = setpoint - sensorVal;
    reimsum += error*Dt;
    
    // Calculate proportional effort
    EffP = Kp*gainScaler*error;
    
    // Calculate integral effort
    EffI = Ki*gainScaler*reimsum;
    
    // Calculate derivative effort
    EffD = Kd*gainScaler*(error - errlast)/Dt;
    
    // Update last error
    errlast = error;
    
    // Sum effort, run the plant
    runPlant(static_cast< int >(EffP + EffI + EffD));
}

#endif /* INC_CONTROLLER_H_ */