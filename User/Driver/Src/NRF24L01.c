#include "Headfile.h"
#include "NRF24L01.H"
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
#define RC_Middle 1500
#define RC_Deadband 80

//-----NRF24L01���ͽ��յ�ַ-----//
const u8 TX_ADDRESS[TX_ADR_WIDTH] = {0x12,0x37,0x96,0x75,0xA1}; //���͵�ַ
const u8 RX_ADDRESS[RX_ADR_WIDTH] = {0x12,0x37,0x96,0x75,0xA1}; //���յ�ַ
u8 Data_buf[32]={0,0};


//-----NRF24L01��������-----//
u8 g_NRFRevData[RX_PLOAD_WIDTH]={0,0};
//-----NRF24L01��������-----//
u8 g_NRFSendData[TX_PLOAD_WIDTH]={0};

//-----NRF24L01���ռ�����-----//
__IO u8 g_NRFRevCnt = 0;

/********************************
��������void NRF24L01_Init(void)
˵������ʼ��24L01
��ڣ���
���ڣ���
��ע����
*********************************/
void NRF24L01_Init(void)
{
	NRF24L01_CE_L; 			//ʹ��24L01
	NRF24L01_CSN_H;			//SPIƬѡȡ��
}

/******************************
��������u8 NRF24L01_Check(void)
˵�������24L01�Ƿ����
��ڣ���
���ڣ�u8
				0���ɹ�;
				1��ʧ��
��ע����
*******************************/
u8 NRF24L01_Check(void)
{
	u8 buf[5] = {0XA5,0XA5,0XA5,0XA5,0XA5}, i;
	NRF24L01_Write_Buf(WRITE_REG_NRF+TX_ADDR,buf,5);//д��5���ֽڵĵ�ַ��TX_ADDRΪ��ַ��WRITE_REG_NRFΪ����
	NRF24L01_Read_Buf(TX_ADDR,buf,5);//����д��ĵ�ַ
	for (i = 0; i < 5; i++)
		if (buf[i] != 0XA5)
			break;
	if (i != 5)
		return 1;//���24L01����
	return 0;//��⵽24L01
}

/************************************************
��������void NRF24L01_Write_Reg(u8 reg, u8 value)
˵����SPIд�Ĵ���
��ڣ�u8 reg		�Ĵ�����ַ
			u8 value	��Ҫд���ֵ
���ڣ���
��ע����
************************************************/
void NRF24L01_Write_Reg(u8 reg, u8 value)
{
	NRF24L01_CSN_L;                 	//ʹ��SPI����
	SPI2_ReadWriteByte(reg);			    //���ͼĴ����ţ�������Զ�ȡ���Ĵ�����״̬
	SPI2_ReadWriteByte(value);      		//д��Ĵ�����ֵ
	NRF24L01_CSN_H;                 	//��ֹSPI����
}

/************************************
��������u8 NRF24L01_Read_Reg(u8 reg)
˵������ȡSPI�Ĵ���ֵ
��ڣ�u8 reg		�Ĵ�����ַ
���ڣ�u8	״̬
��ע����
*************************************/
//
//reg:Ҫ���ļĴ���
u8 NRF24L01_Read_Reg(u8 reg)
{
	u8 reg_val;

 	NRF24L01_CSN_L;          				//ʹ��SPI����
	SPI2_ReadWriteByte(reg);   				//���ͼĴ�����
	reg_val = SPI2_ReadWriteByte(0XFF);	    //��ȡ�Ĵ������ݣ�ֻ��Ҫ��ȡ�������������⴫�����ݹ�ȥ
	NRF24L01_CSN_H;          			    //��ֹSPI����
	return(reg_val);           				//����״ֵ̬
}

/*******************************************************
��������void NRF24L01_Read_Buf(u8 reg, u8 *pBuf, u8 len)
˵������ָ��λ�ö���ָ�����ȵ�����
��ڣ�u8 reg		�Ĵ���(λ��)
			u8 *pBuf	����ָ��
			u8 len		���ݳ���
���ڣ���
��ע����
*******************************************************/
void NRF24L01_Read_Buf(u8 reg, u8 *pBuf, u8 len)
{
	u8 i;

	NRF24L01_CSN_L;           							//ʹ��SPI����
	SPI2_ReadWriteByte(reg);									//���ͼĴ���ֵ(λ��)
	for (i = 0; i < len; i++)
		pBuf[i] = SPI2_ReadWriteByte(0XFF);		//��������
	NRF24L01_CSN_H;       									//�ر�SPI����
}

