#include"sht11.h"

//д�ֽڳ���
char s_write_byte(unsigned char sensor,unsigned char value)    
{ 
	unsigned char i,error=0; 
	for (i=0x80;i>0;i/=2)             //shift bit for masking ��λΪ1��ѭ������
	{ 
		if (i&value)
			switch(sensor)                     //ѡ�����豸
				{	
					case SHT11_1 : Sht11DATA1=1; break;		   //��Ҫ���͵������룬���Ϊ���͵�λ
					case SHT11_2 : Sht11DATA2=1; break;	
				}
    	else Sht11DATA1=0,Sht11DATA2=0;                        
    	SCK=1;                          
    	_nop_();_nop_();_nop_();        //��ʱ3us 
    	SCK=0;
	}
	switch(sensor)              //�ͷ�������
		{	
			case SHT11_1 : Sht11DATA1=1; break;
			case SHT11_2 : Sht11DATA2=1; break;	
		}                         
	SCK=1;      
	switch(sensor)             //���Ӧ���źţ�ȷ��ͨѶ����
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
	return error;                     //error=1 ͨѶ����
}

//���ֽڳ���
char s_read_byte(unsigned char sensor,unsigned char ack)
//----------------------------------------------------------------------------------
{ 
	unsigned char i,val=0;            
	switch(sensor)          					    //�ͷ�������
	{	
		case SHT11_1 : Sht11DATA1=1; break;
		case SHT11_2 : Sht11DATA2=1; break;	
	} 
	for(i=0x80;i>0;i>>=1)          		   //��λΪ1��ѭ������
	{ 
		SCK=1;                         
		switch(sensor)                    //��һλ�����ߵ�ֵ  
			{	
				case SHT11_1 :if(Sht11DATA1) val=(val|i); break;
				case SHT11_2 :if(Sht11DATA2) val=(val|i); break;	
			} 
    	SCK=0;       
	}       
	switch(sensor)           				   //�����У�飬��ȡ������ͨѶ��
		{	
			case SHT11_1 : Sht11DATA1=!ack; break;
			case SHT11_2 : Sht11DATA2=!ack; break;	
		} 	
	SCK=1;                            
	_nop_();_nop_();_nop_();          //��ʱ3us 
	SCK=0;   
	_nop_();_nop_();_nop_();       
	switch(sensor)             			  //�ͷ�������
		{	
			case SHT11_1 : Sht11DATA1=1; break;
			case SHT11_2 : Sht11DATA2=1; break;	
		} 
	return val;
}

//��������
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
		SCK=0;                                   //׼��
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


//���Ӹ�λ
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
	SCK=0;                    //׼��
	for(i=0;i<9;i++)                  //DATA���ָߣ�SCKʱ�Ӵ���9�Σ������������䣬ͨѸ����λ
	{ 
		SCK=1;
    	SCK=0;
	}
	s_transstart(sensor);                   //��������
}


//��λ����
char s_softreset(unsigned char sensor)
// resets the sensor by a softreset 
{ 
	unsigned char error=0; 
	s_connectionreset(sensor);              //�������Ӹ�λ
	error+=s_write_byte(sensor,RESET);       //���͸�λ����
	return error;                     //error=1 ͨѶ����
}


