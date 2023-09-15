#include "led.h"

void led_init()
{
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOC, ENABLE);//ʹ��GPIOFʱ��

  //GPIOF9,F10��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOF, &GPIO_InitStructure);//��ʼ��
	
	GPIO_SetBits(GPIOF,GPIO_Pin_9 | GPIO_Pin_10);//GPIOF9,F10���øߣ�����
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��
	
	GPIO_ResetBits(GPIOC, GPIO_Pin_10 | GPIO_Pin_11);
}

void led0_off()
{
	// mled0 = 0;
	led0 = 1;
	
}

void led0_on()
{
	// mled0 = 1;
	led0 = 0;
}

void led1_off()
{
	// mled1 = 0;
	led1 = 1;
}

void led1_on()
{
	// mled1 = 1;
	led1 = 0;
}

void ledCheckOn(int index){
	switch(index){
		case 0:
			led0_on();
			break;
		case 1:
			led1_on();
			break;
		default:
			break;
	}
}

void ledCheckOff(int index){
	// switch(index){
	// 	case 0:
	// 		led0_off();
	// 		break;
	// 	case 1:
	// 		led1_off();
	// 		break;
	// 	default:
	// 		break;
	// }
	led0_off();
	led1_off();
}
void ledCheck(int index){
	switch(index){
		case 0:
			mled0 = ~mled0;
			break;
		case 1:
			mled1 = ~mled1;
			break;
		default:
			break;
	}
}



