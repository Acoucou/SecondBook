#include "include.h"

// rtos头文件
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

struct BOOKADDR BOOK[bookNum]; // 定义书籍
unsigned char *msgREs, *dataPtr, *cmd, *bookid;
unsigned int bookIndex = null, musicIndex = null, error_index = null,  error_index1 = null;
unsigned int i;
u8 data[100];
unsigned int music_count = 0;
u8 led_flag = 1, msg_flag = 0;
unsigned int preview_flag = null, getbook_flag = null;
extern lv_obj_t *error_btn;

// 任务句柄
TaskHandle_t StartTask_Handler, CtrlTask_Handler, GUI_Handler, Cmd_Handler, PushBook_Handler, GetBook_Handler, PreviewBook_Handler, Error_handler;
TaskHandle_t Other_Handler, Tip_Handler;
SemaphoreHandle_t BinarySemaphore; // 二值信号量句柄

void test()
{
	// Uart_SendCMD(0x0C, 0, 0);
	// delay_ms(500);
	// setVolume(0x13);
	setVolume(0x03);
}

void Start_task(void *pvParameters)
{
	taskENTER_CRITICAL(); // 进入临界区
	// 创建二值信号量
	BinarySemaphore = xSemaphoreCreateBinary();

	xTaskCreate((TaskFunction_t)GUI_task, (const char *)"task1_task", (uint16_t)1024, (void *)NULL, (UBaseType_t)1, (TaskHandle_t *)&GUI_Handler);
	xTaskCreate((TaskFunction_t)Cmd_task, (const char *)"task2_task", (uint16_t)128, (void *)NULL, (UBaseType_t)3, (TaskHandle_t *)&Cmd_Handler);
	xTaskCreate((TaskFunction_t)CtrlTask_task, (const char *)"task0_task", (uint16_t)128, (void *)NULL, (UBaseType_t)10, (TaskHandle_t *)&CtrlTask_Handler);
	xTaskCreate((TaskFunction_t)PushBook_task, (const char *)"task3_task", (uint16_t)128, (void *)NULL, (UBaseType_t)4, (TaskHandle_t *)&PushBook_Handler);
	xTaskCreate((TaskFunction_t)GetBook_task, (const char *)"task4_task", (uint16_t)128, (void *)NULL, (UBaseType_t)5, (TaskHandle_t *)&GetBook_Handler);
	xTaskCreate((TaskFunction_t)PreviewBook_task, (const char *)"task4_task", (uint16_t)128, (void *)NULL, (UBaseType_t)6, (TaskHandle_t *)&PreviewBook_Handler);
	xTaskCreate((TaskFunction_t)Error_task, (const char *)"task5_task", (uint16_t)1024, (void *)NULL, (UBaseType_t)7, (TaskHandle_t *)&Error_handler);
	xTaskCreate((TaskFunction_t)Other_task, (const char *)"task6_task", (uint16_t)128, (void *)NULL, (UBaseType_t)3, (TaskHandle_t *)&Other_Handler);
	xTaskCreate((TaskFunction_t)Tip_task, (const char *)"task7_task", (uint16_t)128, (void *)NULL, (UBaseType_t)9, (TaskHandle_t *)&Tip_Handler);

	// 挂起
	vTaskSuspend(PushBook_Handler);
	vTaskSuspend(GetBook_Handler);
	vTaskSuspend(PreviewBook_Handler);
	

	vTaskDelete(StartTask_Handler); // 删除开始任务
	taskEXIT_CRITICAL(); // 退出临界区
}

