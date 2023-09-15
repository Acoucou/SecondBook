#ifndef __MYCT_IIC_H
#define __MYCT_IIC_H
#include "sys.h"	    


//IO��������
#define CT_SDA_IN()  {GPIOF->MODER&=~(3<<(2*11));GPIOF->MODER|=0<<2*11;}	//PF11����ģʽ
#define CT_SDA_OUT() {GPIOF->MODER&=~(3<<(2*11));GPIOF->MODER|=1<<2*11;} 	//PF11���ģʽ
//IO��������	 
#define CT_IIC_SCL    PBout(0) 	//SCL
#define CT_IIC_SDA    PFout(11) //SDA	 
#define CT_READ_SDA   PFin(11)  //����SDA 
 

//IIC���в�������
void CT_IIC_Init(void);                	//��ʼ��IIC��IO��				 
void CT_IIC_Start(void);				//����IIC��ʼ�ź�
void CT_IIC_Stop(void);	  				//����IICֹͣ�ź�
void CT_IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 CT_IIC_Read_Byte(unsigned char ack);	//IIC��ȡһ���ֽ�
u8 CT_IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void CT_IIC_Ack(void);					//IIC����ACK�ź�
void CT_IIC_NAck(void);					//IIC������ACK�ź�

#endif







