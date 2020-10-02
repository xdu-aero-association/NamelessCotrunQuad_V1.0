#include "Headfile.h"
#include "PPM.h"

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
/***************************************************
������: void PPM_Init(void)
˵��:	PPM���ճ�ʼ��
���:	��
����:	��
��ע:	�ϵ��ʼ��������һ��
****************************************************/
void PPM_Init()
{
GPIO_InitTypeDef GPIO_InitStructure;
EXTI_InitTypeDef EXTI_InitStructure;
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;//GPIO_Pin_0
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;//��������
//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_Init(GPIOA, &GPIO_InitStructure);
GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource8);
EXTI_InitStructure.EXTI_Line = EXTI_Line8;
EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
EXTI_InitStructure.EXTI_LineCmd	= ENABLE;
EXTI_Init(&EXTI_InitStructure);

}

uint16 PPM_Sample_Cnt=0;
uint16 PPM_Isr_Cnt=0;
u32 Last_PPM_Time=0;
u32 PPM_Time=0;
u16 PPM_Time_Delta=0;
u16 PPM_Time_Max=0;
uint16 PPM_Start_Time=0;
uint16 PPM_Finished_Time=0;
uint16 PPM_Is_Okay=0;
uint16 PPM_Databuf[8]={0};

/***************************************************
������: void EXTI9_5_IRQHandler(void)
˵��:	PPM�����жϺ���
���:	��
����:	��
��ע:	�����ʼ����ʼ������
****************************************************/
void EXTI9_5_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line8) != RESET)
  {
//ϵͳ����ʱ���ȡ����λus
    Last_PPM_Time=PPM_Time;
    PPM_Time=10000*TIME_ISR_CNT
                   +TIM2->CNT/2;//us
    PPM_Time_Delta=PPM_Time-Last_PPM_Time;
    //PPM�жϽ����ж�
    if(PPM_Isr_Cnt<100)  PPM_Isr_Cnt++;
   //PPM������ʼ
    if(PPM_Is_Okay==1)
    {
    PPM_Sample_Cnt++;
    //��Ӧͨ��д�뻺����
    PPM_Databuf[PPM_Sample_Cnt-1]=PPM_Time_Delta;
    //���ν�������
      if(PPM_Sample_Cnt>=8)
        PPM_Is_Okay=0;
    }
    if(PPM_Time_Delta>=2050)//֡������ƽ����2ms=2000us
    {
      PPM_Is_Okay=1;
      PPM_Sample_Cnt=0;
    }
  }
  EXTI_ClearITPendingBit(EXTI_Line8);
}