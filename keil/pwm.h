#ifndef _PWM_H
#define _PWM_H

#include"reg52.h"

//舵机设置
//  Minimum Control Pulse:0.054m
// 	Maximum Control Pulse:2m

sbit PWM_out = P3^5;//输出PWM信号

extern  unsigned int D,D1; //占空比为50%
extern  unsigned int THHL;//每隔0.5ms溢出
extern int time;

void pwm_init(void);
void pwm_set(unsigned char x);

#endif 