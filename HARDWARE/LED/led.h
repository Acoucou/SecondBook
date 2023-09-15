#ifndef __LED_H__
#define __LED_H__

#include "sys.h"

#define led0 PFout(9)
#define led1 PFout(10)

#define mled0 PCout(10)
#define mled1 PCout(11)

void led_init(void); //Led��ʼ��

void led0_on(void); // ����

void led0_off(void); // �ص�

void led1_on(void); // ����

void led1_off(void); // �ص�

void ledCheckOn(int index);
void ledCheckOff(int index);

#endif

