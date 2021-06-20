#ifndef AERO_SERVO
#define AERO_SERVO

#include <Arduino.h>
#include <Servo.h>

#define SERVO_LEFT  0
#define SERVO_RIGHT 1

void setup_servos();
void set_angle(int servo, int angle);

#endif