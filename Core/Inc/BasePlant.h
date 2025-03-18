/**
 * @file BasePlant.h
 * @author Jojo Penrose
 * @date 17 Mar 2025
 * @brief Plant base class to use with a closed-loop controller.
 *
 * Derive your plant from public BasePlant to provide access to the effort input
 * method to a separate controller object.
 */
// BasePlant.h
#ifndef INC_BASEPLANT_H_
#define INC_BASEPLANT_H_

#include "stm32l4xx_hal.h"

/**
 * @class BasePlant
 * @brief An interface class to give controller & scheduler access to a plant obj.
 *
 * Override RunPlant(int) with your plant's effort input function.
 */
class BasePlant {
public:
    virtual void RunPlant (int effort) {};
};

#endif /* INC_BASEPLANT_H_ */
