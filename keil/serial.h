#ifndef __SERIAL_H
#define __SERIAL_H

#include"reg52.h"

extern xdata char receive[10];
extern xdata char flag;

void Serialinit();
void Sendchar( unsigned char da);
void Sendstring(int x,char *str);


#endif