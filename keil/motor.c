#include"motor.h"

int count = 0,motor_a = 0,motor_time = 0;
float pid_out = 50,motor_speed;

void motor_init()
{
																//16微秒@11.0592MHz
	TMOD = 0x21; //定时器0和2开启；
  TH1 = 0xff;//高四位初值
  TL1 = 0X9b;//低四位初值
  EA = 1; 	   //开总中断
  ET1 = 1;	   //T1开时定时器溢出
  TR1 = 1;	   //开启定时器

  //外部中断测速
	
		EX1 = 1;    //开启1号外部中断
 
  //控制寄存器TCON
		IT1 = 1;   // 设置外部中断触发方式.  // 0-低电平触发  // 1-负跳变触发
	
		EX0 = 1;    //开启0号外部中断
 
  //控制寄存器TCON
		IT0 = 1;   // 设置外部中断触发方式.  // 0-低电平触发  // 1-负跳变触发
}

void timer_T1() interrupt 3       //定时器1中断函数
{
		TH1=0xc0;//0xc0
		TL1=0Xc0;//0xc0
    count++;
//		if(count<(unsigned int)pid_out)	  //pwm1输出(unsigned int)pid_out
//		 motor_out=1;
//		else
//		 motor_out=0;
//		if(count>1000) count=0;	
}

void int1() interrupt 2
{
	motor_a++;
}