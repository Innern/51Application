#include <reg52.h>
#include "TempDS18B20.h"

sbit DSPORT=P3^7;

uchar DSRAM[9]={0};  // DS18B20 RAM��9�ֽ�����

// ��ʱ��������ʱ��Լims
void delay1ms(uint i)
{
	uint j;
	for(;i>0;i--)
	{
		for(j=110;j>0;j--);
	}
}

// DS18B20��ʼ����������ʼ���ɹ�����0��ʧ�ܷ���1
uchar DS18B20Init()
{
	uchar i;
	DSPORT=0;  // ����������
	i=80;
	while(i--);  // ��ʱ480us
	DSPORT=1;  // ����������
	i=0;
	while(DSPORT)
	{
		delay1ms(1);  // ���������ߺ���ʱ����480us
		i++;
		if(i>5)
		{
			return 1;
		}
		
	}
	return 0;
}

// ��DS18B20�������ݣ���д���ݵ�DS18B20
void DS18B20WriteByte(uchar dat)
{
	uchar i, j;
	j=0;
	for(i=0;i<8;i++)
	{
		DSPORT=0;  // ����������
		j++;  // ��ʱ15us
		j++;
		DSPORT=dat&0x01;
		j=10;  // ��ʱ60us
		while(j--);
		DSPORT=1;
		dat=dat>>1;
	}
}

// ��DS18B20�ж�ȡһ���ֽڵ�����
uchar DS18B20ReadByte()
{
	uchar i, j;
	uchar bi, dat;
	j=0;
	dat=0;
	for(i=0;i<8;i++)
	{
		DSPORT=0;  // ����������
		j++;  // ��ʱ1us
		DSPORT=1;
		j++;  // ��ʱ10us
		j++;
		bi=DSPORT;
		dat= (dat>>1)|(bi<<7);
		j=7;
		while(j--);
	}
	
	return dat;
}

// DS18B20����33H�����ȡROM�е�64λ���к�
void DS18B20GetROM()
{
	uchar i;
	uchar ID[8];
	DS18B20Init();
	delay1ms(1);
	DS18B20WriteByte(0x33);  // ����0x33����
	for(i=0;i<8;i++)
	{
		ID[i]=DS18B20ReadByte();
	}
}

// DS18B20ת���¶ȣ���λ->����0xcc����ROM����->����0x44�¶�ת������
void DS18B20ChangeTemp()
{
	DS18B20Init();
	delay1ms(1);
	DS18B20WriteByte(0xcc);  // ����0xcc����
	DS18B20WriteByte(0x44);  // ����0x44����
}

// DS18B20���Ͷ�ȡ�¶������λ->����0xcc����ROM����->����0xBE���洢������
void DS18B20ReadTempCom()
{
	DS18B20Init();
	delay1ms(1);
	DS18B20WriteByte(0xcc);  // ����0xcc����
	DS18B20WriteByte(0xBE);  // ����0xBE����
}

// DS18B20��ȡ��⵽���¶ȣ��¶�ת��->���Ͷ��¶�����->��ȡ����
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

// ��ȡDS18B20�е�RAM�е�8�ֽ�����
void DS18B20ReadRAM()
{
	uchar i;
	DS18B20ChangeTemp();  // ת���¶�
	DS18B20ReadTempCom();  // ׼����ȡ����
	for(i=0;i<8;i++)  // ʵ�ʶ�ȡ����ʱ��ǰ8�������һ��CRC�ֽڲ���
	{
		DSRAM[i]=DS18B20ReadByte();  // ���ζ�ȡRAM�е�����
	}
}

// DS18B20���Ͷ�����48H����RAM��2��3�ֽڵ����ݸ��Ƶ�EEPROM
void DS18B20RAM2EEPROM()
{
	DS18B20Init();
	delay1ms(1);
	DS18B20WriteByte(0xcc);  // ����0xcc����
	DS18B20WriteByte(0x48);  // ����0x48����
}

// DS18B20���Ͷ�����B8H����EEPROM�е����ݻָ���RAM��2��3�ֽ�
void DS18B20EEPROM2RAM()
{
	DS18B20Init();
	delay1ms(1);
	DS18B20WriteByte(0xcc);  // ����0xcc����
	DS18B20WriteByte(0xB8);  // ����0xB8����
}

// DS18B20����4EH���׼�������������¶�
void DS18B20SetRangeCom()
{
	DS18B20Init();
	delay1ms(1);
	DS18B20WriteByte(0xcc);  // ����0xcc����
	DS18B20WriteByte(0x4E);  // ����0x4E����
}

// DS18B20���Ͷ�����4EH����RAM��2��3�ֽ�д�������¶�
// ���Ƚ�ԭ��������д�뵽�Դ���EEPROM��
// ����4EH��������������¶�
void DS18B20SetRange(char low, char high)
{
	uchar tmpTL, tmpTH;
	// ���¶�ת���洢
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
	
	DS18B20RAM2EEPROM();  // ��ԭ��������������д�뵽EEPROM
	DS18B20SetRangeCom();  // ׼�����������¶�ֵд��RAM
	DS18B20WriteByte(tmpTL);  // �����������¶�ֵ
	DS18B20WriteByte(tmpTH);
	
}