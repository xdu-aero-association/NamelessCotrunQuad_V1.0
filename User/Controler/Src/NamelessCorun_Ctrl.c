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

Vector3_Nav Earth_Frame_Accel_Target={0,0,0};   //��������������ϵ����������������Ŀ���˶����ٶ�����
Vector3_Nav Earth_Frame_Pos_Err={0,0,0};        //��������������ϵ����������������wλ��ƫ��
Vector2_Ang Body_Frame_Accel_Target={0,0};      //��������������ϵ��������(Y��)������(X��)����Ŀ���˶����ٶ�����
Vector2_Ang Body_Frame_Speed_Feedback={0,0};    //��������������ϵ��������(Y��)������(X��)����Ŀ���˶��ٶȷ���
Vector2_Ang Body_Frame_Pos_Err={0,0};           //���巽����λ��ƫ��
Vector2_Ang Body_Frame_Brake_Speed={0,0};       //���巽����ɲ���ٶ�

uint16 High_Pos_Control_Cnt=0;//�߶�λ�ÿ��Ƽ�����
uint16 High_Vel_Control_Cnt=0;//�߶��ٶȿ��Ƽ�����
/*****************ң�����г�����**********************/
#define  Deadband       300//������λ����
#define  Deadzone_Min   350//���Ÿ˸��������ٶ�ʱ�����г��ٽ�ֵ
#define  Deadzone_Max   650//���Ÿ˸��������ٶ�ʱ�����г��ٽ�ֵ
#define  Thr_Top 1000//����������г�
#define  Thr_Buttom 0//����������г�
#define  Climb_Up_Speed_Max    500//������������ٶȣ�cm/s
#define  Climb_Down_Speed_Max  200//��������½��ٶȣ�cm/s
#define  Thr_Start  1150//��ת��������������ǲ����ã�̫��ᵼ�¹�����
#define  Thr_Fly_Start  1250//���������
#define  Thr_Min 1000
#define  Thr_Idle 1100//���ŵ���
uint16 Motor_PWM_1,Motor_PWM_2,Motor_PWM_3,Motor_PWM_4;//�ĸ�������PWM
uint8 MotorTest=0xff;//�����Ų���



float Yaw_Feedforward=0.25;//ƫ����ǰ������;
float Yaw_Vel_Feedforward_Output=0;//��ֱ�ٶ�ǰ�����������;
float Yaw_Vel_Feedforward_Rate=1.0;//��ֱ�ٶ�ǰ����������APM����Ϊ1��0.45;
float Yaw_Vel_Feedforward_Delta=0;//��ֱ�����ٶȱ仯��;
float Last_Yaw_Vel_Target=0;
float Yaw_Vel_Target=0;

uint8_t Controler_High_Mode=1,Last_Controler_High_Mode=1;
uint8_t Controler_Horizontal_Mode=1,Last_Controler_Horizontal_Mode=1;

#define Self_Balance_Mode 1//���ȡ�����̬�����Ų���
#define High_Hold_Mode    2//����ģʽ
#define Pos_Hold_Mode 3//����ģʽ

uint8_t Control_Mode_Change=0;
#define Self_To_High  1//�����ж���
#define High_To_Pos   2//�����ж���

//GPS�����´�˿���ģʽ��ֱ����̬��
uint8 GPS_Speed_Control_Mode=0;
#define Speed_Mode 0
#define Angle_Mode 1

