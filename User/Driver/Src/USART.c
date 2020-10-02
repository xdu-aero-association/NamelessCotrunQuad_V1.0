#include "Headfile.h"
#include "usart.h"
#include "stdio.h"

/*----------------------------------------------------------------------------------------------------------------------/
        *               ������ֻ��������ѧϰʹ�ã���Ȩ����Ȩ���������ƴ��Ŷӣ�
        *               �����ƴ��Ŷӽ��ɿس���Դ���ṩ�������ߣ�
        *               ������ҪΪ�����ƴ��Ŷ��ṩ������
        *               δ��������ɣ����ý�Դ�����ṩ������
        *               ���ý�Դ����ŵ����Ϲ�����������أ�
        *               �������Դ�����Ĳ�����緢��������Ϊ��
        *               �����ƴ��Ŷӽ���֮�Է��ɽ��������
-----------------------------------------------------------------------------------------------------------------------/
        *               ������Ϣ���ܶ���ֹ��ǰ�����������˳���������
        *               ��Դ���ף���ѧ����ϧ��ף������Ϯ�����׳ɹ�������
-----------------------------------------------------------------------------------------------------------------------/
	*		�����ƴ���Դ�ɿ� V1.1	�人�Ƽ���ѧ  By.YuYi
	*		CSDN����: http://blog.csdn.net/u011992534
	*               �ſ�ID��NamelessCotrun����С��
	*               �����ƴ���Դ�ɿ�QQȺ��540707961
        *               https://shop348646912.taobao.com/?spm=2013.1.1000126.2.5ce78a88ht1sO2
        *               �ٶ�����:�����ƴ���Դ�ɿ�
        *               �޸�����:2017/10/30
        *               �汾��V1.1
        *               ��Ȩ���У�����ؾ���
        *               Copyright(C) �人�Ƽ���ѧ�����ƴ��Ŷ� 2017-2019
        *               All rights reserved
----------------------------------------------------------------------------------------------------------------------*/
DMA_InitTypeDef DMA_InitStructure;
u16 DMA1_MEM_LEN;//����DMAÿ�����ݴ��͵ĳ���
/*
DMA1�ĸ�ͨ����������Ĵ�����ʽ�ǹ̶���,���Ҫ���ݲ�ͬ��������޸�
�Ӵ洢��->����ģʽ/8λ���ݿ��/�洢������ģʽ
DMA_CHx:DMAͨ��CHx      cpar:�����ַ
cmar:�洢����ַ         cndtr:���ݴ�����
*/
void Quad_DMA_Config(DMA_Channel_TypeDef* DMA_CHx,u32 cpar,u32 cmar,u16 cndtr)
{
 	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//ʹ��DMA����
        DMA_DeInit(DMA_CHx);   //��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ
	DMA1_MEM_LEN=cndtr;
	DMA_InitStructure.DMA_PeripheralBaseAddr = cpar;  //DMA����ADC����ַ
	DMA_InitStructure.DMA_MemoryBaseAddr =cmar;//DMA�ڴ����ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;  //������Ϊ���ݴ����Ŀ�ĵ�
	DMA_InitStructure.DMA_BufferSize = cndtr;  //DMAͨ����DMA����Ĵ�С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //�����ַ�Ĵ�������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //�ڴ��ַ�Ĵ�������
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //���ݿ��Ϊ8λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //���ݿ��Ϊ8λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //��������������ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMAͨ�� xӵ�������ȼ�
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
	DMA_Init(DMA_CHx, &DMA_InitStructure);  //����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��USART1_Tx_DMA_Channel����ʶ�ļĴ���

}
void Quad_DMA1_Config(DMA_Channel_TypeDef* DMA_CHx,u32 cpar,u32 cmar,u16 cndtr)
{
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//ʹ��DMA����
        DMA_DeInit(DMA_CHx);   //��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ
	DMA1_MEM_LEN=cndtr;
	DMA_InitStructure.DMA_PeripheralBaseAddr = cpar;  //DMA����ADC����ַ
	DMA_InitStructure.DMA_MemoryBaseAddr =cmar;//DMA�ڴ����ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;  //������Ϊ���ݴ����Ŀ�ĵ�
	DMA_InitStructure.DMA_BufferSize = cndtr;  //DMAͨ����DMA����Ĵ�С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //�����ַ�Ĵ�������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //�ڴ��ַ�Ĵ�������
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //���ݿ��Ϊ8λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //���ݿ��Ϊ8λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //��������������ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh; //DMAͨ�� xӵ�������ȼ�
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
	DMA_Init(DMA_CHx, &DMA_InitStructure);  //����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��USART1_Tx_DMA_Channel����ʶ�ļĴ���
}
void Quad_DMA_Enable(DMA_Channel_TypeDef*DMA_CHx)//����һ��DMA����
{
	DMA_Cmd(DMA_CHx, DISABLE );
        //�ر�USART1 TX DMA1 ��ָʾ��ͨ��
        DMA_InitStructure.DMA_BufferSize =DMA1_MEM_LEN;
	DMA_Init(DMA1_Channel4, &DMA_InitStructure);
 	DMA_Cmd(DMA_CHx, ENABLE);  //ʹ��USART1 TX DMA1 ��ָʾ��ͨ��
}
void    Quad_DMA1_USART1_SEND(u32 SendBuff,u16 len)//DMA---USART1����
{
	Quad_DMA_Config(DMA1_Channel4,(u32)&USART1->DR,(u32)SendBuff,len);//DMA1ͨ��4,����Ϊ����1,�洢��ΪSendBuff,����len.
	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
	Quad_DMA_Enable(DMA1_Channel4);
	//while(DMA_GetFlagStatus(DMA1_FLAG_TC4) != SET);
	//DMA_ClearFlag(DMA1_FLAG_TC4);//���������ɱ�־
}
void    Quad_DMA1_USART3_SEND(u32 SendBuff,u16 len)//DMA---USART1����
{
	Quad_DMA1_Config(DMA1_Channel2,(u32)&USART3->DR,(u32)SendBuff,len);//DMA1ͨ��4,����Ϊ����1,�洢��ΪSendBuff,����len.
	USART_DMACmd(USART3, USART_DMAReq_Tx, ENABLE);
	Quad_DMA_Enable(DMA1_Channel2);
	//while(DMA_GetFlagStatus(DMA1_FLAG_TC4) != SET);
	//DMA_ClearFlag(DMA1_FLAG_TC4);//���������ɱ�־
}
/***************************************************
������: void USART1_Init(unsigned long bound)
˵��:	����1��ʼ��
���:	������
����:	��
��ע:	�ϵ��ʼ��������һ��
****************************************************/
void USART1_Init(unsigned long bound)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1 | RCC_APB2Periph_AFIO, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);


	USART_InitStructure.USART_BaudRate = bound;//
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8bits
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//stop bit is 1
	USART_InitStructure.USART_Parity = USART_Parity_No;//no parity
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//no Hardware Flow Control
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;//enable tx and rx
	USART_Init(USART1, &USART_InitStructure);//

	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//rx interrupt is enable
	USART_Cmd(USART1, ENABLE);

}


