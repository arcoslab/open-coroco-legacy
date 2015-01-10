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

#define MINIMUM_admittance_FREQUENCY 0.5f
#define MINIMUM_admittance_REF_FREQUENCY 0.5f
void shutdown_admittance_speed (float reference_frequency,float actual_frequency,bool* shutdown)
{


    if      ( *shutdown           == false && 
              actual_frequency>-MINIMUM_admittance_FREQUENCY && 
              actual_frequency<MINIMUM_admittance_FREQUENCY  &&
              reference_frequency>-MINIMUM_admittance_REF_FREQUENCY && 
              reference_frequency<MINIMUM_admittance_REF_FREQUENCY  
              ) 
    { *shutdown = true ;}
 
    else if      ( *shutdown           == true && 
              actual_frequency>-MINIMUM_SVM_FREQUENCY && 
              actual_frequency<MINIMUM_SVM_FREQUENCY  &&
              reference_frequency>-MINIMUM_admittance_REF_FREQUENCY && 
              reference_frequency<MINIMUM_admittance_REF_FREQUENCY ) 
    { *shutdown = true ;}

    else                                                                                                                         
    { *shutdown = false;}
}


#define MINIMUM_GEAR_POSITION 10.0f
#define MAX_SHUTDOWN_TIME     5.0f //s
#define MINIMUM_REF_FREQ      5.0f
void shutdown_SVM_position (float reference_frequency,float reference_position_change,float reference_position,float actual_position,bool* shutdown)
{
    static float shutdown_time=0;
    static float shutdown_error=0.0f;
    static bool start_shutdown_counter=false;

    shutdown_error=reference_position-actual_position;

    if      ( reference_frequency>-MINIMUM_REF_FREQ  &&
              reference_frequency< MINIMUM_REF_FREQ  && 
               *shutdown           == false      && 
              reference_position_change == 0.0f && 
              shutdown_error < MINIMUM_GEAR_POSITION     && 
              shutdown_error >-MINIMUM_GEAR_POSITION         ) 
    {
         *shutdown = true ;
         shutdown_time=0;
    }


    else if (reference_frequency>-MINIMUM_REF_FREQ  &&
              reference_frequency< MINIMUM_REF_FREQ  && shutdown_time>=MAX_SHUTDOWN_TIME &&
             shutdown_error < MINIMUM_GEAR_POSITION     && 
                shutdown_error >-MINIMUM_GEAR_POSITION     && *shutdown==false)
    {
        *shutdown=true;
        //shutdown_time=0;
        start_shutdown_counter=false;
    }

    else if ( reference_frequency>-MINIMUM_REF_FREQ  &&
              reference_frequency< MINIMUM_REF_FREQ  && 
              *shutdown           == false      && 
              shutdown_error < MINIMUM_GEAR_POSITION     && 
              shutdown_error >-MINIMUM_GEAR_POSITION         ) 
    { 
        *shutdown = false ;
        start_shutdown_counter=true;
     
    }
    
    else if ( *shutdown           == true  &&  
              reference_position_change == 0.0f
            )                                                                                      
    { *shutdown = true ;
       shutdown_time=0;
    }


 
    else                                                                                                                         
    { *shutdown = false;}


    if (start_shutdown_counter==true)
    { shutdown_time+=TICK_PERIOD*2.0f;}

    //else 
    //{shutdown_time=0.0f;}


    //if (*shutdown==true)
      //  shutdown_time=0.0f;
}

void simple_shutdown(float control_signal,bool* shutdown)
{
    if (control_signal==0.0f)
        *shutdown=true;
    else
        *shutdown=false;
}

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

float Ia_peak__short_pulse=0.0f;
float Ib_peak__short_pulse=0.0f;
float Ic_peak__short_pulse=0.0f;
float initial_rotor_angle=0.0f;
float absolute_initial_rotor_angle=0.0f;
int   initial_rotor_zone=0;