float  Speed_Hold_Pitch=0,Speed_Hold_Roll=0;
uint16_t High_Hold_Throttle=0;
uint8_t  High_Hold_SetFlag=0;
uint8_t  Pos_Hold_SetFlag=0;
uint16_t  HomePoint_Is_Okay=0;
void Controler_Mode_Select()
{
   Last_Controler_High_Mode=Controler_High_Mode;//�ϴθ߶ȿ���ģʽ
   Last_Controler_Horizontal_Mode=Controler_Horizontal_Mode;//�ϴ�λ�ÿ���ģʽ

   if(PPM_Databuf[4]>=1900)       Controler_High_Mode=2;//��ѹ�ƶ���
   else if(PPM_Databuf[4]<=1100)  Controler_High_Mode=1;//����̬����

   if(PPM_Databuf[5]>=1900)        Controler_Horizontal_Mode=2;//ˮƽλ�ÿ���
   else if(PPM_Databuf[5]<=1100)  Controler_Horizontal_Mode=1;//��̬���ȿ���

   if(Controler_High_Mode!=Last_Controler_High_Mode)
   {
     if(Controler_High_Mode==2)  {Control_Mode_Change=1;High_Hold_SetFlag=0;}//�����ж���
   }
   else Control_Mode_Change=0;//��ģʽ�л�


   if(Controler_Horizontal_Mode!=Last_Controler_Horizontal_Mode)//λ��ͨ�����л�
   {
     if(Controler_Horizontal_Mode==2)  {Control_Mode_Change=2;Pos_Hold_SetFlag=0;}//�����ж���
   }
   else Control_Mode_Change=0;//��ģʽ�л�



   if(Controler_High_Mode==High_Hold_Mode//����Ϊ����ģʽ
      &&Last_Controler_High_Mode==Self_Balance_Mode//�ϴ�Ϊ����ģʽ
      &&High_Hold_SetFlag==0)//�߶�ֻ����һ��
   {
      High_Hold_Throttle=Throttle_Control;//���浱ǰ����ֵ��ֻ��һ��
    /*******************����ǰ�ߵ���ֱλ�ù�����ΪĿ��߶�***************************/
      Total_Controler.High_Position_Control.Expect
        =NamelessQuad.Position[_YAW];//�����ز���˲��Ĺߵ��߶�����Ϊ�����߶�
      High_Hold_SetFlag=1;
   }


   if(Controler_High_Mode==Pos_Hold_Mode//����Ϊ����ģʽ
      &&Control_Mode_Change==2//�����ж���
       &&Pos_Hold_SetFlag==0
        &&GPS_Sate_Num>=9//��λ���ǳ���9��
         &&GPS_Quality<=3.0//ˮƽ�������Ӵ���6������,ˮƽλ������ֻ����һ��
          &&GPS_Home_Set==1)
   {
     /*******************����ǰ�ߵ�ˮƽλ�ù�����ΪĿ����ͣ��************************/
      Total_Controler.Latitude_Position_Control.Expect
        =NamelessQuad.Position[_ROLL];
      Total_Controler.Longitude_Position_Control.Expect
        =NamelessQuad.Position[_PITCH];
      Pos_Hold_SetFlag=1;
   }
/******��ǰ��λΪ����ģʽ������ʾ��ͣ��δ���ã�˵��֮ǰδ�������ö����������������********
   1����ʼͨ�������ж���ģʽʱ��GPS״̬δ������ͣ������
   2����ʼͨ�������ж���ģʽʱ��GPS״̬δ������ͣ������֮����������Ȼδ����GPS����������
   3��֮ǰGPS״̬������ͣ������������GPS�ź��������Զ��л���������GPS����������
*******�����жϵ����Ƿ����㶨������������������������ͣ�㣬������붨��ģʽ******/
   if(Controler_Horizontal_Mode==2
      &&Pos_Hold_SetFlag==0)
   {
      if(GPS_Sate_Num>=9//��λ���ǳ���9��
         &&GPS_Quality<=3.0//ˮƽ�������Ӵ���6������
           &&GPS_Home_Set==1
           )
      {
      /*******************����ǰ�ߵ�ˮƽλ�ù�����ΪĿ����ͣ��************************/
        Total_Controler.Latitude_Position_Control.Expect=NamelessQuad.Position[_ROLL];
        Total_Controler.Longitude_Position_Control.Expect=NamelessQuad.Position[_PITCH];
        Pos_Hold_SetFlag=1;
      }
   }
/******������GPS����ģʽ����Pos_Hold_SetFlag��1��������붨��ģʽ*****************/
}



uint16 Value_Limit(uint16 Min,uint16 Max,uint16 Data)
{
   if(Data>=Max) Data=Max;
   else if(Data<=Min) Data=Min;
   return Data;
}


float Yaw_Base=0;
uint16 Yaw_Cnt=0;
void Angle_Control()//�ǶȻ���
{
      //�Ƕȷ���
     Total_Controler.Pitch_Angle_Control.FeedBack=Pitch;
     PID_Control(&Total_Controler.Pitch_Angle_Control);
     Total_Controler.Roll_Angle_Control.FeedBack =Roll;
     PID_Control(&Total_Controler.Roll_Angle_Control);


     if(Yaw_Control==0)//ƫ����������λ
     {
          if(Yaw_Cnt<=500)//��ͷģʽ���ɻ��ϵ��һ��ʱ������ƫ���ǣ������ơ��������ں���Ҫһ��ʱ�䣬����ȡ500
          {
            Yaw_Cnt++;
          }
          if(Total_Controler.Yaw_Angle_Control.Expect==0||Yaw_Cnt<=500)//����ʱ���Ƕ�����ֵ
          {
            Total_Controler.Yaw_Angle_Control.Expect=Yaw;
          }
          Total_Controler.Yaw_Angle_Control.FeedBack=Yaw;//ƫ���Ƿ���

          PID_Control_Yaw(&Total_Controler.Yaw_Angle_Control);//ƫ���Ƕȿ���
          Total_Controler.Yaw_Gyro_Control.Expect=Total_Controler.Yaw_Angle_Control.Control_OutPut;//ƫ�����ٶȻ���������Դ��ƫ���Ƕȿ��������
     }
     else//����ƫ������˺�ֻ�����ڻ����ٶȿ���
     {
        Total_Controler.Yaw_Angle_Control.Expect=0;//ƫ����������0,�����нǶȿ���
        Total_Controler.Yaw_Gyro_Control.Expect=Yaw_Control;//ƫ�����ٶȻ�������ֱ����Դ��ң���������
     }

}