void UART1_Send(unsigned char tx_buf)
{
  while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET);//���������fputcʱ��һ����
  USART_SendData(USART1 , tx_buf);//�����ַ�������ĵ����ַ�
  while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET);
}




void USART1_Send(unsigned char *tx_buf, int len)
{
        USART_ClearFlag(USART1, USART_FLAG_TC);
	USART_ClearITPendingBit(USART1, USART_FLAG_TXE);
	while(len--)
	{
	 USART_SendData(USART1, *tx_buf);
	 while(USART_GetFlagStatus(USART1, USART_FLAG_TC) != 1);
	 USART_ClearFlag(USART1, USART_FLAG_TC);
	 USART_ClearITPendingBit(USART1, USART_FLAG_TXE);
	 tx_buf++;
	}

}

void USART1_Receive(unsigned char *rx_buf, int len)
{
	//rx_count = 0;
	//rx_length = len;
	//rx_address = rx_buf;
}

int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (unsigned char) ch);
	while (!(USART1->SR & USART_FLAG_TXE));
	return (ch);
}

typedef struct
{
  uint32_t Head;
  float DataBuf[8];
  uint32_t End;
}DMA_Vcan_Buff;

DMA_Vcan_Buff  Vcan_Buff;
#define DMA_SEND_PERIOD 3//4*5=20ms,����̫С�����ڹ۲첨��
void DMA_Send_StateMachine(void)
{
  static uint16_t DMA_Send_Cnt=0;
  DMA_Send_Cnt++;
  if(DMA_Send_Cnt>=DMA_SEND_PERIOD)
  {
   Vcan_Buff.Head=0xfc030000;

  Vcan_Buff.DataBuf[0]=NamelessQuad.Position[_YAW];
  Vcan_Buff.DataBuf[1]=NamelessQuad.Speed[_YAW];
  Vcan_Buff.DataBuf[2]=NamelessQuad.Acceleration[_YAW];;
  Vcan_Buff.DataBuf[3]=Altitude_Estimate;
  Vcan_Buff.DataBuf[4]=Origion_NamelessQuad.Acceleration[_YAW];
  Vcan_Buff.DataBuf[5]=Filter_Feedback_NamelessQuad.Acceleration[_YAW];
  Vcan_Buff.DataBuf[6]=Throttle_Control;
  Vcan_Buff.DataBuf[7]=Throttle;


/*
  Vcan_Buff.DataBuf[0]=Pitch;
  Vcan_Buff.DataBuf[1]=Roll;
  Vcan_Buff.DataBuf[2]=ACCE_X;
  Vcan_Buff.DataBuf[3]=ACCE_Y;
  Vcan_Buff.DataBuf[4]=Origion_NamelessQuad.Acceleration[_YAW];
  Vcan_Buff.DataBuf[5]=Filter_Feedback_NamelessQuad.Acceleration[_YAW];
  //Vcan_Buff.DataBuf[6]=Total_Controler.High_Position_Control.Expect;
  //Vcan_Buff.DataBuf[7]=FilterBefore_NamelessQuad.Acceleration[_YAW];
  Vcan_Buff.DataBuf[7]=Gyro_Delta_Length;
  Vcan_Buff.DataBuf[6]=Acceleration_Length;

  Vcan_Buff.DataBuf[0]=NamelessCotrunOptical.Position.x;
  Vcan_Buff.DataBuf[1]=NamelessCotrunOptical.Speed.x;
  Vcan_Buff.DataBuf[2]=SINS_Accel_Body.x;
  Vcan_Buff.DataBuf[3]=SINS_Accel_Body.y;
  Vcan_Buff.DataBuf[4]=OptFlow_Vel_X;
  Vcan_Buff.DataBuf[5]=OptFlow_Vel_Y;
  Vcan_Buff.DataBuf[6]=NamelessCotrun_OptFlow.x_integral;
  Vcan_Buff.DataBuf[7]=NamelessCotrun_OptFlow.y_integral;
*/
/*
  Vcan_Buff.DataBuf[0]=NamelessQuad.Position[_PITCH];
  Vcan_Buff.DataBuf[1]=NamelessQuad.Speed[_PITCH];
  Vcan_Buff.DataBuf[2]=GPS_Vel.E;
  Vcan_Buff.DataBuf[3]=Earth_Frame_To_XYZ.E;

  Vcan_Buff.DataBuf[4]=NamelessQuad.Position[_ROLL];
  Vcan_Buff.DataBuf[5]=NamelessQuad.Speed[_ROLL];
  Vcan_Buff.DataBuf[6]=GPS_Vel.N;
 Vcan_Buff.DataBuf[7]=Earth_Frame_To_XYZ.N;
*/
  //Vcan_Buff.DataBuf[0]=Acce_Correct[0];
  //Vcan_Buff.DataBuf[1]=Acce_Correct[1];
  //Vcan_Buff.DataBuf[2]=Acce_Correct[2];
  //Vcan_Buff.DataBuf[3]=imu.accelRaw[0];
  //Vcan_Buff.DataBuf[4]=imu.accelRaw[1];
 // Vcan_Buff.DataBuf[6]=imu.accelRaw[2];
/*
  Vcan_Buff.DataBuf[0]=ADRC_Roll_Controller.x1;
  Vcan_Buff.DataBuf[1]=ADRC_Roll_Controller.x2;
  Vcan_Buff.DataBuf[2]=Total_Controler.Roll_Angle_Control.Control_OutPut;

  Vcan_Buff.DataBuf[3]=Roll_Gyro;
  Vcan_Buff.DataBuf[4]=ADRC_Roll_Controller.z1;
  Vcan_Buff.DataBuf[5]=ADRC_Roll_Controller.z2;
  Vcan_Buff.DataBuf[6]=Total_Controler.Roll_Gyro_Control.Control_OutPut;
  Vcan_Buff.DataBuf[7]=ADRC_Roll_Controller.u0;
  //Vcan_Buff.DataBuf[7]=FilterBefore_NamelessQuad.Acceleration[_YAW];
  //Vcan_Buff.DataBuf[7]=Gyro_Delta_Length;
  //Vcan_Buff.DataBuf[6]=Acceleration_Length;
*/
  Vcan_Buff.End=0x000003fc;
  Quad_DMA1_USART1_SEND((u32)(&Vcan_Buff),sizeof(Vcan_Buff));
  DMA_Send_Cnt=0;
  }
}


