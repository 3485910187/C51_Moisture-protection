#include"pwm.h"

 unsigned int D = 50,D1 = 50; //ռ�ձ�Ϊ50%
 unsigned int THHL=65536-20;//ÿ��20ms���  

int time=0;

void pwm_init(void)
{	
	TMOD = 0x21; //��ʱ��0��2������
  TH0 = THHL /256;//����λ��ֵ
  TL0 = THHL %256;//����λ��ֵ
  EA = 1; 	   //�����ж�
  ET0 = 1;	   //T0��ʱ��ʱ�����
  TR0 = 1;	   //������ʱ��
	
	PWM_out = 1;
}

void pwm_set(unsigned char angle)  // xֵΪ0-180
{
//	u16 tmp; 	//��ʱ�洢����
	
	if(angle<=0) angle=0;			//�Ƕ�С�ڵ���0����0
	if(angle>=180) angle=180;		//�Ƕȴ��ڵ���180����180
	
	//������ָƫ����Ǳ��˷���������þ���ֵ��Ӧ������ʵ��·���������滷�����ⲿ�ִ�����е���
//	if(angle != 0 && angle%45==0) angle=angle-5;	//�ǶȲ�Ϊ0����45����ʱ����5������ƫ��
// �������
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