void SVM_starting_open_loop(bool open_loop,float* VsD, float*VsQ, float Ud, float max_open_loop_frequency,float frequency,float reference_frequency)
{
    static float extra_voltage_angle=0.0f;
    static float extra_load_angle=0.0f;
    static float extra_load_angle_increase=0.0f;
    static bool increase_permission=true;


    if (open_loop==false) 
    { 
        extra_voltage_angle=0.0f;
        extra_load_angle=0.0f;
        //extra_load_angle_increase=0.0f;
        *VsD=*VsD;
        *VsQ=*VsQ;
    } 
    else if (open_loop==true) 
    { 
        if (reference_frequency==0.0f) 
        {    
        //if (t_e_ref==0.0f) {  
            extra_voltage_angle=0.0f;
            extra_load_angle=0.0f;
            extra_load_angle_increase=0.0f;
            increase_permission=true;
         }
        //else if (CUR_FREQ<ref_freq_SVM) extra_load_angle_increase=0.000005f;
        //else if (CUR_FREQ<250.0f) extra_load_angle_increase=0.000001f; //extra_load_angle_increase=0.00005f;
        //else if (w_r<10.0f) extra_load_angle_increase=0.00001f; //extra_load_angle_increase=0.00005f;
        //else if (w_r<ref_freq_SVM && increase_permission==true) 


        if (reference_frequency>0.0f)
        {
            if (frequency<max_open_loop_frequency && increase_permission==true)
            {   
                extra_load_angle_increase=MAXIMUM_OPEN_LOOP_ANGLE_INCREASE; //extra_load_angle_increase=0.00005f;
            }
            else  
            { 
                extra_load_angle_increase=0.0f;
                increase_permission=false;
            }

            extra_load_angle=extra_load_angle+extra_load_angle_increase;
            extra_voltage_angle=extra_voltage_angle+extra_load_angle;
            if (extra_voltage_angle>=360.0f) {extra_voltage_angle=extra_voltage_angle-360.0f;}

            if (reference_frequency==0.0f) 
            //if (t_e_ref==0.0f)
            {
                *VsD = 0.0f;
                *VsQ = 0.0f;
            }
            else                 
            {                                 
                 //extra_voltage_angle=45.0f;
                 *VsD = 20.0f*Ud*fast_cos(extra_voltage_angle);
                 *VsQ = 20.0f*Ud*fast_sine(extra_voltage_angle);
            }
        }

        else 
        {
            if (frequency>max_open_loop_frequency && increase_permission==true)
            {   
                extra_load_angle_increase=MAXIMUM_OPEN_LOOP_ANGLE_INCREASE; //extra_load_angle_increase=0.00005f;
            }
            else  
            { 
                extra_load_angle_increase=0.0f;
                increase_permission=false;
            }

            extra_load_angle=extra_load_angle-extra_load_angle_increase;
            extra_voltage_angle=extra_voltage_angle+extra_load_angle;
            if (extra_voltage_angle>=360.0f) {extra_voltage_angle=extra_voltage_angle-360.0f;}
            if (extra_voltage_angle<0.0f)    {extra_voltage_angle=extra_voltage_angle+360.0f;}

            if (reference_frequency==0.0f) 
            //if (t_e_ref==0.0f)
            {
                *VsD = 0.0f;
                *VsQ = 0.0f;
            }
            else                 
            {                                 
                 //extra_voltage_angle=45.0f;
                 *VsD = 20.0f*Ud*fast_cos(extra_voltage_angle);
                 *VsQ = 20.0f*Ud*fast_sine(extra_voltage_angle);
            }
        }

     } 
  //SVM_pi_control=extra_voltage_angle;
  //pi_max=extra_load_angle;
}


/*
void SVM_starting_open_loop(bool open_loop,float* VsD, float*VsQ, float Ud)
{
    static float extra_voltage_angle=0.0f;
    static float extra_load_angle=0.0f;
    static float extra_load_angle_increase=0.0f;


    if (open_loop==false) { extra_voltage_angle=0.0f;
                            extra_load_angle=0.0f;
                            //extra_load_angle_increase=0.0f;
                            *VsD=*VsD;
                            *VsQ=*VsQ;

                            //psi_rotating_angle_SVM=1.0f;

                          } 
    else if (open_loop==true) { //if (ref_freq_SVM==0) {    
                                if (t_e_ref==0) {  
                                                        extra_voltage_angle=0.0f;
                                                        extra_load_angle=0.0f;
                                                        extra_load_angle_increase=0.0f;
                                                     }
                                //else if (CUR_FREQ<ref_freq_SVM) extra_load_angle_increase=0.000005f;
                                else if (CUR_FREQ<100.0f) extra_load_angle_increase=0.000005f;
                                else                extra_load_angle_increase=0.0f;
                                

                                extra_load_angle=extra_load_angle+extra_load_angle_increase;
                                extra_voltage_angle=extra_voltage_angle+extra_load_angle;
                                if (extra_voltage_angle>=360.0f) {extra_voltage_angle=extra_voltage_angle-360.0f;}

                             //if (ref_freq_SVM==0) {
                             if (t_e_ref==0) {  
                                                     *VsD = 0.0f;
                                                     *VsQ = 0.0f;
                                                  }
                             else                 {
                                                     *VsD = Ud*fast_cos(extra_voltage_angle);
                                                     *VsQ = Ud*fast_sine(extra_voltage_angle);
                                                  }
                                
              

                           } 
}
*/



