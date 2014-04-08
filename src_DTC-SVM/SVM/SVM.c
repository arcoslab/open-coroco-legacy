/*
 *This file is part of the open-coroco project.
 *
 *  Copyright (C) 2013  Sebastian Chinchilla Gutierrez <tumacher@gmail.com>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */



float SVM_V_s_ref_D(float psi_s_ref, float psi_s, float psi_s_angle, float phase_advance,float i_sD, float R_s,float T_s)
{
  //return ( psi_s_ref*cosf((psi_s_angle+phase_advance)*PI/180.0f) - psi_s*cosf(psi_s_angle*PI/180.0f) )/T_s  +  i_sD*R_s;
  //return ( psi_s_ref*cosf((psi_s_angle+phase_advance)*0.01745329251994329576f) - psi_s*cosf(psi_s_angle*0.01745329251994329576f) )/T_s  +  i_sD*R_s;
  ////return ( psi_s_ref*fast_cos((psi_s_angle+phase_advance)*0.01745329251994329576f) - psi_s*fast_cos(psi_s_angle*0.01745329251994329576f) )/T_s  +  i_sD*R_s;

  //catching_NaNs();
  //####catching_NaNs_data (psi_s_angle+phase_advance, fast_cos((psi_s_angle+phase_advance)), fast_cos(psi_s_angle));
  return ( psi_s_ref*fast_cos((psi_s_angle+phase_advance)) - psi_s*fast_cos(psi_s_angle) )/T_s  +  i_sD*R_s;
}

float SVM_V_s_ref_Q(float psi_s_ref, float psi_s, float psi_s_angle, float phase_advance,float i_sQ, float R_s,float T_s)
{
  //return ( psi_s_ref*sinf((psi_s_angle+phase_advance)*PI/180.0f) - psi_s*sinf(psi_s_angle*PI/180.0f) )/T_s  +  i_sQ*R_s; 
  //return ( psi_s_ref*sinf((psi_s_angle+phase_advance)*0.01745329251994329576f) - psi_s*sinf(psi_s_angle*0.01745329251994329576f) )/T_s  +  i_sQ*R_s; 
  //return ( psi_s_ref*fast_sine((psi_s_angle+phase_advance)*0.01745329251994329576f) - psi_s*fast_sine(psi_s_angle*0.01745329251994329576f) )/T_s  +  i_sQ*R_s;

  //catching_NaNs();
  //##catching_NaNs_data (psi_s_angle+phase_advance, fast_sine((psi_s_angle+phase_advance)), fast_sine(psi_s_angle));

  return ( psi_s_ref*fast_sine((psi_s_angle+phase_advance)) - psi_s*fast_sine(psi_s_angle) )/T_s  +  i_sQ*R_s;
}

void SVM_Maximum_allowed_V_s_ref(float* VsD, float* VsQ,float* V_s_ref,float U_d,bool initial_rotor_start)
{
  //if   (*V_s_ref<=U_d/sqrtf(3.0f)) { *V_s_ref = *V_s_ref;       }
  //else                            { *V_s_ref = U_d/sqrtf(3.0f); }

  float voltage_magnitude = *V_s_ref; 
/*
  if (initial_rotor_start==true)
                                              {
                                                    *V_s_ref = *V_s_ref;
                                                    *VsD     = *VsD;
                                                    *VsQ     = *VsQ;  
                                              }
  else*/ if   (*V_s_ref<=U_d/1.73205080756887729352f) { 
                                                    *V_s_ref = *V_s_ref;
                                                    *VsD     = *VsD;
                                                    *VsQ     = *VsQ;
                                               }

  else                                         { 
                                                  //shrinking quadrature and direct components in order to fit under the maximum circle
                                                    *VsD     = *VsD * (U_d/1.73205080756887729352f) / voltage_magnitude;//(*V_s_ref); 
                                                    *VsQ     = *VsQ * (U_d/1.73205080756887729352f) / voltage_magnitude;//(*V_s_ref);
                                                  //adjusting the voltage magnitude to the value of the radious of the circule
                                                     *V_s_ref = U_d/1.73205080756887729352f;  
                                               }


}

