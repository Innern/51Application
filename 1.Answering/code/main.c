/*
	ʵ�ֹ��ܣ�8·��������ơ�
						��1�� ���¡���ʼ��������ſ�ʼ��������������ָʾ������

						��2�����¡���ʼ�����������������������ָʾ����7���������ʾ�����߱�š�
						
						P3.0�˿����ӡ���ʼ��������
						P3.6�˿�����һ��LED��ָʾ��������
						P0�ڽ�7������ܣ���ʾ�����߱�ţ�
						P2�ڽ�8������ʽ���������α��Ϊ1~8��
						P1�ڽ�������ָʾ�ƣ���P2�ڵ�ĳһ�������������º󣬶�Ӧ��������ָʾ������
						
						
	[2024-01-31] zoya
*/

#include <reg51.h>
#include <intrins.h>

typedef unsigned char uchar;

#define GPIO_SMG	P0  // �����
#define GPIO_LED	P1  // ������ָʾ��
#define GPIO_BTN	P2  // �����߰���

unsigned char code smg[]={0x00, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f, 
	0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71, 0x00};

sbit START_BTN=P3^0;
sbit START=P3^2;
sbit START_LED=P3^6;
bit enStart=0;

uchar tmp;
uchar dispNum;
	
void delay(uchar c)
{
	uchar i,j;
	for(;c>0;c--)
	{
		for(i=0;i<142;i++)
			for(j=0;j<2;j++);
	}
}

void digProc(void)
{
	switch(tmp)
	{
		case 0xfe: dispNum=1; break;
		case 0xfd: dispNum=2; break;
		case 0xfb: dispNum=3; break;
		case 0xf7: dispNum=4; break;
		case 0xef: dispNum=5; break;
		case 0xdf: dispNum=6; break;
		case 0xbf: dispNum=7; break;
		case 0x7f: dispNum=8; break;
		default: dispNum=0; break;
	}
	
}

void main()
{
	EA=1;
	EX0=1;
	IT0=1;
	tmp=0xff;
	while(1)
	{
		if(0==START_BTN)  // ��ʼ��������
		{
			delay(10);  // ��ʼ��������
			if(0==START_BTN)
			{
				START_LED=0;  // ������ʼָʾ��
				enStart=1;  // ָʾ���Կ�ʼ���𣬵������𰴼����´����жϴ������
				tmp=0xff;  // ��ʼ��
			}
		}
		digProc();  // �����������ʾ������
		GPIO_LED=tmp;  // ����ָʾ�ư������𰴼������ĸ�����
		GPIO_SMG=smg[dispNum];  // �������ʾ�ĸ����𰴼�������
	}
}

void Int0() interrupt 0
{
	if(enStart)
	{
		enStart=0;  // �������ٰ������������Ч
		EX0=0;
		delay(10);
		EX0=1;
		tmp=GPIO_BTN;
		START_LED=1;  // ������Ϩ��ʼָʾ��
	}
}