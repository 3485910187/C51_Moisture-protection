#include"motor.h"

int count = 0,motor_a = 0,motor_time = 0;
float pid_out = 50,motor_speed;

void motor_init()
{
																//16΢��@11.0592MHz
	TMOD = 0x21; //��ʱ��0��2������
  TH1 = 0xff;//����λ��ֵ
  TL1 = 0X9b;//����λ��ֵ
  EA = 1; 	   //�����ж�
  ET1 = 1;	   //T1��ʱ��ʱ�����
  TR1 = 1;	   //������ʱ��

  //�ⲿ�жϲ���
	
		EX1 = 1;    //����1���ⲿ�ж�
 
  //���ƼĴ���TCON
		IT1 = 1;   // �����ⲿ�жϴ�����ʽ.  // 0-�͵�ƽ����  // 1-�����䴥��
	
		EX0 = 1;    //����0���ⲿ�ж�
 
  //���ƼĴ���TCON
		IT0 = 1;   // �����ⲿ�жϴ�����ʽ.  // 0-�͵�ƽ����  // 1-�����䴥��
}

void timer_T1() interrupt 3       //��ʱ��1�жϺ���
{
		TH1=0xc0;//0xc0
		TL1=0Xc0;//0xc0
    count++;
//		if(count<(unsigned int)pid_out)	  //pwm1���(unsigned int)pid_out
//		 motor_out=1;
//		else
//		 motor_out=0;
//		if(count>1000) count=0;	
}

void int1() interrupt 2
{
	motor_a++;
}