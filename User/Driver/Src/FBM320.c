#include "Headfile.h"
#include "FBM320.h"
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
//#define	fbm320_address	0x6c
#define	fbm320_address	(0x6D<<1)//ADDR����
//#define	fbm320_address	(0x6C<<1)//ADDR����
int32_t UP_S=0, UT_S=0, RP_S=0, RT_S=0, OffP_S=0;
int32_t UP_I=0, UT_I=0, RP_I=0, RT_I=0, OffP_I=0;
float H_S=0, H_I=0;
float Rpress;
uint16_t C0_S, C1_S, C2_S, C3_S, C6_S, C8_S, C9_S, C10_S, C11_S, C12_S;
uint32_t C4_S, C5_S, C7_S;
uint16_t C0_I, C1_I, C2_I, C3_I, C6_I, C8_I, C9_I, C10_I, C11_I, C12_I;
uint32_t C4_I, C5_I, C7_I;
uint8_t	Formula_Select=0;
// Read 1 bytes from the fbm320
int fbm320ReadIn_One(unsigned char address)
{
	uint8_t	data[1];
	data[0]=Single_ReadI2C(fbm320_address,address);
	return	(int)data[0];
}
// Read 3 bytes from the fbm320
long	int fbm320ReadInt_Three(unsigned char address)
{
	long int data;
	data=Three_ReadI2C(fbm320_address,address);
	return	data;
}


void Coefficient(void)													//Receive Calibrate Coefficient
{
	uint8_t i;
	uint16_t R[10];
	uint16_t C0=0, C1=0, C2=0, C3=0, C6=0, C8=0, C9=0, C10=0, C11=0, C12=0;
	uint32_t C4=0, C5=0, C7=0;

	for(i=0; i<9; i++)
		R[i]=((uint16_t)Single_ReadI2C(fbm320_address,0xAA + (i*2))<<8)
                  | Single_ReadI2C(fbm320_address,0xAB + (i*2));
	R[9]=((uint16_t)Single_ReadI2C(fbm320_address,0xA4)<<8)
                      | Single_ReadI2C(fbm320_address,0xF1);

	if(((Formula_Select & 0xF0) == 0x10)
           || ((Formula_Select & 0x0F) == 0x01))
	{
		C0 = R[0] >> 4;
		C1 = ((R[1] & 0xFF00) >> 5) | (R[2] & 7);
		C2 = ((R[1] & 0xFF) << 1) | (R[4] & 1);
		C3 = R[2] >> 3;
		C4 = ((uint32_t)R[3] << 2) | (R[0] & 3);
		C5 = R[4] >> 1;
		C6 = R[5] >> 3;
		C7 = ((uint32_t)R[6] << 3) | (R[5] & 7);
		C8 = R[7] >> 3;
		C9 = R[8] >> 2;
		C10 = ((R[9] & 0xFF00) >> 6) | (R[8] & 3);
		C11 = R[9] & 0xFF;
		C12 = ((R[0] & 0x0C) << 1) | (R[7] & 7);
	}
	else
	{
		C0 = R[0] >> 4;
		C1 = ((R[1] & 0xFF00) >> 5) | (R[2] & 7);
		C2 = ((R[1] & 0xFF) << 1) | (R[4] & 1);
		C3 = R[2] >> 3;
		C4 = ((uint32_t)R[3] << 1) | (R[5] & 1);
		C5 = R[4] >> 1;
		C6 = R[5] >> 3;
		C7 = ((uint32_t)R[6] << 2) | ((R[0] >> 2) & 3);
		C8 = R[7] >> 3;
		C9 = R[8] >> 2;
		C10 = ((R[9] & 0xFF00) >> 6) | (R[8] & 3);
		C11 = R[9] & 0xFF;
		C12 = ((R[5] & 6) << 2) | (R[7] & 7);
	}
	C0_I = C0;
	C1_I = C1;
	C2_I = C2;
	C3_I = C3;
	C4_I = C4;
	C5_I = C5;
	C6_I = C6;
	C7_I = C7;
	C8_I = C8;
	C9_I = C9;
	C10_I = C10;
	C11_I = C11;
	C12_I = C12;
}