void Gyro_Control()//���ٶȻ�
{
if(GYRO_CONTROL_MODE==PID_MODE)//���������������̬�ڻ����ٶȿ���������PID������
{
     /***************�ڻ����ٶ�����****************/
     Total_Controler.Pitch_Gyro_Control.Expect=Total_Controler.Pitch_Angle_Control.Control_OutPut;
     Total_Controler.Roll_Gyro_Control.Expect=Total_Controler.Roll_Angle_Control.Control_OutPut;
     /***************�ڻ����ٶȷ���****************/
     Total_Controler.Pitch_Gyro_Control.FeedBack=Pitch_Gyro;
     Total_Controler.Roll_Gyro_Control.FeedBack=Roll_Gyro;

     /***************�ڻ����ٶȿ���****************/
     PID_Control_Div_LPF(&Total_Controler.Pitch_Gyro_Control);
     PID_Control_Div_LPF(&Total_Controler.Roll_Gyro_Control);
}
else if(GYRO_CONTROL_MODE==ADRC_MODE)//���������������̬�ڻ����ٶȿ���������ADRC�Կ��ſ�����
{


}
else//�����á�����ֻѡ��һ��ģʽ
{
    /***************�ڻ����ٶ�����****************/
    Total_Controler.Pitch_Gyro_Control.Expect=Total_Controler.Pitch_Angle_Control.Control_OutPut;
    Total_Controler.Roll_Gyro_Control.Expect=Total_Controler.Roll_Angle_Control.Control_OutPut;
    /***************�ڻ����ٶȷ���****************/
    Total_Controler.Pitch_Gyro_Control.FeedBack=Pitch_Gyro;
    Total_Controler.Roll_Gyro_Control.FeedBack=Roll_Gyro;

    /***************�ڻ����ٶȿ���****************/
    PID_Control_Div_LPF(&Total_Controler.Pitch_Gyro_Control);
    PID_Control_Div_LPF(&Total_Controler.Roll_Gyro_Control);


}

//ƫ����ǰ������
 Total_Controler.Yaw_Gyro_Control.FeedBack=Yaw_Gyro;
 PID_Control_Div_LPF(&Total_Controler.Yaw_Gyro_Control);
 //**************************ƫ����ǰ������**********************************
 Total_Controler.Yaw_Gyro_Control.Control_OutPut=Total_Controler.Yaw_Gyro_Control.Control_OutPut
   +Yaw_Feedforward*Total_Controler.Yaw_Gyro_Control.Expect;//ƫ����ǰ������
 if(Total_Controler.Yaw_Gyro_Control.Control_OutPut>=Total_Controler.Yaw_Angle_Control.Control_OutPut_Limit)
   Total_Controler.Yaw_Gyro_Control.Control_OutPut=Total_Controler.Yaw_Angle_Control.Control_OutPut_Limit;
 if(Total_Controler.Yaw_Gyro_Control.Control_OutPut<=-Total_Controler.Yaw_Angle_Control.Control_OutPut_Limit)
   Total_Controler.Yaw_Gyro_Control.Control_OutPut=-Total_Controler.Yaw_Angle_Control.Control_OutPut_Limit;
}

