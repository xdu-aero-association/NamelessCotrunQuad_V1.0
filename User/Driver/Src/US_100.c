#include "Headfile.h"
#include "US_100.h"
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
float US_Distance=0;
float US_100_Distance(uint8 MSB,uint8 LSB)
{
  return (256*(MSB)+(LSB))/1.0;
}

float US_100_Temperature(uint8 data)
{
  return (data-45)/1.0;
}

uint8 HC_SR04_StartFlag=0;
float HC_SR04_Distance=0;
uint16 HC_SR04_RSSI=1;
void HC_SR04_Start(void)
{
  if(HC_SR04_StartFlag==0)
  {
    HC_SR04_OUT_HIGH;
    delay_us(20);
    HC_SR04_OUT_LOW;
    EXTI->IMR |=EXTI_Line1;
    HC_SR04_StartFlag=1;
    HC_SR04_RSSI=255;
  }
   HC_SR04_RSSI--;
}

void HC_SR04_Init(void)
{
      GPIO_InitTypeDef  GPIO_InitStructure;
      EXTI_InitTypeDef EXTI_InitStructure;

      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
      GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//�������
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
      GPIO_Init(GPIOB, &GPIO_InitStructure);

      GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;//��������
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
      GPIO_Init(GPIOA, &GPIO_InitStructure);

      GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1);
      EXTI_InitStructure.EXTI_Line = EXTI_Line1;
      EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
      EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
      EXTI_InitStructure.EXTI_LineCmd	= ENABLE;
      EXTI_Init(&EXTI_InitStructure);

      //EXTI->IMR &=~EXTI_Line1;//�ر��ⲿ�ж�
}

void HC_SR04_UP()
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;//��������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);


  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1);
  EXTI_InitStructure.EXTI_Line = EXTI_Line1;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd= ENABLE;
  EXTI_Init(&EXTI_InitStructure);
}

void HC_SR04_DN()
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//��������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);


  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1);
  EXTI_InitStructure.EXTI_Line = EXTI_Line1;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd= ENABLE;
  EXTI_Init(&EXTI_InitStructure);
}

u32 Test_Cnt1,Test_Cnt2=0,Test_Delta=0;
uint16 Exti_Cnt=0;
uint16 Sample_Cnt=0;
void EXTI1_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line1) != RESET)
  {
   Sample_Cnt++;
   if(Sample_Cnt==1)//��������
   {
    Exti_Cnt++;
    Test_Cnt1=10000*TIME_ISR_CNT
                      +TIM2->CNT/2;
    HC_SR04_DN();
   }
   else//���½���
   {
   Test_Cnt2=10000*TIME_ISR_CNT
                      +TIM2->CNT/2;
   HC_SR04_StartFlag=0;
   HC_SR04_UP();
   //EXTI->IMR &=~EXTI_Line1;//�ر��ⲿ�ж�
   Sample_Cnt=0;
   Test_Delta=(Test_Cnt2-Test_Cnt1);
   HC_SR04_Distance=(Test_Cnt2-Test_Cnt1)*(325)/20000.0;
   }

  }
  EXTI_ClearITPendingBit(EXTI_Line1);
}





