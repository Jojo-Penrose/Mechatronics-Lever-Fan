# Mechatronics-Lever-Fan
## Overview
The purpose of this project is to refresh my memory on C++ and retrain my brain on OOP after using C heavily in recent projects. Additionally, it serves as a brief showcase of my experience writing mechatronics embedded systems software for the STM32.

<img src="https://github.com/user-attachments/assets/43ec21c6-a867-4206-829f-bbf028549976" width="300">

Functionally, the project consists of a swinging lever fixed at one end to the shaft of a rotary encoder. A small drone fan motor attached is at the other end. Driving the motor causes the fan to create a force at the end perpendicular to the lever in the plane of rotation, which in turn causes the lever to rotate about the end attached to the encoder. The encoder sends angular position data to the microcontroller, which it uses in turn to control the speed of the DC motor. The goal is for the microcontroller to use closed-loop control to stabilize the lever at a pre-determined angle.

## Control System Components
The microcontroller code will need, at minimum:

* A motor control task
* An encoder measurement task
* A closed-loop controller
* A scheduler

Some of the necessary tasks and code were already developed in my previous ME 507 coursework, lab assignments, and final project. Given that they were all written in C, though, the challenge comes in converting all of that code from procedural C to object-oriented C++. Additionally, the STM32CubeIDE for programming the Nucleo uses HAL libraries written entirely in C, so massaging back and forth between the languages is unavoidable and non-trivial. 

## First Version Complete: 3/17/25
First functional prototype is done. The motor and encoder classes are derived from BasePlant and BaseSensor base classes that have virtual RunPlant and PollSensor methods, respectively. By overriding these in derived classes, they become callable from pointers to those objects within other classes that only need to understand BasePlant and BaseSensor. So, the controller class doesn't need to know the exact implementation, just that it has a "plant" to drive and a "sensor" to get feedback from. This makes it very easy to port BasePlant, BaseSensor, and controller to any other project using any "plant" and "sensor". For this, the motor and encoder classes themselves are fairly straighforward. A controller object is instantiated with a pointer to an STM32 timer handle assumed to be counting microseconds, a pointer to an object derived from BasePlant, and a pointer to an object derived from BaseSensor. Then, set gains and setpoint, and run the controller on an interval.

As far as task scheduling goes, the current scheme is round-robin on a 1ms period. Timer 5's output compare ISR goes off every 1000us. It raises a flag, which signals the main loop to call the controller run method, which in turn reads sensor, reads Timer 5 elapsed time between calls, computes PID gains, and then pipes the sum of their efforts into the plant. If the UART transmit buffer is empty, then it will also report the current encoder angle to the terminal.

### Demo video showing setpoint 35.0 degrees:
https://github.com/user-attachments/assets/ca14ebcc-83bd-46b4-874f-0d4ce1688e5d

The gains were tuned by experiment. A little bit of testing with the lever's free vibration showed that the friction in the encoder doesn't behave like an ideal damper, and it seems to kick into a kind of stiction when the lever speed decreases. Meaning, at the start of a step when the motor effort is high and the lever is swinging quickly, it behaves like a second-order pendulum with damping ratio close to 0.05 (approximated using log decrement method). But when the lever starts to settle, still moving, the pivot suddenly gets really stiff. This is noticeable if you lift the lever and let it swing freely -- the lever tends to do a couple of fast oscillations before suddenly stopping at an angle within 3 or 4 degrees of zero. As a consequence, the system is extremely sensitive to integral gain. Ki should be low when the speed is high because the damping is so low. But once the lever starts slowing down, Ki needs to be fairly high to overcome the static friction and pull into the setpoint. You can see the lever "twitch" into the setpoint every time this happens. It was found that Ki = 0.0000015 is fairly stable (shown in the video), but 0.0000020 overshoots and is wildly unstable. PID control assumes the system is LTI and this one super isn't, but for the purposes of this demonstration, it works. Something like feed-forward might work better.

The other problem with integral gain is that the gain gets multiplied by the delta time between calls. Since the schedule is set to a wide 1ms, this control effort is huge compared to the others. The next version should be set up to run the controller a lot faster.

Currently, RunPlant and PollSensor call the actual run and poll methods for the Plant and Sensor objects. For the motor, that means running all of the conversions from effort to duty cycle and writing to the PWM timer CCR registers. For the encoder, that means reading the encoder timer count CNT registers. The next goal is to lower the overhead of RunPlant and PollSensor by reducing those methods to writing an effort value to a BasePlant attribute and returning the sensor value from a BaseSensor attribute. Then, the actual motor run and encoder polling methods can be run as independent tasks that work off of whatever they happen to find in their own attributes rathen than be 100% "puppeted" by the controller. Once all three tasks are separated (plus one for updating the UART terminal), a task scheduler will be written to control the program flow.

Additionally, the PETG lever is way too heavy. The motor taps out at about 40 degrees. Need to redesign the arm for serious weight reduction and reprint it. PID gains will have to be retuned.
