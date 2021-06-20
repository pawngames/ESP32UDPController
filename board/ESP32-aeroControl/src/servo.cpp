#include <aero_servo.h>

int servoLpin = GPIO_NUM_2;
int servoRpin = GPIO_NUM_5;

Servo servo_l;
Servo servo_r;

void setup_servos() {
  servo_l.attach(servoLpin, 0, 0, 180, 1000, 2000);
  servo_r.attach(servoRpin, 1, 0, 180, 1000, 2000);
}

void set_angle(int servo, int angle){
    switch(servo){
        case SERVO_LEFT:
            servo_l.write(angle);
        break;
        case SERVO_RIGHT:
            servo_r.write(angle);
        break;
    }
}