/********************************************************
��������void NRF24L01_Write_Buf(u8 reg, u8 *pBuf, u8 len)
˵������ָ��λ��дָ�����ȵ�����
��ڣ�u8 reg		�Ĵ���(λ��)
			u8 *pBuf	����ָ��
			u8 len		���ݳ���
���ڣ���
��ע����
********************************************************/
void NRF24L01_Write_Buf(u8 reg, u8 *pBuf, u8 len)
{
	u8 i;

 	NRF24L01_CSN_L;          						//ʹ��SPI����
	SPI2_ReadWriteByte(reg);							//���ͼĴ���ֵ(λ��)
	for	(i = 0; i < len; i++)
		SPI2_ReadWriteByte(*pBuf++); 			//д������
	NRF24L01_CSN_H;       							//�ر�SPI����
}

/***************************************
��������u8 NRF24L01_TxPacket(u8 *txbuf)
˵��������NRF24L01����һ������
��ڣ�u8 *txbuf	�����������׵�ַ
���ڣ�U8	�������״��
��ע����
****************************************/
u8 NRF24L01_TxPacket(u8 *txbuf)
{
	u8 sta;
	NRF24L01_CE_L;																						//Ƭѡ
        NRF24L01_Write_Buf(WR_TX_PLOAD, txbuf, TX_PLOAD_WIDTH);		//д���ݵ�TX BUF  32���ֽ�
 	NRF24L01_CE_H;
	while (NRF24L01_IRQ != 0);																//�ȴ��������
	sta = NRF24L01_Read_Reg(STATUS);  												//��ȡ״̬�Ĵ�����ֵ
	NRF24L01_Write_Reg(WRITE_REG_NRF + STATUS, sta); 					//���TX_DS��MAX_RT�жϱ�־
	if (sta & MAX_TX)																					//�ﵽ����ط�����
	{
		NRF24L01_Write_Reg(FLUSH_TX, 0xFF);											//���TX FIFO�Ĵ���
		return MAX_TX;
	}
	if (sta & TX_OK)																					//�������
	{
		return TX_OK;
	}
	return 0xFF;																							//����ԭ����ʧ��
}

/***************************************
��������u8 NRF24L01_RxPacket(u8 *rxbuf)
˵��������NRF24L01����һ������
��ڣ�u8 *txbuf	�����������׵�ַ
���ڣ�u8	0���������
		    1�����ղ��ɹ�
��ע����
****************************************/
u8 NRF24L01_RxPacket(u8 *rxbuf)
{
	u8 sta;
	sta = NRF24L01_Read_Reg(STATUS);  							//��ȡ״̬�Ĵ�����ֵ
	NRF24L01_Write_Reg(WRITE_REG_NRF + STATUS,sta); 			//���TX_DS��MAX_RT�жϱ�־
	if (sta & RX_OK)											//���յ�����
	{
		NRF24L01_Read_Buf(RD_RX_PLOAD, rxbuf, RX_PLOAD_WIDTH);	//��ȡ����
		NRF24L01_Write_Reg(FLUSH_RX, 0xFF);						//���RX FIFO�Ĵ���
		return 0;
	}
	return 1;													//û�յ��κ�����
}

