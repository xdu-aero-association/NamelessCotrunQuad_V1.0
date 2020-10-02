#include "Headfile.h"
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

Sensor_Okay_Flag Sensor_Init_Flag;
/***************************************************
������: void HardWave_Init(void)
˵��:	����Ӳ����Դ��ʼ��
���:	��
����:	��
��ע:	�ϵ�����е���
ע���ߣ�����С��
****************************************************/
void HardWave_Init(void)
{
   SystemInit();
   cycleCounterInit();//ϵͳ�δ�ʱ�ӳ�ʼ��������׼��ʱ
   SysTick_Config(SystemCoreClock / 1000);//SysTick����ϵͳtick��ʱ������ʼ�����жϣ�1ms
   USART1_Init(115200);//�����ڳ�ʼ��
   OLED_Init();         //��ʾ����ʼ��
   Bling_Init();        //ָʾ�ơ�����IO��ʼ��
   Key_Init();          //������ʼ��
   PPM_Init();          //PPMң�������ճ�ʼ��
   HC_SR04_Init();      //��������ʼ��
   PWM_Init();          //PWM��ʼ����TIM4

   SPI2_Configuration();
   NRF24L01_Init();//
   //while(NRF24L01_Check())
  //{
    printf("24L01 Check Failed!\r\n");
    //printf("Please Check!\r\n");
    delay_ms(100);
  //}
   NRF24L01_RX_Mode();

   Sensor_Init_Flag.NRF24L01_Okay=1;
   QuadInit();
/*******************IMU��ʼ����ʼ*************************/
   delay_ms(2000);
/***********MPU6050��ʼ��***************/
   IIC_GPIO_Config();
   InitMPU6050_GY86();//MPU6050��ʼ�������ѡ����ò���Ƶ�ʡ����̵�
   delay_ms(500);
   IMU_Calibration();//��������ƫ�궨
   Sensor_Init_Flag.MPU6050_Okay=1;
/***********HMC5883��ʼ��***************/
   delay_ms(100);
   QuadInit();
/***********������+��ѹ�Ƴ�ʼ��***************/
   delay_ms(500);
#ifdef IMU_BOARD_GY86//GY86ģ�������ΪHMC5883L
   HMC5883L_Initial();
   Sensor_Init_Flag.Mag_Okay=1;
   QuadInit();
   Baro_init();
   Read_MS5611_Offset();
   Sensor_Init_Flag.Baro_Okay=1;
#endif
#ifdef IMU_BOARD_NC686//NC686ģ�������ΪIST8310����ѹ��ΪSPL01_001
   IST8310_Init();
   Sensor_Init_Flag.Mag_Okay=1;
   QuadInit();
   spl0601_init();
   Sensor_Init_Flag.Baro_Okay=1;
   QuadInit();
#endif
#ifdef IMU_BOARD_NC683//NC686ģ�������ΪIST8310����ѹ��ΪFBM320
   IST8310_Init();
   Sensor_Init_Flag.Mag_Okay=1;
   QuadInit();
   FBM320_Init();
   Sensor_Init_Flag.Baro_Okay=1;
   QuadInit();
#endif
/*******************IMU��ʼ������*************************/
   Quad_Start_Bling();//LED����Ԥ��ʾ
   delay_ms(500);
   Parameter_Init();//������������ʼ��
   LCD_CLS();//����
   USART2_Init(38400);//����2������GPS
   USART3_Init(19200);//����3������
   Total_PID_Init();//PID��������ʼ��

   //�Կ��ſ�������ʼ��

   TIM2_Configuration_Cnt();//TIM2
   Timer4_Configuration();//TIM4
   NVIC_Configuration();//�ж����ȼ�����
}

/***************************************************
������: void NVIC_Configuration(void)
˵��:	�ж����ȼ�����
���:	��
����:	��
��ע:	�ϵ�����е���
ע���ߣ�����С��
****************************************************/
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;//����NVIC��ʼ���ṹ��
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//���ȼ����2������μ�misc.h line80

//�ɿ�ϵͳ��ʱ��
  NVIC_InitStructure.NVIC_IRQChannel =TIM2_IRQn ;//������ʱ��
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  //-----NRF24L01�����ж�-----//
  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;//IRQ�ж�ͨ��-->NRF24L01,PB12
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//����ʽ���ȼ���
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;//�����ȼ���
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ��ͨ��
  NVIC_Init(&NVIC_InitStructure);//��ʼ��NVIC

  //������
  NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

//����2������ģ��
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);


//PPM���ջ�
  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

//GPS���ݽ����ж�
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//�����ж�2
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

//�����ж�1����Ӧɽ����λ����������
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

//�ɿ�������ȶ�ʱ��
  NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
  NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}