uint8_t Rec_Cnt=0;
uint8_t Rec_Start=0;
uint8_t RecBag[3]={0};
uint8_t Rec_Ready=0;
uint16_t Rec_Ust=0;
void USART1_IRQHandler(void)
{
	unsigned char Res;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //?����??D??(?����?��?��?��y?Y��?D?��?0x0d 0x0a?��?2)
	{
		Rec_Ust++;
	  Res = USART_ReceiveData(USART1);
    if(Res==0xff)
		{
		Rec_Ready=1;
    Rec_Cnt=0;
		}
		if(Rec_Ready==1)
		{
		  Rec_Cnt++;
		}
    if(Rec_Cnt>=3)
		{
		RecBag[Rec_Cnt-3]=Res;
		if(Rec_Cnt==5)
		{
		Rec_Cnt=0;
		Rec_Ready=0;
	  //UART1_Send(RecBag[0]);
	  //UART1_Send(RecBag[1]);
	  //UART1_Send(RecBag[2]);
		}
		}

USART_ClearITPendingBit(USART1, USART_IT_RXNE);

    }
}



void wust_sendccd(unsigned char *ccdaddr, int16_t ccdsize)
{
    #define CMD_CCD   2
    uint8 cmdf[2] = {CMD_CCD, ~CMD_CCD};
    uint8 cmdr[2] = {~CMD_CCD, CMD_CCD};
    USART1_Send(cmdf, sizeof(cmdf));
    USART1_Send(ccdaddr, ccdsize);
    USART1_Send(cmdr, sizeof(cmdr));
}
void wust_sendware(unsigned char *wareaddr, int16_t waresize)
{
    #define CMD_WARE     3
    uint8 cmdf[2] = {CMD_WARE, ~CMD_WARE};
    uint8 cmdr[2] = {~CMD_WARE, CMD_WARE};
    USART1_Send(cmdf, sizeof(cmdf));
    USART1_Send(wareaddr, waresize);
    USART1_Send(cmdr, sizeof(cmdr));
}



