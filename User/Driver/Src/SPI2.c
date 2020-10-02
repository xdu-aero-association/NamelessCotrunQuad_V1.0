#include "Headfile.h"
#include "SPI2.h"

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
/************************************
��������void SPI_Configuration(void)
˵����SPI����
��ڣ���
���ڣ���
��ע������SPI2
************************************/
void SPI2_Configuration(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;//����GPIO�ṹ��
        SPI_InitTypeDef SPI_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB
                             | RCC_APB2Periph_GPIOC  | RCC_APB2Periph_AFIO, ENABLE);
	//-----NRF24L01-----//
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;		   //PA11,12-->NRF_CE,NRF_CSN
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;				   //�������
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;				   //50MHz��תƵ��
        GPIO_Init(GPIOC, &GPIO_InitStructure);							   //��ʼ��IO

        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12;   		//PB12-->NRF_IRQ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50MHz��תƵ��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		//��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);


	//-----SPI2-----//
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;	//PB13-->SCK,PB14-->MISO,PB15-->MOSI
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;  												//PB13/14/15�����������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);



	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;					//����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;				//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;						//����ͬ��ʱ�ӵĿ���״̬Ϊ�͵�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;					//����ͬ��ʱ�ӵĵ�һ�������أ��������½������ݱ�����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;						//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ2��36MHz/2/4=4.5MHz
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;				//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;						//CRCֵ����Ķ���ʽ����ȫ˫��ģʽ��CRC��Ϊ���һ���ֽڷ��ͳ�ȥ
	SPI_Init(SPI2, &SPI_InitStructure);  							//����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���

	SPI_Cmd(SPI2, ENABLE); 											//ʹ��SPI����

	GPIO_SetBits(GPIOB, GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15); //PB13/14/15����
}

/***************************************
��������u8 SPI_ReadWriteByte(u8 txData)
˵����SPI��дһ���ֽں���
��ڣ�u8 txData MOSIд�����ֽ�
���ڣ�u8	MISOд����ֽ�
��ע������ֵΪ0ʱ������û�з���/���ճɹ�
****************************************/
u8 SPI2_ReadWriteByte(u8 txData)
{
	u8 retry = 0;

	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) //���ָ����SPI��־λ�������:���ͻ���ձ�־λ
	{
		retry++;
		if (retry > 200)			//��ʱ
			return 0;
	}
	SPI_I2S_SendData(SPI2, txData); 							//ͨ������SPIx����һ������
	retry = 0;
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET) //���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
	{
		retry++;
		if(retry > 200)				//��ʱ
			return 0;
	}
	return SPI_I2S_ReceiveData(SPI2); 						//����ͨ��SPIx������յ�����
}
