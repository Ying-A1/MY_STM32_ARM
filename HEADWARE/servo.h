#ifndef __SERVO_H
#define __SERVO_H

#include "stm32f10x.h"

void Servo_ping(u8 Servo_ID);
void Servo_SetAngle(u8 Servo_ID,int set_angle);

#endif