void UART2_Send(unsigned char tx_buf)
{
  while(USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET);//���������fputcʱ��һ����
  USART_SendData(USART2 , tx_buf);//�����ַ�������ĵ����ַ�
  while(USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET);
}

void USART2_Send(unsigned char *tx_buf, int len)
{
	USART_ClearFlag(USART2, USART_FLAG_TC);
	USART_ClearITPendingBit(USART2, USART_FLAG_TXE);
	while(len--)
      {
	USART_SendData(USART2, *tx_buf);
	while(USART_GetFlagStatus(USART2, USART_FLAG_TC) != 1);
	USART_ClearFlag(USART2, USART_FLAG_TC);
	USART_ClearITPendingBit(USART2, USART_FLAG_TXE);
	tx_buf++;
      }
}


unsigned char Buffer[2]={9,8};
void USART2_Init(unsigned long bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO
                               |RCC_APB2Periph_GPIOA , ENABLE);//����2

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);//����2 ����

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);


	USART_InitStructure.USART_BaudRate = bound;//
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8bits
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//stop bit is 1
	USART_InitStructure.USART_Parity = USART_Parity_No;//no parity
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//no Hardware Flow Control
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;//enable tx and rx
	USART_Init(USART2, &USART_InitStructure);//

	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);//rx interrupt is enable
	USART_Cmd(USART2, ENABLE);


        //USART2_Send((unsigned char *)Buffer,2);
        UART2_Send(0xAA);
}




