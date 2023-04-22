#include"sht11.h"

//写字节程序
char s_write_byte(unsigned char sensor,unsigned char value)    
{ 
	unsigned char i,error=0; 
	for (i=0x80;i>0;i/=2)             //shift bit for masking 高位为1，循环右移
	{ 
		if (i&value)
			switch(sensor)                     //选择发送设备
				{	
					case SHT11_1 : Sht11DATA1=1; break;		   //和要发送的数相与，结果为发送的位
					case SHT11_2 : Sht11DATA2=1; break;	
				}
    	else Sht11DATA1=0,Sht11DATA2=0;                        
    	SCK=1;                          
    	_nop_();_nop_();_nop_();        //延时3us 
    	SCK=0;
	}
	switch(sensor)              //释放数据线
		{	
			case SHT11_1 : Sht11DATA1=1; break;
			case SHT11_2 : Sht11DATA2=1; break;	
		}                         
	SCK=1;      
	switch(sensor)             //检查应答信号，确认通讯正常
		{	
			case SHT11_1 : error=Sht11DATA1; break;
			case SHT11_2 : error=Sht11DATA2; break;	
		} 		                
	_nop_();_nop_();_nop_();
	SCK=0;        
	switch(sensor)             
		{	
			case SHT11_1 : Sht11DATA1=1; break;
			case SHT11_2 : Sht11DATA2=1; break;	
		} 
	return error;                     //error=1 通讯错误
}

//读字节程序
char s_read_byte(unsigned char sensor,unsigned char ack)
//----------------------------------------------------------------------------------
{ 
	unsigned char i,val=0;            
	switch(sensor)          					    //释放数据线
	{	
		case SHT11_1 : Sht11DATA1=1; break;
		case SHT11_2 : Sht11DATA2=1; break;	
	} 
	for(i=0x80;i>0;i>>=1)          		   //高位为1，循环右移
	{ 
		SCK=1;                         
		switch(sensor)                    //读一位数据线的值  
			{	
				case SHT11_1 :if(Sht11DATA1) val=(val|i); break;
				case SHT11_2 :if(Sht11DATA2) val=(val|i); break;	
			} 
    	SCK=0;       
	}       
	switch(sensor)           				   //如果是校验，读取完后结束通讯；
		{	
			case SHT11_1 : Sht11DATA1=!ack; break;
			case SHT11_2 : Sht11DATA2=!ack; break;	
		} 	
	SCK=1;                            
	_nop_();_nop_();_nop_();          //延时3us 
	SCK=0;   
	_nop_();_nop_();_nop_();       
	switch(sensor)             			  //释放数据线
		{	
			case SHT11_1 : Sht11DATA1=1; break;
			case SHT11_2 : Sht11DATA2=1; break;	
		} 
	return val;
}

//启动传输
void s_transstart(unsigned char sensor)
// generates a transmission start 
//       _____         ________
// DATA:      |_______|
//           ___     ___
// SCK : ___|   |___|   |______
{ 
		switch(sensor)             
			{	
				case SHT11_1 : Sht11DATA1=1; break;
				case SHT11_2 : Sht11DATA2=1; break;	
			} 
		SCK=0;                                   //准备
   	_nop_();
  	SCK=1;
   	_nop_();
		switch(sensor)             
			{	
				case SHT11_1 : Sht11DATA1=0; break;
				case SHT11_2 : Sht11DATA2=0; break;	
			} 
   	_nop_();
  	SCK=0; 
  	 _nop_();_nop_();_nop_();
   	SCK=1;
   	_nop_();
		switch(sensor)             
			{	
				case SHT11_1 : Sht11DATA1=1; break;
				case SHT11_2 : Sht11DATA2=1; break;	
			}    
  	_nop_();
   	SCK=0;     
}


//连接复位
void s_connectionreset(unsigned char sensor)
// communication reset: DATA-line=1 and at least 9 SCK cycles followed by transstart
//       _____________________________________________________         ________
// DATA:                                                      |_______|
//          _    _    _    _    _    _    _    _    _        ___     ___
// SCK : __| |__| |__| |__| |__| |__| |__| |__| |__| |______|   |___|   |______
{ 
	unsigned char i; 
	switch(sensor)             
		{	
			case SHT11_1 : Sht11DATA1=1; break;
			case SHT11_2 : Sht11DATA2=1; break;	
		} 	
	SCK=0;                    //准备
	for(i=0;i<9;i++)                  //DATA保持高，SCK时钟触发9次，发送启动传输，通迅即复位
	{ 
		SCK=1;
    	SCK=0;
	}
	s_transstart(sensor);                   //启动传输
}


//软复位程序
char s_softreset(unsigned char sensor)
// resets the sensor by a softreset 
{ 
	unsigned char error=0; 
	s_connectionreset(sensor);              //启动连接复位
	error+=s_write_byte(sensor,RESET);       //发送复位命令
	return error;                     //error=1 通讯错误
}


