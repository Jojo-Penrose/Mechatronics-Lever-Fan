/**
 * @file controller.cpp
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
// controller.cpp
#include "controller.h"

/**
 * @brief   Controller constructor.
 *
 * @param   htim            Handle to timer.
 * @param   Plant_          Pointer to BasePlant-derived plant object.
 * @param   Sensor_         Pointer to BaseSensor-derived sensor object.
 */
control_obj :: control_obj (TIM_HandleTypeDef * htim, BasePlant * Plant_, BaseSensor * Sensor_) {
    // Store members
    Timer = htim;
    Plant = Plant_;
    Sensor = Sensor_;
    
    // Grab current time
    tLast = Timer->Instance->CNT;
    
    // Read sensor
    readSensor();
}

/**
 * @brief   Set controller gains.
 *
 * @param   Kp_         Proportional gain.
 * @param   Ki_         Integral gain.
 * @param   Kd_         Derivative gain.
 * @param   scaler_     Gain scaler.
 */
void control_obj :: setGains (float Kp_, float Ki_, float Kd_, float scaler_) {
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
void control_obj :: setPoint (float setpoint_) {
    // Update setpoint
    setpoint = setpoint_;
}

/**
 * @brief   Send controller output to the plant.
 *
 * @param   val     Effort value to send to plant.
 */
void control_obj :: runPlant (int val) {
    // Point to BasePlant RunPlant function, pass val
    Plant->RunPlant(val);
}

/**
 * @brief   Read input from the sensor.
 *
 */
void control_obj :: readSensor () {
    // Point to BaseSensor PollSensor function, load sensorVal
    sensorVal = static_cast< float >( Sensor->PollSensor() );
}

/**
 * @brief   Get current time from Timer.
 *
 */
void control_obj :: getDt () {
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
void control_obj :: runControl () {
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