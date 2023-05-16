#ifndef __SHT11_H
#define __SHT11_H

#include <reg52.h>
#include <intrins.h>

#define noACK 0				//继续传输数据，用于判断是否结束通讯
#define ACK   1             //结束数据传输；
							//地址  命令  读/写
#define STATUS_REG_W 0x06   //000   0011    0
#define STATUS_REG_R 0x07   //000   0011    1
#define MEASURE_TEMP 0x03   //000   0001    1
#define MEASURE_HUMI 0x05   //000   0010    1
#define RESET        0x1e   //000   1111    0

#define PX P2
//sbit Sht11DATA1 = P2^5;
//sbit Sht11DATA2= P2^6;
sbit SCK = P2^7;

enum {TEMP,HUMI};
extern unsigned char SHT11_1,SHT11_2;     //定义设备枚举sensor

typedef xdata union		   //定义共同类型
{ unsigned int i;	   //i表示测量得到的温湿度数据（int 形式保存的数据） 
 float f;			   //f表示测量得到的温湿度数据（float 形式保存的数据）
} value;

/********     SHT11函数声明      ********/
void s_connectionreset(unsigned char sensor);
char s_measure(unsigned char sensor,unsigned char *p_value, unsigned char *p_checksum, unsigned char mode);
void calc_sth10(float *p_humidity ,float *p_temperature);
//float calc_dewpoint(float h,float t);

void TH_output(unsigned char sensor,int *T,int *H);

void Writbit_PX(char bite,char dat);
bit Readbit_PX(char bite);

#endif