float SVM_V_s_relative_angle(float V_s_ref_angle)
{
  float V_s_relative_angle=0.0f;
  
  if      ( V_s_ref_angle>=  0.0f && V_s_ref_angle< 60.0f) { V_s_relative_angle=V_s_ref_angle-  0.0f; }  //sector S1
  else if ( V_s_ref_angle>= 60.0f && V_s_ref_angle<120.0f) { V_s_relative_angle=V_s_ref_angle- 60.0f; }  //sector S2
  else if ( V_s_ref_angle>=120.0f && V_s_ref_angle<180.0f) { V_s_relative_angle=V_s_ref_angle-120.0f; }  //sector S3
  else if ( V_s_ref_angle>=180.0f && V_s_ref_angle<240.0f) { V_s_relative_angle=V_s_ref_angle-180.0f; }  //sector S4
  else if ( V_s_ref_angle>=240.0f && V_s_ref_angle<300.0f) { V_s_relative_angle=V_s_ref_angle-240.0f; }  //sector S5
  else if ( V_s_ref_angle>=300.0f && V_s_ref_angle<360.0f) { V_s_relative_angle=V_s_ref_angle-300.0f; }  //sector S6
  else                                                     { V_s_relative_angle=360.0;                }  //failed sector
  
  return V_s_relative_angle;
}

float SVM_T1(float T_s,float U_s, float U1, float V_s_ref_relative_angle)
{
  //return T_s*(U_s/U1)*( sinf( (PI/180.0f)*(60.0f-V_s_ref_relative_angle) )/sinf(60.0f*PI/180.0f) );
  //return T_s*(U_s/U1)*( sinf     ( 0.01745329251994329576f*(60.0f-V_s_ref_relative_angle) )/0.86602540378443864676f );
  //return T_s*(U_s/U1)*( fast_sine( 0.01745329251994329576f*(60.0f-V_s_ref_relative_angle) )/0.86602540378443864676f );
  return T_s*(U_s/U1)*( fast_sine( (60.0f-V_s_ref_relative_angle) )/0.86602540378443864676f );
}

float SVM_T2(float T_s,float U_s, float U2, float V_s_ref_relative_angle)
{
  //return T_s*(U_s/U2)*( sinf(V_s_ref_relative_angle*PI/180.0f)/sinf(60.0f*PI/180.0f) );
  //return T_s*(U_s/U2)*( sinf(V_s_ref_relative_angle*0.01745329251994329576f)/0.86602540378443864676f );
  //return T_s*(U_s/U2)*( fast_sine(V_s_ref_relative_angle*0.01745329251994329576f)/0.86602540378443864676f );
  return T_s*(U_s/U2)*( fast_sine(V_s_ref_relative_angle)/0.86602540378443864676f );
}
/*
float SVM_Taon(float T_s, float T1, float T2)
{
  return (T_s-T1-T2)/2.0f;
}
*/
float SVM_T_min_on(float T_s, float T1, float T2)
{
  return (T_s-T1-T2)/2.0f;
}
/*
float SVM_Tbon(float Taon, float T1)
{
  return (Taon+T1);
}
*/
float SVM_T_med_on(float T_min_on, float T1,float T2,float V_s_ref_angle)//T1)
{
  if      ( V_s_ref_angle>=  0.0f && V_s_ref_angle< 60.0f) { return (T_min_on+T2); }  //sector S1
  else if ( V_s_ref_angle>= 60.0f && V_s_ref_angle<120.0f) { return (T_min_on+T1); }  //sector S2
  else if ( V_s_ref_angle>=120.0f && V_s_ref_angle<180.0f) { return (T_min_on+T2); }  //sector S3
  else if ( V_s_ref_angle>=180.0f && V_s_ref_angle<240.0f) { return (T_min_on+T1); }  //sector S4
  else if ( V_s_ref_angle>=240.0f && V_s_ref_angle<300.0f) { return (T_min_on+T2); }  //sector S5
  else if ( V_s_ref_angle>=300.0f && V_s_ref_angle<360.0f) { return (T_min_on+T1); }  //sector S6
  else                                                     { return (T_min_on   ); }  //failed sector
}
/*
float SVM_Tcon(float Tbon, float T2)
{
  return (Tbon+T2);
}
*/
float SVM_T_max_on(float T_med_on, float T1,float T2,float V_s_ref_angle)//T2)
{
  if      ( V_s_ref_angle>=  0.0f && V_s_ref_angle< 60.0f) { return (T_med_on+T1); }  //sector S1
  else if ( V_s_ref_angle>= 60.0f && V_s_ref_angle<120.0f) { return (T_med_on+T2); }  //sector S2
  else if ( V_s_ref_angle>=120.0f && V_s_ref_angle<180.0f) { return (T_med_on+T1); }  //sector S3
  else if ( V_s_ref_angle>=180.0f && V_s_ref_angle<240.0f) { return (T_med_on+T2); }  //sector S4
  else if ( V_s_ref_angle>=240.0f && V_s_ref_angle<300.0f) { return (T_med_on+T1); }  //sector S5
  else if ( V_s_ref_angle>=300.0f && V_s_ref_angle<360.0f) { return (T_med_on+T2); }  //sector S6
  else                                                     { return (T_med_on   ); }  //failed sector
}