/*****************************************************
��������void NRF24L01_RX_Mode(void)
˵������ʼ��NRF24L01��RXģʽ
��ڣ���
���ڣ���
��ע��
����RX��ַ,дRX���ݿ��,ѡ��RFƵ��,�����ʺ�LNA HCURR
��CE��ߺ�,������RXģʽ,�����Խ���������
******************************************************/
void NRF24L01_RX_Mode(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
        EXTI_ClearITPendingBit(EXTI_Line12);//����жϱ�־
        GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource12); //ѡ���жϹܽ�PB12
											//����EXTI�ṹ��
	//���ڷ���ģʽ���Ѿ����ж����жϣ����Ե��л��ɽ���ģʽʱ����Ҫ������������
	EXTI_InitStructure.EXTI_Line = EXTI_Line12;							//EXTI���жϿ�ͨ
        EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;				    //����Ϊ�жϻ����¼�
        EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;				//�½��ش���
        EXTI_InitStructure.EXTI_LineCmd = ENABLE;							//ʹ��EXTI���ж�
        EXTI_Init(&EXTI_InitStructure);

	NRF24L01_CE_L;
	NRF24L01_Write_Buf(WRITE_REG_NRF+TX_ADDR,(u8*)TX_ADDRESS,TX_ADR_WIDTH);	//дTX�ڵ��ַ
	NRF24L01_Write_Buf(WRITE_REG_NRF+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH);//дRX�ڵ��ַ
	NRF24L01_Write_Reg(WRITE_REG_NRF+EN_AA,0x01);    						//ʹ��ͨ��0���Զ�Ӧ��
	NRF24L01_Write_Reg(WRITE_REG_NRF+EN_RXADDR,0x01);						//ʹ��ͨ��0�Ľ��յ�ַ
	NRF24L01_Write_Reg(WRITE_REG_NRF+RF_CH,40);	    						//����RFͨ��Ƶ��
	NRF24L01_Write_Reg(WRITE_REG_NRF+RX_PW_P0,RX_PLOAD_WIDTH);				//ѡ��ͨ��0����Ч���ݿ��
	NRF24L01_Write_Reg(WRITE_REG_NRF+RF_SETUP,0x0f);						//����TX�������,0db����,2Mbps,���������濪��
	NRF24L01_Write_Reg(WRITE_REG_NRF+NRF24L01_CONFIG,0x0f);				//���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ
	NRF24L01_CE_H; 																																	//CEΪ��,�������ģʽ
}

/*****************************************************************************************
��������void NRF24L01_TX_Mode(void)
˵������ʼ��NRF24L01��TXģʽ
��ڣ���
���ڣ���
��ע��
����TX��ַ,дTX���ݿ��,����RX�Զ�Ӧ��ĵ�ַ,���TX��������,ѡ��RFƵ��,�����ʺ�LNA HCURR
PWR_UP,CRCʹ��
CEΪ�ߴ���10us,����������
******************************************************************************************/
void NRF24L01_TX_Mode(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;//����EXTI�ṹ��
	//���ڽ���ģʽ�µ�IRQ��������Ϊ�ж��������ţ����Ե��л��ɷ���ģʽʱ����Ҫ���ж��������ŵĹ���ȡ��
	EXTI_InitStructure.EXTI_Line = EXTI_Line12;//EXTI���жϿ�ͨ
        EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//����Ϊ�жϻ����¼�
        EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//�½��ش���
        EXTI_InitStructure.EXTI_LineCmd =ENABLE;//ʧ��EXTI���ж�
        EXTI_Init(&EXTI_InitStructure);

	NRF24L01_CE_L;
	NRF24L01_Write_Buf(WRITE_REG_NRF+TX_ADDR,(u8*)TX_ADDRESS,TX_ADR_WIDTH);	//дTX�ڵ��ַ
	NRF24L01_Write_Buf(WRITE_REG_NRF+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH); //����TX�ڵ��ַ,��ҪΪ��ʹ��ACK
	NRF24L01_Write_Reg(WRITE_REG_NRF+EN_AA,0x01); //ʹ��ͨ��0�Զ�Ӧ��
	NRF24L01_Write_Reg(WRITE_REG_NRF+EN_RXADDR,0x01); //ʹ��ͨ��0�Ľ��յ�ַ
	NRF24L01_Write_Reg(WRITE_REG_NRF+SETUP_RETR,0x1a);//�����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:15��
	NRF24L01_Write_Reg(WRITE_REG_NRF+RF_CH,40);//����RFͨ��Ϊ40
	NRF24L01_Write_Reg(WRITE_REG_NRF+RF_SETUP,0x0f);//����TX�������,0db����,2Mbps,���������濪��
	NRF24L01_Write_Reg(WRITE_REG_NRF+NRF24L01_CONFIG,0x0e);//���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ,���������ж�
	NRF24L01_CE_H;																															//CEΪ��,10us����������
}


