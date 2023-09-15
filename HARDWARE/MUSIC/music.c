#include "music.h"

u8 send_buf[10];

void send_byte(unsigned char s)
{
	USART_SendData(USART2, s);									//��������
	while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);		//�ȴ��������
}

// ���͵���ָ��
void SendCmd(u8 len) 
{ 
	 u8 i = 0 ; 
	 send_byte(0x7E); //��ʼ
	 for(i=0; i<len; i++) //����
	 { 
	 	send_byte(send_buf[i]) ; 
	 } 
	 send_byte(0xEF) ; //����
}

// ���У��
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

//��������ָ��
void Uart_SendCMD(u8 CMD ,u8 feedback , u16 dat) 
{ 
	 send_buf[0] = 0xff; //�����ֽ� 
	 send_buf[1] = 0x06; //����
	 send_buf[2] = CMD; //����ָ��
	 send_buf[3] = feedback;//�Ƿ���Ҫ����
	 send_buf[4] = (u8)(dat >> 8);//datah 
	 send_buf[5] = (u8)(dat); //datal 
	 DoSum(&send_buf[0],6); //У��
	 SendCmd(8); //���ʹ�֡����
} 

// ѡ������
void setMusic(u16 side){
	Uart_SendCMD(0x03, 0, side);
}

// ָ���ļ��в�������
void setMusicFolder(u16 side){
	Uart_SendCMD(0x0F, 0, side);
}

// ָ��������С
void setVolume(u16 side){
	Uart_SendCMD(0x06, 0, side);
}

// ����
void start(void){
	Uart_SendCMD(0x0E, 0, 0);
}

// ��ͣ
void stop(){
	Uart_SendCMD(0x0D, 0, 0);
}
