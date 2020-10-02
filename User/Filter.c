#include "Headfile.h"
#include "Filter.h"
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

//-----Butterworth����-----//
Butter_Parameter Butter_80HZ_Parameter_Acce={
  //200hz---80hz
1,     1.14298050254,   0.4128015980962,
0.638945525159,    1.277891050318,    0.638945525159
};

Butter_Parameter Butter_60HZ_Parameter_Acce={
  //200hz---60hz
1,   0.3695273773512,   0.1958157126558,
0.3913357725018,   0.7826715450035,   0.3913357725018
};

Butter_Parameter Butter_51HZ_Parameter_Acce={
  //200hz---51hz
1,  0.03680751639284,   0.1718123812701,
0.3021549744157,   0.6043099488315,   0.3021549744157,
};

Butter_Parameter Butter_30HZ_Parameter_Acce={
  //200hz---30hz
1,  -0.7477891782585,    0.272214937925,
0.1311064399166,   0.2622128798333,   0.1311064399166
};
Butter_Parameter Butter_20HZ_Parameter_Acce={
  //200hz---20hz
  1,    -1.14298050254,   0.4128015980962,
  0.06745527388907,   0.1349105477781,  0.06745527388907
};
Butter_Parameter Butter_15HZ_Parameter_Acce={
  //200hz---15hz
  1,   -1.348967745253,   0.5139818942197,
  0.04125353724172,  0.08250707448344,  0.04125353724172
};

Butter_Parameter Butter_10HZ_Parameter_Acce={
  //200hz---10hz
  1,   -1.561018075801,   0.6413515380576,
  0.02008336556421,  0.04016673112842,  0.02008336556421};
Butter_Parameter Butter_5HZ_Parameter_Acce={
  //200hz---5hz
  1,   -1.778631777825,   0.8008026466657,
  0.005542717210281,  0.01108543442056, 0.005542717210281
};

Butter_Parameter Butter_2HZ_Parameter_Acce={
  //200hz---2hz
  1,   -1.911197067426,   0.9149758348014,
  0.0009446918438402,  0.00188938368768,0.0009446918438402
};


Butter_BufferData Butter_Buffer[3];
Butter_BufferData Butter_Buffer_Feedback[3];
float Butter_parameter[2][3]={
  //200hz---21hz
        0.07319880848434,   0.1463976169687,  0.07319880848434,
        1,   -1.102497726129,   0.3952929600662
};

/*************************************************
������:	float LPButter_Acce5Hz(float curr_input)
˵��:	���ٶȼƵ�ͨ�˲���
���:	float curr_input	��ǰ������ٶȼ�
����:	��
��ע:	2��Butterworth��ͨ�˲���
		Cutoff Fre. 5Hz
*************************************************/


float LPButterworth(float curr_input,Butter_BufferData *Buffer,Butter_Parameter *Parameter)
{
        /* ���ٶȼ�Butterworth�˲� */
	/* ��ȡ����x(n) */
        static int LPB_Cnt=0;
        Buffer->Input_Butter[2]=curr_input;
        if(LPB_Cnt>=100)
        {
	/* Butterworth�˲� */
        Buffer->Output_Butter[2]=
         Parameter->b[0] * Buffer->Input_Butter[2]
        +Parameter->b[1] * Buffer->Input_Butter[1]
	+Parameter->b[2] * Buffer->Input_Butter[0]
        -Parameter->a[1] * Buffer->Output_Butter[1]
        -Parameter->a[2] * Buffer->Output_Butter[0];
        }
        else
        {
          Buffer->Output_Butter[2]=Buffer->Input_Butter[2];
          LPB_Cnt++;
        }
	/* x(n) ���б��� */
        Buffer->Input_Butter[0]=Buffer->Input_Butter[1];
        Buffer->Input_Butter[1]=Buffer->Input_Butter[2];
	/* y(n) ���б��� */
        Buffer->Output_Butter[0]=Buffer->Output_Butter[1];
        Buffer->Output_Butter[1]=Buffer->Output_Butter[2];


        return Buffer->Output_Butter[2];
}




