#include"reg52.h"
#include"12864.h"
#include"sht11.h"
#include"serial.h"
#include"delay.h"
#include"pwm.h"
#include"string.h"
#include"stdlib.h"
#include"stdio.h"
#include"pid.h"
#include"motor.h"

sbit ADD = P1^2;
sbit MODE = P1^3;
sbit MINUS = P1^4;
sbit LED_red = P1^5;
sbit LED_green = P1^6;

xdata int Threshold=15,Angle=90,IHumidity=0,OHumidity=0,T;
xdata char key_mod=0,sys_mod=0,flag=0;
code char com[4][10]={"SetModAT","SetModM","Set","Open"};
xdata char receive[10]=0;
float set_speed=0.0;

int abs(int l)
{
	if(l>0)return l;
	else return l*=-1;
}
 
void Key_Scan(bit mode)
{  
 static bit key_up=1;//按键按松开标志
 if(mode)key_up=1;  //支持连按    
 if(key_up&&(ADD==0||MODE==0||MINUS==0))
 {
//  DelayMs(1);//去抖动 
  key_up=0;
  if(ADD==0)
	{
		switch(key_mod%3)
		{
			case 0: sys_mod=0; break;	
			case 1: if(Angle<90&&sys_mod==1)Angle++; break;	
			case 2: if(Threshold<100)Threshold++; break;				
		}
	}
  else if(MODE==0)key_mod++;
  else if(MINUS==0)
	{
		switch(key_mod%3)
		{
			case 0: sys_mod=1; break;	
			case 1: if(Angle>-90&&sys_mod==1)Angle--; break;	
			case 2: if(Threshold>0)Threshold--; break;				
		}
	}
 }else if(ADD==1&&MODE==1&&MINUS==1)key_up=1;       // 无按键按下
}

void lcd_display()
{
	if(sys_mod)
	Display_string(0,6,"Set ");
	else
	Display_string(0,6,"Auto");		

	Display_string(1,0,"IdH");
	if(IHumidity<1000){
	Display_FH(1,2,8*4,0);
	Display_SZ(1,2,8*5,IHumidity/100);
	Display_SZ(1,2,8*6,IHumidity%100/10);}
	else
	Display_string(1,4,"100");			
	Display_string(1,9,"OdH");
	if(OHumidity<1000){
	Display_FH(2,2,8*5,0);
	Display_SZ(2,2,8*6,OHumidity/100);
	Display_SZ(2,2,8*7,OHumidity%100/10);}
	else
	Display_string(1,13,"100");	

	Display_string(2,0,"Sta Open");
	if(Angle >= 0)
	Display_FH(2,4,8*3,1);		
	else 
	Display_FH(2,4,8*3,2);
	Display_SZ(2,4,8*4,abs(Angle)/10);
	Display_SZ(2,4,8*5,abs(Angle)%10);	
	
	Display_string(3,0,"Warning");	
	Display_SZ(2,6,8*4,Threshold/10);
	Display_SZ(2,6,8*5,Threshold%10);	
	
	switch(key_mod%3)
	{
		case 0: Display_FH(2,0,8*7,10),Display_FH(2,4,8*7,0),Display_FH(2,6,8*7,0); break;	
		case 1: Display_FH(2,4,8*7,10),Display_FH(2,0,8*7,0),Display_FH(2,6,8*7,0); break;	
		case 2: Display_FH(2,6,8*7,10),Display_FH(2,0,8*7,0),Display_FH(2,4,8*7,0); break;				
	}	
}

void led()
{
	if(OHumidity/10-IHumidity/10 > Threshold && sys_mod == 0)
	{
		if(OHumidity > 50 && IHumidity >50)Angle=-90;
		LED_red = 1,LED_green = 0;
	}
	else LED_green = 1,LED_red = 0;
	if(IHumidity/10>70)set_speed=IHumidity/10-50;
	else set_speed=0.0;
}

void serial_send()
{
	Sendstring(1,"Id Humidity    ");
	if(IHumidity<1000){
	Sendchar(IHumidity/100+'0');
	Sendchar(IHumidity%100/10+'0');}
	else Sendstring(1,"100");
	Sendstring(1,"\r\n");	

	Sendstring(1,"Od Humidity    ");
	if(OHumidity<1000){
	Sendchar(OHumidity/100+'0');
	Sendchar(OHumidity%100/10+'0');}
	else Sendstring(1,"100");
	Sendstring(1,"\r\n");	
	
	Sendstring(1,"Sta      Open(");
	if(Angle >= 0)
	Sendchar('+');		
	else 
	Sendchar('-');	
	Sendchar(abs(Angle)/10+'0');
	Sendchar(abs(Angle)%10+'0');		
	Sendstring(1,")/Close");
	Sendstring(1,"\r\n");	
	
	Sendstring(1,"Warning        ");	
	Sendchar(Threshold/10+'0');
	Sendchar(Threshold%10+'0');	
	Sendstring(1,"\r\n\r\n\r\n");		
}
void serial_re()
{
	if(flag == 1)
	{
		char cmp[8]=0,num[5]=0;
		int x=0,y=0;
		while(receive[x] != '\0')
		{
			if(receive[x] >= 'A' && receive[x] <= 'Z' || receive[x] >= 'a' && receive[x] <= 'z') 
			{
				cmp[y++] =receive[x];
			}			
			x++;
			if(receive[x] == '\0')cmp[y]='\0';
		}x=0,y=0;
		while(receive[x] != '\0')
		{
			if(receive[x] >= '0' && receive[x] <= '9' || receive[x] == '-') 
			{
				num[y++] =receive[x];
			}			
			x++;
			if(receive[x] == '\0')num[y]='\0';
		}x=0,y=0;		
		
		x=atoi(num);    //数字字符串转换int 函数，需要引用头文件“stdlib.h”
		
		for(y=0;y<4;y++)
		{
			if(strcmp(cmp,com[y]) == 0)break;
		}
		if(y<4)               //有指令
		{
			switch(y)
			{
				case 0: sys_mod = 0;   break;
				case 1: sys_mod = 1;   break;
				case 2: Threshold = x; break;
//				case 3: Angle = x;     break;				//写到这里输入负数显示有问题,就写到下面了
			}
		}
		receive[10]=0;
		flag=0;
		if(y==3 && sys_mod==1)Angle=x;                         //这里
	}
}

void main()
{
	char aaa[10]=0;
	InitLCD();
	Serialinit();
	pwm_init();
	motor_init();
	pid_init(&pid_test_moto,1000,1000,6,2.3,2.5);  //6,2.3,2.5
	while(1)
	{
		pwm_set(Angle+90);
		Key_Scan(1);
		TH_output(SHT11_1,&T,&IHumidity);
		TH_output(SHT11_2,&T,&OHumidity);
		lcd_display();
		led();
		if(time>=100){time=0;serial_send();}   //两秒发送
		serial_re();
		
//		sprintf(aaa,"%.2f,%.2f",set_speed,motor_speed);
//		Sendstring(1,aaa);		
//		Sendstring(1,"\r\n");		
		
	}
}

void int0() interrupt 0    //用来计时
{
			motor_time++;
			if(motor_time >= 100)   //定时测速1s
		{
			motor_time=0;
			motor_speed=motor_a*2.5;
			motor_a=0;
			pid_out=pid_calc(&pid_test_moto,motor_speed,set_speed);
		}
		
		if(count<(unsigned int)pid_out)
		motor_out=1;
		else
		 motor_out=0;
		if(count>1000) count=0;	
}