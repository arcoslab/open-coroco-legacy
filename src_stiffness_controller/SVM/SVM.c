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
  //return ( psi_s_ref*cosf((psi_s_angle+phase_advance)) - psi_s*cosf(psi_s_angle) )/T_s  +  i_sD*R_s;

}

float SVM_V_s_ref_Q(float psi_s_ref, float psi_s, float psi_s_angle, float phase_advance,float i_sQ, float R_s,float T_s)
{
  //return ( psi_s_ref*sinf((psi_s_angle+phase_advance)*PI/180.0f) - psi_s*sinf(psi_s_angle*PI/180.0f) )/T_s  +  i_sQ*R_s; 
  //return ( psi_s_ref*sinf((psi_s_angle+phase_advance)*0.01745329251994329576f) - psi_s*sinf(psi_s_angle*0.01745329251994329576f) )/T_s  +  i_sQ*R_s; 
  //return ( psi_s_ref*fast_sine((psi_s_angle+phase_advance)*0.01745329251994329576f) - psi_s*fast_sine(psi_s_angle*0.01745329251994329576f) )/T_s  +  i_sQ*R_s;

  //catching_NaNs();
  //##catching_NaNs_data (psi_s_angle+phase_advance, fast_sine((psi_s_angle+phase_advance)), fast_sine(psi_s_angle));

  return ( psi_s_ref*fast_sine((psi_s_angle+phase_advance)) - psi_s*fast_sine(psi_s_angle) )/T_s  +  i_sQ*R_s;
  //return ( psi_s_ref*sinf((psi_s_angle+phase_advance)) - psi_s*(psi_s_angle) )/T_s  +  i_sQ*R_s;

}