void wust_sendimage(unsigned char *wareaddr, int16_t waresize)
{
    #define CMD_Image    1
    uint8 cmdf[2] = {CMD_Image, ~CMD_Image};
    uint8 cmdr[2] = {~CMD_Image, CMD_Image};
    USART1_Send(cmdf, sizeof(cmdf));
    USART1_Send(wareaddr, waresize);
    USART1_Send(cmdr, sizeof(cmdr));
}


void USART3_Init(unsigned long bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = bound;//
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8bits
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//stop bit is 1
	USART_InitStructure.USART_Parity = USART_Parity_No;//no parity
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//no Hardware Flow Control
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;//enable tx and rx
	USART_Init(USART3, &USART_InitStructure);//
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);//rx interrupt is enable
	USART_Cmd(USART3, ENABLE);
}

void USART3_Send(unsigned char tx_buf)
{
  while(USART_GetFlagStatus(USART3, USART_FLAG_TC) != 1);
  USART_SendData(USART3, tx_buf);
  USART_ClearFlag(USART3, USART_FLAG_TC);
  USART_ClearITPendingBit(USART3, USART_FLAG_TXE);
}
void UART3_Send(unsigned char *tx_buf, int len)
{
		USART_ClearFlag(USART3, USART_FLAG_TC);
		USART_ClearITPendingBit(USART3, USART_FLAG_TXE);
	while(len--)
	{
		USART_SendData(USART3, *tx_buf);
		while(USART_GetFlagStatus(USART3, USART_FLAG_TC) != 1);
		USART_ClearFlag(USART3, USART_FLAG_TC);
		USART_ClearITPendingBit(USART3, USART_FLAG_TXE);
		tx_buf++;
	}
}


Testime Time_UASRT3;
uint8_t Frame_Header_IS_Okay=0;
uint8_t OpticalFlow_Data_Buffer[12];
uint8_t OpticalFlow_Data_Cnt=0;
uint8_t OpticalFlow_Data_IS_Okay=0;
void USART3_IRQHandler(void)
{
      if(USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == SET)
      {
        uint8_t ch3=USART_ReceiveData(USART3);
        if(Frame_Header_IS_Okay==1)
        {
          OpticalFlow_Data_Cnt++;
          if(OpticalFlow_Data_Cnt>=2)
          {
            OpticalFlow_Data_Buffer[OpticalFlow_Data_Cnt-2]=ch3;
          }
          if(OpticalFlow_Data_Cnt>=13)
          {
            Frame_Header_IS_Okay=0;
            OpticalFlow_Data_IS_Okay=1;
          }
        }

        if(ch3==0xfe&&Frame_Header_IS_Okay==0)
        {
          Frame_Header_IS_Okay=1;
          OpticalFlow_Data_Cnt=0;
        }

      }
      USART_ClearITPendingBit(USART3, USART_IT_RXNE);
}



/*
float pre_LastHigh=0,LastHigh=0;
uint8 US_100_Cnt=0,US_Data[2]={0};
float US_100_Max=0,US_100_Min=0;
void USART3_IRQHandler(void)
{
  unsigned char ch;
  float tempa,tempb,tempc,max,min;             //���ھ�ֵ�˲�
  if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
  {
  if(US_100_Start==1)
  {
    US_Data[US_100_Cnt]=USART_ReceiveData(USART3);
    US_100_Cnt++;

    if(US_100_Cnt>=2)
    {
    US_100_Cnt=0;
    US_100_Start=0;
    US_100_Finished=1;
    US_Distance=US_100_Distance(US_Data[0],US_Data[1]);//mm

    tempa=pre_LastHigh;//��ֵ�˲�����֤�õ�ֵ������
    tempb=LastHigh;
    tempc=US_Distance;
    max = tempa > tempb ? tempa:tempb;
    max = max > tempc ? max:tempc;
    min = tempa < tempb ? tempa:tempb;
    min = min < tempc ? min:tempc;
    if(tempa > min && tempa < max)    US_Distance = tempa;
    if(tempb > min  && tempb < max )  US_Distance = tempb;
    if(tempc > min  &&  tempc < max)  US_Distance = tempc;
    pre_LastHigh = LastHigh;//���ٶȵ��Ƹ�ֵ
    LastHigh = US_Distance;

    if(US_Distance>=2200)  US_Distance=2200;
    if(US_Distance<=40)  US_Distance=40;
    if(US_Distance>US_100_Max)  US_100_Max=US_Distance;

    }
  }

  USART_ClearITPendingBit(USART3, USART_IT_RXNE);
  }
}
*/