void CtrlTask_task(void *pvParameters)
{
	BaseType_t xHigherPriorityTaskWoken;
	while (1)
	{
		switch (fun)
		{
		case 1: // 放书
			bookIndex = checkBookAddress(BOOK);

			if (bookIndex != null)
			{
				addBook(BOOK, bookid, bookIndex);
				printf("添加成功：%s\r\n", BOOK[bookIndex].bookId);
				// 打印数据
				for (i = 0; i <= 9; i++)
				{
					printf("\r\nbookId:%s    2:%d    3:%d\r\n", BOOK[i].bookId, BOOK[i].index, BOOK[i].flag);
				}
			}
			bookIndex = findId(BOOK, bookid); // 查找位置bookChack[i].index
			musicIndex = 1;

			xSemaphoreGiveFromISR(BinarySemaphore, &xHigherPriorityTaskWoken); // 释放二值信号量，发出提示

			vTaskResume(PushBook_Handler);
			vTaskSuspend(Error_handler);
			vTaskDelay(1);
			break;
		case 2:								  // 取书
			bookIndex = findId(BOOK, bookid); // 查找位置BOOK[i].index
			musicIndex = 2;

			xSemaphoreGiveFromISR(BinarySemaphore, &xHigherPriorityTaskWoken); // 释放二值信号量，发出提示

			vTaskResume(GetBook_Handler);
			vTaskSuspend(Error_handler);
			vTaskDelay(1);
			break;
		case 3: // 预览书籍
			bookIndex = findId(BOOK, bookid); // 查找位置BOOK[i].index
			musicIndex = 3;

			xSemaphoreGiveFromISR(BinarySemaphore, &xHigherPriorityTaskWoken); // 释放二值信号量，发出提示

			vTaskResume(GetBook_Handler);
			vTaskSuspend(Error_handler);
			break;
		case 4: // 归还
			bookIndex = checkBookAddress(BOOK);

			if (bookIndex != null)
			{
				addBook(BOOK, bookid, bookIndex);
				printf("添加成功：%s\r\n", BOOK[bookIndex].bookId);
				// 打印数据
				for (i = 0; i <= 9; i++)
				{
					printf("\r\nbookId:%s    2:%d    3:%d\r\n", BOOK[i].bookId, BOOK[i].index, BOOK[i].flag);
				}
			}
			bookIndex = findId(BOOK, bookid); // 查找位置bookChack[i].index
			musicIndex = 5;

			xSemaphoreGiveFromISR(BinarySemaphore, &xHigherPriorityTaskWoken); // 释放二值信号量，发出提示

			vTaskResume(PushBook_Handler);
			vTaskSuspend(Error_handler);
			vTaskDelay(1);
			break;
		case 10: // 挂起
			vTaskSuspend(PushBook_Handler);
			vTaskResume(Error_handler);
			break;
		case 11:
			vTaskSuspend(GetBook_Handler);
			vTaskResume(Error_handler);
			break;
		case 12:
			vTaskSuspend(PreviewBook_Handler);
			vTaskResume(Error_handler);
			break;
		default:
			break;
		}
		fun = null;
		vTaskDelay(1);
	}
}

void GUI_task(void *pvParameters)
{
	while (1)
	{
		tp_dev.scan(0);	   // 触摸扫描
		lv_task_handler(); // lvgl事务处理
		vTaskDelay(1);
	}
}

void Cmd_task(void *pvParameters)
{
	while (1)
	{
		if (strlen(esp8266_buf) != 0 && strstr(esp8266_buf, "book") != NULL) // 小程序消息处理
		{
			printf("消息: %s\r\n", esp8266_buf);

			// 解析数据
			dataPtr = ESP8266_GetIPD(300, "book");		 // 获得json数据
			fingString(dataPtr, "cmd", "\"", cmd);		 // 获得命令
			fingString(dataPtr, "bookId", "\"", bookid); // 获得书籍id

			if (strcmp(cmd, "pushbook") == 0) // 通过小程序控制，放置书籍指令
			{
				printf("正在放书----\r\n");
				fun = 1;
			}
			else if (strcmp(cmd, "removebook") == 0) // 取出书籍指令
			{
				printf("开始取出书籍----: %s\r\n", bookid);
				fun = 2;
			}
			else if (strcmp(cmd, "prebook") == 0) // 取出书籍指令
			{
				printf("开始预览书籍: %s-----\r\n", bookid);
				fun = 3;
			}
			else if (strcmp(cmd, "returnbook") == 0) // 归还书籍指令
			{
				printf("开始归还书籍: %s-----\r\n", bookid);
				fun = 4;
			}
			else
			{
				// do nothing
			}
			memset(cmd, 0, 20);

			msg_flag = 1;
			ESP8266_Clear();
		}

		vTaskDelay(1);
	}
}