float SVM_speed_close_loop_of_voltage_frequency(float reference_frequency, float frequency,bool close_loop_active, float* VsD, float* VsQ,float Ud,bool shutdown)
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
        //sensorless_pure_speed_SVM_pi_controller(reference_frequency,frequency,&extra_load_angle_increase); 
        pure_speed_SVM_pi_controller_variable_frequency(reference_frequency,frequency,&extra_load_angle_increase); 
        //extra_load_angle=extra_load_angle+extra_load_angle_increase;
        //extra_load_angle=frequency*360.0f*(2.0f*TICK_PERIOD)+extra_load_angle_increase;
        //extra_voltage_angle=extra_voltage_angle+extra_load_angle;

        extra_voltage_angle=extra_voltage_angle+frequency*360.0f*(2.0f*TICK_PERIOD)+extra_load_angle_increase;


        if (extra_voltage_angle>=360.0f) {extra_voltage_angle=extra_voltage_angle-360.0f;}
        if (extra_voltage_angle<0.0f)    {extra_voltage_angle=extra_voltage_angle+360.0f;}

       *VsD = 20.0f*Ud*fast_cos(extra_voltage_angle);
       *VsQ = 20.0f*Ud*fast_sine(extra_voltage_angle);
        pi_max=extra_voltage_angle;
   } 

   return extra_load_angle;
}



void SVM_speed_close_loop(float reference_frequency, float frequency,bool close_loop_active, float* VsD, float* VsQ)
{
    //float psi_rotating_angle;
    

    if (close_loop_active==false) 
    { 
        *VsD=*VsD;
        *VsQ=*VsQ;
        //psi_rotating_angle_SVM=3.0f;
    } 
    else if (reference_frequency==0.0f)
    { 
        *VsD=0.0f;
        *VsQ=0.0f; 
    }
    else if (close_loop_active==true )//&& CUR_FREQ<500.0f) 
    {  
       //psi_rotating_angle_SVM=reference_frequency;//FIXED_LOAD_ANGLE;//15.0f;
       sensorless_speed_pi_controller(reference_frequency,frequency, &psi_rotating_angle_SVM); 

       *VsD = SVM_V_s_ref_D (psi_s_ref,psi_s,psi_s_alpha_SVM,psi_rotating_angle_SVM,i_sD,R_s,2.0f*TICK_PERIOD);
       *VsQ = SVM_V_s_ref_Q (psi_s_ref,psi_s,psi_s_alpha_SVM,psi_rotating_angle_SVM,i_sQ,R_s,2.0f*TICK_PERIOD);
                                                     
    }
    else 
    {
      //psi_rotating_angle_SVM=reference_frequency;//FIXED_LOAD_ANGLE;//15.0f;
      sensorless_speed_pi_controller(reference_frequency,frequency, &psi_rotating_angle_SVM); 

      *VsD = SVM_V_s_ref_D (psi_s_ref,psi_s,psi_s_alpha_SVM,psi_rotating_angle_SVM,i_sD,R_s,2.0f*TICK_PERIOD);
      *VsQ = SVM_V_s_ref_Q (psi_s_ref,psi_s,psi_s_alpha_SVM,psi_rotating_angle_SVM,i_sQ,R_s,2.0f*TICK_PERIOD);
    }
/*
    else if (close_loop_active==true && frequency>=450.0f) {    psi_rotating_angle_SVM=0.0f;
                                                            
                                                        *VsD = SVM_V_s_ref_D (psi_s_ref,psi_s,psi_s_alpha_SVM,psi_rotating_angle_SVM,i_sD,R_s,2.0f*TICK_PERIOD);
                                                        *VsQ = SVM_V_s_ref_Q (psi_s_ref,psi_s,psi_s_alpha_SVM,psi_rotating_angle_SVM,i_sQ,R_s,2.0f*TICK_PERIOD);
                                                   }
*/
}

