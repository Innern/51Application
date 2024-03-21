#include <reg52.h>
#include "TempDS18B20.h"

sbit DSPORT=P3^7;

uchar DSRAM[9]={0};  // DS18B20 RAM的9字节数据

// 延时函数，延时大约ims
void delay1ms(uint i)
{
	uint j;
	for(;i>0;i--)
	{
		for(j=110;j>0;j--);
	}
}

// DS18B20初始化函数，初始化成功返回0，失败返回1
uchar DS18B20Init()
{
	uchar i;
	DSPORT=0;  // 数据线拉低
	i=80;
	while(i--);  // 延时480us
	DSPORT=1;  // 数据线拉高
	i=0;
	while(DSPORT)
	{
		delay1ms(1);  // 数据线拉高后延时至少480us
		i++;
		if(i>5)
		{
			return 1;
		}
		
	}
	return 0;
}

// 向DS18B20发送数据，即写数据到DS18B20
void DS18B20WriteByte(uchar dat)
{
	uchar i, j;
	j=0;
	for(i=0;i<8;i++)
	{
		DSPORT=0;  // 数据线拉低
		j++;  // 延时15us
		j++;
		DSPORT=dat&0x01;
		j=10;  // 延时60us
		while(j--);
		DSPORT=1;
		dat=dat>>1;
	}
}

// 从DS18B20中读取一个字节的数据
uchar DS18B20ReadByte()
{
	uchar i, j;
	uchar bi, dat;
	j=0;
	dat=0;
	for(i=0;i<8;i++)
	{
		DSPORT=0;  // 数据线拉低
		j++;  // 延时1us
		DSPORT=1;
		j++;  // 延时10us
		j++;
		bi=DSPORT;
		dat= (dat>>1)|(bi<<7);
		j=7;
		while(j--);
	}
	
	return dat;
}

// DS18B20发送33H命令，读取ROM中的64位序列号
void DS18B20GetROM()
{
	uchar i;
	uchar ID[8];
	DS18B20Init();
	delay1ms(1);
	DS18B20WriteByte(0x33);  // 发送0x33命令
	for(i=0;i<8;i++)
	{
		ID[i]=DS18B20ReadByte();
	}
}

// DS18B20转换温度，复位->发送0xcc跳过ROM命令->发送0x44温度转换命令
void DS18B20ChangeTemp()
{
	DS18B20Init();
	delay1ms(1);
	DS18B20WriteByte(0xcc);  // 发送0xcc命令
	DS18B20WriteByte(0x44);  // 发送0x44命令
}

// DS18B20发送读取温度命令，复位->发送0xcc跳过ROM命令->发送0xBE读存储器命令
void DS18B20ReadTempCom()
{
	DS18B20Init();
	delay1ms(1);
	DS18B20WriteByte(0xcc);  // 发送0xcc命令
	DS18B20WriteByte(0xBE);  // 发送0xBE命令
}

// DS18B20读取检测到的温度，温度转换->发送读温度命令->读取数据
int DS18B20ReadTemp()
{
	int temp;
	uchar tmH, tmL;
	DS18B20ChangeTemp();
	DS18B20ReadTempCom();
	tmL=DS18B20ReadByte();
	tmH=DS18B20ReadByte();
	temp=tmH;
	temp = (temp<<8)|tmL;
	return temp;
}

// 读取DS18B20中的RAM中的8字节数据
void DS18B20ReadRAM()
{
	uchar i;
	DS18B20ChangeTemp();  // 转换温度
	DS18B20ReadTempCom();  // 准备读取数据
	for(i=0;i<8;i++)  // 实际读取数据时读前8个，最后一个CRC字节不读
	{
		DSRAM[i]=DS18B20ReadByte();  // 依次读取RAM中的数据
	}
}

// DS18B20发送读命令48H，将RAM中2、3字节的数据复制到EEPROM
void DS18B20RAM2EEPROM()
{
	DS18B20Init();
	delay1ms(1);
	DS18B20WriteByte(0xcc);  // 发送0xcc命令
	DS18B20WriteByte(0x48);  // 发送0x48命令
}

// DS18B20发送读命令B8H，将EEPROM中的内容恢复到RAM的2、3字节
void DS18B20EEPROM2RAM()
{
	DS18B20Init();
	delay1ms(1);
	DS18B20WriteByte(0xcc);  // 发送0xcc命令
	DS18B20WriteByte(0xB8);  // 发送0xB8命令
}

// DS18B20发送4EH命令，准备设置上下限温度
void DS18B20SetRangeCom()
{
	DS18B20Init();
	delay1ms(1);
	DS18B20WriteByte(0xcc);  // 发送0xcc命令
	DS18B20WriteByte(0x4E);  // 发送0x4E命令
}

// DS18B20发送读命令4EH，向RAM的2、3字节写上下限温度
// 首先将原来的数据写入到自带的EEPROM中
// 发送4EH命令，设置上下限温度
void DS18B20SetRange(char low, char high)
{
	uchar tmpTL, tmpTH;
	// 将温度转换存储
	if(low<0)
	{
		tmpTL=~low;
		tmpTL+=1;
		tmpTL|=0x80;
	}
	if(high<0)
	{
		tmpTH=~high;
		tmpTH+=1;
		tmpTH|=0x80;
	}
	
	DS18B20RAM2EEPROM();  // 将原来的上下限数据写入到EEPROM
	DS18B20SetRangeCom();  // 准备将上下限温度值写入RAM
	DS18B20WriteByte(tmpTL);  // 传送上下限温度值
	DS18B20WriteByte(tmpTH);
	
}