void PushBook_task(void *pvParameters)
{
	BaseType_t xHigherPriorityTaskWoken;
	while (1)
	{
		if (checkKeyStateOn(bookIndex))
		{
			printf("书籍放置成功----\r\n");

			sendAck("book", bookid, 1); // 添加成功，发送反馈

			bookIndex = null; // 置位
			musicIndex = 8;
			memset(bookid, 0, 20);

			xSemaphoreGiveFromISR(BinarySemaphore, &xHigherPriorityTaskWoken); // 释放二值信号量
			fun = 10;
		}
		vTaskDelay(1);
	}
}

void GetBook_task(void *pvParameters)
{
	BaseType_t xHigherPriorityTaskWoken;
	while (1)
	{
		if (checkKeyStateOff(bookIndex))
		{
			sendAck("book", bookid, 0); // 发送消息

			//  清除书籍信息
			removeBook(BOOK, bookIndex);

			// 打印数据
			printf("取出成功：%s----\r\n", BOOK[bookIndex].bookId);

			for (i = 0; i <= 9; i++)
			{
				printf("\r\n1:%s    2:%d    3:%d\r\n", BOOK[i].bookId, BOOK[i].index, BOOK[i].flag);
			}

			bookIndex = null; // 置位
			musicIndex = 9;
			memset(bookid, 0, 20);

			xSemaphoreGiveFromISR(BinarySemaphore, &xHigherPriorityTaskWoken); // 释放二值信号量
			fun = 11;
		}
		vTaskDelay(1);
	}
}

void PreviewBook_task(void *pvParameters)
{
	BaseType_t xHigherPriorityTaskWoken;
	while (1)
	{
		// if (checkKeyStateOff(bookIndex))
		// {
		// 	sendAck("book", bookid, 2); // 发送消息

		// 	//  清除书籍信息
		// 	removeBook(BOOK, bookIndex);

		// 	// 打印数据
		// 	printf("取出成功：%s----\r\n", BOOK[bookIndex].bookId);

		// 	for (i = 0; i <= 9; i++)
		// 	{
		// 		printf("\r\n1:%s    2:%d    3:%d\r\n", BOOK[i].bookId, BOOK[i].index, BOOK[i].flag);
		// 	}

		// 	bookIndex = null; // 置位
		// 	musicIndex = null;
		// 	memset(bookid, 0, 20);

		// 	xSemaphoreGiveFromISR(BinarySemaphore, &xHigherPriorityTaskWoken); // 释放二值信号量
		// 	fun = 12;
		// }
		vTaskDelay(1);
	}
}

void Error_task(void *pvParameters)
{
	BaseType_t xHigherPriorityTaskWoken;
	while (1)
	{
		if (fun == null && error_index == null && error_index1 == null)
		{
			for (i = 0; i < bookNum; i++)
			{
				if (BOOK[i].flag == 1 && checkKeyStateOff(i))
				{
					printf("取出书籍错误语音提示-----error_index: %d\r\n", i);

					// 发送异常消息
					// sendAck("book", BOOK[i].bookId, null);

					error_index = i;

					// 播放语音提示
					musicIndex = 6;
					xSemaphoreGiveFromISR(BinarySemaphore, &xHigherPriorityTaskWoken); // 释放二值信号量，发出提示
				}
				else if (BOOK[i].flag == 0 && checkKeyStateOn(i))
				{
					BOOK[i].flag = 1;
					error_index1 = i;
					
					// 播放声音，开灯
					bookIndex = i;
					musicIndex = 7;
					xSemaphoreGiveFromISR(BinarySemaphore, &xHigherPriorityTaskWoken); // 释放二值信号量，发出提示
					
					printf("放置书籍错误语音提示-----\r\n");
				}
			}
		}
		if (error_index != null)
		{
			// printf(">>>>preview_flag: %d error_index:%d \r\n", preview_flag, error_index);
			// 发送lvgl自定义事件，弹出选择框
			if(preview_flag == null)
			{
				lv_event_send(error_btn, LV_EVENT_RELEASED, NULL);
				preview_flag = 2;
			}

			if (preview_flag == 1)  // 预览书籍 | 取出书籍
			{
				// BOOK[i].previewFlag = error_index;
				sendAck("book", BOOK[error_index].bookId, 0); // 发送消息

				//  清除书籍信息
				removeBook(BOOK, error_index);

				musicIndex = 10;
				preview_flag = null;
				error_index = null;
				xSemaphoreGiveFromISR(BinarySemaphore, &xHigherPriorityTaskWoken); // 释放二值信号量，发出提示
			}
			else if(preview_flag == 0)  // 放回书籍
			{
				bookIndex = error_index;
				musicIndex = 1;
				preview_flag = null;
				error_index = null;

				xSemaphoreGiveFromISR(BinarySemaphore, &xHigherPriorityTaskWoken); // 释放二值信号量，发出提示

				vTaskResume(PushBook_Handler);
				vTaskSuspend(Error_handler);
			}
		}
		if(error_index1 != null)
		{
			if(checkKeyStateOff(error_index1))
			{
				BOOK[error_index1].flag = 0;
				error_index1 = null;

				// 播放声音，关灯
				bookIndex = null;
				musicIndex = 9;
				xSemaphoreGiveFromISR(BinarySemaphore, &xHigherPriorityTaskWoken); // 释放二值信号量，发出提示
			}
		}

		vTaskDelay(1);
	}
}