void SVM_phase_duty_cycles(float *duty_A, float *duty_B, float *duty_C,float V_s_ref_angle,float T_max_on, float T_med_on,float T_min_on)//float Taon, float Tbon,float Tcon)
{
  //sector S1
  if      ( V_s_ref_angle>=  0.0f && V_s_ref_angle< 60.0f) {
                                                             *duty_A=T_max_on;//Tbon;     
                                                             *duty_B=T_med_on;//Taon;
                                                             *duty_C=T_min_on;//Tcon;
                                                           }
  //sector S2
  else if ( V_s_ref_angle>= 60.0f && V_s_ref_angle<120.0f) {
                                                             *duty_A=T_med_on;//Taon;     
                                                             *duty_B=T_max_on;//Tcon;
                                                             *duty_C=T_min_on;//Tbon;
                                                           }
  //sector S3
  else if ( V_s_ref_angle>=120.0f && V_s_ref_angle<180.0f) {
                                                             *duty_A=T_min_on;//Taon;     
                                                             *duty_B=T_max_on;//Tbon;
                                                             *duty_C=T_med_on;//Tcon;
                                                           }
  //sector S4
  else if ( V_s_ref_angle>=180.0f && V_s_ref_angle<240.0f) {
                                                             *duty_A=T_min_on;//Tcon;     
                                                             *duty_B=T_med_on;//Tbon;
                                                             *duty_C=T_max_on;//aon;
                                                           }
  //sector S5
  else if ( V_s_ref_angle>=240.0f && V_s_ref_angle<300.0f) {
                                                             *duty_A=T_med_on;//Tbon;     
                                                             *duty_B=T_min_on;//Tcon;
                                                             *duty_C=T_max_on;//Taon;
                                                           }
  //sector S6
  else if ( V_s_ref_angle>= 300.0f && V_s_ref_angle<360.0f) {
                                                             *duty_A=T_max_on;//Tcon;     
                                                             *duty_B=T_min_on;//Taon;
                                                             *duty_C=T_med_on;//Tbon;
                                                           }
  //failed sector
  else                                                     {
                                                             *duty_A=0.0f;     
                                                             *duty_B=0.0f;
                                                             *duty_C=0.0f;
                                                           }
}

/*
void speed_PID_no_SVM(void)
{
  close_loop=true;
  cur_angle+=2.0f*PI*TICK_PERIOD*ref_freq;

  //converting big angles into something between 0 and 2pi
  if (cur_angle >= (2.0f*PI)) 
  {
    cur_angle=cur_angle-(2.0f*PI);
  }

  if (!close_loop) 
  {
    duty_a=sinf(cur_angle);
    duty_b=sinf(cur_angle+2.0f*PI/3.0f);
    duty_c=sinf(cur_angle+4.0f*PI/3.0f);
  } 

  if (motor_off) 
  {
    duty_a=0.0f;
    duty_b=0.0f;
    duty_c=0.0f;
    attenuation=1.0f;
  }
}
*/