float Acce_Control[3]={0};
float Acce_Control_Feedback[3]={0};
float Acce_SINS[3]={0};
void Acce_Control_Filter()
{
/**********************�ߵ����ٶ�LPF_21hz**************************/
   //Acce_Control[0]=LPButterworth(X_Origion,
   //                 &Butter_Buffer[0],&Butter_15HZ_Parameter_Acce);
   //Acce_Control[1]=LPButterworth(Y_Origion
   //                 ,&Butter_Buffer[1],&Butter_15HZ_Parameter_Acce);
   //Acce_Control[2]=LPButterworth(Z_Origion
   //                 ,&Butter_Buffer[2],&Butter_15HZ_Parameter_Acce);
   Acce_Control[0]=LPButterworth(X_Origion,
                    &Butter_Buffer[0],&Butter_60HZ_Parameter_Acce);
   Acce_Control[1]=LPButterworth(Y_Origion
                    ,&Butter_Buffer[1],&Butter_60HZ_Parameter_Acce);
   Acce_Control[2]=LPButterworth(Z_Origion
                    ,&Butter_Buffer[2],&Butter_60HZ_Parameter_Acce);

   FilterAfter_NamelessQuad.Acceleration[_YAW]=
                      -Sin_Roll* Acce_Control[0]
                        + Sin_Pitch *Cos_Roll * Acce_Control[1]
                           + Cos_Pitch * Cos_Roll * Acce_Control[2];
   FilterAfter_NamelessQuad.Acceleration[_PITCH]=
                      Cos_Yaw* Cos_Roll * Acce_Control[0]
                        +(Sin_Pitch*Sin_Roll*Cos_Yaw-Cos_Pitch * Sin_Yaw) *Acce_Control[1]
                          +(Sin_Pitch * Sin_Yaw+Cos_Pitch * Sin_Roll * Cos_Yaw) * Acce_Control[2];

   FilterAfter_NamelessQuad.Acceleration[_ROLL]=
                      Sin_Yaw* Cos_Roll * Acce_Control[0]
                        +(Sin_Pitch * Sin_Roll * Sin_Yaw +Cos_Pitch * Cos_Yaw) * Acce_Control[1]
                          + (Cos_Pitch * Sin_Roll * Sin_Yaw - Sin_Pitch * Cos_Yaw) * Acce_Control[2];
   FilterAfter_NamelessQuad.Acceleration[_YAW]*=AcceGravity/AcceMax;
   FilterAfter_NamelessQuad.Acceleration[_YAW]-=AcceGravity;
   FilterAfter_NamelessQuad.Acceleration[_YAW]*=100;//���ٶ�cm/s^2
   FilterAfter_NamelessQuad.Acceleration[_PITCH]*=AcceGravity/AcceMax;
   FilterAfter_NamelessQuad.Acceleration[_PITCH]*=100;//���ٶ�cm/s^2
   FilterAfter_NamelessQuad.Acceleration[_ROLL]*=AcceGravity/AcceMax;
   FilterAfter_NamelessQuad.Acceleration[_ROLL]*=100;//���ٶ�cm/s^2

/**********************���ٶȷ�����LPF_15hz***************************/
   Acce_Control_Feedback[0]=LPButterworth(X_Origion,
                    &Butter_Buffer_Feedback[0],&Butter_30HZ_Parameter_Acce);//15
   Acce_Control_Feedback[1]=LPButterworth(Y_Origion
                    ,&Butter_Buffer_Feedback[1],&Butter_30HZ_Parameter_Acce);
   Acce_Control_Feedback[2]=LPButterworth(Z_Origion
                    ,&Butter_Buffer_Feedback[2],&Butter_30HZ_Parameter_Acce);
   Filter_Feedback_NamelessQuad.Acceleration[_YAW]=
                      -Sin_Roll* Acce_Control_Feedback[0]
                        + Sin_Pitch *Cos_Roll * Acce_Control_Feedback[1]
                           + Cos_Pitch * Cos_Roll * Acce_Control_Feedback[2];
   Filter_Feedback_NamelessQuad.Acceleration[_PITCH]=
                      Cos_Yaw* Cos_Roll * Acce_Control_Feedback[0]
                        +(Sin_Pitch*Sin_Roll*Cos_Yaw-Cos_Pitch * Sin_Yaw) *Acce_Control_Feedback[1]
                          +(Sin_Pitch * Sin_Yaw+Cos_Pitch * Sin_Roll * Cos_Yaw) * Acce_Control_Feedback[2];
   Filter_Feedback_NamelessQuad.Acceleration[_ROLL]=
                      Sin_Yaw* Cos_Roll * Acce_Control_Feedback[0]
                        +(Sin_Pitch * Sin_Roll * Sin_Yaw +Cos_Pitch * Cos_Yaw) * Acce_Control_Feedback[1]
                          + (Cos_Pitch * Sin_Roll * Sin_Yaw - Sin_Pitch * Cos_Yaw) * Acce_Control_Feedback[2];
   Filter_Feedback_NamelessQuad.Acceleration[_YAW]*=AcceGravity/AcceMax;
   Filter_Feedback_NamelessQuad.Acceleration[_YAW]-=AcceGravity;
   Filter_Feedback_NamelessQuad.Acceleration[_YAW]*=100;//���ٶ�cm/s^2
   Filter_Feedback_NamelessQuad.Acceleration[_PITCH]*=AcceGravity/AcceMax;
   Filter_Feedback_NamelessQuad.Acceleration[_PITCH]*=100;//���ٶ�cm/s^2
   Filter_Feedback_NamelessQuad.Acceleration[_ROLL]*=AcceGravity/AcceMax;
   Filter_Feedback_NamelessQuad.Acceleration[_ROLL]*=100;//���ٶ�cm/s^2
}

// discrete low pass filter, cuts out the
// high frequency noise that can drive the controller crazy
//derivative = _last_derivative + _d_lpf_alpha * (derivative - _last_derivative);
float set_lpf_alpha(int16_t cutoff_frequency, float time_step)
{
    // calculate alpha
    float lpf_alpha;
    float rc = 1/(2*PI*cutoff_frequency);
    lpf_alpha = time_step / (time_step + rc);
    return lpf_alpha;
}