void SVM_Maximum_allowed_V_s_ref(float* VsD, float* VsQ,float* V_s_ref,float U_d,bool* increase)
{
  float voltage_magnitude = *V_s_ref; 

  if   (*V_s_ref<=U_d/1.73205080756887729352f) { 
                                                    *V_s_ref = *V_s_ref;
                                                    *VsD     = *VsD;
                                                    *VsQ     = *VsQ;
                                                    *increase = false;
                                               }

  else                                         { 
                                                  //shrinking quadrature and direct components in order to fit under the maximum circle
                                                    *VsD     = *VsD * (U_d/1.73205080756887729352f) / voltage_magnitude;//(*V_s_ref); 
                                                    *VsQ     = *VsQ * (U_d/1.73205080756887729352f) / voltage_magnitude;//(*V_s_ref);
                                                  //adjusting the voltage magnitude to the value of the radious of the circule
                                                     *V_s_ref = U_d/1.73205080756887729352f;  
                                                     *increase=true;
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
  //return T_s*(U_s/U1)*( fast_sine( (60.0f-V_s_ref_relative_angle) )/0.86602540378443864676f );
  return T_s*(U_s/U1)*( fast_sine(V_s_ref_relative_angle)/0.86602540378443864676f );

}

float SVM_T2(float T_s,float U_s, float U2, float V_s_ref_relative_angle)
{
  //return T_s*(U_s/U2)*( sinf(V_s_ref_relative_angle*PI/180.0f)/sinf(60.0f*PI/180.0f) );
  //return T_s*(U_s/U2)*( sinf(V_s_ref_relative_angle*0.01745329251994329576f)/0.86602540378443864676f );
  //return T_s*(U_s/U2)*( fast_sine(V_s_ref_relative_angle*0.01745329251994329576f)/0.86602540378443864676f );
  //return T_s*(U_s/U2)*( fast_sine(V_s_ref_relative_angle)/0.86602540378443864676f );
  return T_s*(U_s/U2)* ( fast_sine( (60.0f-V_s_ref_relative_angle) )/0.86602540378443864676f );
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
/*
  if      ( V_s_ref_angle>=  0.0f && V_s_ref_angle< 60.0f) { return (T_min_on+T2); }  //sector S1
  else if ( V_s_ref_angle>= 60.0f && V_s_ref_angle<120.0f) { return (T_min_on+T1); }  //sector S2
  else if ( V_s_ref_angle>=120.0f && V_s_ref_angle<180.0f) { return (T_min_on+T2); }  //sector S3
  else if ( V_s_ref_angle>=180.0f && V_s_ref_angle<240.0f) { return (T_min_on+T1); }  //sector S4
  else if ( V_s_ref_angle>=240.0f && V_s_ref_angle<300.0f) { return (T_min_on+T2); }  //sector S5
  else if ( V_s_ref_angle>=300.0f && V_s_ref_angle<360.0f) { return (T_min_on+T1); }  //sector S6
  else                                                     { return (T_min_on   ); }  //failed sector
*/
  if      ( V_s_ref_angle>=  0.0f && V_s_ref_angle< 60.0f) { return (T_min_on+T1); }  //sector S1
  else if ( V_s_ref_angle>= 60.0f && V_s_ref_angle<120.0f) { return (T_min_on+T2); }  //sector S2
  else if ( V_s_ref_angle>=120.0f && V_s_ref_angle<180.0f) { return (T_min_on+T1); }  //sector S3
  else if ( V_s_ref_angle>=180.0f && V_s_ref_angle<240.0f) { return (T_min_on+T2); }  //sector S4
  else if ( V_s_ref_angle>=240.0f && V_s_ref_angle<300.0f) { return (T_min_on+T1); }  //sector S5
  else if ( V_s_ref_angle>=300.0f && V_s_ref_angle<360.0f) { return (T_min_on+T2); }  //sector S6
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
/*
  if      ( V_s_ref_angle>=  0.0f && V_s_ref_angle< 60.0f) { return (T_med_on+T1); }  //sector S1
  else if ( V_s_ref_angle>= 60.0f && V_s_ref_angle<120.0f) { return (T_med_on+T2); }  //sector S2
  else if ( V_s_ref_angle>=120.0f && V_s_ref_angle<180.0f) { return (T_med_on+T1); }  //sector S3
  else if ( V_s_ref_angle>=180.0f && V_s_ref_angle<240.0f) { return (T_med_on+T2); }  //sector S4
  else if ( V_s_ref_angle>=240.0f && V_s_ref_angle<300.0f) { return (T_med_on+T1); }  //sector S5
  else if ( V_s_ref_angle>=300.0f && V_s_ref_angle<360.0f) { return (T_med_on+T2); }  //sector S6
  else                                                     { return (T_med_on   ); }  //failed sector
*/
  if      ( V_s_ref_angle>=  0.0f && V_s_ref_angle< 60.0f) { return (T_med_on+T2); }  //sector S1
  else if ( V_s_ref_angle>= 60.0f && V_s_ref_angle<120.0f) { return (T_med_on+T1); }  //sector S2
  else if ( V_s_ref_angle>=120.0f && V_s_ref_angle<180.0f) { return (T_med_on+T2); }  //sector S3
  else if ( V_s_ref_angle>=180.0f && V_s_ref_angle<240.0f) { return (T_med_on+T1); }  //sector S4
  else if ( V_s_ref_angle>=240.0f && V_s_ref_angle<300.0f) { return (T_med_on+T2); }  //sector S5
  else if ( V_s_ref_angle>=300.0f && V_s_ref_angle<360.0f) { return (T_med_on+T1); }  //sector S6
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
float strain_gauge =0.0f;

float i_sD=0.0f;
float i_sQ=0.0f;
float i_s=0.0f;
float cita_i_s=0.0f;

float V_sD=0.0f;
float V_sQ=0.0f;
float V_s =0.0f;
float cita_V_s=0.0f;

float required_V_sD=0.0f;
float required_V_sQ=0.0f;
float required_V_s =0.0f;
float required_cita_V_s=0.0f;


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


void shutdown_counter(float ref_frequency,bool* shutdown)
{   
    #define INITIAL     0
    #define COUNTING    1
    #define WORKING     2
    #define END         3

    #define MAX_SHUT_COUNTER 50000
    
    static int state=INITIAL;
    static int shut_counter=0;
    
    if      (state==INITIAL  && ref_frequency==0.0f)  {state=INITIAL ; *shutdown=true;  shut_counter =0 ; }
    else if (state==INITIAL  && ref_frequency!=0.0f)  {state=WORKING ; *shutdown=false; shut_counter =0 ; }
    else if (state==WORKING  && ref_frequency!=0.0f)  {state=WORKING ; *shutdown=false; shut_counter =0 ; }
    else if (state==WORKING  && ref_frequency==0.0f)  {state=COUNTING; *shutdown=false; shut_counter+=1; }
    else if (state==COUNTING && ref_frequency!=0.0f)  {state=WORKING ; *shutdown=false; shut_counter =0; }
    else if (state==COUNTING && ref_frequency==0.0f && shut_counter<MAX_SHUT_COUNTER)  {state=COUNTING ; *shutdown=false; shut_counter +=1; }
    else if (state==COUNTING && ref_frequency==0.0f && shut_counter>=MAX_SHUT_COUNTER) {state=INITIAL  ; *shutdown=true ; shut_counter  =0; }
    else    {state=INITIAL; *shutdown=false; shut_counter=0;}  

                
         
}




#define FIRST_HALF  0
#define SECOND_HALF 1
float center_aligned_state=FIRST_HALF;

float Ia_peak__short_pulse=0.0f;
float Ib_peak__short_pulse=0.0f;
float Ic_peak__short_pulse=0.0f;
float initial_rotor_angle=0.0f;
float absolute_initial_rotor_angle=0.0f;
int   initial_rotor_zone=0;






float SVM_speed_close_loop_of_voltage_frequency_old(float reference_frequency, float frequency,bool close_loop_active, float* VsD, float* VsQ,float Ud,bool shutdown)
{

    static float extra_voltage_angle=0.0f;
    static float extra_load_angle=0.0f;
    static float extra_load_angle_increase=0.0f;


    if (shutdown==true)
    {
        *VsD=0.0f;
        *VsQ=0.0f;

        //extra_voltage_angle=0.0f;
        extra_load_angle=0.0f;
        extra_load_angle_increase=0.0f;
        
    }

    else if (close_loop_active==false) 
    { 
        *VsD=*VsD;
        *VsQ=*VsQ;
        //extra_voltage_angle=0.0f;
        extra_load_angle=0.0f;
        extra_load_angle_increase=0.0f;
    } 

    else if (close_loop_active==true )
    {  
        sensorless_pure_speed_SVM_pi_controller(reference_frequency,frequency,&extra_load_angle_increase); 

        extra_load_angle=extra_load_angle+extra_load_angle_increase;
        //extra_load_angle=frequency*360.0f*(2.0f*TICK_PERIOD)+extra_load_angle_increase;
        extra_voltage_angle=extra_voltage_angle+extra_load_angle;
        if (extra_voltage_angle>=360.0f) {extra_voltage_angle=extra_voltage_angle-360.0f;}
        if (extra_voltage_angle<0.0f)    {extra_voltage_angle=extra_voltage_angle+360.0f;}

       *VsD = 20.0f*Ud*fast_cos(extra_voltage_angle);
       *VsQ = 20.0f*Ud*fast_sine(extra_voltage_angle);
   } 

   return extra_load_angle;
}

float SVM_speed_close_loop_of_voltage_frequency(float reference_frequency, float frequency,bool close_loop_active, float* VsD, float* VsQ,float Ud,bool shutdown)
{

    static float extra_voltage_angle=0.0f;
    static float extra_load_angle=0.0f;
    static float extra_load_angle_increase=0.0f;
    //float Value;

    if (shutdown==true)
    {
        *VsD=0.0f;
        *VsQ=0.0f;

        //extra_voltage_angle=0.0f;
        extra_load_angle=0.0f;
        extra_load_angle_increase=0.0f;
        
    }

    else if (close_loop_active==true )
    {  
        sensorless_pure_speed_SVM_pi_controller(reference_frequency,frequency,&extra_load_angle_increase); 
        
        extra_load_angle_increase=90.0f;


        extra_voltage_angle=extra_voltage_angle+frequency*360.0f*(2.0f*TICK_PERIOD)+extra_load_angle_increase;

        if (extra_voltage_angle>=360.0f) {extra_voltage_angle=extra_voltage_angle-360.0f;}
        if (extra_voltage_angle<0.0f)    {extra_voltage_angle=extra_voltage_angle+360.0f;}


       //*VsD = SVM_V_s_ref_D (psi_s_ref,psi_s,psi_s_alpha_SVM,extra_voltage_angle,i_sD,R_s,2.0f*TICK_PERIOD);
       //*VsQ = SVM_V_s_ref_Q (psi_s_ref,psi_s,psi_s_alpha_SVM,extra_voltage_angle,i_sQ,R_s,2.0f*TICK_PERIOD);


        *VsD = 20.0f*Ud*fast_cos(extra_voltage_angle);
        *VsQ = 20.0f*Ud*fast_sine(extra_voltage_angle);
        pi_max=frequency*360.0f*(2.0f*TICK_PERIOD)+extra_load_angle_increase;//Value;//extra_voltage_angle;
   } 

   return extra_load_angle;
}



#define INITIAL_SVM    0
#define OPEN_LOOP_SVM  1
#define CLOSE_LOOP_SVM 2


#define IS_ANGLE_OFFSET_0 (120.0f)//110.0f
#define IS_ANGLE_OFFSET_1 (0.0f)  //
#define ISD_CORRECTION (1.0f)
#define ISQ_CORRECTION (1.15574f)
void  DTC_SVM(void)
{

static bool shutdown=true;
static bool increase_flux  = false;

if (center_aligned_state==FIRST_HALF)
{

  i_sD     = direct_stator_current_i_sD     (i_sA);
  i_sQ     = quadrature_stator_current_i_sQ (i_sA,i_sB);

  static float psi_sD_i_neglected=0.0f;
  static float psi_sQ_i_neglected=0.0f;

  flux_linkage_estimator(2.0f*TICK_PERIOD,V_sD,V_sQ,i_sD,i_sQ,R_s,CUR_FREQ,&psi_sD,&psi_sQ,&psi_s,&psi_s_alpha_SVM);
  flux_linkage_estimator_neglected_currents (2.0f*TICK_PERIOD,V_sD,V_sQ,&psi_sD_i_neglected,&psi_sQ_i_neglected);
                                                  
  //actual value
  //it has to be multiplied by two in order because the switching frequency
  //is half the pwm frequency due to the two-cycle center-aligned signal
  //w_r = 0.15915494309189533576f*rotor_speed_w_r (psi_sD,psi_sQ,TICK_PERIOD*2.0f); 
  
  //using neglected-currents flux-linkage estimator
  w_r = 0.15915494309189533576f*rotor_speed_w_r (psi_sD_i_neglected,psi_sQ_i_neglected,TICK_PERIOD*2.0f);  
  w_r = wr_moving_average_filter(w_r); 
  hall_freq=frequency_direction_two_hall_sensors_AB(CUR_FREQ);
  if (w_r!=w_r) w_r=0.0f;

  //t_e       = electromagnetic_torque_estimation_t_e   (psi_sD,i_sQ,psi_sQ,i_sD,pole_pairs);
  t_e       = electromagnetic_torque_estimation_t_e   (psi_sD_i_neglected,i_sQ,psi_sQ_i_neglected,i_sD,pole_pairs);
  //t_e =  te_moving_average_filter(t_e);

  psi_s_ref=psi_F;
  
  //--------------------------------SVM algorithm--------------------------------------------//


} 


else
{

/*
  ref_freq_SVM = admittance_controller    (
                                            stiffness,
                                            damping,
                                            reference_electric_angle,
                                            electric_angle,
                                            strain_gauge
                                            );
*/
  
  /**************************************************************/
  /*********Admitance Controller*********************************/
  /**************************************************************/
  /*ref_freq_SVM = admittance_controller    (
                                            stiffness,
                                            damping,
                                            reference_electric_angle,
                                            electric_angle,
                                            strain_gauge
                                            );
  */


  electric_angle= electric_angle+
                            /*DTC-SVM PID controller*/                            

                            /*SVM with PID controller on the frequency calculated with the hall sensor*/
                            //SVM_speed_close_loop_of_voltage_frequency(ref_freq_SVM,hall_freq,true,&V_sD,&V_sQ,U_d,shutdown); 
  
                            /*SVM with lineal increase of electric frequency [based on frecuency calculated with the flux-linkage estimator]*/ 
                            SVM_speed_close_loop_of_voltage_frequency_old(ref_freq_SVM,w_r      ,true,&V_sD,&V_sQ,U_d,shutdown);

#define MAX_GEAR_CYCLES 100.0f

  if (reference_electric_angle>=360.0f*pole_pairs*gear_ratio*MAX_GEAR_CYCLES)
    reference_electric_angle=reference_electric_angle-360.0f*pole_pairs*gear_ratio*MAX_GEAR_CYCLES;
  if (electric_angle<-  360.0f*pole_pairs*gear_ratio*MAX_GEAR_CYCLES)
    reference_electric_angle=reference_electric_angle+360.0f*pole_pairs*gear_ratio*MAX_GEAR_CYCLES;              

  if (electric_angle>=360.0f*pole_pairs*gear_ratio*MAX_GEAR_CYCLES)
    electric_angle=electric_angle-360.0f*pole_pairs*gear_ratio*MAX_GEAR_CYCLES;
  if (electric_angle<-360.0f*pole_pairs*gear_ratio*MAX_GEAR_CYCLES)
    electric_angle=electric_angle+360.0f*pole_pairs*gear_ratio*MAX_GEAR_CYCLES;



  fast_vector_angle_and_magnitude(V_sQ,V_sD,&V_s,&cita_V_s);

  required_V_sD     =   V_sD;
  required_V_sQ     =   V_sQ;
  required_V_s      =   V_s;
  required_cita_V_s =   cita_V_s;


  SVM_Maximum_allowed_V_s_ref (&V_sD,&V_sQ,&V_s,U_d*UD_PERCENTAGE,&increase_flux);//0.70f);
  V_s_ref_relative_angle = SVM_V_s_relative_angle      (cita_V_s);


  float V_s_duty_cycle=0.0f;
  float U_max=0.0f;
  V_s_duty_cycle=V_s/(0.66666666666666666666f*U_d); //V_s_duty_cycle=V_s/( (2.0f/3.0f) *U_d);
  U_max=U_d*0.66666666666666666666f;

  T1       = SVM_T1       (V_s_duty_cycle,V_s,U_max, V_s_ref_relative_angle);
  T2       = SVM_T2       (V_s_duty_cycle,V_s,U_max, V_s_ref_relative_angle);
  

  T_min_on =SVM_T_min_on (1.0f, T1, T2); //T_min_on = SVM_T_min_on (1.0f, T1, T2);
  T_med_on =SVM_T_med_on (T_min_on, T1,T2,cita_V_s);
  T_max_on =SVM_T_max_on (T_med_on,T1,T2,cita_V_s);

  SVM_phase_duty_cycles           (&duty_a, &duty_b, &duty_c, cita_V_s,T_max_on,T_med_on,T_min_on);
  
  duty_a=0.8f;
  duty_b=0.0f;
  duty_c=0.0f;

  

  shutdown_counter(ref_freq_SVM,&shutdown);
  SVM_voltage_switch_inverter_VSI ( duty_a,  duty_b,  duty_c,shutdown);
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



