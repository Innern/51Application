/*
	ʵ�ֹ��ܣ��¶ȿ���ϵͳ��
		�����Ե��¶ȳ�������ʱ����������&ֱ�����������
		���¶ȵ�������ʱ����������&�̵���������
	[2023-12-25] zoya
*/
#include <reg52.h>
#include "TempDS18B20.h"

typedef unsigned char u8;
typedef unsigned int u16;

#define GPIO_LED P0
sbit LSA=P2^2;
sbit LSB=P2^3;
sbit LSC=P2^4;

sbit MOTOR=P1^0;
sbit RELAY=P1^1;
sbit BEEP=P1^5;

sbit K1=P3^1;
sbit K2=P3^0;
sbit K4=P3^3;  // ��ʼ���а���

u8 code smg[]={0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};
u8 display[8];

float tmpL=23.0;// �¶�������
float	tmpH=25.0; 
float curTmp;  // ��ǰ��ʾ�¶�

u8 enRead;

// ��ʱ����
void delay(u16 i)
{
	while(i--);
}

// �������ʾ����
void DigDisplay()
{
	u8 i;
	for(i=0;i<8;i++)
	{
		switch(i)
		{
			case 0: LSA=1; LSB=1; LSC=1; break;  // Y7
			case 1: LSA=0; LSB=1; LSC=1; break;  // Y6
			case 2: LSA=1; LSB=0; LSC=1; break;  // Y5
			case 3: LSA=0; LSB=0; LSC=1; break;  // Y4
			case 4: LSA=1; LSB=1; LSC=0; break;  // Y3
			case 5: LSA=0; LSB=1; LSC=0; break;  // Y2
			case 6: LSA=1; LSB=0; LSC=0; break;  // Y1
			case 7: LSA=0; LSB=0; LSC=0; break;  // Y0
		}
		
		GPIO_LED=display[i];
		delay(200);
		GPIO_LED=0x00;
	}
}

// �¶ȴ�����
void TempPros(int temp)
{
	float tp;
	if(temp<0)
	{
		display[0]=0x40;  // -
		temp=temp-1;
		temp=~temp;
		tp=temp;
		temp=tp*0.0625*100+0.5;
	}
	else
	{
		display[0]=0x00;
		tp=temp;
		temp=tp*0.0625*100+0.5;
	}
	
	// ����¶ȳ������ޣ����������������ת��
	if((tp*0.0625)>tmpH)
	{
		BEEP=1;
		MOTOR=1;
		RELAY=1;
		
	}
	else if((tp*0.0625)<tmpL)  // ����¶ȵ������ޣ��������������̵�������
	{
		BEEP=1;
		MOTOR=0;
		RELAY=0;
	}
	else
	{
		BEEP=0;
		MOTOR=0;
		RELAY=1;
	}
	
	display[1]=smg[temp%10000/1000];
	display[2]=smg[temp%1000/100]|0x80;  // |0x80��ʾС����
	display[3]=smg[temp%100/10];
	display[4]=smg[temp%10/1];
	display[5]=0x39;
	
}

void DataPros()
{
	int temp=curTmp*100;
	if(temp<0)
	{
		display[0]=0x40;  // -
	} 
	else
	{
		display[0]=0x00;
	}
	display[1]=smg[temp%10000/1000];
	display[2]=smg[temp%1000/100]|0x80;  // |0x80��ʾС����
	display[3]=smg[temp%100/10];
	display[4]=smg[temp%10/1];
	display[5]=0x39;
	
}

// ����������
void KeyPros()
{
	u8 i;
	// K1�ϵ��¶�����
	if(0==K1)
	{
		delay1ms(10);
		if(0 == K1)
		{
			tmpH=tmpH+1;
			if(tmpH>125)
				tmpH=125;
			curTmp=tmpH;
			DataPros();
		}
		while(!K1);
	}
	
	// K2�µ��¶�����
	if(0==K2)
	{
		delay1ms(10);
		if(0 == K2)
		{
			tmpL=tmpL-1;
			if(tmpL<-55)
				tmpL=-55;
			curTmp=tmpL;
			DataPros();
		}
		while(!K2);
	}
	
	// ��K4��ϵͳ��ʼ���У���ȡ��ǰ�¶ȣ��ж��Ƿ񳬹��¶ȷ�Χ
	if(0==K4)
	{
		delay1ms(10);
		if(0==K4)
		{
			enRead=~enRead;
			if(0==enRead)
			{
				for(i=0;i<8;i++)
				{
					display[i]=0;
				}
			}
		}
		while(!K4);
	}
}

void main()
{
	enRead=0;
	BEEP=0;
	MOTOR=0;
	RELAY=1;

	while(1)
	{
		if(0!=enRead)
			TempPros(DS18B20ReadTemp());  // ϵͳ����ʱĬ������ܲ���ʾ��������K4ʱ��ʾʵʱ�¶�
		KeyPros();
		DigDisplay();
	}
}