/*
u8 GPS_Buf[2][100]={0};
unsigned char GPS_GPGGA_Buf[6];
unsigned int GPS_Data_Cnt=0;
unsigned GPSx_Cnt=0,GPSx_Finish_Flag=1,GPSx_Data_Cnt=0;
u16 GPS_ISR_CNT=0;
uint8_t GPxCnt=0;
uint16 GPS_Update_finished=0;
 void USART3_IRQHandler(void)
{
  unsigned char ch;
  if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
  {
  GPS_ISR_CNT++;
  if(GPS_ISR_CNT>=2000)  GPS_ISR_CNT=2000;


  ch=USART_ReceiveData(USART3);
  if(ch=='$')
  {
    GPxCnt++;
    if(GPxCnt>=3)  {GPxCnt=1;GPS_Update_finished=1;}

    GPS_Buf[GPxCnt-1][99]='\0';//�ַ�������ĩβ����\0��
    GPSx_Data_Cnt=0;
    GPSx_Finish_Flag=0;
  }
  else if(ch=='*')
  {
    GPSx_Finish_Flag=1;
    GPS_Buf[GPxCnt-1][GPSx_Data_Cnt++]=ch;
    GPS_Buf[GPxCnt-1][99]='\0';
  }

  if(GPSx_Data_Cnt<99&&GPSx_Finish_Flag==0)
  {
   GPS_Buf[GPxCnt-1][GPSx_Data_Cnt++]=ch;
  }


  USART_ClearITPendingBit(USART3, USART_IT_RXNE);
  }
}
*/


u8 GPS_Buf[2][100]={0};
unsigned char GPS_GPGGA_Buf[6];
unsigned int GPS_Data_Cnt=0;
unsigned GPSx_Cnt=0,GPSx_Finish_Flag=1,GPSx_Data_Cnt=0;
u16 GPS_ISR_CNT=0;
uint8_t GPxCnt=0;

uint16 Ublox_Try_Cnt=0;
uint8 Ublox_Try_Buf[5]={0};
uint16 Ublox_Try_Makesure=0;
uint16 Ublox_Try_Start=0;

uint8 Ublox_Data[95]={0};
uint16 Ublox_Cnt=0;
uint16 Ublox_Receive=0;
uint16 GPS_Update_finished=0;
uint16 GPS_Update_finished_Correct_Flag=0;

Testime GPS_Time_Delta;
void USART2_IRQHandler(void)
{
  unsigned char ch;
if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
{

  if(GPS_ISR_CNT<=2000)
  {
    GPS_ISR_CNT++;
  }

  ch=USART_ReceiveData(USART2);

  if(Ublox_Try_Makesure==1)
  {
     Ublox_Data[Ublox_Cnt++]=ch;
     if(Ublox_Cnt==94)
     {
       Ublox_Cnt=0;
       Ublox_Try_Makesure=0;
       GPS_Update_finished=1;
       GPS_Update_finished_Correct_Flag=1;
       Test_Period(&GPS_Time_Delta);//GPS���ݸ��¼������
     }
  }

  if(Ublox_Try_Makesure==0
     &&ch==0xB5)
  {
    Ublox_Try_Start=1;
    Ublox_Try_Cnt=0;
  }

  if(Ublox_Try_Start==1)
  {
    Ublox_Try_Cnt++;
    if(Ublox_Try_Cnt>=5)
    {
      Ublox_Try_Start=0;
      Ublox_Try_Cnt=0;

      if(ch==0x5C) Ublox_Try_Makesure=1;
      else Ublox_Try_Makesure=0;
    }
  }

 }
  USART_ClearITPendingBit(USART2, USART_IT_RXNE);

}

