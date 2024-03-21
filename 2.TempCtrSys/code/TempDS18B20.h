#ifndef __TEMPDS18B20_H_
#define __TEMPDS18B20_H_

#ifndef uchar
typedef unsigned char uchar;
#endif

#ifndef uint
typedef unsigned int uint;
#endif

extern uchar DSRAM[9];

// 延时函数，延时大约ims
void delay1ms(uint i);

// DS18B20初始化函数，初始化成功返回0，失败返回1
uchar DS18B20Init();

// 向DS18B20发送数据，即写数据到DS18B20
void DS18B20WriteByte(uchar dat);

// 从DS18B20中读取一个字节的数据
uchar DS18B20ReadByte();

// DS18B20发送33H命令，读取ROM中的64位序列号
void DS18B20GetROM();

// DS18B20转换温度，复位->发送0xcc跳过ROM命令->发送0x44温度转换命令
void DS18B20ChangeTemp();

// DS18B20发送读取温度命令，复位->发送0xcc跳过ROM命令->发送0xBE读存储器命令
void DS18B20ReadTempCom();

// DS18B20读取检测到的温度，温度转换->发送读温度命令->读取数据
int DS18B20ReadTemp();

// 读取DS18B20中的RAM中的字节数据
void DS18B20ReadRAM();

// DS18B20发送读命令48H，将RAM中2、3字节的数据复制到EEPROM
void DS18B20RAM2EEPROM();

// DS18B20发送读命令B8H，将EEPROM中的内容恢复到RAM的2、3字节
void DS18B20EEPROM2RAM();

// DS18B20发送4EH命令，准备设置上下限温度
void DS18B20SetRangeCom();

// DS18B20发送读命令4EH，向RAM的2、3字节写上下限温度
// 首先将原来的数据写入到自带的EEPROM中
// 发送4EH命令，设置上下限温度
void DS18B20SetRange(char low, char high);
#endif