void  SVM_voltage_switch_inverter_VSI(float duty_A,float duty_B,float duty_C,bool shutdown)
{
  //DTC-SVM switching selection
  float Attenuation;

  if (shutdown==false)
  {
    Attenuation=1.0f;

    //-------------SA: S1 and S4------------------------------------
    timer_set_oc_mode      (TIM1, TIM_OC1, TIM_OCM_PWM1);
    timer_enable_oc_output (TIM1, TIM_OC1 );  //S1
    timer_enable_oc_output (TIM1, TIM_OC1N);  //S4}

    //-------------SB: S3 and S6------------------------------------
    timer_set_oc_mode      (TIM1, TIM_OC2, TIM_OCM_PWM1);
    timer_enable_oc_output (TIM1, TIM_OC2 );  //S3
    timer_enable_oc_output (TIM1, TIM_OC2N);  //S6


    //-------------SC: S5 and S2-------------------------------------
    timer_set_oc_mode(TIM1, TIM_OC3, TIM_OCM_PWM1);
    timer_enable_oc_output (TIM1, TIM_OC3 );  //S5 on
    timer_enable_oc_output (TIM1, TIM_OC3N);  //S2 off
  }  

  else
  {
    Attenuation=0.0f;
    duty_A=0.0f;
    duty_B=0.0f;
    duty_C=0.0f;

    //-------------SA: S1 and S4------------------------------------
    timer_set_oc_mode      (TIM1, TIM_OC1, TIM_OCM_PWM1);
    timer_disable_oc_output (TIM1, TIM_OC1 );  //S1
    timer_disable_oc_output (TIM1, TIM_OC1N);  //S4}

    //-------------SB: S3 and S6------------------------------------
    timer_set_oc_mode      (TIM1, TIM_OC2, TIM_OCM_PWM1);
    timer_disable_oc_output (TIM1, TIM_OC2 );  //S3
    timer_disable_oc_output (TIM1, TIM_OC2N);  //S6


    //-------------SC: S5 and S2-------------------------------------
    timer_set_oc_mode(TIM1, TIM_OC3, TIM_OCM_PWM1);
    timer_disable_oc_output (TIM1, TIM_OC3 );  //S5 on
    timer_disable_oc_output (TIM1, TIM_OC3N);  //S2 off
  }  

  //Set the capture compare value for OC1.
  timer_set_oc_value(TIM1, TIM_OC1, duty_A*Attenuation*PWM_PERIOD_ARR);
  //Set the capture compare value for OC1.
  timer_set_oc_value(TIM1, TIM_OC2, duty_B*Attenuation*PWM_PERIOD_ARR);
  //Set the capture compare value for OC1.
  timer_set_oc_value(TIM1, TIM_OC3, duty_C*Attenuation*PWM_PERIOD_ARR);
}


int S_A=0;
int S_B=0;
int S_C=0;

float S_A_f=0;
float S_B_f=0;
float S_C_f=0;

float i_sA = 0.0f;
float i_sB = 0.0f;
float U_d  = 0.0f;

float i_sD=0.0f;
float i_sQ=0.0f;
float i_s=0.0f;
float cita_i_s=0.0f;

float V_sD=0.0f;
float V_sQ=0.0f;
float V_s =0.0f;
float cita_V_s=0.0f;

float psi_sD=0.0f;
float psi_sQ=0.0f;
float psi_s =0.0f;
int psi_alpha=0;
float psi_s_alpha_SVM=0.0f;

//float w_r=0;
float t_e=0.0f;

float psi_s_ref=0.0f;
//float t_e_ref=10.0f;

int   d_psi=0.0f;
int   d_te=0.0f;
float psi_delta_percentage=5.0f;
float t_e_delta_percentage=25.0f;//0.8f;

//SVM variables

float T1=0.0f;
float T2=0.0f;
float T_min_on=0.0f;
float T_med_on=0.0f;
float T_max_on=0.0f;
float V_s_ref_relative_angle=0.0f;

//motor parameters;
float R_s        = R_s_0;
float pole_pairs = pole_pairs_0;
float L_sq       = L_s_q_0;
float psi_F      = psi_F_0;

#define MINIMUM_SVM_FREQUENCY 0.5f
void shutdown_SVM_speed (float reference_frequency,float actual_frequency,bool* shutdown)
{
    if      ( *shutdown           == false && 
              reference_frequency == 0.0f  && 
              actual_frequency>-MINIMUM_SVM_FREQUENCY && 
              actual_frequency<MINIMUM_SVM_FREQUENCY) 
    { *shutdown = true ;}
    
    else if ( *shutdown           == true  &&  
              reference_frequency == 0.0f)                                                                                      
    { *shutdown = true ;}
 
    else                                                                                                                         
    { *shutdown = false;}
}
#define MINIMUM_SVM_TORQUE 0.01f
void shutdown_SVM_torque (float torque_reference,float actual_torque,bool* shutdown)
{
    if      ( *shutdown           == false && 
              torque_reference == 0.0f  && 
              actual_torque>-MINIMUM_SVM_TORQUE && 
              actual_torque<MINIMUM_SVM_TORQUE) 
    { *shutdown = true ;}
    
    else if ( *shutdown           == true  &&  
              torque_reference == 0.0f)                                                                                      
    { *shutdown = true ;}
 
    else                                                                                                                         
    { *shutdown = false;}
}


#define FIRST_HALF  0
#define SECOND_HALF 1
float center_aligned_state=FIRST_HALF;


char catched_wr         = '0';
char catched_VsD        = '0';
char catched_VsQ        = '0';
char catched_psisD      = '0';
char catched_psisQ      = '0';
char catched_te         = '0';
char catched_pi_control = '0';
bool first_catch        = true;
bool something_catched  = false;

