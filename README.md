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

For the most part, the necessary tasks and code were already developed in my previous ME 507 coursework, lab assignments, and final project. Given that they were all written in C, though, the challenge comes in converting all of that code from procedural C to object-oriented C++. Additionally, the STM32CubeIDE for programming the Nucleo uses HAL libraries written entirely in C, so massaging back and forth between the languages is unavoidable and non-trivial. 
