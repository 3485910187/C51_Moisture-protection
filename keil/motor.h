#ifndef _MOTOR_H
#define _MOTOR_H

#include"reg52.h"

sbit motor_out = P3^4;//���PWM�ź�
extern int count,motor_a,motor_time;
extern float pid_out,motor_speed;

void motor_init();

#endif