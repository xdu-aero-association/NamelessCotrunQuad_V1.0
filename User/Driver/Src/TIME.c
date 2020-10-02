#include "Headfile.h"
#include "Time.h"
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

void Timer4_Configuration(void)
{
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
TIM_DeInit(TIM4);
TIM_TimeBaseStructure.TIM_Period=5000;
TIM_TimeBaseStructure.TIM_Prescaler= (72 - 1);
TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
TIM_ClearFlag(TIM4, TIM_FLAG_Update);
TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
TIM_Cmd(TIM4, ENABLE);
}


uint8 US_100_Start=0,US_100_Finished=1,US_100_Sampling_Cnt=0,US_100_Retry_Cnt=0;
uint8 ADNS3080_Cnt=0;
uint8 HC_SR04_Cnt=0;
Testime Time4_Delta;
uint16_t High_Okay_flag=0;
void TIM4_IRQHandler(void)//5msˢ��һ��
{
 if( TIM_GetITStatus(TIM4,TIM_IT_Update)!=RESET )
 {
      Test_Period(&Time4_Delta);//ϵͳ����ʱ�������
      NRF24L01_RC();//ң������ѯ���գ����ж˷�ʽ
 /*************���ٶȼơ��������������ɼ�***********/
      GET_MPU_DATA();//1.4ms
 /*************������+��ѹ��״̬������***********/
#ifdef IMU_BOARD_GY86
      MS5611_StateMachine_20ms();
      HMC5883L_StateMachine();
#endif
#ifdef IMU_BOARD_NC686
      SPL06_001_StateMachine();
      Get_Mag_IST8310();
#endif
#ifdef IMU_BOARD_NC683
      FBM320_StateMachine();
      Get_Mag_IST8310();
#endif
 /*************��̬����***********/
     AHRSUpdate_GraDes_Delay_Corretion(X_w_av,Y_w_av,Z_w_av,
                             X_g_av,Y_g_av,Z_g_av);
    HC_SR04_Cnt++;
    if(HC_SR04_Cnt>=15)//75ms
    {
      HC_SR04_Start();
      HC_SR04_Cnt=0;
    }
#if  (Optical_Enable==1)
    Optflow_Statemachine();
#endif
    SINS_Prepare();//�õ�������Ե���ϵ�������˶����ٶ�
    if(High_Okay_flag==1)//�߶ȹߵ��ں�
    {
      Strapdown_INS_High();
      //Strapdown_INS_High_Kalman();
    }
    if(GPS_ISR_CNT>=300
    &&GPS_Update_finished==1)//GPS_PVT��������Ϻ󣬿�ʼ����
    {
      GPS_PVT_Parse();//GPS��������֡����
      GPS_Update_finished=0;
      Set_GPS_Home();//����Home��
    }
    if(GPS_Sate_Num>=9       //��λ����
          &&GPS_Quality<=3.0//��λ�ź�����,��Ч��λ
            &&GPS_Home_Set==1)//Home��������
    {
      //Strapdown_INS_Horizontal();
      Filter_Horizontal();
      Bling_Set(&Light_4,2000,1000,0.5,0,GPIOA,WORK_LED,1);
    }

    Total_Control();//�ܿ�����
    Control_Output();//�����������
    if(PPM_Isr_Cnt==100)//PPM���������Ž��д������궨���
    {
        Accel_Calibration_Check();//���ٶȱ궨���
        Mag_Calibration_Check();//�����Ʊ궨���
    }
    Bling_Working(Bling_Mode);//״ָ̬ʾ��

    Angle_Calculate();//���ٶȼ�ŷ���Ǽ���
    DMA_Send_StateMachine();//DMA���䣬ֻʹ��ɽ����λ��
    //ANO_SEND_StateMachine();//DMA���䣬ֻʹ��ANO����վ
    //DMA_SEND_Tradeoff();//DMA���䣬��·����վ��ͬʱ����

    TIM_ClearITPendingBit(TIM4,TIM_FLAG_Update);
 }
}
