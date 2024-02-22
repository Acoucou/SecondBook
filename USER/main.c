#include "include.h"

// rtosͷ�ļ�
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

struct BOOKADDR BOOK[bookNum]; // �����鼮
unsigned char *msgREs, *dataPtr, *cmd, *bookid;
unsigned int bookIndex = null, musicIndex = null, error_index = null,  error_index1 = null;
unsigned int i;
u8 data[100];
unsigned int music_count = 0;
u8 led_flag = 1, msg_flag = 0;
unsigned int preview_flag = null, getbook_flag = null;
extern lv_obj_t *error_btn;

// ������
TaskHandle_t StartTask_Handler, CtrlTask_Handler, GUI_Handler, Cmd_Handler, PushBook_Handler, GetBook_Handler, PreviewBook_Handler, Error_handler;
TaskHandle_t Other_Handler, Tip_Handler;
SemaphoreHandle_t BinarySemaphore; // ��ֵ�ź������

void test()
{
	// Uart_SendCMD(0x0C, 0, 0);
	// delay_ms(500);
	// setVolume(0x13);
	setVolume(0x03);
}

void Start_task(void *pvParameters)
{
	taskENTER_CRITICAL(); // �����ٽ���
	// ������ֵ�ź���
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

	// ����
	vTaskSuspend(PushBook_Handler);
	vTaskSuspend(GetBook_Handler);
	vTaskSuspend(PreviewBook_Handler);
	

	vTaskDelete(StartTask_Handler); // ɾ����ʼ����
	taskEXIT_CRITICAL(); // �˳��ٽ���
}

