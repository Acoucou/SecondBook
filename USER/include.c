#include "include.h"

void hardware_init(){
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); //设置系统中断优先级分组4
	delay_init(168);								//初始化延时函数
	uart_init(115200);								//初始化串口波特率为115200

	led_init();				//初始化LED
	KEY_Init();				//按键初始化
	BEEP_Init();			//蜂鸣器初始化
	
	usart2_Init(9600);
	usart3_Init(115200);
	printf("esp init\r\n\r\n");
	// ESP8266_Init();
	UsartPrintf(USART_DEBUG, "Hardware init OK\r\n");

	LCD_Init();				// LCD初始化
	FSMC_SRAM_Init();		//初始化外部sram
	tp_dev.init();			//触摸屏初始化
	TIM3_Int_Init(999, 83); //定时器初始化(1ms中断),用于给lvgl提供1ms的心跳节拍
	//TIM4_Int_Init(10000-1,8400-1);	//定时器时钟84M，分频系数8400，所以84M/8400=10Khz的计数频率，计数10000次为1000ms   
	
	lv_init();			  // lvgl系统初始化
	lv_port_disp_init();  // lvgl显示接口初始化,放在lv_init()的后面
	lv_port_indev_init(); // lvgl输入接口初始化,放在lv_init()的后面

	mApp_start();
}

