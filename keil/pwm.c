#include"pwm.h"

 unsigned int D = 50,D1 = 50; //占空比为50%
 unsigned int THHL=65536-20;//每隔20ms溢出  

int time=0;

void pwm_init(void)
{	
	TMOD = 0x21; //定时器0和2开启；
  TH0 = THHL /256;//高四位初值
  TL0 = THHL %256;//低四位初值
  EA = 1; 	   //开总中断
  ET0 = 1;	   //T0开时定时器溢出
  TR0 = 1;	   //开启定时器
	
	PWM_out = 1;
}

void pwm_set(unsigned char angle)  // x值为0-180
{
//	u16 tmp; 	//临时存储变量
	
	if(angle<=0) angle=0;			//角度小于等于0，置0
	if(angle>=180) angle=180;		//角度大于等于180，置180
	
	//以下所指偏差仅是本人仿真测试所得经验值，应根据真实电路或其他仿真环境对这部分代码进行调试
//	if(angle != 0 && angle%45==0) angle=angle-5;	//角度不为0且是45倍数时，减5，降低偏差
// 舵机设置
//  Minimum Control Pulse:0.054m
// 	Maximum Control Pulse:2m
	
	D=(angle*10)+17;	

}

void time_intt1(void) interrupt 1  
{ 
	static f=0;
	if(f)
	{D1 = D;f=0;}
	else 
	{D1 = 20000-D;f=1;time++;}
	
	THHL=65536-D1;
	TH0=THHL/256;TL0=THHL%256; 
	PWM_out = !PWM_out;
}   

