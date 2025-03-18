/**
 * @file BaseSensor.h
 * @author Jojo Penrose
 * @date 17 Mar 2025
 * @brief Sensor base class to use with a closed-loop controller.
 *
 * Derive your sensor from public BaseSensor to provide access to the sensor polling
 * method to a separate controller object.
 */
// BaseSensor.h
#ifndef INC_BASESENSOR_H_
#define INC_BASESENSOR_H_

#include "stm32l4xx_hal.h"

/**
 * @class BaseSensor
 * @brief An interface class to give controller & scheduler access to a sensor obj.
 *
 * Override PollSensor(int) with your plant's sensor read function.
 */
class BaseSensor {
public:
    virtual float PollSensor () { return 0; };
};

#endif /* INC_BASESENSOR_H_ */