//Calculate Real Pressure & Temperautre
void Calculate(int32_t UP, int32_t UT)
{
	int8_t C12=0;
	int16_t C0=0, C2=0, C3=0, C6=0, C8=0, C9=0, C10=0, C11=0;
	int32_t C1=0, C4=0, C5=0, C7=0;
	int32_t RP=0, RT=0;
        int32_t DT=0,DT2=0;
        int32_t X01=0, X02=0, X03=0, X11=0, X12=0;
        int32_t X13=0, X21=0, X22=0, X23=0, X24=0;
        int32_t X25=0, X26=0, X31=0, X32=0;
        int32_t CF=0, PP1=0, PP2=0, PP3=0, PP4=0;
	C0 = C0_I;
	C1 = C1_I;
	C2 = C2_I;
	C3 = C3_I;
	C4 = C4_I;
	C5 = C5_I;
	C6 = C6_I;
	C7 = C7_I;
	C8 = C8_I;
	C9 = C9_I;
	C10 = C10_I;
	C11 = C11_I;
	C12 = C12_I;
	if(((Formula_Select & 0xF0) == 0x10) || ((Formula_Select & 0x0F) == 0x01))			//For FBM320-02
	{
		DT	=	(int32_t)(((UT - 8388608) >> 4) + (C0 << 4));
		X01	=	(C1 + 4459) * DT >> 1;
		X02	=	((((C2 - 256) * DT) >> 14) * DT) >> 4;
		X03	=	(((((C3 * DT) >> 18) * DT) >> 18) * DT);
		RT	=	((2500 << 15) - X01 - X02 - X03) >> 15;

		DT2	=	(X01 + X02 + X03) >> 12;

		X11	=	((C5 - 4443) * DT2);
		X12	=	(((C6 * DT2) >> 16) * DT2) >> 2;
		X13	=	((X11 + X12) >> 10) + ((C4 + 120586) << 4);

		X21	=	((C8 + 7180) * DT2) >> 10;
		X22	=	(((C9 * DT2) >> 17) * DT2) >> 12;
		if(X22 >= X21)
			X23	=	X22 - X21;
		else
			X23	=	X21 - X22;
		X24	=	(X23 >> 11) * (C7 + 166426);
		X25	=	((X23 & 0x7FF) * (C7 + 166426)) >> 11;
		if((X22 - X21) < 0)
			X26	=	((0 - X24 - X25) >> 11) + C7 + 166426;
		else
			X26	=	((X24 + X25) >> 11) + C7 + 166426;

		PP1	=	((UP - 8388608) - X13) >> 3;
		PP2	=	(X26 >> 11) * PP1;
		PP3	=	((X26 & 0x7FF) * PP1) >> 11;
		PP4	=	(PP2 + PP3) >> 10;

		CF	=	(2097152 + C12 * DT2) >> 3;
		X31	=	(((CF * C10) >> 17) * PP4) >> 2;
		X32	=	(((((CF * C11) >> 15) * PP4) >> 18) * PP4);
		RP	=	((X31 + X32) >> 15) + PP4 + 99880;
	}
	else																																						//For FBM320
	{
		DT	=	((UT - 8388608) >> 4) + (C0 << 4);
		X01	=	(C1 + 4418) * DT >> 1;
		X02	=	((((C2 - 256) * DT) >> 14) * DT) >> 4;
		X03	=	(((((C3 * DT) >> 18) * DT) >> 18) * DT);
		RT = ((2500 << 15) - X01 - X02 - X03) >> 15;

		DT2	=	(X01 + X02 + X03) >>12;

		X11	=	(C5 * DT2);
		X12	=	(((C6 * DT2) >> 16) * DT2) >> 2;
		X13	=	((X11 + X12) >> 10) + ((C4 + 211288) << 4);

		X21	=	((C8 + 7209) * DT2) >> 10;
		X22	=	(((C9 * DT2) >> 17) * DT2) >> 12;
		if(X22 >= X21)
			X23	=	X22 - X21;
		else
			X23	=	X21 - X22;
		X24	=	(X23 >> 11) * (C7 + 285594);
		X25	=	((X23 & 0x7FF) * (C7 + 285594)) >> 11;
		if((X22 - X21) < 0)
			X26	=	((0 - X24 - X25) >> 11) + C7 + 285594;
		else
			X26	=	((X24 + X25) >> 11) + C7 + 285594;
		PP1	=	((UP - 8388608) - X13) >> 3;
		PP2	=	(X26 >> 11) * PP1;
		PP3	=	((X26 & 0x7FF) * PP1) >> 11;
		PP4	=	(PP2 + PP3) >> 10;

		CF	=	(2097152 + C12 * DT2) >> 3;
		X31	=	(((CF * C10) >> 17) * PP4) >> 2;
		X32	=	(((((CF * C11) >> 15) * PP4) >> 18) * PP4);
		RP = ((X31 + X32) >> 15) + PP4 + 99880;
	}

	RP_I = RP;
	RT_I = RT;
}

