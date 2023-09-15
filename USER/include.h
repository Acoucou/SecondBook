#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "key.h"
#include "beep.h"
#include "timer.h"
#include "sram.h"
#include "touch.h"
#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "mAPP.h"

#include "usart2.h"
#include "music.h"
#include "usart3.h"
#include "esp8266.h"
#include "msg.h"

#include "string.h"
#include "stdlib.h"
#include "checkBook.h"

void hardware_init(void);


// 任务函数
void Start_task(void *pvParameters);
void CtrlTask_task(void *pvParameters);
void GUI_task(void *pvParameters);
void Cmd_task(void *pvParameters);
void PushBook_task(void *pvParameters);
void GetBook_task(void *pvParameters);
void PreviewBook_task(void *pvParameters);
void Error_task(void *pvParameters);
void Other_task(void *pvParameters);
void Tip_task(void *pvParameters);


