#ifndef __CHACK_H
#define __CHACK_H

#include "sys.h"

#define bookNum 20
#define null 20000

extern struct BOOKADDR
{
	char bookId[20];  // �������ID
	int index;  // �鼮λ��
	int flag;   // �鼮�Ƿ���ڱ�־
	unsigned char previewFlag;
} BOOK[bookNum];

int chackAddr(struct BOOKADDR BOOK[]);
void addBook(struct BOOKADDR BOOK[], char *bookId, int index);
void removeBook(struct BOOKADDR BOOK[], int index);
void sendAck(unsigned char *topic, char *id, int statu);
void sendAck2(unsigned char *topic, char *id);
int findId(struct BOOKADDR BOOK[], char *bookId);

#endif
