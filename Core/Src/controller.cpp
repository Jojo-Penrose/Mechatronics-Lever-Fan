/**
 * @file controller.h
 * @author Jojo Penrose
 * @date 16 Mar 2025
 * @brief Closed-loop PID controller for the STM32.
 *
 * Because of the generic templates used in controller class, this file
 * can't actually have any methods in it for it to work. Sorry!
 * 
 * If methods are defined here, the compiler won't know ahead of time what
 * classes are valid for PlantClass and SensorClass. The two solutions are:
 *  1) Keep method definitions in controller.h so the compiler can look at
 *     them AFTER going through main.cpp and seeing the classes it needs
 *  2) Fill the end of this .cpp fill with a list of POSSIBLE classes for
 *     PlantClass and SensorClass.
 * Since we want this controller to be as portable as possible, option 1)
 * is what we'll use.
 */
// controller.h
#include "controller.h"