uint8 FBMTEST=0;
void FBM320_Init(void)																	//Initial FBM320 SPI or I2C Bus Protocol
{
	if(Single_ReadI2C(fbm320_address,0x6B) == 0x42)
	{
	//	if((I2C_Read(fbm320_address,0xf4) & 0x40) == 0x40)
		Formula_Select |= 0x01;
		Coefficient();
		Single_WriteI2C(fbm320_address,0xF4, 0x2E);
		delay_ms(5);
		UT_I = fbm320ReadInt_Three(0xf6);
		Single_WriteI2C(fbm320_address,0xF4, 0xF4);
		delay_ms(20);
		UP_I = fbm320ReadInt_Three(0xf6);
		Calculate( UP_I, UT_I);
	}
	Single_WriteI2C(fbm320_address,0xF4, 0x2E);
        FBMTEST=Single_ReadI2C(fbm320_address,0x6B);
}


float FBM320_Offset=0;
float Get_Altitude_FBM320(float baroPress)
{
    float Tempbaro=(float)(baroPress/FBM320_Offset)*1.0;
    True_Altitude = 4433000.0f * (1 - powf((float)(Tempbaro),0.190295f));
    return True_Altitude;
}


Butterworth_Buffer Butter_FBM320;
Butterworth_Parameter FBM320_LPF_Parameter={
//50 2hz
1,   -1.647459981077,   0.7008967811884,
0.01335920002786,  0.02671840005571,  0.01335920002786,
};
float FBM320_LPF(float curr_inputer,Butterworth_Buffer *Buffer,Butterworth_Parameter *Parameter)
{
        /* ���ٶȼ�Butterworth�˲� */
        Buffer->Input_Butter[2]=curr_inputer;/* ��ȡ����x(n) */
	/* Butterworth�˲� */
        Buffer->Output_Butter[2]=
         Parameter->b[0] * Buffer->Input_Butter[2]
        +Parameter->b[1] * Buffer->Input_Butter[1]
	+Parameter->b[2] * Buffer->Input_Butter[0]
        -Parameter->a[1] * Buffer->Output_Butter[1]
        -Parameter->a[2] * Buffer->Output_Butter[0];
	/* x(n) ���б��� */
        Buffer->Input_Butter[0]=Buffer->Input_Butter[1];
        Buffer->Input_Butter[1]=Buffer->Input_Butter[2];
	/* y(n) ���б��� */
        Buffer->Output_Butter[0]=Buffer->Output_Butter[1];
        Buffer->Output_Butter[1]=Buffer->Output_Butter[2];
        return (Buffer->Output_Butter[2]);
}

int16_t FBM320_Ready_Cnt=0;

uint8 FBM320_Offset_Okay=0;
float FBM320_Filter_P=0;
float FBM320_Filter_high=0,FBM320_Filter_high_Origion=0;
void FBM320_StateMachine(void)
{
  static unsigned char FBM32_Cnt=0;
  FBM32_Cnt++;
  if(FBM32_Cnt==1)//��ȡ�¶�
  {
    UT_I = fbm320ReadInt_Three(0xf6);
    Single_WriteI2C(fbm320_address,0xF4, 0xF4);//������ѹת�������߷ֱ�����ת��ʱ��15ms
  }
  else if(FBM32_Cnt==5)////��ȡ��ѹ��24ms
  {
    UP_I = fbm320ReadInt_Three(0xf6);
    Single_WriteI2C(fbm320_address,0xF4, 0x2E);//�����¶�ת����ʱ��4ms
    Calculate( UP_I, UT_I);
    FBM32_Cnt=0;
    if(FBM320_Ready_Cnt<=500) FBM320_Ready_Cnt++;
    if(FBM320_Ready_Cnt==499)
    {
      FBM320_Offset=RP_I;
      FBM320_Offset_Okay=1;
      High_Okay_flag=1;
    }

    if(FBM320_Offset_Okay==1)//��ʼ��ѹ����������
      {
        FBM320_Filter_P=FBM320_LPF(RP_I,
                                   &Butter_FBM320,
                                   &FBM320_LPF_Parameter);
        //FBM320_Filter_P=RP_I;
        FBM320_Filter_high=Get_Altitude_FBM320(FBM320_Filter_P);
        FBM320_Filter_high_Origion=Get_Altitude_FBM320(RP_I);
      }
   }
}