/*
//��״̬�Ĵ���
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

//д״̬�Ĵ���
char s_write_statusreg(unsigned char *p_value)
// writes the status register with checksum (8-bit)
{ 
	unsigned char error=0;
	s_transstart();                   //transmission start
	error+=s_write_byte(STATUS_REG_W);//send command to sensor
	error+=s_write_byte(*p_value);    //send value of status register
	return error;                     //error>=1 in case of no response form the sensor
}															*/			   

           
//��ʪ�Ȳ���
char s_measure(unsigned char sensor,unsigned char *p_value, unsigned char *p_checksum, unsigned char mode)
// �����¶Ȼ���ʪ��ת�����ɲ���mode����ת�����ݣ�
{ 
//	enum {TEMP,HUMI};		 //�Ѿ���ͷ�ļ��ж���
	 unsigned error=0;
	 unsigned int i;

	s_transstart(sensor);                   //��������
	switch(mode)                     //ѡ��������
    {	
		case TEMP : error+=s_write_byte(sensor,MEASURE_TEMP); break;		   //�����¶�
    	case HUMI : error+=s_write_byte(sensor,MEASURE_HUMI); break;		   //����ʪ��
    	default     : break; 
	}
	for (i=0;i<65535;i++)
	{
		if(sensor == SHT11_1)if(Sht11DATA1==0) break; //�ȴ���������
		if(sensor == SHT11_2)if(Sht11DATA2==0) break; //�ȴ���������
	}
	if(sensor == SHT11_1)if(Sht11DATA1) error+=1;                // �����ʱ��������û�����ͣ�˵���������� 
	if(sensor == SHT11_2)if(Sht11DATA2) error+=1; 	
	*(p_value) =s_read_byte(sensor,ACK);    //����һ���ֽڣ����ֽ� (MSB)
	*(p_value+1)=s_read_byte(sensor,ACK);    //���ڶ����ֽڣ����ֽ� (LSB)
	*p_checksum =s_read_byte(sensor,noACK); //read CRCУ����
	return error;					 // error=1 ͨѶ����
}

//��ʪ��ֵ��ȱ任���¶Ȳ���
void calc_sth10(float *p_humidity ,float *p_temperature)
{ 
	code const float C1=-4.0;              // 12λʪ�Ⱦ��� ������ʽ
	code const float C2=+0.0405;           // 12λʪ�Ⱦ��� ������ʽ
	code const float C3=-0.0000028;        // 12λʪ�Ⱦ��� ������ʽ
//	code const float T1=+0.01;             // 14λ�¶Ⱦ��� 5V����  ������ʽ
//	code const float T2=+0.00008;          // 14λ�¶Ⱦ��� 5V����  ������ʽ

	 xdata float rh=*p_humidity;             // rh:      12λ ʪ�� 
	 xdata float t=*p_temperature;           // t:       14λ �¶�
	 xdata float rh_lin;                     // rh_lin: ʪ�� linearֵ
	 xdata float rh_true;                    // rh_true: ʪ�� tureֵ
	 xdata float t_C;                        // t_C   : �¶� ��

	t_C=t*0.01 - 40;                  //�����¶�
	rh_lin=C3*rh*rh + C2*rh + C1;     //���ʪ�ȷ����Բ���
//	rh_true=(t_C-25)*(T1+T2*rh)+rh_lin;   //���ʪ�ȶ����¶������Բ���
	rh_true=rh_lin-3;   //ȡ��ʪ�Ȳ���
	if(rh_true <16.9)rh_true-=1;
		if(rh_true >71.9)rh_true+=1;
			if(rh_true >86.9)rh_true+=1;
				if(rh_true >95.9)rh_true+=1;

	if(rh_true>100)rh_true=100;       //ʪ���������
	if(rh_true<0.1)rh_true=0.1;       //ʪ����С����

	*p_temperature=t_C;               //�����¶Ƚ��
	*p_humidity=rh_true;              //����ʪ�Ƚ��
}

//������¶Ⱥ�ʪ�ȼ���¶��
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

	value humi_val,temp_val;		//����������ͬ�壬һ������ʪ�ȣ�һ�������¶�
//	float dew_point;			    //���ڼ�¼¶��ֵ
	unsigned char error;		    //���ڼ����Ƿ���ִ���
	unsigned char checksum;			//CRC		
	
		error=0;					   //��ʼ��error=0����û�д���
		error+=s_measure(sensor,(unsigned char*)&temp_val.i,&checksum,TEMP); //�¶Ȳ���
		error+=s_measure(sensor,(unsigned char*)&humi_val.i,&checksum,HUMI); //ʪ�Ȳ���
    	if(error!=0) s_connectionreset(sensor);                 ////�����������ϵͳ��λ
    	else
    	{ 
			humi_val.f=(float)humi_val.i;                   //ת��Ϊ������
     		temp_val.f=(float)temp_val.i;                   //ת��Ϊ������
     		calc_sth10(&humi_val.f,&temp_val.f);            //�������ʪ�ȼ��¶�
//   		dew_point=calc_dewpoint(humi_val.f,temp_val.f); //����e dew_point
   			*T=temp_val.f*10;
     		*H=humi_val.f*10;
			}
}

