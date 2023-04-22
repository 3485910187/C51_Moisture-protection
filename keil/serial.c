#include"serial.h"

//sfr T2MOD  = 0xC9;

void Serialinit()
{               //定时器2作为串口，配置波特率为9600
//	T2MOD = 0x01;
	T2CON = 0x30;
	TH2 = 0xFF;
	TL2 = 0xDC;
	RCAP2H = 0xFF;
	RCAP2L = 0xDC;
	SCON = 0x50;
	PCON = 0x00;
	TR2 = 1;
	ES = 1;
	EA = 1;
}

void Sendchar( unsigned char da)
{
	SBUF = da;
	while(!TI);
	TI = 0;
}

void Sendstring(int x,char *str)
{
  char *p = str;
	int i;
	for(i=0;i<x;i++)
	{
		p = str;
		while(*p != '\0')
		{
			Sendchar(*p++);
		}
	}
}


//接受中断

void Seral_interrput() interrupt 4
{
	static unsigned char a=0;//定义一个静态变量来保存接受的字符串
  char temp;
	if(RI)
	{
		temp=SBUF;
		
		if(temp != 0x0D)//不等于\n说明没接收完
		{
			receive[a]=temp;
			a++;
		}
		else
		{
//			receive[a]='\r';//因为判断漏掉了
//			receive[a+1]='\n';//因为判断漏掉了
			receive[a]='\0';//加上结束符号
			a=0;
			flag=1;//接收指令完成标志位
		}
		RI=0;
	}
}