char catched_psi_s_ref              ='o';
char catched_psi_s                  ='o';
char catched_psi_s_alpha_SVM        ='o';
char catched_psi_rotating_angle_SVM ='o';
char catched_i_sD                   ='o';
char catched_i_sQ                   ='o';
char catched_R_s                    ='o';
char catched_tick_period            ='o';

float catched_value_wr         = 0.0f;
float catched_value_VsD        = 0.0f;
float catched_value_VsQ        = 0.0f;
float catched_value_psisD      = 0.0f;
float catched_value_psisQ      = 0.0f;
float catched_value_te         = 0.0f;
float catched_value_pi_control = 0.0f;


float catched_value_psi_s_ref              =0.0f;
float catched_value_psi_s                  =0.0f;
float catched_value_psi_s_alpha_SVM        =0.0f;
float catched_value_psi_rotating_angle_SVM =0.0f;
float catched_value_i_sD                   =0.0f;
float catched_value_i_sQ                   =0.0f;
float catched_value_R_s                    =0.0f;
float catched_value_tick_period            =0.0f;

float catched_data_4                       =0.0f;
float catched_data_5                       =0.0f;
float catched_data_6                       =0.0f;

//

void catched_values (void)
{
    catched_value_wr         = w_r;
    catched_value_VsD        = V_sD;
    catched_value_VsQ        = V_sQ;
    catched_value_psisD      = psi_sD;
    catched_value_psisQ      = psi_sQ;
    catched_value_te         = t_e;
    catched_value_pi_control = pi_control;


    catched_value_psi_s_ref              =psi_s_ref;
    catched_value_psi_s                  =psi_s;
    catched_value_psi_s_alpha_SVM        =psi_s_alpha_SVM;
    catched_value_psi_rotating_angle_SVM =psi_rotating_angle_SVM;
    catched_value_i_sD                   =i_sD;
    catched_value_i_sQ                   =i_sQ;
    catched_value_R_s                    =R_s;
    catched_value_tick_period            =TICK_PERIOD*2.0f;
}

void catched_values_data (float data_4, float data_5, float data_6)
{
    catched_value_wr         = w_r;
    catched_value_VsD        = V_sD;
    catched_value_VsQ        = V_sQ;
    catched_value_psisD      = psi_sD;
    catched_value_psisQ      = psi_sQ;
    catched_value_te         = t_e;
    catched_value_pi_control = pi_control;


    catched_value_psi_s_ref              =psi_s_ref;
    catched_value_psi_s                  =psi_s;
    catched_value_psi_s_alpha_SVM        =psi_s_alpha_SVM;
    catched_value_psi_rotating_angle_SVM =psi_rotating_angle_SVM;
    catched_value_i_sD                   =i_sD;
    catched_value_i_sQ                   =i_sQ;
    catched_value_R_s                    =R_s;
    catched_value_tick_period            =TICK_PERIOD*2.0f;
    
    catched_data_4  = data_4;
    catched_data_5  = data_5;
    catched_data_6  = data_6;
    
}

void catching_NaNs (void)
{

    if (w_r            !=w_r           && first_catch==true)  { catched_wr         ='f'; something_catched=true;}
    if (V_sD           !=V_sD          && first_catch==true)  { catched_VsD        ='D'; something_catched=true;}
    if (V_sQ           !=V_sQ          && first_catch==true)  { catched_VsQ        ='Q'; something_catched=true;}
    if (psi_sD         !=psi_sD        && first_catch==true)  { catched_psisD      ='d'; something_catched=true;}
    if (psi_sQ         !=psi_sQ        && first_catch==true)  { catched_psisQ      ='q'; something_catched=true;}
    if (t_e            !=t_e           && first_catch==true)  { catched_te         ='t'; something_catched=true;}
    if (SVM_pi_control !=SVM_pi_control&& first_catch==true)  { catched_pi_control ='i'; something_catched=true;}

    if(psi_s_ref             !=psi_s_ref             &&first_catch==true){catched_psi_s_ref              ='r'; something_catched=true;}
    if(psi_s                 !=psi_s                 &&first_catch==true){catched_psi_s                  ='s'; something_catched=true;}
    if(psi_s_alpha_SVM       !=psi_s_alpha_SVM       &&first_catch==true){catched_psi_s_alpha_SVM        ='a'; something_catched=true;}
    if(psi_rotating_angle_SVM!=psi_rotating_angle_SVM&&first_catch==true){catched_psi_rotating_angle_SVM ='R'; something_catched=true;}
    if (i_sD                 !=i_sD                  &&first_catch==true){catched_i_sD                   ='D'; something_catched=true;}
    if (i_sQ                 !=i_sQ                  &&first_catch==true){catched_i_sQ                   ='Q'; something_catched=true;}
    if (R_s                  !=R_s                   &&first_catch==true){catched_R_s                    ='r'; something_catched=true;}
    if (TICK_PERIOD          !=TICK_PERIOD           &&first_catch==true){catched_tick_period            ='T'; something_catched=true;}
 
    if (something_catched==true && first_catch==true)    { catched_values ();}
   
    if (something_catched==true)    {first_catch=false; }

}

