#include "include.h"

void hardware_init(){
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); //����ϵͳ�ж����ȼ�����4
	delay_init(168);								//��ʼ����ʱ����
	uart_init(115200);								//��ʼ�����ڲ�����Ϊ115200

	led_init();				//��ʼ��LED
	KEY_Init();				//������ʼ��
	BEEP_Init();			//��������ʼ��
	
	usart2_Init(9600);
	usart3_Init(115200);
	printf("esp init\r\n\r\n");
	// ESP8266_Init();
	UsartPrintf(USART_DEBUG, "Hardware init OK\r\n");

	LCD_Init();				// LCD��ʼ��
	FSMC_SRAM_Init();		//��ʼ���ⲿsram
	tp_dev.init();			//��������ʼ��
	TIM3_Int_Init(999, 83); //��ʱ����ʼ��(1ms�ж�),���ڸ�lvgl�ṩ1ms����������
	//TIM4_Int_Init(10000-1,8400-1);	//��ʱ��ʱ��84M����Ƶϵ��8400������84M/8400=10Khz�ļ���Ƶ�ʣ�����10000��Ϊ1000ms   
	
	lv_init();			  // lvglϵͳ��ʼ��
	lv_port_disp_init();  // lvgl��ʾ�ӿڳ�ʼ��,����lv_init()�ĺ���
	lv_port_indev_init(); // lvgl����ӿڳ�ʼ��,����lv_init()�ĺ���

	mApp_start();
}