uint16_t NRF24L01_CNT=0;
Testime NRF_Time;
void EXTI15_10_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line12) != RESET)
  {
    Test_Period(&NRF_Time);
    NRF24L01_CNT++;
  }
  EXTI_ClearITPendingBit(EXTI_Line12);
}




int16_t Throttle_Rate=0;
int16_t Get_Thr_Rate(float Thr)//���ű仯��
{
  static float Thr_Rec[20];
  uint16_t i=0;
  for(i=19;i>0;i--)
  {
     Thr_Rec[i]=Thr_Rec[i-1];
  }
  Thr_Rec[0]=Thr;
  return (int16_t)((Thr_Rec[0]-Thr_Rec[9])/1.0f);
}


uint16 Throttle_Control=0;
int16 Pitch_Control=0,Roll_Control=0,Yaw_Control=0;
int16 Target_Angle[2]={0};
uint8 RC_Control[32];
uint16 Last_Throttle_Control,Pre_Last_Throttle_Control;
uint16 Throttle_Base=1000,TempThrottle_Control;
uint16 QuadData[2];
uint8 QuadRemoteFlag[4]={0};
int16 RC_NewData[4]={0};
uint16 RC_Init_Cnt=0;
uint16_t Controler_State=0;
uint16_t Auto_ReLock_Cnt=0;//�Զ�����������
uint8_t Auto_Relock_Flag=0;
uint8_t Auto_Relock_Flag_Set=0;
int16 Y_T=0,T_temp=0;
int16 Temp_RC=0;
uint16_t Unlock_Makesure_Cnt=0,Lock_Makesure_Cnt=0;

