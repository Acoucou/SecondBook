#include "key.h"
#include "delay.h"


// ������ʼ��
void KEY_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOE, ENABLE);//ʹ��GPIOA,GPIOEʱ��
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4; //KEY0 KEY1 KEY2��Ӧ����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��GPIOE2,3,4
	
	 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//WK_UP��Ӧ����PA0
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN ;//����
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA0
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;//WK_UP��Ӧ����PA0
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA0
}

u8 key_scan(void)
{	 
	u8 key_num = 0;
	
	if(key0 == 0)	{delay_ms(20); while(key0 == 0);	delay_ms(20); key_num = 1;}
	if(key1 == 0)	{delay_ms(20); while(key1 == 0);	delay_ms(20); key_num = 2;}
	if(key2 == 0)	{delay_ms(20); while(key2 == 0);	delay_ms(20); key_num = 3;}
	if(WK_UP == 1)	{delay_ms(20); while(WK_UP == 1); 	delay_ms(20); key_num = 4;}
	
	return key_num;
}


int checkKeyStateOff(int index)
{	 
	switch(index){
		case 0:
			if(mkey0 == 0){
				delay_ms(10);
				return 1;
			}
			break;
		case 1:
			if(mkey1 == 0){
				delay_ms(10);
				return 1;
			}
			break;
		default:
			return 0;
	}

	return 0;
}

int checkKeyStateOn(int index)
{	 
	switch(index){
		case 0:
			if(mkey0 == 1){
				delay_ms(10);
				return 1;
			}
			break;
		case 1:
			if(mkey1 == 1){
				delay_ms(10);
				return 1;
			}
			break;
		default:
			return 0;
	}

	return 0;
}