void catching_NaNs_data (float data_4, float data_5, float data_6)
{

    if (w_r            !=w_r           && first_catch==true)  { catched_wr         ='f'; something_catched=true;}
    if (V_sD           !=V_sD          && first_catch==true)  { catched_VsD        ='D'; something_catched=true;}
    if (V_sQ           !=V_sQ          && first_catch==true)  { catched_VsQ        ='Q'; something_catched=true;}
    if (psi_sD         !=psi_sD        && first_catch==true)  { catched_psisD      ='d'; something_catched=true;}
    if (psi_sQ         !=psi_sQ        && first_catch==true)  { catched_psisQ      ='q'; something_catched=true;}
    if (t_e            !=t_e           && first_catch==true)  { catched_te         ='t'; something_catched=true;}
    if (SVM_pi_control !=SVM_pi_control&& first_catch==true)  { catched_pi_control ='i'; something_catched=true;}

    if(psi_s_ref             !=psi_s_ref             &&first_catch==true){catched_psi_s_ref              ='r'; something_catched=true;}
    if(psi_s                 !=psi_s                 &&first_catch==true){catched_psi_s                  ='s'; something_catched=true;}
    if(psi_s_alpha_SVM       !=psi_s_alpha_SVM       &&first_catch==true){catched_psi_s_alpha_SVM        ='a'; something_catched=true;}
    if(psi_rotating_angle_SVM!=psi_rotating_angle_SVM&&first_catch==true){catched_psi_rotating_angle_SVM ='R'; something_catched=true;}
    if (i_sD                 !=i_sD                  &&first_catch==true){catched_i_sD                   ='D'; something_catched=true;}
    if (i_sQ                 !=i_sQ                  &&first_catch==true){catched_i_sQ                   ='Q'; something_catched=true;}
    if (R_s                  !=R_s                   &&first_catch==true){catched_R_s                    ='r'; something_catched=true;}
    if (TICK_PERIOD          !=TICK_PERIOD           &&first_catch==true){catched_tick_period            ='T'; something_catched=true;}
 
    if (data_4  !=data_4    &&first_catch==true){catched_data_4 ='1'; something_catched=true;}
    if (data_5  !=data_5    &&first_catch==true){catched_data_5 ='2'; something_catched=true;}
    if (data_6  !=data_6    &&first_catch==true){catched_data_6 ='3'; something_catched=true;}
 

    if (something_catched==true && first_catch==true)    { catched_values_data (data_4,data_5,data_6);}
   
    if (something_catched==true)    {first_catch=false; }

}

