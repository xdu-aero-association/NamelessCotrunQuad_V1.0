#include "Headfile.h"
#include "string.h"
#include "oledfont.h"
#include "oled.h"
#include "stdlib.h"
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

#define XLevelL  		0x00
#define XLevelH  		0x10
#define XLevel     ((XLevelH&0x0F)*16+XLevelL)
#define Max_Column 	128
#define Max_Row  		64
#define Brightness 	0xCF
#define X_WIDTH 		128
#define Y_WIDTH 		64

/*------------дһ�����ݣ�Ƭѡ����-----------*/
void LCD_WrDat(unsigned char dat)
{
	unsigned char i=8;
	LCD_DCout(Bit_SET);
	for(i=0;i<8;i++)				//����8λ���ݣ�ʱ����������Ч
	{
		LCD_SCLout(Bit_RESET);
		if(dat&0x80)	LCD_SDAout(Bit_SET);
		else	LCD_SDAout(Bit_RESET);
		LCD_SCLout(Bit_SET);
		dat<<=1;
	}
}
/*------------д���Ƭѡ����-------------*/
void LCD_WrCmd(unsigned char cmd)
{
	unsigned char i=8;
	LCD_DCout(Bit_RESET);
	for(i=0;i<8;i++)			//����8λ���ݣ�ʱ����������Ч
	{
		LCD_SCLout(Bit_RESET);
		if(cmd&0x80)	LCD_SDAout(Bit_SET);
		else LCD_SDAout(Bit_RESET);
		LCD_SCLout(Bit_SET);
		cmd<<=1;
	}
	LCD_DCout(Bit_SET);
}
/*----------------��������------------------*/
void LCD_Set_Pos(unsigned char x, unsigned char y)
{
  LCD_WrCmd(0xb0+y);
  LCD_WrCmd(((x&0xf0)>>4)|0x10);
  LCD_WrCmd((x&0x0f)|0x01);
}
/*----------------ȫ����ʾ-----------------*/
void LCD_Fill(unsigned char bmp_dat)
{
  unsigned char y,x;
  for(y=0;y<8;y++)
  {
    LCD_WrCmd(0xb0+y);
    LCD_WrCmd(0x01);
    LCD_WrCmd(0x10);
    for(x=0;x<X_WIDTH;x++)
    {
      LCD_WrDat(bmp_dat);
    }
  }
}
/*---------------LCD��λ-----------------*/
void LCD_CLS(void)
{
  unsigned char y,x;
  for(y=0;y<8;y++)
  {
    LCD_WrCmd(0xb0+y);
    LCD_WrCmd(0x01);
    LCD_WrCmd(0x10);
    for(x=0;x<X_WIDTH;x++)
      LCD_WrDat(0);
  }
}
/*------��ʾ6X8һ���׼��ASCII�ַ�������ʾ����Ϊ��x��y��------*/
void LCD_P6x8Str(unsigned char x,unsigned char  y,unsigned char ch[])
{
  unsigned char c=0,i=0,j=0;
  while (ch[j]!='\0')
  {
    c =ch[j]-32;
    if(x>126){x=0;y++;}
    LCD_Set_Pos(x,y);
    for(i=0;i<6;i++)
      LCD_WrDat(F6x8[c][i]);
    x+=6;
    j++;
	}
}
//��ʾһ��6X8���ַ�
void LCD_P6x8Char(unsigned char x,unsigned char  y,unsigned char ucData)
{
    unsigned char i, ucDataTmp;
    ucDataTmp = ucData-32;
    if(x > 126)
    {
      x= 0;
      y++;
    }
    LCD_Set_Pos(x, y);
    for(i = 0; i < 6; i++)
    {
      LCD_WrDat(F6x8[ucDataTmp][i]);
    }
}
/*--------------��ʾ6X8�ĸ�����--------------*/
void write_6_8_number(unsigned char x,unsigned char y, float number)
{
  unsigned char i=0;
  unsigned char temp[16];
  unsigned char *point=temp;
  float decimal;
  int data;
  if(number<0)
  {
     temp[0]='-';
     LCD_P6x8Char(x,y,temp[0]);
     x+=6;
     number=-number;
  }
  data=(int)number;
  decimal=number-data;					//С������

  if(data>=1000000000)           //�Ƿ��ܱ�10^9����
  {
     temp[i]=48+data/1000000000;
     data=data%1000000000;
     i++;
	}
  if(data>=100000000)           //�Ƿ��ܱ�10^8����
  {
     temp[i]=48+data/100000000;
     data=data%100000000;
     i++;
	}
  else
    if(data<100000000&&i!=0)
    {
			temp[i]=0+48;
			i++;
		}
  if(data>=10000000)           //�Ƿ��ܱ�10^7����
  {
     temp[i]=48+data/10000000;
     data=data%10000000;
     i++;
	}
  else
    if(data<10000000&&i!=0)
    {
     temp[i]=0+48;
     i++;
		}
  if(data>=1000000)           //�Ƿ��ܱ�10^6����
  {
     temp[i]=48+data/1000000;
     data=data%1000000;
     i++;
	}
  else
    if(data<1000000&&i!=0)
    {
     temp[i]=0+48;
     i++;
		}
  if(data>=100000)           //�Ƿ��ܱ�100000����
  {
     temp[i]=48+data/100000;
     data=data%100000;
     i++;
	}
  else
    if(data<100000&&i!=0)
    {
     temp[i]=0+48;
     i++;
		}
  if(data>=10000)           //�Ƿ��ܱ�10000����
  {
     temp[i]=48+data/10000;
     data=data%10000;
     i++;
	}
  else
    if(data<10000&&i!=0)
    {
     temp[i]=0+48;
     i++;
		}
  if(data>=1000)           //�Ƿ��ܱ�1000����
  {
     temp[i]=48+data/1000;
     data=data%1000;
     i++;
	}
  else
    if(data<1000&&i!=0)
    {
     temp[i]=0+48;
     i++;
		}
  if(data>=100)              //�Ƿ��ܱ�100����
  {
    temp[i]=48+data/100;
    data=data%100;
    i++;
	}
  else
    if(data<100&&i!=0)
    {
     temp[i]=0+48;
     i++;
		}
  if(data>=10)                  //�Ƿ��ܱ�10����
  {
     temp[i]=48+data/10;
     data=data%10;
     i++;
	}
  else
    if(data<10&&i!=0)
    {
     temp[i]=48;
     i++;
		}
  temp[i]=48+data;
  if(decimal>=0.0001)           //�ж��Ƿ���С������
  {
    i++;
    temp[i]='.';                //��ʾС����
    i++;
    data=(int)(decimal*1000);
    temp[i]=48+data/100;
    data=data%100;
    i++;
    if(data>0)
    {
       temp[i]=48+data/10;
       data=data%10;
    }
    if(data>=0)
    {
       i++;
       temp[i]=data+48;
    }
  }
  i++;
  temp[i]='\0';
    LCD_P6x8Str(x,y,point);
}
/*------��ʾ8X16һ���׼��ASCII�ַ�������ʾ����Ϊ��x��y��------*/
void LCD_P8x16Str(unsigned char x,unsigned char  y,unsigned char ch[])
{
  unsigned char c=0,i=0,j=0;
  while (ch[j]!='\0')
  {
    c =ch[j]-32;
    if(x>120){x=0;y++;}
    LCD_Set_Pos(x,y);
    for(i=0;i<8;i++)
      LCD_WrDat(F8X16[c*16+i]);
    LCD_Set_Pos(x,y+1);
    for(i=0;i<8;i++)
      LCD_WrDat(F8X16[c*16+i+8]);
    x+=8;
    j++;
  }
}
//��ʾһ��8X16���ַ�
void LCD_P8x16Char(unsigned char x,unsigned char  y,unsigned char ch)
{
    unsigned char c=0,i=0,j=0;
    c =ch-32;
    if(x>120){x=0;y++;}
    LCD_Set_Pos(x,y);
  	for(i=0;i<8;i++)
  	  LCD_WrDat(F8X16[c*16+i]);
  	LCD_Set_Pos(x,y+1);
  	for(i=0;i<8;i++)
  	  LCD_WrDat(F8X16[c*16+i+8]);
  	x+=8;
  	j++;
}
/*---------------------��ʾ8X16�ĸ�����--------------------*/
void write_8_16_number(unsigned char x,unsigned char y, float number)
{
  unsigned char i=0;
  unsigned char temp[16];
  unsigned char *point=temp;
  float decimal;
  int data;

  if(number<0)
  {
     temp[0]='-';
     LCD_P8x16Char(x,y,temp[0]);
     x+=1;
     number=-number;
  }
  data=(int)number;
  decimal=number-data;     //С������
  if(data>=1000)           //�Ƿ�ɱ�1000����
  {
     temp[i]=48+data/1000;
     data=data%1000;
     i++;
  }
  if(data>=100)              //�ɷ�100����
  {
    temp[i]=48+data/100;
    data=data%100;
    i++;
  }
  else
    if(data<100&&i!=0)
    {
     temp[i]=0+48;
     i++;
    }
  if(data>=10)                  //�ɷ�10����
  {
     temp[i]=48+data/10;
     data=data%10;
     i++;
  }
  else
    if(data<10&&i!=0)
    {
     temp[i]=48;
     i++;
    }
  temp[i]=48+data;
  if(decimal>=0.0001)           //�ж��ǹ�ΪС��
  {
    i++;
    temp[i]='.';                //��ʾС����
    i++;
    data=(int)(decimal*1000);
    temp[i]=48+data/100;
    data=data%100;
    i++;
    if(data>0)
    {
       temp[i]=48+data/10;
       data=data%10;
    }
    if(data>=0)
    {
       i++;
       temp[i]=data+48;
    }
  }
  i++;
  temp[i]='\0';
    LCD_P8x16Str(x,y,point);
}