void Tip_task(void *pvParameters)
{
	BaseType_t err = pdFALSE;
	while (1)
	{
		if (BinarySemaphore != NULL)
		{
			err = xSemaphoreTake(BinarySemaphore, portMAX_DELAY); // 获取信号量
			if (err == pdTRUE)									  // 获取信号量成功
			{
				// led指示灯
				if (bookIndex != null)
				{
					printf("ledon: %d\r\n", bookIndex);
					ledCheckOn(bookIndex); // 传入index，指示位置
				}
				else
				{
					ledCheckOff(bookIndex);
				}

				// 语音提示
				/*
					1、帮助提示音
					2、放书提示音
					3、取书提示音
					4、预览书提示音
					5、归还书提示音
				*/
				switch (musicIndex)
				{
				case 1:
					setMusicFolder(0x0102); // 放书提示音
					printf("mu1\r\n");
					break;
				case 2:
					setMusicFolder(0x0103); // 取书提示音
					printf("mu2");
					break;
				case 3:
					setMusicFolder(0x0104); // 预览提示音
					printf("mu3");
					break;
				case 4:
					setMusicFolder(0x0101); // 帮助
					printf("mu4");
					break;
				case 5:
					setMusicFolder(0x0105); // 归还
					printf("mu5");
					break;
				case 6:
					setMusicFolder(0x0106); // 取出错误
					printf("mu6");
					break;
				case 7:
					setMusicFolder(0x0107); // 放置错误
					printf("mu7");
					break;
				case 8:
					setMusicFolder(0x0108); // 放置错误
					printf("mu8");
					break;
				case 9:
					setMusicFolder(0x0109); // 放置错误
					printf("mu9");
					break;
				case 10:
					setMusicFolder(0x0110); // 预览成功
					printf("mu10");
					break;
				default:
					break;
				}
			}
		}
		else if (err == pdFALSE)
		{
			vTaskDelay(1);
		}
	}
}

// 刷卡任务
void Other_task(void *pvParameters)
{
	char status;
	unsigned char TagType[2], SelectedSnr[4]; 

	while (1)
	{
		status= PcdRequest(REQ_ALL,TagType);
		if(status == MI_OK)
		{
			status = PcdAnticoll(SelectedSnr);
			if(status == MI_OK)
			{
				printf("card\r\n");

				if(getbook_flag != null)
				{
					getbook_flag = null;
					preview_flag = 1;

					BEEP = ~BEEP;
					vTaskDelay(100);
					BEEP = ~BEEP;

					WaitCardOff();
				}
			}
		}
		vTaskDelay(100);
	}
}

int main(void)
{
	hardware_init();
	test(); // 用于测试功能

	cmd = (u8 *)malloc(sizeof(char) * 20);	  // 申请50字节用于接收消息
	bookid = (u8 *)malloc(sizeof(char) * 20); // 保存bookid

	// 创建开始任务
	xTaskCreate((TaskFunction_t)Start_task,			 // 任务函数
				(const char *)"start_task",			 // 任务名称
				(uint16_t)128,						 // 任务堆栈大小
				(void *)NULL,						 // 传递给任务函数的参数
				(UBaseType_t)1,						 // 任务优先级
				(TaskHandle_t *)&StartTask_Handler); // 任务句柄

	vTaskStartScheduler(); // 开启任务调度
}
