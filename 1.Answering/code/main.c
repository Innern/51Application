/*
	实现功能：8路抢答器设计。
						（1） 按下”开始“按键后才开始抢答，且抢答允许指示灯亮；

						（2）按下“开始”键后如果有人抢答，则抢答指示灯灭，7段数码管显示抢答者编号。
						
						P3.0端口连接“开始”按键；
						P3.6端口连接一个LED，指示抢答允许；
						P0口接7段数码管，显示抢答者编号；
						P2口接8个独立式按键，依次编号为1~8；
						P1口接抢答者指示灯，当P2口的某一个独立按键按下后，对应的抢答者指示灯亮；
						
						
	[2024-01-31] zoya
*/

#include <reg51.h>
#include <intrins.h>

typedef unsigned char uchar;

#define GPIO_SMG	P0  // 数码管
#define GPIO_LED	P1  // 抢答者指示灯
#define GPIO_BTN	P2  // 抢答者按键

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
		if(0==START_BTN)  // 开始按键按下
		{
			delay(10);  // 开始按键消抖
			if(0==START_BTN)
			{
				START_LED=0;  // 点亮开始指示灯
				enStart=1;  // 指示可以开始抢答，当有抢答按键按下触发中断处理服务
				tmp=0xff;  // 初始化
			}
		}
		digProc();  // 处理数码管显示的数据
		GPIO_LED=tmp;  // 抢答指示灯按照抢答按键设置哪个灯亮
		GPIO_SMG=smg[dispNum];  // 数码管显示哪个抢答按键被按下
	}
}

void Int0() interrupt 0
{
	if(enStart)
	{
		enStart=0;  // 有抢答，再按其它抢答键无效
		EX0=0;
		delay(10);
		EX0=1;
		tmp=GPIO_BTN;
		START_LED=1;  // 有抢答，熄灭开始指示灯
	}
}