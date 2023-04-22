#ifndef _PWM_H
#define _PWM_H

#include"reg52.h"

//�������
//  Minimum Control Pulse:0.054m
// 	Maximum Control Pulse:2m

sbit PWM_out = P3^5;//���PWM�ź�

extern  unsigned int D,D1; //ռ�ձ�Ϊ50%
extern  unsigned int THHL;//ÿ��0.5ms���
extern int time;

void pwm_init(void);
void pwm_set(unsigned char x);

#endif 