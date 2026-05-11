#ifndef __MOTOR__H
#define __MOTOR__H

#include <Arduino.h>
#include <driver/pcnt.h>
#include "Pinout.h"

void Motor_Init();
void Motor_SetSpeed(int speed_L, int speed_R);
int32_t Encoder_GetCount_L();
int32_t Encoder_GetCount_R();

#endif