uint16 Throttle=0,Last_Throttle=0,Throttle_Hover=1450;
void Main_Leading_Control(void)
{
/*********************����ң�����л���λ���ɿؽ��벻ͬģʽ****************************/
    if(Controler_High_Mode==1//��̬���ȶ�ģʽ
       &&Controler_Horizontal_Mode==1)//GPS���㵵λδ����
  {
     Total_Controler.Pitch_Angle_Control.Expect=Target_Angle[0];
     Total_Controler.Roll_Angle_Control.Expect=Target_Angle[1];

     if(Throttle_Control<=1000)   Throttle=1000;
     //else Throttle=Throttle_Control;//����ֱ����Դ��ң�������Ÿ���
     else Throttle=Throttle_Control*set_lpf_alpha(2,0.02)+(1-set_lpf_alpha(2,0.02))*Last_Throttle;//����ֱ����Դ��ң�������Ÿ���
     Last_Throttle=Throttle;
  }
  else if(Controler_High_Mode==2//����ģʽ
          &&Controler_Horizontal_Mode==1)//GPS���㵵λδ����
  {
/**************************����ģʽ��ˮƽ��̬��������Դ��ң����******************************************/

#if  (Optical_Enable==0)
     Total_Controler.Pitch_Angle_Control.Expect=Target_Angle[0];
     Total_Controler.Roll_Angle_Control.Expect=Target_Angle[1];
#else   //����������ͣ
    if(Roll_Control==0
        &&Pitch_Control==0)//��ˮƽң��������
    {
        //λ������
        if(NamelessCotrunOptical.Postion_Expect.x==0&&NamelessCotrunOptical.Postion_Expect.y==0)//����˻��к�ֻ����һ��
        {
          NamelessCotrunOptical.Postion_Expect.x=NamelessCotrunOptical.Position.x;
          NamelessCotrunOptical.Postion_Expect.y=NamelessCotrunOptical.Position.y;
        }
        NamelessCotrunOptical.Postion_Feedback.x=NamelessCotrunOptical.Position.x;
        NamelessCotrunOptical.Postion_Feedback.y=NamelessCotrunOptical.Position.y;//λ�÷���

        NamelessCotrunOptical.Postion_Err.x=NamelessCotrunOptical.Postion_Expect.x-NamelessCotrunOptical.Postion_Feedback.x;
        NamelessCotrunOptical.Postion_Err.y=NamelessCotrunOptical.Postion_Expect.y-NamelessCotrunOptical.Postion_Feedback.y;//λ��ƫ��

        NamelessCotrunOptical.Speed_Expect.x=constrain_float(0.1*NamelessCotrunOptical.Postion_Err.x,-100,100);//�ٶ�����
        NamelessCotrunOptical.Speed_Expect.y=constrain_float(0.1*NamelessCotrunOptical.Postion_Err.y,-100,100);

        NamelessCotrunOptical.Speed_Feedback.x=NamelessCotrunOptical.Speed.x;
        NamelessCotrunOptical.Speed_Feedback.y=NamelessCotrunOptical.Speed.y;//�ٶȷ���

        NamelessCotrunOptical.Speed_Err.x=NamelessCotrunOptical.Speed_Expect.x-NamelessCotrunOptical.Speed_Feedback.x;
        NamelessCotrunOptical.Speed_Err.y=NamelessCotrunOptical.Speed_Expect.y-NamelessCotrunOptical.Speed_Feedback.y;//�ٶ�ƫ��


        Total_Controler.Pitch_Angle_Control.Expect=constrain_float(-0.15*NamelessCotrunOptical.Speed_Err.y,-15,15);//�Ƕ�����
        Total_Controler.Roll_Angle_Control.Expect=constrain_float(0.15*NamelessCotrunOptical.Speed_Err.x,-15,15);//�Ƕ�����
    }
    else
    {
        Total_Controler.Pitch_Angle_Control.Expect=Target_Angle[0];
        Total_Controler.Roll_Angle_Control.Expect=Target_Angle[1];
        NamelessCotrunOptical.Postion_Expect.x=0;
        NamelessCotrunOptical.Postion_Expect.y=0;

    }
#endif
/*�߶ȿ�������1��*/
/********
    **
    **
    **
    **
    **
 ********/
/*******************************�߶ȿ�������ʼ****************************************/
/****************���ߣ��߶�λ�û�+�ٶȻ�+���ٶȻ����������ڷֱ�Ϊ8ms��4ms��4ms*******************/
      if(RC_NewData[0]>=Deadzone_Min
         &&RC_NewData[0]<=Deadzone_Max)
     {
          //�߶�λ�û���������ٶ�����
          if(Total_Controler.High_Position_Control.Expect==0)//���Ż��к�ֻ����һ��
          {
            Total_Controler.High_Position_Control.Expect=NamelessQuad.Position[_YAW];//���Ż��к󣬸��¸߶�����
          }
          High_Pos_Control_Cnt++;
          if(High_Pos_Control_Cnt>=2)//��ֱ�߶ȿ�������2*5=10ms
          {
              High_Pos_Control_Cnt=0;
              Total_Controler.High_Position_Control.FeedBack=NamelessQuad.Position[_YAW];//����
              PID_Control(&Total_Controler.High_Position_Control);//���θ߶�λ�ÿ�����
              //�ڻ��ٶ�����
#ifdef YAW_VEL_FEEDFORWARD  //�ٶ�ǰ��������
              Yaw_Vel_Target=Total_Controler.High_Position_Control.Control_OutPut;//�����ٶ�����
              Yaw_Vel_Feedforward_Delta=(Yaw_Vel_Target-Last_Yaw_Vel_Target)/(2*Delta);//�ٶ������仯��
              Yaw_Vel_Feedforward_Output=Yaw_Vel_Feedforward_Rate*Yaw_Vel_Feedforward_Delta;
              Last_Yaw_Vel_Target=Yaw_Vel_Target;//�ϴ��ٶ�����
#endif
              Total_Controler.High_Speed_Control.Expect=Total_Controler.High_Position_Control.Control_OutPut;
                ;
          }
     }
     else if(RC_NewData[0]>Deadzone_Max)//���������ٶ�����
     {
            //���Ÿ����ơ������ٶ�����
           Total_Controler.High_Speed_Control.Expect=Climb_Up_Speed_Max*(RC_NewData[0]-Deadzone_Max)/(Thr_Top-Deadzone_Max);//��������ٶ�50cm/s
           Total_Controler.High_Position_Control.Expect=0;//λ�û�������0
     }
     else if(RC_NewData[0]<Deadzone_Min)//�����½��ٶ�����
     {
            //���Ÿ����ơ������ٶ�����
           Total_Controler.High_Speed_Control.Expect=Climb_Down_Speed_Max*(RC_NewData[0]-Deadzone_Min)/(Deadzone_Min-Thr_Buttom);//����½��ٶ�40cm/s
           Total_Controler.High_Position_Control.Expect=0;//λ�û�������0
     }

/*�߶ȿ�������2��*/
/********
        *
        *
   ****
 *
 *
 ********/

/*******************************��ֱ�ٶȿ�������ʼ*******************************************************************/
     High_Vel_Control_Cnt++;
     if(High_Vel_Control_Cnt>=1)//��ֱ�߶ȿ�������5*1=5ms
     {
        High_Vel_Control_Cnt=0;
        Total_Controler.High_Speed_Control.FeedBack=NamelessQuad.Speed[_YAW];//�ߵ��ٶȹ��Ƹ��ٶȷ���
        PID_Control_Div_LPF(&Total_Controler.High_Speed_Control);//���θ߶��ٶȿ���
     }
/*******************************��ֱ�ٶȿ���������******************************************************************/

/*�߶ȿ�������3��*/
/********
       **
     **
   **
     **
       **
 ********/

/*******************************��ֱ���ٶȿ�������ʼ******************************************************************/
      Total_Controler.High_Acce_Control.Expect=Total_Controler.High_Speed_Control.Control_OutPut;//���ٶ�����
#ifdef YAW_VEL_FEEDFORWARD  //�ٶ�ǰ��������
      Total_Controler.High_Acce_Control.Expect+=Yaw_Vel_Feedforward_Output;//����ǰ�����������
#endif
      Total_Controler.High_Acce_Control.FeedBack=Filter_Feedback_NamelessQuad.Acceleration[_YAW];//���ٶȷ���
      PID_Control_Err_LPF(&Total_Controler.High_Acce_Control);//���θ߶ȼ��ٶȿ���
/*******************************��ֱ���ٶȿ���������******************************************************************/
      if(YAW_Pos_Control_Accel_Disable==0)
    {
      Throttle=Int_Sort(//Throttle_Hover//��ͣ����
              High_Hold_Throttle+Total_Controler.High_Acce_Control.Control_OutPut);//������Դ�ڸ߶ȼ��ٶȿ��������
    }
    else
    {
      Throttle=Int_Sort(//Throttle_Hover//��ͣ����
              High_Hold_Throttle+Total_Controler.High_Speed_Control.Control_OutPut);//������Դ�ڸ߶��ٶȿ��������
    }
/*****************************************�߶ȿ������������������ſ�����***********************************************************/
  }
  else if(Controler_High_Mode==2//����ģʽ
          &&Controler_Horizontal_Mode==2)//GPS���㵵λ������)
  {
/*�߶ȿ�������1��*/
/********
    **
    **
    **
    **
    **
 ********/
/*******************************�߶ȿ�������ʼ****************************************/
/****************���ߣ��߶�λ�û�+�ٶȻ�+���ٶȻ����������ڷֱ�Ϊ8ms��4ms��4ms*******************/
      if(RC_NewData[0]>=Deadzone_Min&&RC_NewData[0]<=Deadzone_Max)
     {
          //�߶�λ�û���������ٶ�����
          if(Total_Controler.High_Position_Control.Expect==0)//���Ż��к�ֻ����һ��
          {
            Total_Controler.High_Position_Control.Expect=NamelessQuad.Position[_YAW];//���Ż��к󣬸��¸߶�����
          }
          High_Pos_Control_Cnt++;
          if(High_Pos_Control_Cnt>=2)//��ֱ�߶ȿ�������4*5=10ms
          {
              High_Pos_Control_Cnt=0;
              Total_Controler.High_Position_Control.FeedBack=NamelessQuad.Position[_YAW];//����
              PID_Control(&Total_Controler.High_Position_Control);//���θ߶�λ�ÿ�����
              //�ڻ��ٶ�����
#ifdef YAW_VEL_FEEDFORWARD  //�ٶ�ǰ��������
              Yaw_Vel_Target=Total_Controler.High_Position_Control.Control_OutPut;//�����ٶ�����
              Yaw_Vel_Feedforward_Delta=(Yaw_Vel_Target-Last_Yaw_Vel_Target)/(2*Delta);//�ٶ������仯��
              Yaw_Vel_Feedforward_Output=Yaw_Vel_Feedforward_Rate*Yaw_Vel_Feedforward_Delta;
              Last_Yaw_Vel_Target=Yaw_Vel_Target;//�ϴ��ٶ�����
#endif
              Total_Controler.High_Speed_Control.Expect=Total_Controler.High_Position_Control.Control_OutPut;
          }
     }
     else if(RC_NewData[0]>Deadzone_Max)//���������ٶ�����
     {
            //���Ÿ����ơ������ٶ�����
           Total_Controler.High_Speed_Control.Expect=Climb_Up_Speed_Max*(RC_NewData[0]-Deadzone_Max)/(Thr_Top-Deadzone_Max);//��������ٶ�50cm/s
           Total_Controler.High_Position_Control.Expect=0;//λ�û�������0
     }
     else if(RC_NewData[0]<Deadzone_Min)//�����½��ٶ�����
     {
            //���Ÿ����ơ������ٶ�����
           Total_Controler.High_Speed_Control.Expect=Climb_Down_Speed_Max*(RC_NewData[0]-Deadzone_Min)/(Deadzone_Min-Thr_Buttom);//����½��ٶ�40cm/s
           Total_Controler.High_Position_Control.Expect=0;//λ�û�������0
     }

/*�߶ȿ�������2��*/
/********
        *
        *
   ****
 *
 *
 ********/

/*******************************��ֱ�ٶȿ�������ʼ*******************************************************************/
     High_Vel_Control_Cnt++;
     if(High_Vel_Control_Cnt>=1)//��ֱ�߶ȿ�������5*1=5ms
     {
        High_Vel_Control_Cnt=0;
        Total_Controler.High_Speed_Control.FeedBack=NamelessQuad.Speed[_YAW];//�ߵ��ٶȹ��Ƹ��ٶȷ���
        PID_Control_Div_LPF(&Total_Controler.High_Speed_Control);//���θ߶��ٶȿ���
     }
/*******************************��ֱ�ٶȿ���������******************************************************************/

/*�߶ȿ�������3��*/
/********
       **
     **
   **
     **
       **
 ********/

/*******************************��ֱ���ٶȿ�������ʼ******************************************************************/
      Total_Controler.High_Acce_Control.Expect=Total_Controler.High_Speed_Control.Control_OutPut;//���ٶ�����
#ifdef YAW_VEL_FEEDFORWARD  //�ٶ�ǰ��������
      Total_Controler.High_Acce_Control.Expect+=Yaw_Vel_Feedforward_Output;//����ǰ�����������
#endif
      Total_Controler.High_Acce_Control.FeedBack=Filter_Feedback_NamelessQuad.Acceleration[_YAW];//���ٶȷ���
      PID_Control_Err_LPF(&Total_Controler.High_Acce_Control);//���θ߶ȼ��ٶȿ���
/*******************************��ֱ���ٶȿ���������******************************************************************/
      if(YAW_Pos_Control_Accel_Disable==0)
    {
      Throttle=Int_Sort(//Throttle_Hover//��ͣ����
              High_Hold_Throttle+Total_Controler.High_Acce_Control.Control_OutPut);//������Դ�ڸ߶ȼ��ٶȿ��������
    }
    else
    {
      Throttle=Int_Sort(//Throttle_Hover//��ͣ����
              High_Hold_Throttle+Total_Controler.High_Speed_Control.Control_OutPut);//������Դ�ڸ߶��ٶȿ��������
    }
/*****************************************�߶ȿ������������������ſ�����**********************************************/


/*******************************ˮƽλ�ÿ�������ʼ***********************************************************/
  if(GPS_Sate_Num>=9//��λ���ǳ���9��
       &&GPS_Quality<=3.0//ˮƽ�������Ӵ���6������
         &&GPS_Home_Set==1
         )
   {
     if(Roll_Control==0
        &&Pitch_Control==0)//��ˮƽң��������
    {
    //λ������,��γ�������ٶȡ��߶�
    if(Total_Controler.Latitude_Position_Control.Expect==0&&Total_Controler.Longitude_Position_Control.Expect==0)//����˻��к�ֻ����һ��
    {
      Total_Controler.Latitude_Position_Control.Expect=NamelessQuad.Position[_ROLL];
      Total_Controler.Longitude_Position_Control.Expect=NamelessQuad.Position[_PITCH];
    }

    //λ�÷�������Դ�ڵ�ǰ�ߵ���λ�ù���
      Total_Controler.Latitude_Position_Control.FeedBack=NamelessQuad.Position[_ROLL];
      Total_Controler.Longitude_Position_Control.FeedBack=NamelessQuad.Position[_PITCH];
    //��������ϵ��E��N������λ��ƫ��
      Earth_Frame_Pos_Err.N=Total_Controler.Latitude_Position_Control.Expect-Total_Controler.Latitude_Position_Control.FeedBack;
      Earth_Frame_Pos_Err.E=Total_Controler.Longitude_Position_Control.Expect-Total_Controler.Longitude_Position_Control.FeedBack;
    //��������ϵ�»���Pitch��Roll������λ��ƫ��
      Body_Frame_Pos_Err.Pit=-Earth_Frame_Pos_Err.E*Sin_Yaw+Earth_Frame_Pos_Err.N*Cos_Yaw;
      Body_Frame_Pos_Err.Rol=Earth_Frame_Pos_Err.E*Cos_Yaw+Earth_Frame_Pos_Err.N*Sin_Yaw;
    //��������ϵ�»���Pitch��Roll����������ɲ���ٶȣ�����Ϊ���������㲻����PID_Control()����
      Body_Frame_Pos_Err.Pit=constrain_float(Body_Frame_Pos_Err.Pit,-Total_Controler.Latitude_Position_Control.Err_Max, Total_Controler.Latitude_Position_Control.Err_Max);//λ��ƫ���޷�����λcm
      Body_Frame_Pos_Err.Rol=constrain_float(Body_Frame_Pos_Err.Rol,-Total_Controler.Longitude_Position_Control.Err_Max,Total_Controler.Longitude_Position_Control.Err_Max);//λ��ƫ���޷�����λcm

      Body_Frame_Brake_Speed.Pit=Total_Controler.Latitude_Position_Control.Kp*Body_Frame_Pos_Err.Pit;
      Body_Frame_Brake_Speed.Rol=Total_Controler.Longitude_Position_Control.Kp*Body_Frame_Pos_Err.Rol;
      //�ٶȿ���������
      Total_Controler.Latitude_Speed_Control.Expect =Body_Frame_Brake_Speed.Pit;
      Total_Controler.Longitude_Speed_Control.Expect=Body_Frame_Brake_Speed.Rol;
    //����ϵ��ˮƽ�ٶȣ�ת������������ϵX-Y������
    //������Pitch��Roll����ˮƽ�ٶȿ���
      Body_Frame_Speed_Feedback.Pit=-NamelessQuad.Speed[_PITCH]*Sin_Yaw+NamelessQuad.Speed[_ROLL]*Cos_Yaw;
      Body_Frame_Speed_Feedback.Rol=NamelessQuad.Speed[_PITCH]*Cos_Yaw+NamelessQuad.Speed[_ROLL]*Sin_Yaw;
    //�����巽���ٶȷ�����
      Total_Controler.Latitude_Speed_Control.FeedBack=Body_Frame_Speed_Feedback.Pit;//��ͷPitch����Y������
      Total_Controler.Longitude_Speed_Control.FeedBack=Body_Frame_Speed_Feedback.Rol;//���Roll����X������
    //�����巽���ٶȿ�����
      //PID_Control(&Total_Controler.Latitude_Speed_Control);
      //PID_Control(&Total_Controler.Longitude_Speed_Control);
      PID_Control_Err_LPF(&Total_Controler.Latitude_Speed_Control);
      PID_Control_Err_LPF(&Total_Controler.Longitude_Speed_Control);

      Total_Controler.Pitch_Angle_Control.Expect=-Total_Controler.Latitude_Speed_Control.Control_OutPut;
      Total_Controler.Roll_Angle_Control.Expect=Total_Controler.Longitude_Speed_Control.Control_OutPut;
 /*******************************ˮƽλ�ÿ���������***********************************************************/
    }
    else //ֻ����ˮƽ�ٶȿ��ƣ���ˮƽλ�ÿ���
   {
      //�����������1����������ϵ�ĺ����ٶȿ��ƣ�
     //             2����������ϵ�����ϵ��ٶȿ���
     if(GPS_Speed_Control_Mode==Angle_Mode)//�ƶ�����ˣ���Ӧ�����Ƕ�
     {
       Total_Controler.Pitch_Angle_Control.Expect=Target_Angle[0];
       Total_Controler.Roll_Angle_Control.Expect=Target_Angle[1];
     }
     else//�ƶ�����ˣ���Ӧ������������ϵ����Pitch,Roll�����˶��ٶ�
     {
      Total_Controler.Latitude_Speed_Control.Expect =-Target_Angle[0]*4.0;// ��������ٶ�50*3/100=1.5m/s
      Total_Controler.Longitude_Speed_Control.Expect=Target_Angle[1]*4.0;
       //����ϵ��ˮƽ�ٶȣ�ת������������ϵX-Y������
       //������Pitch��Roll����ˮƽ�ٶȿ���
      Body_Frame_Speed_Feedback.Pit=-NamelessQuad.Speed[_PITCH]*Sin_Yaw+NamelessQuad.Speed[_ROLL]*Cos_Yaw;
      Body_Frame_Speed_Feedback.Rol=NamelessQuad.Speed[_PITCH]*Cos_Yaw+NamelessQuad.Speed[_ROLL]*Sin_Yaw;

      Total_Controler.Latitude_Speed_Control.FeedBack=Body_Frame_Speed_Feedback.Pit;//��ͷPitch����Y������
      Total_Controler.Longitude_Speed_Control.FeedBack=Body_Frame_Speed_Feedback.Rol;//���Roll����X������

      //PID_Control(&Total_Controler.Latitude_Speed_Control);
      //PID_Control(&Total_Controler.Longitude_Speed_Control);
      PID_Control_Err_LPF(&Total_Controler.Latitude_Speed_Control);
      PID_Control_Err_LPF(&Total_Controler.Longitude_Speed_Control);

      Total_Controler.Pitch_Angle_Control.Expect=-Total_Controler.Latitude_Speed_Control.Control_OutPut;
      Total_Controler.Roll_Angle_Control.Expect=Total_Controler.Longitude_Speed_Control.Control_OutPut;
     }

      Total_Controler.Latitude_Position_Control.Expect=0;
      Total_Controler.Longitude_Position_Control.Expect=0;
   }
   }
  else//�����㶨����������������ˮƽ��̬
  {
/********��GPS����ģʽλ��0��ֱ�ӽ�����̬ģʽ���ȴ�GPS�ź��ٴ���������ʱ��***********
*********�Զ��л���GPS����ģʽ�����Controler_Mode_Select����������й���**********/
     Pos_Hold_SetFlag=0;//��0�������ص�λ��Ϊ����ģʽʱ��
                        //�ڿ���ģʽ�����Լ��Ƿ������ٴν���GPS����ģʽ
     Total_Controler.Pitch_Angle_Control.Expect=Target_Angle[0];
     Total_Controler.Roll_Angle_Control.Expect=Target_Angle[1];
  }
  }

}
/************��̬�����������Ƕ�+���ٶ�****************/
void Altitude_Control(void)
{
  Angle_Control();//�Ƕȿ���
  Gyro_Control();//���ٶȿ���
}


