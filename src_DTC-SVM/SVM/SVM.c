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
  return ( psi_s_ref*cosf((psi_s_angle+phase_advance)*PI/180.0f) - psi_s*cos(psi_s_angle*PI/180.0f) )/T_s  +  i_sD*R_s; 
}

float SVM_V_s_ref_Q(float psi_s_ref, float psi_s, float psi_s_angle, float phase_advance,float i_sQ, float R_s,float T_s)
{
  return ( psi_s_ref*sinf((psi_s_angle+phase_advance)*PI/180.0f) - psi_s*sin(psi_s_angle*PI/180.0f) )/T_s  +  i_sQ*R_s; 
}

void SVM_Maximum_allowed_V_s_ref(float* V_s_ref,float U_d)
{
  if   (*V_s_ref<=U_d/sqrt(3.0f)) { *V_s_ref = *V_s_ref;       }
  else                            { *V_s_ref = U_d/sqrt(3.0f); }
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
  return T_s*(U_s/U1)*( sinf( (PI/180.0f)*(60.0f-V_s_ref_relative_angle) )/sinf(60.0f*PI/180.0f) );
}

float SVM_T2(float T_s,float U_s, float U2, float V_s_ref_relative_angle)
{
  return T_s*(U_s/U2)*( sinf(V_s_ref_relative_angle*PI/180.0f)/sinf(60.0f*PI/180.0f) ); 
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



void  SVM_voltage_switch_inverter_VSI(float duty_A,float duty_B,float duty_C,float Attenuation)
{
  //DTC-SVM switching selection


  //-------------SA: S1 and S4------------------------------------
  timer_set_oc_mode      (TIM1, TIM_OC1, TIM_OCM_PWM1);
  timer_enable_oc_output (TIM1, TIM_OC1 );  //S1
  timer_enable_oc_output (TIM1, TIM_OC1N);  //S4


  //-------------SB: S3 and S6------------------------------------
  timer_set_oc_mode      (TIM1, TIM_OC2, TIM_OCM_PWM1);
  timer_enable_oc_output (TIM1, TIM_OC2 );  //S3
  timer_enable_oc_output (TIM1, TIM_OC2N);  //S6


  //-------------SC: S5 and S2-------------------------------------
  timer_set_oc_mode(TIM1, TIM_OC3, TIM_OCM_PWM1);
  timer_enable_oc_output (TIM1, TIM_OC3 );  //S5 on
  timer_enable_oc_output (TIM1, TIM_OC3N);  //S2 off
  

  /* Set the capture compare value for OC1. */
  timer_set_oc_value(TIM1, TIM_OC1, duty_A*Attenuation*PWM_PERIOD_ARR);
  /* Set the capture compare value for OC1. */
  timer_set_oc_value(TIM1, TIM_OC2, duty_B*Attenuation*PWM_PERIOD_ARR);
  /* Set the capture compare value for OC1. */
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
float psi_rotating_angle_SVM=0.0f;
float w_r=0;
float t_e=0.0f;

float psi_s_ref=0.0f;
//float t_e_ref=10.0f;

int   d_psi=0.0f;
int   d_te=0.0f;
float psi_delta_percentage=5.0f;
float t_e_delta_percentage=25.0f;//0.8f;

//SVM variables
float phase_advance_SVM=0.0f;
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



void  DTC_SVM(void)
{
  //---------------------------------DTC algorithm--------------------------------------------//

  i_sD     = direct_stator_current_i_sD     (i_sA);
  i_sQ     = quadrature_stator_current_i_sQ (i_sA,i_sB);
  i_s      = vector_magnitude               (i_sQ,i_sD);
  cita_i_s = vector_angle                   (i_sQ,i_sD);

  psi_sD          = direct_stator_flux_linkage_estimator_psi_sD     (TICK_PERIOD,V_sD,i_sD,R_s);
  psi_sQ          = quadrature_stator_flux_linkage_estimator_psi_sQ (TICK_PERIOD,V_sQ,i_sQ,R_s);
  psi_s           = stator_flux_linkage_magnite_psi_s               (psi_sD,psi_sQ);
  psi_s_alpha_SVM = vector_angle                                    (psi_sQ,psi_sD);
  w_r             = (1.0f/(2.0f*PI))*rotor_speed_w_r                                 (psi_sD,psi_sQ,TICK_PERIOD);
  

  t_e       = electromagnetic_torque_estimation_t_e   (psi_sD,i_sQ,psi_sQ,i_sD,pole_pairs);
  //t_e_ref = DTC_torque_reference_PI                 (CUR_FREQ, ref_freq);
  //psi_s_ref = stator_flux_linkage_reference_psi_s_ref (psi_F,t_e_ref,L_sq,pole_pairs);

  

  psi_s_ref = psi_F;


  //--------------------------------SVM algorithm--------------------------------------------//

  //phase_advance_SVM=-100.0f/80.0f;
  //sensorless_pi_controller(ref_freq,w_r/(2.0f*PI),&attenuation,&phase_advance_SVM);
  //ref_freq=30.0f;

  if (dtc_on)
  {
/*
    if (w_r/(2.0f*PI < ref_freq)) 
    {  
      //sensorless_open_loop(&ref_freq_SVM, &attenuation,PWMFREQ_F,120.0f,0.5f);
      //phase_advance_SVM = psi_advance_calculator(ref_freq_SVM,PWMFREQ_F);
      psi_rotating_angle =  psi_advance_calculator(ref_freq_SVM,PWMFREQ_F);
    }

    sensorless_open_loop(&ref_freq_SVM, &attenuation,PWMFREQ_F,120.0f,0.5f);
    psi_rotating_angle_SVM = psi_advance_calculator(ref_freq_SVM,PWMFREQ_F);
*/

    psi_finitite_state_machine (ref_freq_SVM,w_r, &psi_rotating_angle_SVM);

  }

//V_sD                   = SVM_V_s_ref_D               (psi_s_ref,psi_s,psi_s_alpha_SVM,phase_advance_SVM,i_sD,R_s,TICK_PERIOD);
//V_sQ                   = SVM_V_s_ref_Q               (psi_s_ref,psi_s,psi_s_alpha_SVM,phase_advance_SVM,i_sQ,R_s,TICK_PERIOD);
  V_sD                   = SVM_V_s_ref_D               (psi_s_ref,psi_s,psi_s_alpha_SVM,psi_rotating_angle_SVM,i_sD,R_s,TICK_PERIOD);
  V_sQ                   = SVM_V_s_ref_Q               (psi_s_ref,psi_s,psi_s_alpha_SVM,psi_rotating_angle_SVM,i_sQ,R_s,TICK_PERIOD);

  V_s                    = vector_magnitude            (V_sQ,V_sD);
  cita_V_s               = vector_angle                (V_sQ,V_sD);
  V_s_ref_relative_angle = SVM_V_s_relative_angle      (cita_V_s);
                           //SVM_Maximum_allowed_V_s_ref (&V_s,U_d);
			   SVM_Maximum_allowed_V_s_ref (&V_s,U_d*attenuation);

  T1       = SVM_T1       (1.0f,V_s,U_d*2.0f/3.0f, V_s_ref_relative_angle);
  T2       = SVM_T2       (1.0f,V_s,U_d*2.0f/3.0f, V_s_ref_relative_angle);
//T1       = SVM_T1       (1.0f,V_s,attenuation*U_d*2.0f/3.0f, V_s_ref_relative_angle);
//T2       = SVM_T2       (1.0f,V_s,attenuation*U_d*2.0f/3.0f, V_s_ref_relative_angle);

  T_min_on = SVM_T_min_on (1.0f, T1, T2);
  T_med_on = SVM_T_med_on (T_min_on, T1,T2,cita_V_s);
  T_max_on = SVM_T_max_on (T_med_on,T1,T2,cita_V_s);
   

  if (dtc_on)
  {
    SVM_phase_duty_cycles           (&duty_a, &duty_b, &duty_c, cita_V_s,T_max_on,T_med_on,T_min_on);
  //SVM_voltage_switch_inverter_VSI ( duty_a,  duty_b,  duty_c, attenuation);
    SVM_voltage_switch_inverter_VSI ( duty_a,  duty_b,  duty_c, 1.0f);
 
    if (first_dtc==true)
    {
      first_dtc=false;
      collecting_current=true;
    }
  }


}