uint8_t data_to_send[50];
/************************************************************/
//1�����ͻ�����Ϣ����̬������״̬��
void ANO_Data_Send_Status(void)
{
	u8 _cnt=0;
	vs16 _temp;
	vs32 _temp2;
	u8 sum = 0;
	u8 i;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x01;
	data_to_send[_cnt++]=0;

	_temp = (int)(Roll*100);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (int)(Pitch*100);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (int)(Yaw*100);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);

	_temp2 = (vs32)(100*NamelessQuad.Position[_YAW]);//��λcm
	data_to_send[_cnt++]=BYTE3(_temp2);
	data_to_send[_cnt++]=BYTE2(_temp2);
	data_to_send[_cnt++]=BYTE1(_temp2);
	data_to_send[_cnt++]=BYTE0(_temp2);

        data_to_send[_cnt++]=0x01;//����ģʽ
        data_to_send[_cnt++]=Controler_State;//����0������1

	data_to_send[3] = _cnt-4;
	sum = 0;
	for(i=0;i<_cnt;i++)
		sum += data_to_send[i];
	data_to_send[_cnt++]=sum;
        Quad_DMA1_USART3_SEND((u32)(data_to_send),_cnt);
        //UART3_Send(data_to_send, _cnt);
}

void ANO_DT_Send_Senser(s16 a_x,s16 a_y,s16 a_z,s16 g_x,s16 g_y,s16 g_z,s16 m_x,s16 m_y,s16 m_z)
{
    u8 _cnt=0;
    vs16 _temp;
    u8 sum = 0;
    u8 i=0;
    data_to_send[_cnt++]=0xAA;
    data_to_send[_cnt++]=0xAA;
    data_to_send[_cnt++]=0x02;
    data_to_send[_cnt++]=0;

    _temp = a_x;
    data_to_send[_cnt++]=BYTE1(_temp);
    data_to_send[_cnt++]=BYTE0(_temp);
    _temp = a_y;
    data_to_send[_cnt++]=BYTE1(_temp);
    data_to_send[_cnt++]=BYTE0(_temp);
    _temp = a_z;
    data_to_send[_cnt++]=BYTE1(_temp);
    data_to_send[_cnt++]=BYTE0(_temp);

    _temp = g_x;
    data_to_send[_cnt++]=BYTE1(_temp);
    data_to_send[_cnt++]=BYTE0(_temp);
    _temp = g_y;
    data_to_send[_cnt++]=BYTE1(_temp);
    data_to_send[_cnt++]=BYTE0(_temp);
    _temp = g_z;
    data_to_send[_cnt++]=BYTE1(_temp);
    data_to_send[_cnt++]=BYTE0(_temp);

    _temp = m_x;
    data_to_send[_cnt++]=BYTE1(_temp);
    data_to_send[_cnt++]=BYTE0(_temp);
    _temp = m_y;
    data_to_send[_cnt++]=BYTE1(_temp);
    data_to_send[_cnt++]=BYTE0(_temp);
    _temp = m_z;
    data_to_send[_cnt++]=BYTE1(_temp);
    data_to_send[_cnt++]=BYTE0(_temp);

    data_to_send[3] = _cnt-4;

    sum = 0;
    for(i=0;i<_cnt;i++)
        sum += data_to_send[i];
    data_to_send[_cnt++] = sum;
    Quad_DMA1_USART3_SEND((u32)(data_to_send),_cnt);
    //UART3_Send(data_to_send, _cnt);
}
void ANO_DT_Send_RCData(u16 thr,u16 yaw,u16 rol,u16 pit,u16 aux1,u16 aux2,u16 aux3,u16 aux4,u16 aux5,u16 aux6)
{
    u8 _cnt=0;
    u8 i=0;
    u8 sum = 0;
    data_to_send[_cnt++]=0xAA;
    data_to_send[_cnt++]=0xAA;
    data_to_send[_cnt++]=0x03;
    data_to_send[_cnt++]=0;
    data_to_send[_cnt++]=BYTE1(thr);
    data_to_send[_cnt++]=BYTE0(thr);
    data_to_send[_cnt++]=BYTE1(yaw);
    data_to_send[_cnt++]=BYTE0(yaw);
    data_to_send[_cnt++]=BYTE1(rol);
    data_to_send[_cnt++]=BYTE0(rol);
    data_to_send[_cnt++]=BYTE1(pit);
    data_to_send[_cnt++]=BYTE0(pit);
    data_to_send[_cnt++]=BYTE1(aux1);
    data_to_send[_cnt++]=BYTE0(aux1);
    data_to_send[_cnt++]=BYTE1(aux2);
    data_to_send[_cnt++]=BYTE0(aux2);
    data_to_send[_cnt++]=BYTE1(aux3);
    data_to_send[_cnt++]=BYTE0(aux3);
    data_to_send[_cnt++]=BYTE1(aux4);
    data_to_send[_cnt++]=BYTE0(aux4);
    data_to_send[_cnt++]=BYTE1(aux5);
    data_to_send[_cnt++]=BYTE0(aux5);
    data_to_send[_cnt++]=BYTE1(aux6);
    data_to_send[_cnt++]=BYTE0(aux6);

    data_to_send[3] = _cnt-4;

    sum = 0;
    for(i=0;i<_cnt;i++)
        sum += data_to_send[i];

    data_to_send[_cnt++]=sum;
    Quad_DMA1_USART3_SEND((u32)(data_to_send),_cnt);
    //UART3_Send(data_to_send, _cnt);
}

