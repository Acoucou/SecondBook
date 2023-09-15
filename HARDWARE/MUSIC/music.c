#include "music.h"

u8 send_buf[10];

void send_byte(unsigned char s)
{
	USART_SendData(USART2, s);									//发送数据
	while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);		//等待发送完成
}

// 发送单条指令
void SendCmd(u8 len) 
{ 
	 u8 i = 0 ; 
	 send_byte(0x7E); //起始
	 for(i=0; i<len; i++) //数据
	 { 
	 	send_byte(send_buf[i]) ; 
	 } 
	 send_byte(0xEF) ; //结束
}

// 求和校验
void DoSum( u8 *Str, u8 len) 
{ 
	 u16 xorsum = 0; 
	 u8 i; 
	 for(i=0; i<len; i++) 
	 { 
	 	xorsum = xorsum + Str[i]; 
	 } 
	 xorsum = 0 -xorsum; 
	 *(Str+i) = (u8)(xorsum >>8); 
	 *(Str+i+1) = (u8)(xorsum & 0x00ff); 
}

//发送整条指令
void Uart_SendCMD(u8 CMD ,u8 feedback , u16 dat) 
{ 
	 send_buf[0] = 0xff; //保留字节 
	 send_buf[1] = 0x06; //长度
	 send_buf[2] = CMD; //控制指令
	 send_buf[3] = feedback;//是否需要反馈
	 send_buf[4] = (u8)(dat >> 8);//datah 
	 send_buf[5] = (u8)(dat); //datal 
	 DoSum(&send_buf[0],6); //校验
	 SendCmd(8); //发送此帧数据
} 

// 选择音乐
void setMusic(u16 side){
	Uart_SendCMD(0x03, 0, side);
}

// 指定文件夹播放音乐
void setMusicFolder(u16 side){
	Uart_SendCMD(0x0F, 0, side);
}

// 指定音量大小
void setVolume(u16 side){
	Uart_SendCMD(0x06, 0, side);
}

// 播放
void start(void){
	Uart_SendCMD(0x0E, 0, 0);
}

// 暂停
void stop(){
	Uart_SendCMD(0x0D, 0, 0);
}