void SVM_torque_close_loop(float reference_torque, float torque,bool close_loop_active, float* VsD, float* VsQ)
{
    //float psi_rotating_angle;

    if (close_loop_active==false) { *VsD=*VsD;
                                    *VsQ=*VsQ;

                                    //psi_rotating_angle_SVM=6.0f;
                                  } 
    else if (close_loop_active==true && CUR_FREQ<500.0f) {  //psi_rotating_angle_SVM=7.0f;
                                                            sensorless_torque_pi_controller(reference_torque,torque, &psi_rotating_angle_SVM); 

                                                            *VsD = SVM_V_s_ref_D (psi_s_ref,psi_s,psi_s_alpha_SVM,psi_rotating_angle_SVM,i_sD,R_s,2.0f*TICK_PERIOD);
                                                            *VsQ = SVM_V_s_ref_Q (psi_s_ref,psi_s,psi_s_alpha_SVM,psi_rotating_angle_SVM,i_sQ,R_s,2.0f*TICK_PERIOD);
                                                         }

    else if (close_loop_active==true && CUR_FREQ>=450.0f) {    //psi_rotating_angle_SVM=0.0f;
                                                                sensorless_torque_pi_controller(reference_torque,torque, &psi_rotating_angle_SVM); 
                                                                *VsD = SVM_V_s_ref_D (psi_s_ref,psi_s,psi_s_alpha_SVM,psi_rotating_angle_SVM,i_sD,R_s,2.0f*TICK_PERIOD);
                                                                *VsQ = SVM_V_s_ref_Q (psi_s_ref,psi_s,psi_s_alpha_SVM,psi_rotating_angle_SVM,i_sQ,R_s,2.0f*TICK_PERIOD);
                                                          }

}


#define INITIAL_SVM    0
#define OPEN_LOOP_SVM  1
#define CLOSE_LOOP_SVM 2

//void SVM_loop_control(float frequency,float maximum_open_loop_frequency,float te_ref, float freq_ref, bool* open_loop, bool* close_loop_SVM)
//{
    //static int SVM_loop_state=INITIAL_SVM;
    //*open_loop=true;
    //*close_loop_SVM=false;
/*
    *open_loop=false;
    *close_loop_SVM=true;
*/
/*
    if      (SVM_loop_state==INITIAL_SVM && te_ref==0.0f && freq_ref==0.0f)  {   SVM_loop_state=INITIAL_SVM;
                                                                                 *open_loop=false;
                                                                                 *close_loop_SVM=false;
                                                                             }
    else if (SVM_loop_state==INITIAL_SVM && (te_ref!=0.0f || freq_ref!=0.0f) )  {   SVM_loop_state=OPEN_LOOP_SVM;
                                                                                    *open_loop=true;
                                                                                    *close_loop_SVM=false;
                                                                          
                                                                                }
    else if (SVM_loop_state==OPEN_LOOP_SVM && frequency<maximum_open_loop_frequency)  {   SVM_loop_state=OPEN_LOOP_SVM;
                                                                                          *open_loop=true;
                                                                                          *close_loop_SVM=false;
                                                                              
                                                                                      }


    else if (SVM_loop_state==OPEN_LOOP_SVM && frequency>=maximum_open_loop_frequency)  {  SVM_loop_state=CLOSE_LOOP_SVM;
                                                                                          *open_loop=false;
                                                                                          *close_loop_SVM=true;
                                                                                       }
*/

/*    
    else if (SVM_loop_state==CLOSE_LOOP_SVM && shutdown==false)  {  SVM_loop_state=CLOSE_LOOP_SVM;
                                                                    *open_loop=false;
                                                                    *close_loop_SVM=true;
                                                                 }

    else if (SVM_loop_state==CLOSE_LOOP_SVM && shutdown==true)  {  SVM_loop_state=INITIAL_SVM;
                                                                    *open_loop=false;
                                                                    *close_loop_SVM=false;
                                                                 }
    */
/*
    else if (SVM_loop_state==CLOSE_LOOP_SVM)
    {  
        SVM_loop_state=CLOSE_LOOP_SVM;
        *open_loop=false;
        *close_loop_SVM=true;
    }

    else if (SVM_loop_state==CLOSE_LOOP_SVM)  
    {  SVM_loop_state=INITIAL_SVM;
       *open_loop=false;
       *close_loop_SVM=false;
    }
*/
//--
//}

