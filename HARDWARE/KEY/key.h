#ifndef __KEY_H__
#define __KEY_H__

#include "sys.h"

#include "checkBook.h"

#define key0 PEin(4)   	//PE4
#define key1 PEin(3)	//PE3 
#define key2 PEin(2)	//PE2
#define WK_UP PAin(0)	//PA0

#define mkey0 PAin(11)	//PE2
#define mkey1 PAin(12)	//PE2

void KEY_Init(void);

u8 key_scan(void);  //°´¼üÉ¨Ãèº¯Êý

int checkKeyStateOn(int index);
int checkKeyStateOff(int index);

#endif