void ANO_DT_Send_GPSData(u8 Fixstate,
                          u8 GPS_Num,
                          u32 log,
                          u32 lat,
                          int16 gps_head)
{
    u8 sum = 0;
    u8 _cnt=0;
    u8 i=0;
    data_to_send[_cnt++]=0xAA;
    data_to_send[_cnt++]=0xAA;
    data_to_send[_cnt++]=0x04;
    data_to_send[_cnt++]=0;
    data_to_send[_cnt++]=Fixstate;
    data_to_send[_cnt++]=GPS_Num;

    data_to_send[_cnt++]=BYTE3(log);
    data_to_send[_cnt++]=BYTE2(log);
    data_to_send[_cnt++]=BYTE1(log);
    data_to_send[_cnt++]=BYTE0(log);

    data_to_send[_cnt++]=BYTE3(lat);
    data_to_send[_cnt++]=BYTE2(lat);
    data_to_send[_cnt++]=BYTE1(lat);
    data_to_send[_cnt++]=BYTE0(lat);

    data_to_send[_cnt++]=BYTE1(gps_head);
    data_to_send[_cnt++]=BYTE0(gps_head);

    data_to_send[3] = _cnt-4;

    sum = 0;
    for(i=0;i<_cnt;i++)
        sum += data_to_send[i];

    data_to_send[_cnt++]=sum;
    Quad_DMA1_USART3_SEND((u32)(data_to_send),_cnt);
    //UART3_Send(data_to_send, _cnt);
}

int16_t ANO_Cnt=0;
void ANO_SEND_StateMachine()
{
 ANO_Cnt++;
 if(ANO_Cnt==1)
 {
    ANO_Data_Send_Status();
 }
 else if(ANO_Cnt==2)
 {
    ANO_DT_Send_Senser((int16_t)X_g_av,(int16_t)Y_g_av,(int16_t)Z_g_av,
                       (int16_t)X_w_av,(int16_t)Y_w_av,(int16_t)Z_w_av,
                       (int16_t)DataMag.x,(int16_t)DataMag.y,(int16_t)DataMag.z);
 }
 else if(ANO_Cnt==3)
 {
  ANO_DT_Send_RCData(PPM_Databuf[2],PPM_Databuf[3],
                     PPM_Databuf[0],PPM_Databuf[1],
                     PPM_Databuf[4],PPM_Databuf[5],
                     PPM_Databuf[6],PPM_Databuf[7],0,0);
 }
 else if(ANO_Cnt==4)
 {
    ANO_DT_Send_GPSData(1,GPS_Sate_Num,Longitude_Origion,Latitude_Origion,10);
    ANO_Cnt=0;
 }
  else if(ANO_Cnt==5)
 {
    ANO_Cnt=0;
 }
}


uint16_t DMA_SEND_CNT=0;
//���ڹ���ͬһ��DMA������ͬʱ���������һ����֡
void DMA_SEND_Tradeoff(void)
{
     DMA_SEND_CNT++;
     if(DMA_SEND_CNT<=16&&DMA_SEND_CNT>=4)
     {
         DMA_Send_StateMachine();//DMA����
     }
     else if(DMA_SEND_CNT==20)//100ms����һ��
     {
         ANO_SEND_StateMachine();
         DMA_SEND_CNT=0;
     }
}


