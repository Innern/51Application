#ifndef __TEMPDS18B20_H_
#define __TEMPDS18B20_H_

#ifndef uchar
typedef unsigned char uchar;
#endif

#ifndef uint
typedef unsigned int uint;
#endif

extern uchar DSRAM[9];

// ��ʱ��������ʱ��Լims
void delay1ms(uint i);

// DS18B20��ʼ����������ʼ���ɹ�����0��ʧ�ܷ���1
uchar DS18B20Init();

// ��DS18B20�������ݣ���д���ݵ�DS18B20
void DS18B20WriteByte(uchar dat);

// ��DS18B20�ж�ȡһ���ֽڵ�����
uchar DS18B20ReadByte();

// DS18B20����33H�����ȡROM�е�64λ���к�
void DS18B20GetROM();

// DS18B20ת���¶ȣ���λ->����0xcc����ROM����->����0x44�¶�ת������
void DS18B20ChangeTemp();

// DS18B20���Ͷ�ȡ�¶������λ->����0xcc����ROM����->����0xBE���洢������
void DS18B20ReadTempCom();

// DS18B20��ȡ��⵽���¶ȣ��¶�ת��->���Ͷ��¶�����->��ȡ����
int DS18B20ReadTemp();

// ��ȡDS18B20�е�RAM�е��ֽ�����
void DS18B20ReadRAM();

// DS18B20���Ͷ�����48H����RAM��2��3�ֽڵ����ݸ��Ƶ�EEPROM
void DS18B20RAM2EEPROM();

// DS18B20���Ͷ�����B8H����EEPROM�е����ݻָ���RAM��2��3�ֽ�
void DS18B20EEPROM2RAM();

// DS18B20����4EH���׼�������������¶�
void DS18B20SetRangeCom();

// DS18B20���Ͷ�����4EH����RAM��2��3�ֽ�д�������¶�
// ���Ƚ�ԭ��������д�뵽�Դ���EEPROM��
// ����4EH��������������¶�
void DS18B20SetRange(char low, char high);
#endif