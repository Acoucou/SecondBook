#ifndef _ESP8266_H_
#define _ESP8266_H_

#define REV_OK		0	//接收完成标志
#define REV_WAIT	1	//接收未完成标志

extern unsigned char esp8266_buf[128];

void ESP8266_Init(void);

void ESP8266_Clear(void);

void ESP8266_SendData(unsigned char *topic, unsigned char *data);

unsigned char *ESP8266_GetIPD(unsigned short timeOut, char *topic);

#endif