uint16_t NRF24L01_Channel[10]={0};
void NRF24L01_RC(void )
{
  if(RC_Init_Cnt<=5)
  {
    Throttle_Control=1000;
    Pitch_Control=0;
    Roll_Control=0;
    Yaw_Control=0;
    RC_Init_Cnt++;
  }
/****************PPM����*****************/
#ifdef RC_NRF24L01
 if(NRF24L01_RxPacket(Data_buf)==0)
 {
  RC_Control[0]=Data_buf[0];
  RC_Control[1]=Data_buf[1];

  RC_Control[2]=Data_buf[2];
  RC_Control[3]=Data_buf[3];

  RC_Control[4]=Data_buf[4];
  RC_Control[5]=Data_buf[5];

  RC_Control[6]=Data_buf[6];
  RC_Control[7]=Data_buf[7];

  RC_Control[8]=Data_buf[8];
  RC_Control[9]=Data_buf[9];

  RC_Control[10]=Data_buf[10];
  RC_Control[11]=Data_buf[11];

  RC_Control[12]=Data_buf[12];
  RC_Control[13]=Data_buf[13];
 }
 NRF24L01_Channel[0]=((uint16_t)(RC_Control[6]<<8)|RC_Control[7])*3000/4095;     //�����ch1
 NRF24L01_Channel[1]=((uint16_t)(RC_Control[4]<<8)|RC_Control[5])*3000/4095;     //������ch2
 NRF24L01_Channel[2]=((uint16_t)(RC_Control[0]<<8)|RC_Control[1])*1000/4095+1000;//����ͨ��ch3
 NRF24L01_Channel[3]=((uint16_t)(RC_Control[2]<<8)|RC_Control[3])*3000/4095;     //ƫ��ͨ��ch4

 NRF24L01_Channel[4]=((uint16_t)((RC_Control[12]&0x0008)>>3)*2000);              //��������ch5
 NRF24L01_Channel[5]=((uint16_t)((RC_Control[12]&0x0004)>>2)*2000);              //��������ch6
 NRF24L01_Channel[6]=((uint16_t)((RC_Control[12]&0x0002)>>1)*2000);              //��������ch7
 NRF24L01_Channel[7]=((uint16_t)((RC_Control[12]&0x0001)>>0)*2000);              //��������ch8


 NRF24L01_Channel[8]=((uint16_t)(RC_Control[8]<<8)|RC_Control[9])*3000/4095;
 NRF24L01_Channel[9]=((uint16_t)(RC_Control[10]<<8)|RC_Control[11])*3000/4095;




 if(NRF24L01_Channel[0]<=RC_Middle-RC_Deadband/2)  Roll_Control=-(RC_Middle-RC_Deadband/2-NRF24L01_Channel[0])*50/1500;
 else if(NRF24L01_Channel[0]>=RC_Middle+RC_Deadband/2)  Roll_Control=-(RC_Middle+RC_Deadband/2-NRF24L01_Channel[0])*50/1500;
 else Roll_Control=0;

 if(NRF24L01_Channel[1]<=RC_Middle-RC_Deadband/2)  Pitch_Control=-(RC_Middle-RC_Deadband/2-NRF24L01_Channel[1])*50/1500;
 else if(NRF24L01_Channel[1]>=RC_Middle+RC_Deadband/2)  Pitch_Control=-(RC_Middle+RC_Deadband/2-NRF24L01_Channel[1])*50/1500;
 else Pitch_Control=0;

 Target_Angle[0]=-Pitch_Control;
 Target_Angle[1]=-Roll_Control;

 Temp_RC=(NRF24L01_Channel[2]-1100);
 if(Temp_RC<=5)     Throttle_Control=0;
 else if(Temp_RC>=1000)  Throttle_Control=1000;
 else Throttle_Control=Temp_RC;

 if(NRF24L01_Channel[3]<=RC_Middle-RC_Deadband/2)
   Yaw_Control=(NRF24L01_Channel[3]-(RC_Middle-RC_Deadband/2))*150/1500;
 else if(NRF24L01_Channel[3]>=RC_Middle+RC_Deadband/2)
   Yaw_Control=(NRF24L01_Channel[3]-(RC_Middle+RC_Deadband/2))*150/1500;
 else Yaw_Control=0;
 if(Yaw_Control>=150) Yaw_Control=150;
 else if(Yaw_Control<=-150) Yaw_Control=-150;

 RC_NewData[0]=Throttle_Control;//ң������ԭʼ�г���
 Throttle_Rate=Get_Thr_Rate(Throttle_Control);
 Throttle_Control=Throttle_Base+Throttle_Control;
#endif

/****************PPM����*****************/
#ifdef RC_PPM
 if(PPM_Databuf[0]<=1480)  Roll_Control=(1450-PPM_Databuf[0])*50/400;
 else if(PPM_Databuf[0]>=1520)  Roll_Control=(1550-PPM_Databuf[0])*50/400;
 else Roll_Control=0;

 if(PPM_Databuf[1]<=1450)  Pitch_Control=(1450-PPM_Databuf[1])*50/400;
 else if(PPM_Databuf[1]>=1550)  Pitch_Control=(1550-PPM_Databuf[1])*50/400;
 else Pitch_Control=0;

 Target_Angle[0]=-Pitch_Control;
 Target_Angle[1]=-Roll_Control;

 Temp_RC=(PPM_Databuf[2]-1100);
 if(Temp_RC<=5)     Throttle_Control=0;
 else if(Temp_RC>=1000)  Throttle_Control=1000;
 else Throttle_Control=Temp_RC;

 if(PPM_Databuf[3]<=1450)  Yaw_Control=-(PPM_Databuf[3]-1450)*200/400;
 else if(PPM_Databuf[3]>=1550)  Yaw_Control=-(PPM_Databuf[3]-1550)*200/400;
 else Yaw_Control=0;
 if(Yaw_Control>=200) Yaw_Control=200;
 else if(Yaw_Control<=-200) Yaw_Control=-200;

 RC_NewData[0]=Throttle_Control;//ң������ԭʼ�г���
 Throttle_Rate=Get_Thr_Rate(Throttle_Control);
 Throttle_Control=Throttle_Base+Throttle_Control;
#endif


  if(Throttle_Control==1000
       &&Yaw_Control>=80
         &&Roll_Control<=-40
           &&Pitch_Control<=-40)
    Unlock_Makesure_Cnt++;

  if(Throttle_Control==1000
       &&Yaw_Control>=70
         &&Roll_Control<=-40
           &&Pitch_Control<=-40
             &&Unlock_Makesure_Cnt>250*1)//����2S
  {
      Controler_State=Lock_Controler;
      Unlock_Makesure_Cnt=0;
      Bling_Set(&Light_1,3000,50,0.5,0,GPIOC,GPIO_Pin_4,0);
      Bling_Set(&Light_2,3000,50,0.5,0,GPIOC,GPIO_Pin_5,0);
      Bling_Set(&Light_3,3000,50,0.5,0,GPIOC,GPIO_Pin_10,0);
      Bling_Mode=0;
      Page_Number=0;//OLED�ָ���ҳ
      Reset_Mag_Calibartion(1);
      Reset_Accel_Calibartion(1);
  }

  if(Throttle_Control==1000
      &&Yaw_Control<=-70
        &&Roll_Control>=40
          &&Pitch_Control<=-40)
       Lock_Makesure_Cnt++;

   if(Throttle_Control==1000
      &&Yaw_Control<=-70
        &&Roll_Control>=40
          &&Pitch_Control<=-40
            &&Lock_Makesure_Cnt>250*1)//����2S
  {
      Controler_State=Unlock_Controler;
      Lock_Makesure_Cnt=0;
      Bling_Set(&Light_1,5000,500,0.8,0,GPIOC,GPIO_Pin_4,0);
      Bling_Set(&Light_2,5000,500,0.5,0,GPIOC,GPIO_Pin_5,0);
      Bling_Set(&Light_3,5000,500,0.3,0,GPIOC,GPIO_Pin_10,0);
      Bling_Mode=0;
      Page_Number=0;//OLED�ָ���ҳ
      Reset_Mag_Calibartion(1);
      Reset_Accel_Calibartion(1);
      Auto_ReLock_Cnt=250*6;//����10S
      Auto_Relock_Flag_Set=0;
  }

  if(Controler_State==Unlock_Controler
     &&Auto_Relock_Flag_Set==0//�Զ�����λδ����
      &&Throttle_Control==1000
       &&Pitch_Control==0
        &&Roll_Control==0
         &&Yaw_Control==0)//������ң�л���
  {
      Auto_Relock_Flag=1;//������ʼʱ�����Զ�������־λ
      Auto_Relock_Flag_Set=1;//���ν�����ֻ��λһ��
  }


  if(Auto_Relock_Flag==1)
  {
   if(Throttle_Control==1000
     &&Pitch_Control==0
       &&Roll_Control==0
         &&Yaw_Control==0
          &&Controler_State==Unlock_Controler)//�Զ�����������
    {
     Auto_ReLock_Cnt--;
     if(Auto_ReLock_Cnt<=0)  Auto_ReLock_Cnt=0;
     if(Auto_ReLock_Cnt==0)
     {
        Controler_State=Lock_Controler;//�ٴ�����
        Bling_Set(&Light_1,3000,50,0.5,0,GPIOC,GPIO_Pin_4,0);
        Bling_Set(&Light_2,3000,50,0.5,0,GPIOC,GPIO_Pin_5,0);
        Bling_Set(&Light_3,3000,50,0.5,0,GPIOC,GPIO_Pin_10,0);
        Bling_Mode=0;
     }
    }

    if((Throttle_Control!=1000
        ||Pitch_Control!=0
         ||Roll_Control!=0
          ||Yaw_Control!=0)
            &&Auto_ReLock_Cnt>0)//�Զ�����������
   {
      Auto_Relock_Flag=0;//ֻҪң���ж��������ν������Զ������Ͳ�����
      Bling_Set(&Light_1,3000,200,0.2,0,GPIOC,GPIO_Pin_4,0);
      Bling_Set(&Light_2,3000,200,0.4,0,GPIOC,GPIO_Pin_5,0);
      Bling_Set(&Light_3,3000,200,0.7,0,GPIOC,GPIO_Pin_10,0);
      Bling_Mode=0;
   }
  }
}