void CtrlTask_task(void *pvParameters)
{
	BaseType_t xHigherPriorityTaskWoken;
	while (1)
	{
		switch (fun)
		{
		case 1: // ����
			bookIndex = checkBookAddress(BOOK);

			if (bookIndex != null)
			{
				addBook(BOOK, bookid, bookIndex);
				printf("��ӳɹ���%s\r\n", BOOK[bookIndex].bookId);
				// ��ӡ����
				for (i = 0; i <= 9; i++)
				{
					printf("\r\nbookId:%s    2:%d    3:%d\r\n", BOOK[i].bookId, BOOK[i].index, BOOK[i].flag);
				}
			}
			bookIndex = findId(BOOK, bookid); // ����λ��bookChack[i].index
			musicIndex = 1;

			xSemaphoreGiveFromISR(BinarySemaphore, &xHigherPriorityTaskWoken); // �ͷŶ�ֵ�ź�����������ʾ

			vTaskResume(PushBook_Handler);
			vTaskSuspend(Error_handler);
			vTaskDelay(1);
			break;
		case 2:								  // ȡ��
			bookIndex = findId(BOOK, bookid); // ����λ��BOOK[i].index
			musicIndex = 2;

			xSemaphoreGiveFromISR(BinarySemaphore, &xHigherPriorityTaskWoken); // �ͷŶ�ֵ�ź�����������ʾ

			vTaskResume(GetBook_Handler);
			vTaskSuspend(Error_handler);
			vTaskDelay(1);
			break;
		case 3: // Ԥ���鼮
			bookIndex = findId(BOOK, bookid); // ����λ��BOOK[i].index
			musicIndex = 3;

			xSemaphoreGiveFromISR(BinarySemaphore, &xHigherPriorityTaskWoken); // �ͷŶ�ֵ�ź�����������ʾ

			vTaskResume(GetBook_Handler);
			vTaskSuspend(Error_handler);
			break;
		case 4: // �黹
			bookIndex = checkBookAddress(BOOK);

			if (bookIndex != null)
			{
				addBook(BOOK, bookid, bookIndex);
				printf("��ӳɹ���%s\r\n", BOOK[bookIndex].bookId);
				// ��ӡ����
				for (i = 0; i <= 9; i++)
				{
					printf("\r\nbookId:%s    2:%d    3:%d\r\n", BOOK[i].bookId, BOOK[i].index, BOOK[i].flag);
				}
			}
			bookIndex = findId(BOOK, bookid); // ����λ��bookChack[i].index
			musicIndex = 5;

			xSemaphoreGiveFromISR(BinarySemaphore, &xHigherPriorityTaskWoken); // �ͷŶ�ֵ�ź�����������ʾ

			vTaskResume(PushBook_Handler);
			vTaskSuspend(Error_handler);
			vTaskDelay(1);
			break;
		case 10: // ����
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
		tp_dev.scan(0);	   // ����ɨ��
		lv_task_handler(); // lvgl������
		vTaskDelay(1);
	}
}

void Cmd_task(void *pvParameters)
{
	while (1)
	{
		if (strlen(esp8266_buf) != 0 && strstr(esp8266_buf, "book") != NULL) // С������Ϣ����
		{
			printf("��Ϣ: %s\r\n", esp8266_buf);

			// ��������
			dataPtr = ESP8266_GetIPD(300, "book");		 // ���json����
			fingString(dataPtr, "cmd", "\"", cmd);		 // �������
			fingString(dataPtr, "bookId", "\"", bookid); // ����鼮id

			if (strcmp(cmd, "pushbook") == 0) // ͨ��С������ƣ������鼮ָ��
			{
				printf("���ڷ���----\r\n");
				fun = 1;
			}
			else if (strcmp(cmd, "removebook") == 0) // ȡ���鼮ָ��
			{
				printf("��ʼȡ���鼮----: %s\r\n", bookid);
				fun = 2;
			}
			else if (strcmp(cmd, "prebook") == 0) // ȡ���鼮ָ��
			{
				printf("��ʼԤ���鼮: %s-----\r\n", bookid);
				fun = 3;
			}
			else if (strcmp(cmd, "returnbook") == 0) // �黹�鼮ָ��
			{
				printf("��ʼ�黹�鼮: %s-----\r\n", bookid);
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
			printf("�鼮���óɹ�----\r\n");

			sendAck("book", bookid, 1); // ��ӳɹ������ͷ���

			bookIndex = null; // ��λ
			musicIndex = 8;
			memset(bookid, 0, 20);

			xSemaphoreGiveFromISR(BinarySemaphore, &xHigherPriorityTaskWoken); // �ͷŶ�ֵ�ź���
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
			sendAck("book", bookid, 0); // ������Ϣ

			//  ����鼮��Ϣ
			removeBook(BOOK, bookIndex);

			// ��ӡ����
			printf("ȡ���ɹ���%s----\r\n", BOOK[bookIndex].bookId);

			for (i = 0; i <= 9; i++)
			{
				printf("\r\n1:%s    2:%d    3:%d\r\n", BOOK[i].bookId, BOOK[i].index, BOOK[i].flag);
			}

			bookIndex = null; // ��λ
			musicIndex = 9;
			memset(bookid, 0, 20);

			xSemaphoreGiveFromISR(BinarySemaphore, &xHigherPriorityTaskWoken); // �ͷŶ�ֵ�ź���
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
		// 	sendAck("book", bookid, 2); // ������Ϣ

		// 	//  ����鼮��Ϣ
		// 	removeBook(BOOK, bookIndex);

		// 	// ��ӡ����
		// 	printf("ȡ���ɹ���%s----\r\n", BOOK[bookIndex].bookId);

		// 	for (i = 0; i <= 9; i++)
		// 	{
		// 		printf("\r\n1:%s    2:%d    3:%d\r\n", BOOK[i].bookId, BOOK[i].index, BOOK[i].flag);
		// 	}

		// 	bookIndex = null; // ��λ
		// 	musicIndex = null;
		// 	memset(bookid, 0, 20);

		// 	xSemaphoreGiveFromISR(BinarySemaphore, &xHigherPriorityTaskWoken); // �ͷŶ�ֵ�ź���
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
					printf("ȡ���鼮����������ʾ-----error_index: %d\r\n", i);

					// �����쳣��Ϣ
					// sendAck("book", BOOK[i].bookId, null);

					error_index = i;

					// ����������ʾ
					musicIndex = 6;
					xSemaphoreGiveFromISR(BinarySemaphore, &xHigherPriorityTaskWoken); // �ͷŶ�ֵ�ź�����������ʾ
				}
				else if (BOOK[i].flag == 0 && checkKeyStateOn(i))
				{
					BOOK[i].flag = 1;
					error_index1 = i;
					
					// ��������������
					bookIndex = i;
					musicIndex = 7;
					xSemaphoreGiveFromISR(BinarySemaphore, &xHigherPriorityTaskWoken); // �ͷŶ�ֵ�ź�����������ʾ
					
					printf("�����鼮����������ʾ-----\r\n");
				}
			}
		}
		if (error_index != null)
		{
			// printf(">>>>preview_flag: %d error_index:%d \r\n", preview_flag, error_index);
			// ����lvgl�Զ����¼�������ѡ���
			if(preview_flag == null)
			{
				lv_event_send(error_btn, LV_EVENT_RELEASED, NULL);
				preview_flag = 2;
			}

			if (preview_flag == 1)  // Ԥ���鼮 | ȡ���鼮
			{
				// BOOK[i].previewFlag = error_index;
				sendAck("book", BOOK[error_index].bookId, 0); // ������Ϣ

				//  ����鼮��Ϣ
				removeBook(BOOK, error_index);

				musicIndex = 10;
				preview_flag = null;
				error_index = null;
				xSemaphoreGiveFromISR(BinarySemaphore, &xHigherPriorityTaskWoken); // �ͷŶ�ֵ�ź�����������ʾ
			}
			else if(preview_flag == 0)  // �Ż��鼮
			{
				bookIndex = error_index;
				musicIndex = 1;
				preview_flag = null;
				error_index = null;

				xSemaphoreGiveFromISR(BinarySemaphore, &xHigherPriorityTaskWoken); // �ͷŶ�ֵ�ź�����������ʾ

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

				// �����������ص�
				bookIndex = null;
				musicIndex = 9;
				xSemaphoreGiveFromISR(BinarySemaphore, &xHigherPriorityTaskWoken); // �ͷŶ�ֵ�ź�����������ʾ
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
			err = xSemaphoreTake(BinarySemaphore, portMAX_DELAY); // ��ȡ�ź���
			if (err == pdTRUE)									  // ��ȡ�ź����ɹ�
			{
				// ledָʾ��
				if (bookIndex != null)
				{
					printf("ledon: %d\r\n", bookIndex);
					ledCheckOn(bookIndex); // ����index��ָʾλ��
				}
				else
				{
					ledCheckOff(bookIndex);
				}

				// ������ʾ
				/*
					1��������ʾ��
					2��������ʾ��
					3��ȡ����ʾ��
					4��Ԥ������ʾ��
					5���黹����ʾ��
				*/
				switch (musicIndex)
				{
				case 1:
					setMusicFolder(0x0102); // ������ʾ��
					printf("mu1\r\n");
					break;
				case 2:
					setMusicFolder(0x0103); // ȡ����ʾ��
					printf("mu2");
					break;
				case 3:
					setMusicFolder(0x0104); // Ԥ����ʾ��
					printf("mu3");
					break;
				case 4:
					setMusicFolder(0x0101); // ����
					printf("mu4");
					break;
				case 5:
					setMusicFolder(0x0105); // �黹
					printf("mu5");
					break;
				case 6:
					setMusicFolder(0x0106); // ȡ������
					printf("mu6");
					break;
				case 7:
					setMusicFolder(0x0107); // ���ô���
					printf("mu7");
					break;
				case 8:
					setMusicFolder(0x0108); // ���ô���
					printf("mu8");
					break;
				case 9:
					setMusicFolder(0x0109); // ���ô���
					printf("mu9");
					break;
				case 10:
					setMusicFolder(0x0110); // Ԥ���ɹ�
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

// ˢ������
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
	test(); // ���ڲ��Թ���

	cmd = (u8 *)malloc(sizeof(char) * 20);	  // ����50�ֽ����ڽ�����Ϣ
	bookid = (u8 *)malloc(sizeof(char) * 20); // ����bookid

	// ������ʼ����
	xTaskCreate((TaskFunction_t)Start_task,			 // ������
				(const char *)"start_task",			 // ��������
				(uint16_t)128,						 // �����ջ��С
				(void *)NULL,						 // ���ݸ��������Ĳ���
				(UBaseType_t)1,						 // �������ȼ�
				(TaskHandle_t *)&StartTask_Handler); // ������

	vTaskStartScheduler(); // �����������
}