float Active_Para1,Active_Para2;
void Total_Control(void)
{
  static uint16_t Mode_Check_Cnt=0;
  /***********��PPM_ch6��ch7Ϊģ��ͨ��ʱ��������Ϊʵʱ���ߵ���ʹ��******/
  Active_Para1=PPM_Databuf[6]-1000;
  Active_Para2=PPM_Databuf[7]-1000;
  Active_Para1=0.1*constrain_float(Active_Para1-500,-500,500);
  //Active_Para2=0.0001*constrain_float(Active_Para2,0,1000);
  //Total_Controler.High_Speed_Control.Kp=Active_Para1;
  //Total_Controler.High_Speed_Control.Ki=Active_Para2;

  /*************������ģʽѡ��******************/
  Mode_Check_Cnt++;
  if(Mode_Check_Cnt>=4)//ÿ20ms���һ�Σ�PPM�ź�ˢ������Ϊ20ms
  {
    Controler_Mode_Select();
    Mode_Check_Cnt=0;
  }
  /*************����������******************/
  Main_Leading_Control();
  /*************��̬��������*****************/
  Altitude_Control();
}


uint16_t Landon_Earth_Cnt=0;
uint8_t Landon_Earth_Flag=0;
uint8_t Landon_Earth_Flag_Set=0;
void Landon_Earth_Check(void)//�Լ촥�ؽ��뵡��ģʽ
{
   if(ABS(NamelessQuad.Acceleration[_YAW])<=15//��ֱZ���˶����ٶȺ�С
       &&Throttle_Control==Thr_Min//���Ÿ˴�������г�λ
         &&Gyro_Length<=2.0//���غ�����ת
          &&ABS(NamelessQuad.Speed[_YAW])<=20.0)//�ٶȺ�С
     Landon_Earth_Cnt++;
   else Landon_Earth_Cnt/=2;
   if(Landon_Earth_Cnt>=10000)  Landon_Earth_Cnt=10000;//��ֹ���
   if(Landon_Earth_Cnt>=250*4)//����4S
   {
     Landon_Earth_Flag=1;//��½��־λ
   }

   if(ABS(Throttle_Rate)>=1.0E-5f)//ֻҪ���ű仯�ʲ�Ϊ0���������½��־λ
   {
     Landon_Earth_Flag=0;
     Landon_Earth_Cnt=0;
   }

}

