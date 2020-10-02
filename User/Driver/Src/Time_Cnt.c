#include "Headfile.h"
#include "Time_Cnt.h"
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
void TIM2_Configuration_Cnt(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    TIM_DeInit(TIM2);
    //TIM_TimeBaseStructure.TIM_Period = 60000;//30ms
    //TIM_TimeBaseStructure.TIM_Period = 20000;//10ms
    TIM_TimeBaseStructure.TIM_Period = 20000;//20ms

    TIM_TimeBaseStructure.TIM_Prescaler = 36-1; //36M/36/2=0.5us
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM2, ENABLE);
}


#define Hour         3
#define Minute       2
#define Second       1
#define MicroSecond  0
uint16 Time_Sys[4]={0};
uint16 Microsecond_Cnt=0;
u32 TIME_ISR_CNT=0;
void TIM2_IRQHandler(void)//10ms
{
 if( TIM_GetITStatus(TIM2,TIM_IT_Update)!=RESET )
 {

   TIME_ISR_CNT++;

   Microsecond_Cnt++;
   if(Microsecond_Cnt>=100)//1s
   {
     Microsecond_Cnt=0;
     Time_Sys[Second]++;
     if(Time_Sys[Second]>=60)//1min
     {
     Time_Sys[Second]=0;
     Time_Sys[Minute]++;
     if(Time_Sys[Minute]>=60)//1hour
     {
     Time_Sys[Minute]=0;
     Time_Sys[Hour]++;
     }
     }
   }
 Time_Sys[MicroSecond]=Microsecond_Cnt;
 TIM_ClearITPendingBit(TIM2,TIM_FLAG_Update);
 }
}


void Test_Period(Testime *Time_Lab)
{
   Time_Lab->Last_Time=Time_Lab->Now_Time;
   Time_Lab->Now_Time=(10000*TIME_ISR_CNT
                      +TIM2->CNT/2)/1000.0;//��λms
   Time_Lab->Time_Delta=Time_Lab->Now_Time-Time_Lab->Last_Time;
   Time_Lab->Time_Delta_INT=(uint16)(Time_Lab->Time_Delta);
}
