#include "checkBook.h"
#include "string.h"
#include "esp8266.h"

extern struct BOOKADDR BOOK[bookNum];

int checkBookAddress(struct BOOKADDR BOOK[])
{
	int i = 0;
	for (i = 0; i < 9; i++)
	{
		if (BOOK[i].flag == 0)
		{
			return i;
		}
	}
	return null;
}

void addBook(struct BOOKADDR BOOK[], char *bookId, int index)
{
	strcpy(BOOK[index].bookId, bookId);
	BOOK[index].index = index;
	BOOK[index].flag = 1;
}

void removeBook(struct BOOKADDR BOOK[], int index)
{
	memset(BOOK[index].bookId, 0, sizeof(BOOK[index].bookId));
	BOOK[index].flag = 0;
}

void sendAck(unsigned char *topic, char *id, int statu)
{
	char cmdBuf[100];

	ESP8266_Clear(); //清空接收缓存

	sprintf(cmdBuf, "AT+MQTTPUB=0,\"%s\",\"{\\\"id\\\": \\\"%s\\\"\\\,\\\"bookAck\\\": \\\"%d\\\"}\",1,0\r\n", topic, id, statu);
	//printf("%s", cmdBuf);

	while (ESP8266_SendCmd(cmdBuf, "OK"))
		;
	delay_ms(500);
}

void sendAck2(unsigned char *topic, char *id)
{
	char cmdBuf[100];

	ESP8266_Clear(); //清空接收缓存

	sprintf(cmdBuf, "AT+MQTTPUB=0,\"%s\",\"{\\\"id\\\": \\\"%s\\\"\\\,\\\"bookAck\\\": \\\"0\\\"}\",1,0\r\n", topic, id);
	//printf("%s", cmdBuf);

	while (ESP8266_SendCmd(cmdBuf, "OK"))
		;
	delay_ms(500);
}

// 查找ID
int findId(struct BOOKADDR BOOK[], char *bookId)
{
	int i = 0;
	for (i = 0; i <= 9; i++)
	{
		if (strcmp(BOOK[i].bookId, bookId) == 0)
		{
			return i;
		}
	}

	return null;
}