#define ROTOR_POSITION_UNKNOWN      0
#define VOLTAGE_PULSE_REQUESTED     1
#define WAITING_FOR_PULSE_TO_END    2
#define END_OF_PULSE                3
void initial_rotor_position_voltage(float *VsD,float *VsQ,float *Vs, float *cita_Vs,
                                    float initial_stator_voltage, float initial_rotor_angle, 
                                    bool *initial_rotor_position_ignition, int maximum_pulse_ticks,bool shutdown_motor)
{

    static int initial_rotor_position_state = ROTOR_POSITION_UNKNOWN;
    static int pulse_tick_counter=0;

    if (initial_rotor_position_state== ROTOR_POSITION_UNKNOWN &&
        *initial_rotor_position_ignition==false)
    {

       *Vs=*Vs;
       *cita_Vs=*cita_Vs;
       *VsD=*VsD;
       *VsQ=*VsQ;
/*
       *Vs=0.0f;
       *cita_Vs=0.0f;
       *VsD=0.0f;
       *VsQ=0.0f;
*/


       pulse_tick_counter=0;
       initial_rotor_position_state=ROTOR_POSITION_UNKNOWN;
       *initial_rotor_position_ignition=false;
    }

    else if (initial_rotor_position_state== ROTOR_POSITION_UNKNOWN &&
            *initial_rotor_position_ignition==true && shutdown_motor ==false)
    {

       *Vs=*Vs;
       *cita_Vs=*cita_Vs;
       *VsD=*VsD;
       *VsQ=*VsQ;

/*
       *Vs=0.0f;
       *cita_Vs=0.0f;
       *VsD=0.0f;
       *VsQ=0.0f;
*/

       pulse_tick_counter=0;
       initial_rotor_position_state=ROTOR_POSITION_UNKNOWN;
       *initial_rotor_position_ignition=false;
    }


    else if (initial_rotor_position_state== ROTOR_POSITION_UNKNOWN &&
            *initial_rotor_position_ignition==true && shutdown_motor ==true)
    {
       *Vs    =initial_stator_voltage;
       *cita_Vs=initial_rotor_angle;
       *VsD=initial_stator_voltage;
       *VsQ=0.0f;
       pulse_tick_counter+=1;
       initial_rotor_position_state=WAITING_FOR_PULSE_TO_END;
       *initial_rotor_position_ignition=true;
    }

    else if (initial_rotor_position_state== WAITING_FOR_PULSE_TO_END &&
             pulse_tick_counter<maximum_pulse_ticks)
    {
       *Vs    =initial_stator_voltage;
       *cita_Vs=initial_rotor_angle;
       *VsD=initial_stator_voltage;
       *VsQ=0.0f;
       pulse_tick_counter+=1;
       initial_rotor_position_state=WAITING_FOR_PULSE_TO_END;
       *initial_rotor_position_ignition=true;
    }

    else if (initial_rotor_position_state== WAITING_FOR_PULSE_TO_END &&
             pulse_tick_counter>=maximum_pulse_ticks)
    {
       *Vs    =initial_stator_voltage;
       *cita_Vs=initial_rotor_angle;
       *VsD=initial_stator_voltage;
       *VsQ=0.0f;
       pulse_tick_counter=0;
       initial_rotor_position_state=ROTOR_POSITION_UNKNOWN;
       *initial_rotor_position_ignition=false;
    }
    
    else
    {

       *Vs=*Vs;
       *cita_Vs=*cita_Vs;
       *VsD=*VsD;
       *VsQ=*VsQ;
/*
       *Vs=0.0f;
       *cita_Vs=0.0f;
       *VsD=0.0f;
       *VsQ=0.0f;
*/
       pulse_tick_counter=0;
       initial_rotor_position_state=ROTOR_POSITION_UNKNOWN;
       *initial_rotor_position_ignition=false;
    }


}

