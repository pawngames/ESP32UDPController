#ifndef AERO_GYRO_H
#define AERO_GYRO_H

#include <Arduino.h>
#include <Wire.h>
#include <TrivialKalmanFilter.h>

#define DT_COVARIANCE_RK 4.0 // Estimation of the noise covariances (process)
#define DT_COVARIANCE_QK 1.0   // Estimation of the noise covariances (observation)

void setup_gyro();
void initI2C();
void writeRegMPU(int reg, int val);
void setSleepOff();
void setGyroScale();
void setAccelScale();
uint8_t readRegMPU(uint8_t reg);
void findMPU(int mpu_addr);
void checkMPU(int mpu_addr);
void initMPU();
int* readRawMPU();

#endif