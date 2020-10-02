#include "Headfile.h"
/*----------------------------------------------------------------------------------------------------------------------/
        *               本程序只供购买者学习使用，版权著作权属于无名科创团队，
        *               无名科创团队将飞控程序源码提供给购买者，
        *               购买者要为无名科创团队提供保护，
        *               未经作者许可，不得将源代码提供给他人
        *               不得将源代码放到网上供他人免费下载，
        *               更不能以此销售牟利，如发现上述行为，
        *               无名科创团队将诉之以法律解决！！！
-----------------------------------------------------------------------------------------------------------------------/
        *               生命不息、奋斗不止；前人栽树，后人乘凉！！！
        *               开源不易，且学且珍惜，祝早日逆袭、进阶成功！！！
-----------------------------------------------------------------------------------------------------------------------/
        *		无名科创开源飞控 V1.1	武汉科技大学  By.YuYi
        *		CSDN博客: http://blog.csdn.net/u011992534
        *               优酷ID：NamelessCotrun无名小哥
        *               无名科创开源飞控QQ群：540707961
        *               https://shop348646912.taobao.com/?spm=2013.1.1000126.2.5ce78a88ht1sO2
        *               百度贴吧:无名科创开源飞控
        *               修改日期:2017/10/30
        *               版本：V1.1
        *               版权所有，盗版必究。
        *               Copyright(C) 武汉科技大学无名科创团队 2017-2019
        *               All rights reserved
----------------------------------------------------------------------------------------------------------------------*/


/*************************************************************************************************************************
//----------------------------------------------------------------------------------------------------------------------//
	*		四旋翼飞行器飞控板V1.0	武汉科技大学  By.YuYi
	*		CSDN博客: http://blog.csdn.net/u011992534
	*               优酷ID：NamelessCotrun无名小哥
	*               无名科创开源飞控QQ群：540707961
        *               https://shop348646912.taobao.com/?spm=2013.1.1000126.2.5ce78a88ht1sO2
	*		MCU: 	STM32F103RCT6  72MHz
	*		接口映射表:
	*		默认IMU(MPU6050 + IST8310 + SPL06_001):
	*				IMU_SCL						-->	PB6
	*				IMU_SDA						-->	PB7
	*		OLED:
	*				OLED_D0						-->	PC3
	*				OLED_D1						-->	PC2
	*				OLED_RST					-->	PC1
	*				OLED_DC						-->	PC0
	*				OLED_CS						-->	GND
	*		电调输出:
	*				MOTOR1						-->	PB1	-->	TIM3_CH1
	*				MOTOR2						-->	PB0	-->	TIM3_CH2
	*				MOTOR3						-->	PA7	-->	TIM3_CH3
	*				MOTOR4						-->	PA6	-->	TIM3_CH4
	*		RC输入:
	*				PPM信号					        -->	PA8	-->	TIM1_CH1
	*				SBUS信号					-->	PB11-->	USART3_RX
	*		超声波:
	*				TRIG						-->	PB9  TRIG
	*				ECHO						-->	PA1  IO4
	*		按键:
	*				S1						-->	PC8
	*				S2						-->	PC9
	*		LED指示灯:
	*				LED2(Green)			                -->	PA5
	*				LED3(Blue)				        -->	PC10
	*				LED4(Yellow)			                -->	PC5
	*				LED5(Red)			                -->	PC4
	*		SPI(Extended,3.3V)    自制作遥控器  &  传感器模块  MPU6500+HMC5983+MS5611
	*				SPI2_IRQ				        -->	PB12
	*				SPI2_SCK				        -->	PB13
	*				SPI2_MISO				        -->	PB14
	*				SPI2_MOSI				        -->	PB15
	*				SPI2_CE				          -->	PC6
	*				SPI2_CSN				        -->	PC7
	*		USART1(Wireless,3.3V)   山外多功能调试助手  &  Mavlink
	*				USART1_TX				        -->	PA9
	*				USART1_RX				        -->	PA10
	*		USART2(Extended,5V)    外接GPS
	*				USART2_TX				        -->	PA2
	*				USART2_RX				        -->	PA3
	*		USART3(Extended,3.3V)  备用串口  ANO上位机、遥控器SBUS信号输入
	*				USART3_TX				        -->	PB10
	*				USART3_RX				        -->	PB11
	*		预留IO口
	*				IO1				              -->	PC13
	*				IO2				              -->	PC14
	*				IO3				              -->	PC15
	*				IO4				              -->	PA1
************************************************************************************************************************/
/***************************While(1)里面只进行按键、显示、标定等程序****************************************************/
/***************************主要核心：传感器滤波、姿态解算、惯导、控制等代码在TIME.c里面运行**********************************/


int main()
{
  HardWave_Init();//飞控板内部资源、相关外设初始化
  delay_ms(500);
  while(1)
  {
    Key_Scan();//按键扫描
    QuadShow();//显示屏初始化
    Accel_Calibartion();//加速度计标定
    Mag_Calibartion(DataMag,Yaw_Correct);//磁力计标定
  }
}