void  DTC_SVM(void)
{
  //---------------------------------DTC algorithm--------------------------------------------//
/*
    if      (w_r            !=w_r      )  { catched_wr         ='f'; }
    else if (V_sD           !=V_sD          )  { catched_VsD        ='D'; }
    else if (V_sQ           !=V_sQ          )  { catched_VsQ        ='Q'; }
    else if (psi_sD         !=psi_sD        )  { catched_psisD      ='d'; }
    else if (psi_sQ         !=psi_sQ        )  { catched_psisQ      ='q'; }
    else if (t_e            !=t_e           )  { catched_te         ='t'; }
    else if (SVM_pi_control !=SVM_pi_control)  { catched_pi_control ='i'; }
*/

//catching_NaNs();

if (center_aligned_state==FIRST_HALF)
{
  i_sD     = direct_stator_current_i_sD     (i_sA);
  i_sQ     = quadrature_stator_current_i_sQ (i_sA,i_sB);
  //i_s      = vector_magnitude               (i_sQ,i_sD);
  //cita_i_s = vector_angle                   (i_sQ,i_sD);

  psi_sD          = direct_stator_flux_linkage_estimator_psi_sD     (2.0f*TICK_PERIOD,V_sD,i_sD,R_s);
  psi_sQ          = quadrature_stator_flux_linkage_estimator_psi_sQ (2.0f*TICK_PERIOD,V_sQ,i_sQ,R_s);


  psi_s           = stator_flux_linkage_magnite_psi_s               (psi_sD,psi_sQ);
  ////psi_s_alpha_SVM = vector_angle                                    (psi_sQ,psi_sD);
  psi_s_alpha_SVM = fast_vector_angle                               (psi_sQ,psi_sD);

  //w_r             = (1.0f/(2.0f*PI))*rotor_speed_w_r                                 (psi_sD,psi_sQ,TICK_PERIOD);
  //w_r             = 0.15915494309189533576f*rotor_speed_w_r                                 (psi_sD,psi_sQ,TICK_PERIOD);
  w_r             = 0.15915494309189533576f*rotor_speed_w_r                                 (psi_sD,psi_sQ,TICK_PERIOD*2.0f);  //it has to be multiplied by two in order because the switching frequency is half the pwm frequency due to the two-cycle center-aligned signal
  
  t_e       = electromagnetic_torque_estimation_t_e   (psi_sD,i_sQ,psi_sQ,i_sD,pole_pairs);
  //t_e_ref = DTC_torque_reference_PI                 (CUR_FREQ, ref_freq);
  //psi_s_ref = stator_flux_linkage_reference_psi_s_ref (psi_F,t_e_ref,L_sq,pole_pairs);
  psi_s_ref = psi_F;


  //--------------------------------SVM algorithm--------------------------------------------//
/*
  if (pi_mode==0)   {   sensorless_speed_pi_controller  (ref_freq_SVM   ,w_r, PWMFREQ_F         ,&psi_rotating_angle_SVM    );     }
  else              {   sensorless_torque_pi_controller (t_e_ref        ,t_e, TICK_PERIOD*2.0f  ,&psi_rotating_angle_SVM    );  } 
*/
  //sensorless_torque_pi_controller (t_e_ref        ,t_e, TICK_PERIOD*2.0f  ,&psi_rotating_angle_SVM    );
  sensorless_speed_pi_controller  (ref_freq_SVM   ,w_r, PWMFREQ_F         ,&psi_rotating_angle_SVM    );
  //sensorless_torque_pi_controller_from_speed(t_e_ref, t_e,TICK_PERIOD*2.0f, &psi_rotating_angle_SVM,w_r,&ref_freq_SVM);
  //sensorless_speed_pi_controller            (ref_freq_SVM   ,w_r, PWMFREQ_F         ,&psi_rotating_angle_SVM    );


  V_sD                   = 
    SVM_V_s_ref_D (psi_s_ref,psi_s,psi_s_alpha_SVM,psi_rotating_angle_SVM,i_sD,R_s,2.0f*TICK_PERIOD);
  V_sQ                   = 
    SVM_V_s_ref_Q (psi_s_ref,psi_s,psi_s_alpha_SVM,psi_rotating_angle_SVM,i_sQ,R_s,2.0f*TICK_PERIOD);

gpio_clear(GPIOD, GPIO9);
} 




else
{
    static bool shutdown=true; 

  gpio_set(GPIOD, GPIO9);



  V_s                    = vector_magnitude            (V_sQ,V_sD);  
  ////cita_V_s               = vector_angle                (V_sQ,V_sD);
  cita_V_s               = fast_vector_angle                (V_sQ,V_sD);
             //SVM_Maximum_allowed_V_s_ref (&V_s,U_d);   //maximum U_d



  initial_rotor_position_voltage(&V_sD,&V_sQ,&V_s,&cita_V_s,U_d,0.0f,
                                 &initial_rotor_position_start,10000,shutdown);
  SVM_Maximum_allowed_V_s_ref (&V_sD,&V_sQ,&V_s,U_d*1.0f,initial_rotor_position_start);//0.70f);

  V_s_ref_relative_angle = SVM_V_s_relative_angle      (cita_V_s);



  //T1       = SVM_T1       (1.0f,V_s,U_d*2.0f/3.0f, V_s_ref_relative_angle);
  T1       = SVM_T1       (1.0f,V_s,U_d*0.66666666666666666666f, V_s_ref_relative_angle);
  //T2       = SVM_T2       (1.0f,V_s,U_d*2.0f/3.0f, V_s_ref_relative_angle);
  T2       = SVM_T2       (1.0f,V_s,U_d*0.66666666666666666666f, V_s_ref_relative_angle);

  T_min_on = SVM_T_min_on (1.0f, T1, T2);
  T_med_on = SVM_T_med_on (T_min_on, T1,T2,cita_V_s);
  T_max_on = SVM_T_max_on (T_med_on,T1,T2,cita_V_s);

  SVM_phase_duty_cycles           (&duty_a, &duty_b, &duty_c, cita_V_s,T_max_on,T_med_on,T_min_on);


  //shutdown_SVM_speed (t_e_ref,w_r,&shutdown);
  shutdown_SVM_speed (ref_freq_SVM,w_r,&shutdown);
  //shutdown_SVM_torque (t_e_ref,t_e,&shutdown);


  if (initial_rotor_position_start==false) { shutdown = true; }

  SVM_voltage_switch_inverter_VSI ( duty_a,  duty_b,  duty_c,shutdown);
gpio_clear(GPIOD, GPIO9);



}


if (center_aligned_state==FIRST_HALF)
{
  center_aligned_state=SECOND_HALF;
}
else 
{
  center_aligned_state=FIRST_HALF;
}

}



