#ifndef __SHT11_H
#define __SHT11_H

#include <reg52.h>
#include <intrins.h>

#define noACK 0				//�����������ݣ������ж��Ƿ����ͨѶ
#define ACK   1             //�������ݴ��䣻
							//��ַ  ����  ��/д
#define STATUS_REG_W 0x06   //000   0011    0
#define STATUS_REG_R 0x07   //000   0011    1
#define MEASURE_TEMP 0x03   //000   0001    1
#define MEASURE_HUMI 0x05   //000   0010    1
#define RESET        0x1e   //000   1111    0

sbit Sht11DATA1 = P2^5;
sbit Sht11DATA2= P2^6;
sbit SCK = P2^7;

enum {TEMP,HUMI};
enum {SHT11_1,SHT11_2};     //�����豸ö��sensor

typedef xdata union		   //���干ͬ����
{ unsigned int i;	   //i��ʾ�����õ�����ʪ�����ݣ�int ��ʽ��������ݣ� 
 float f;			   //f��ʾ�����õ�����ʪ�����ݣ�float ��ʽ��������ݣ�
} value;

/********     SHT11��������      ********/
void s_connectionreset(unsigned char sensor);
char s_measure(unsigned char sensor,unsigned char *p_value, unsigned char *p_checksum, unsigned char mode);
void calc_sth10(float *p_humidity ,float *p_temperature);
//float calc_dewpoint(float h,float t);

void TH_output(unsigned char sensor,int *T,int *H);

#endif