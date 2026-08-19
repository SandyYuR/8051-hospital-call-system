/*****************************************************************************
 * 文件名：slave3.c
 * 项目  ：基于 AT89S51 单片机的医院病房呼叫系统 —— 从机 3（房间 3）
 * 功能  ：扫描 8 个按键（对应房间 3 的 8 张病床），按键按下后通过
 *         串口发送呼叫帧 0x30|床号（高 4 位为房间号 3，低 4 位为床号），
 *         收到主机回执 0x55 后，P2 口输出声光提示约 1 秒。
 * 通信  ：RS-485 总线（MAX485），波特率 9600。
 * 编译  ：Keil C51（AT89S51）
 * 说明  ：与 slave1.c / slave2.c 结构相同，仅房间号不同（0x10/0x20/0x30）；
 *         原始工程代码，仅补充注释，未改动任何逻辑。
 *****************************************************************************/

#include<reg51.h>

#define uchar unsigned char 
#define uint unsigned int 

/*****************全局变量定义****************/
uchar keydata;
uchar senddata;
uchar receivedata;
uchar beep;
/*****************串口初始化程序*****************/
void UART_init(void)
{
	TMOD=0x21;
	TL1=0xfd;
	TH1=0xfd;
	TCON=0x50;
	SCON=0x50;
	PCON=0x00;
}
/**********************延时函数************************/
void delay(uint num)
{
	while(num--) ;
}
/*********************键盘读取*********************/
void key(void)
{
	P1=0xff;
	if(P1!=0xff)
	{
		delay(100);
		if(P1!=0xff)
		{
			switch(P1)
			{
				case 0xfe:keydata=0x01;break;
				case 0xfd:keydata=0x02;break;
				case 0xfb:keydata=0x03;break;
				case 0xf7:keydata=0x04;break;
				case 0xef:keydata=0x05;break;
				case 0xdf:keydata=0x06;break;
				case 0xbf:keydata=0x07;break;
				case 0x7f:keydata=0x08;break;
				default: break;
			}
			beep=~P1;
			while(P1!=0xff);
		}
		else keydata=0;
	}
	else keydata=0;
}
/*******************发送程序*******************/
void send(void)
{
	senddata=0x30|keydata;//加上房间号0x30
	SBUF=senddata;
	while(TI==0);
	TI=0;
}
/******************接受程序*******************/
void receive()
{
	while(RI==0);
	RI=0;
	P2=beep;
	delay(30000);
	P2=0x00;
}
void main(void)
{
	UART_init();
	P2=0x00;
	for(;;)
	{
	  	key();
		if(keydata!=0)
		{
			send();
			receive();
			keydata=0;
		}	
	}
} 