/*
//读状态寄存器
char s_read_statusreg(unsigned char *p_value, unsigned char *p_checksum)
//----------------------------------------------------------------------------------
// reads the status register with checksum (8-bit)
{ 
	unsigned char error=0;
	s_transstart();                   //transmission start
	error=s_write_byte(STATUS_REG_R); //send command to sensor
	*p_value=s_read_byte(ACK);        //read status register (8-bit)
	*p_checksum=s_read_byte(noACK);   //read checksum (8-bit) 
	return error;                     //error=1 in case of no response form the sensor
}

//写状态寄存器
char s_write_statusreg(unsigned char *p_value)
// writes the status register with checksum (8-bit)
{ 
	unsigned char error=0;
	s_transstart();                   //transmission start
	error+=s_write_byte(STATUS_REG_W);//send command to sensor
	error+=s_write_byte(*p_value);    //send value of status register
	return error;                     //error>=1 in case of no response form the sensor
}															*/			   

           
//温湿度测量
char s_measure(unsigned char sensor,unsigned char *p_value, unsigned char *p_checksum, unsigned char mode)
// 进行温度或者湿度转换，由参数mode决定转换内容；
{ 
//	enum {TEMP,HUMI};		 //已经在头文件中定义
	 unsigned error=0;
	 unsigned int i;

	s_transstart(sensor);                   //启动传输
	switch(mode)                     //选择发送命令
    {	
		case TEMP : error+=s_write_byte(sensor,MEASURE_TEMP); break;		   //测量温度
    	case HUMI : error+=s_write_byte(sensor,MEASURE_HUMI); break;		   //测量湿度
    	default     : break; 
	}
	for (i=0;i<65535;i++)
	{
		if(sensor == SHT11_1)if(Sht11DATA1==0) break; //等待测量结束
		if(sensor == SHT11_2)if(Sht11DATA2==0) break; //等待测量结束
	}
	if(sensor == SHT11_1)if(Sht11DATA1) error+=1;                // 如果长时间数据线没有拉低，说明测量错误 
	if(sensor == SHT11_2)if(Sht11DATA2) error+=1; 	
	*(p_value) =s_read_byte(sensor,ACK);    //读第一个字节，高字节 (MSB)
	*(p_value+1)=s_read_byte(sensor,ACK);    //读第二个字节，低字节 (LSB)
	*p_checksum =s_read_byte(sensor,noACK); //read CRC校验码
	return error;					 // error=1 通讯错误
}

//温湿度值标度变换及温度补偿
void calc_sth10(float *p_humidity ,float *p_temperature)
{ 
	code const float C1=-4.0;              // 12位湿度精度 修正公式
	code const float C2=+0.0405;           // 12位湿度精度 修正公式
	code const float C3=-0.0000028;        // 12位湿度精度 修正公式
//	code const float T1=+0.01;             // 14位温度精度 5V条件  修正公式
//	code const float T2=+0.00008;          // 14位温度精度 5V条件  修正公式

	 xdata float rh=*p_humidity;             // rh:      12位 湿度 
	 xdata float t=*p_temperature;           // t:       14位 温度
	 xdata float rh_lin;                     // rh_lin: 湿度 linear值
	 xdata float rh_true;                    // rh_true: 湿度 ture值
	 xdata float t_C;                        // t_C   : 温度 ℃

	t_C=t*0.01 - 40;                  //补偿温度
	rh_lin=C3*rh*rh + C2*rh + C1;     //相对湿度非线性补偿
//	rh_true=(t_C-25)*(T1+T2*rh)+rh_lin;   //相对湿度对于温度依赖性补偿
	rh_true=rh_lin-3;   //取消湿度补偿
	if(rh_true <16.9)rh_true-=1;
		if(rh_true >71.9)rh_true+=1;
			if(rh_true >86.9)rh_true+=1;
				if(rh_true >95.9)rh_true+=1;

	if(rh_true>100)rh_true=100;       //湿度最大修正
	if(rh_true<0.1)rh_true=0.1;       //湿度最小修正

	*p_temperature=t_C;               //返回温度结果
	*p_humidity=rh_true;              //返回湿度结果
}

//从相对温度和湿度计算露点
/*float calc_dewpoint(float h,float t)
{
	float logEx,dew_point;
	logEx=0.66077+7.5*t/(237.3+t)+(log10(h)-2);
	dew_point = (logEx - 0.66077)*237.3/(0.66077+7.5-logEx);
	return dew_point;
}		
*/

void TH_output(unsigned char sensor,int *T,int *H)
{

	value humi_val,temp_val;		//定义两个共同体，一个用于湿度，一个用于温度
//	float dew_point;			    //用于记录露点值
	unsigned char error;		    //用于检验是否出现错误
	unsigned char checksum;			//CRC		
	
		error=0;					   //初始化error=0，即没有错误
		error+=s_measure(sensor,(unsigned char*)&temp_val.i,&checksum,TEMP); //温度测量
		error+=s_measure(sensor,(unsigned char*)&humi_val.i,&checksum,HUMI); //湿度测量
    	if(error!=0) s_connectionreset(sensor);                 ////如果发生错误，系统复位
    	else
    	{ 
			humi_val.f=(float)humi_val.i;                   //转换为浮点数
     		temp_val.f=(float)temp_val.i;                   //转换为浮点数
     		calc_sth10(&humi_val.f,&temp_val.f);            //修正相对湿度及温度
//   		dew_point=calc_dewpoint(humi_val.f,temp_val.f); //计算e dew_point
   			*T=temp_val.f*10;
     		*H=humi_val.f*10;
			}
}