uint16_t Throttle_Output=0;
void Throttle_Angle_Compensate()//������ǲ���
{
  float CosPitch_CosRoll=ABS(Cos_Pitch*Cos_Roll);
  float Throttle_Makeup=0;
  float Temp=0;
  if(CosPitch_CosRoll>=0.999999)  CosPitch_CosRoll=0.999999;
  if(CosPitch_CosRoll<=0.000001)  CosPitch_CosRoll=0.000001;
  if(CosPitch_CosRoll<=0.50)  CosPitch_CosRoll=0.50;//Pitch,RollԼ����30��
  if(Throttle>=Thr_Start)//������ת������
  {
      Temp=(uint16_t)(MAX(ABS(100*Pitch),ABS(100*Roll)));
      Temp=constrain_float(9000-Temp,0,3000)/(3000*CosPitch_CosRoll);
      Throttle_Makeup=(Throttle-Thr_Start)*Temp;//������ǲ���
      Throttle_Output=(uint16_t)(Thr_Start+Throttle_Makeup);
      Throttle_Output=(uint16_t)(constrain_float(Throttle_Output,Thr_Start,2000));
  }
  else Throttle_Output=Throttle;
}




void Control_Output()
{
Throttle_Angle_Compensate();//������ǲ���
//Landon_Earth_Check();//��½�����Լ�
if(Controler_State==Unlock_Controler)//����
{
  if(Landon_Earth_Flag==1)//��⵽��½����
  {
      Motor_PWM_1=Thr_Idle;//���ŵ���
      Motor_PWM_2=Thr_Idle;
      Motor_PWM_3=Thr_Idle;
      Motor_PWM_4=Thr_Idle;
  }
  else
  {
     if(Controler_High_Mode==1)//��̬��λ
     {
         if(Throttle>=Thr_Fly_Start)//�����������
         {
            if(GYRO_CONTROL_MODE==PID_MODE)//ˮƽ��̬�����ٶȶ���������Դ��PID
            {
              Motor_PWM_1=Int_Sort(Throttle_Output
                            -Total_Controler.Roll_Gyro_Control.Control_OutPut
                            -Total_Controler.Pitch_Gyro_Control.Control_OutPut
                            +Total_Controler.Yaw_Gyro_Control.Control_OutPut);
              Motor_PWM_2=Int_Sort(Throttle_Output
                            +Total_Controler.Roll_Gyro_Control.Control_OutPut
                            +Total_Controler.Pitch_Gyro_Control.Control_OutPut
                            +Total_Controler.Yaw_Gyro_Control.Control_OutPut);
              Motor_PWM_3=Int_Sort(Throttle_Output
                            +Total_Controler.Roll_Gyro_Control.Control_OutPut
                            -Total_Controler.Pitch_Gyro_Control.Control_OutPut
                            -Total_Controler.Yaw_Gyro_Control.Control_OutPut);
              Motor_PWM_4=Int_Sort(Throttle_Output
                            -Total_Controler.Roll_Gyro_Control.Control_OutPut
                            +Total_Controler.Pitch_Gyro_Control.Control_OutPut
                            -Total_Controler.Yaw_Gyro_Control.Control_OutPut);
            }
            else//ˮƽ��̬�����ٶȶ���������Դ��ADRC
            {


            }
         }
         else//С���������
         {
               Motor_PWM_1=Int_Sort(Throttle_Output);
               Motor_PWM_2=Int_Sort(Throttle_Output);
               Motor_PWM_3=Int_Sort(Throttle_Output);
               Motor_PWM_4=Int_Sort(Throttle_Output);
               Take_Off_Reset();//�����
         }
     }
     else if(Controler_High_Mode==2)//�����йܡ����ߵ�λ
     {
            if(GYRO_CONTROL_MODE==PID_MODE)//ˮƽ��̬�����ٶȶ���������Դ��PID
            {
              Motor_PWM_1=Int_Sort(Throttle_Output
                            -Total_Controler.Roll_Gyro_Control.Control_OutPut
                            -Total_Controler.Pitch_Gyro_Control.Control_OutPut
                            +Total_Controler.Yaw_Gyro_Control.Control_OutPut);
              Motor_PWM_2=Int_Sort(Throttle_Output
                            +Total_Controler.Roll_Gyro_Control.Control_OutPut
                            +Total_Controler.Pitch_Gyro_Control.Control_OutPut
                            +Total_Controler.Yaw_Gyro_Control.Control_OutPut);
              Motor_PWM_3=Int_Sort(Throttle_Output
                            +Total_Controler.Roll_Gyro_Control.Control_OutPut
                            -Total_Controler.Pitch_Gyro_Control.Control_OutPut
                            -Total_Controler.Yaw_Gyro_Control.Control_OutPut);
              Motor_PWM_4=Int_Sort(Throttle_Output
                            -Total_Controler.Roll_Gyro_Control.Control_OutPut
                            +Total_Controler.Pitch_Gyro_Control.Control_OutPut
                            -Total_Controler.Yaw_Gyro_Control.Control_OutPut);
            }
            else//ˮƽ��̬�����ٶȶ���������Դ��ADRC
            {



            }
     }
  }
}
else//δ�����������������λ
{
  Motor_PWM_1=Thr_Min;
  Motor_PWM_2=Thr_Min;
  Motor_PWM_3=Thr_Min;
  Motor_PWM_4=Thr_Min;
}
  Motor_PWM_1=Value_Limit(0,2000,Motor_PWM_1);//������޷�
  Motor_PWM_2=Value_Limit(0,2000,Motor_PWM_2);
  Motor_PWM_3=Value_Limit(0,2000,Motor_PWM_3);
  Motor_PWM_4=Value_Limit(0,2000,Motor_PWM_4);
  if(MotorTest==0x00)
  {
    PWM_Set(1000,1000,1000,1000);
  }
  else
  {
    PWM_Set((0x01&MotorTest) ? Motor_PWM_1:0,
            (0x02&MotorTest) ? Motor_PWM_2:0,
            (0x04&MotorTest) ? Motor_PWM_3:0,
            (0x08&MotorTest) ? Motor_PWM_4:0);
  }
}