void LCD_clear_L(unsigned char x,unsigned char y)
{
	LCD_WrCmd(0xb0+y);
	LCD_WrCmd(0x01);
	LCD_WrCmd(0x10);
        LCD_Set_Pos(x,y);
	for(;x<X_WIDTH;x++)
        {
		LCD_WrDat(0);
        }
}
//��ʾ����ʼ��
void OLEDInit(void)
{

  LCD_SCLout(Bit_RESET);
  delay_ms(100);
  LCD_RSTout(Bit_SET);//�ȴ�RC��λ���

  LCD_WrCmd(0xae);		//--turn off oled panel
  LCD_WrCmd(0x00);		//---set low column address
  LCD_WrCmd(0x10);		//---set high column address
  LCD_WrCmd(0x40);		//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
  LCD_WrCmd(0x81);		//--set contrast control register
  LCD_WrCmd(0xcf); 		// Set SEG Output Current Brightness
  LCD_WrCmd(0xa1);		//--Set SEG/Column Mapping     0xa0???? 0xa1??
  LCD_WrCmd(0xc8);		//Set COM/Row Scan Direction   0xc0???? 0xc8??
  LCD_WrCmd(0xa6);		//--set normal display
  LCD_WrCmd(0xa8);		//--set multiplex ratio(1 to 64)
  LCD_WrCmd(0x3f);		//--1/64 duty
  LCD_WrCmd(0xd3);		//-set display offset Shift Mapping RAM Counter (0x00~0x3F)
  LCD_WrCmd(0x00);		//-not offset
  LCD_WrCmd(0xd5);		//--set display clock divide ratio/oscillator frequency
  LCD_WrCmd(0x80);		//--set divide ratio, Set Clock as 100 Frames/Sec
  LCD_WrCmd(0xd9);		//--set pre-charge period
  LCD_WrCmd(0xf1);		//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
  LCD_WrCmd(0xda);		//--set com pins hardware configuration
  LCD_WrCmd(0x12);
  LCD_WrCmd(0xdb);		//--set vcomh
  LCD_WrCmd(0x40);		//Set VCOM Deselect Level
  LCD_WrCmd(0x20);		//-Set Page Addressing Mode (0x00/0x01/0x02)
  LCD_WrCmd(0x02);		//
  LCD_WrCmd(0x8d);		//--set Charge Pump enable/disable
  LCD_WrCmd(0x14);		//--set(0x10) disable
  LCD_WrCmd(0xa4);		// Disable Entire Display On (0xa4/0xa5)
  LCD_WrCmd(0xa6);		// Disable Inverse Display On (0xa6/a7)
  LCD_WrCmd(0xaf);		//--turn on oled panel
  LCD_Fill(0x00);  		//��ʼ����
  LCD_Set_Pos(0,0);
}

/***************************************************
������: void OLED_Init(void)
˵��:	OLED��ʾ����ʼ��
���:	��
����:	��
��ע:	�ϵ��ʼ��������һ��
****************************************************/
void OLED_Init(void)
{
GPIO_InitTypeDef  GPIO_InitStructure;
RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOC, ENABLE);
GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);
GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 |GPIO_Pin_3;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_Init(GPIOC, &GPIO_InitStructure);

OLEDInit();
}