#define IS_ANGLE_OFFSET_0 (120.0f)//110.0f
#define IS_ANGLE_OFFSET_1 (0.0f)  //
#define ISD_CORRECTION (1.0f)
#define ISQ_CORRECTION (1.15574f)
void  DTC_SVM(void)
{
static bool shutdown=true;
  static bool open_loop_SVM  = false;
  //static bool close_loop_SVM = false;
  static bool increase_flux  = false;

if (center_aligned_state==FIRST_HALF)
{


  
  i_sD     = direct_stator_current_i_sD     (i_sA);
  i_sQ     = quadrature_stator_current_i_sQ (i_sA,i_sB);
  //fast_vector_angle_and_magnitude(i_sQ,i_sD,&i_s,&cita_i_s);

 
/*
  //nan erradication
  if (psi_sD!=psi_sD) psi_sD=0.0f;
  if (psi_sQ!=psi_sQ) psi_sQ=0.0f;
  if (psi_s !=psi_s ) psi_s =0.0f;
  if (t_e!=t_e      ) t_e    =0.0f;
  if (psi_s_alpha_SVM!=psi_s_alpha_SVM) psi_s_alpha_SVM=0.0f;
*/

  static float psi_sD_i_neglected=0.0f;
  static float psi_sQ_i_neglected=0.0f;

  flux_linkage_estimator(2.0f*TICK_PERIOD,V_sD,V_sQ,i_sD,i_sQ,R_s,CUR_FREQ,&psi_sD,&psi_sQ,&psi_s,&psi_s_alpha_SVM);
  //flux_linkage_estimator_neglected_currents (2.0f*TICK_PERIOD,V_sD,V_sQ,&psi_sD_i_neglected,&psi_sQ_i_neglected);

//psi_sD_i_neglected=direct_stator_flux_linkage_estimator_psi_sD     (2.0f*TICK_PERIOD,V_sD,i_sD,R_s,w_r);
//psi_sQ_i_neglected=quadrature_stator_flux_linkage_estimator_psi_sQ (2.0f*TICK_PERIOD,V_sQ,i_sQ,R_s,w_r);

//psi_sD_i_neglected=direct_stator_flux_linkage_estimator_psi_sD     (2.0f*TICK_PERIOD,V_sD,0.0f,R_s,w_r);
//psi_sQ_i_neglected=quadrature_stator_flux_linkage_estimator_psi_sQ (2.0f*TICK_PERIOD,V_sQ,0.0f,R_s,w_r);
  
/*
  //nan erradication
  if (psi_sD!=psi_sD) psi_sD=0.0f;
  if (psi_sQ!=psi_sQ) psi_sQ=0.0f;
  if (psi_s !=psi_s ) psi_s =0.0f;
  if (t_e!=t_e      ) t_e    =0.0f;
  if (psi_s_alpha_SVM!=psi_s_alpha_SVM) psi_s_alpha_SVM=0.0f;
*/


  //w_r             = (1.0f/(2.0f*PI))     *rotor_speed_w_r                                 (psi_sD,psi_sQ,TICK_PERIOD*2.0f);
  //w_r             = 0.15915494309189533576f*rotor_speed_w_r                                 (psi_sD,psi_sQ,TICK_PERIOD*2.0f);  
                                                           //it has to be multiplied by two in order because the switching frequency
                                                           //is half the pwm frequency due to the two-cycle center-aligned signal
  //actual value
  w_r = 0.15915494309189533576f*rotor_speed_w_r (psi_sD,psi_sQ,TICK_PERIOD*2.0f); 
  
  //using neglected-currents flux-linkage estimator
  //w_r = 0.15915494309189533576f*rotor_speed_w_r (psi_sD_i_neglected,psi_sQ_i_neglected,TICK_PERIOD*2.0f);  
  //w_r = wr_moving_average_filter(w_r); 
  hall_freq=frequency_direction_two_hall_sensors_AB(CUR_FREQ);

  if (w_r!=w_r) w_r=0.0f;

  //t_e       = electromagnetic_torque_estimation_t_e   (psi_sD,i_sQ,psi_sQ,i_sD,pole_pairs);
  t_e       = electromagnetic_torque_estimation_t_e   (psi_sD_i_neglected,i_sQ,psi_sQ_i_neglected,i_sD,pole_pairs);
 

  //t_e =  te_moving_average_filter(t_e);


  //t_e_ref = DTC_torque_reference_PI                 (CUR_FREQ, ref_freq);
  //psi_s_ref = stator_flux_linkage_reference_psi_s_ref (psi_F,t_e_ref,L_sq,pole_pairs);

  if (user_input==true)
    psi_s_ref=psi_ref_user;
  else 
    psi_s_ref=psi_F;
  
  //--------------------------------SVM algorithm--------------------------------------------//

  //SVM_starting_open_loop(open_loop_SVM,&V_sD,&V_sQ,U_d,MAXIMUM_OPEN_LOOP_SPEED,CUR_FREQ,ref_freq_SVM);
  //SVM_starting_open_loop(open_loop_SVM,&V_sD,&V_sQ,U_d,ref_freq_SVM,CUR_FREQ,ref_freq_SVM);
  SVM_starting_open_loop(open_loop_SVM,&V_sD,&V_sQ,U_d,ref_freq_SVM,hall_freq,ref_freq_SVM);
} 


else
{
  //SVM_speed_close_loop(ref_freq_SVM,CUR_FREQ,close_loop_SVM,&V_sD,&V_sQ);
  //SVM_speed_close_loop(ref_freq_SVM,w_r,close_loop_SVM,&V_sD,&V_sQ);
  //SVM_speed_close_loop_of_voltage_frequency(ref_freq_SVM,CUR_FREQ,close_loop_SVM,&V_sD,&V_sQ,U_d,shutdown);   



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
                            SVM_speed_close_loop_of_voltage_frequency(ref_freq_SVM,CUR_FREQ,true,&V_sD,&V_sQ,U_d,shutdown); 
                            //SVM_speed_close_loop_of_voltage_frequency(ref_freq_SVM,hall_freq,true,&V_sD,&V_sQ,U_d,shutdown); 
                            hall_freq=CUR_FREQ;
                          //SVM_speed_close_loop_of_voltage_frequency(ref_freq_SVM,w_r,true,&V_sD,&V_sQ,U_d,shutdown); 
                          //SVM_speed_close_loop_of_voltage_frequency(ref_freq_SVM,hall_freq,close_loop_SVM,&V_sD,&V_sQ,U_d,shutdown); 


#define MAX_GEAR_CYCLES 100.0f

  if (reference_electric_angle>=360.0f*pole_pairs*gear_ratio*MAX_GEAR_CYCLES)
    reference_electric_angle=reference_electric_angle-360.0f*pole_pairs*gear_ratio*MAX_GEAR_CYCLES;
  if (electric_angle<-  360.0f*pole_pairs*gear_ratio*MAX_GEAR_CYCLES)
    reference_electric_angle=reference_electric_angle+360.0f*pole_pairs*gear_ratio*MAX_GEAR_CYCLES;              

  if (electric_angle>=360.0f*pole_pairs*gear_ratio*MAX_GEAR_CYCLES)
    electric_angle=electric_angle-360.0f*pole_pairs*gear_ratio*MAX_GEAR_CYCLES;
  if (electric_angle<-360.0f*pole_pairs*gear_ratio*MAX_GEAR_CYCLES)
    electric_angle=electric_angle+360.0f*pole_pairs*gear_ratio*MAX_GEAR_CYCLES;



/*
  if (shutdown==true)
    electric_angle=0.0f;
*/
  //SVM_torque_close_loop(t_e_ref,t_e,close_loop_SVM,&V_sD,&V_sQ);
  //SVM_loop_control(hall_freq,MAXIMUM_OPEN_LOOP_SPEED,t_e_ref,ref_freq_SVM,&open_loop_SVM,&close_loop_SVM); 

  fast_vector_angle_and_magnitude(V_sQ,V_sD,&V_s,&cita_V_s);

  required_V_sD     =   V_sD;
  required_V_sQ     =   V_sQ;
  required_V_s      =   V_s;
  required_cita_V_s =   cita_V_s;

  SVM_Maximum_allowed_V_s_ref (&V_sD,&V_sQ,&V_s,U_d*UD_PERCENTAGE,&increase_flux);//0.70f);
  V_s_ref_relative_angle = SVM_V_s_relative_angle      (cita_V_s);


  //cheating
  //V_sD=required_V_sD;
  //V_sQ=required_V_sQ;

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
  

  //shutdown_SVM_speed (ref_freq_SVM,w_r,&shutdown);
  /*shutdown_SVM_position (   ref_freq_SVM,
                            reference_change_electric_angle/(pole_pairs*gear_ratio) ,
                            reference_electric_angle/(pole_pairs*gear_ratio)        ,
                            electric_angle/(pole_pairs*gear_ratio),
                            &shutdown);*/
  //shutdown_admittance_speed (ref_freq_SVM,hall_freq,&shutdown);
  //simple_shutdown(reference_change_electric_angle/(pole_pairs*gear_ratio),&shutdown);
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



