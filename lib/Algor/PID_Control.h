#ifndef __PID__CONTROL__H
#define __PID__CONTROL__H

#include <Arduino.h>
#include <math.h>
#include "motor.h"
#include "mpu6050.h"

void Balance_Init();
float Balance_GetPitch(float dt_actual);
float Balance_Complete_PID(void);
void Balance_SetTarget(float angle_deg);
void Calibrate_Gyro();

#endif
