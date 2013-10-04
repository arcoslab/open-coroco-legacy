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
  return ( psi_s_ref*cosf(psi_s_angle+phase_advance) - psi_s*cos(psi_s_angle) )/T_s  +  i_sD*R_s; 
}

float SVM_V_s_ref_Q(float psi_s_ref, float psi_s, float psi_s_angle, float phase_advance,float i_sQ, float R_s,float T_s)
{
  return ( psi_s_ref*sinf(psi_s_angle+phase_advance) - psi_s*sin(psi_s_angle) )/T_s  +  i_sQ*R_s; 
}

float SVM_T1(float T_s,float U_s, float U1, float V_s_ref_relative_angle)
{
  return T_s*(U_s/U1)*( sinf(60.0f*PI/180.0f-V_s_ref_relative_angle)/sinf(60.0f*PI/180.0f) );
}

float SVM_T2(float T_s,float U_s, float U2, float V_s_ref_relative_angle)
{
  return T_s*(U_s/U2)*( sinf(V_s_ref_relative_angle)/sinf(60.0f*PI/180.0f) ); 
}

float SVM_Taon(float T_s, float T1, float T2)
{
  return (T_s-T1-T2)/2.0f;
}

float SVM_Tbon(float Taon, float T1)
{
  return (Taon+T1);
}

float SVM_Tcon(float Tbon, float T2)
{
  return (Tbon+T2);
}

void SVM_phase_duty_cycles(float *duty_A, float *duty_B, float *duty_C,float V_s_ref_angle,float Taon, float Tbon,float Tcon)
{
  //sector S1
  if      ( V_s_ref_angle>=  0.0f && V_s_ref_angle< 60.0f) {
                                                             *duty_A=Tbon;     
                                                             *duty_B=Taon;
                                                             *duty_C=Tcon;
                                                           }
  //sector S2
  else if ( V_s_ref_angle>= 60.0f && V_s_ref_angle<120.0f) {
                                                             *duty_A=Taon;     
                                                             *duty_B=Tcon;
                                                             *duty_C=Tbon;
                                                           }
  //sector S3
  else if ( V_s_ref_angle>=120.0f && V_s_ref_angle<180.0f) {
                                                             *duty_A=Taon;     
                                                             *duty_B=Tbon;
                                                             *duty_C=Tcon;
                                                           }
  //sector S4
  else if ( V_s_ref_angle>=180.0f && V_s_ref_angle<240.0f) {
                                                             *duty_A=Tcon;     
                                                             *duty_B=Tbon;
                                                             *duty_C=Taon;
                                                           }
  //sector S5
  else if ( V_s_ref_angle>=240.0f && V_s_ref_angle<300.0f) {
                                                             *duty_A=Tbon;     
                                                             *duty_B=Tcon;
                                                             *duty_C=Taon;
                                                           }
  //sector S6
  else if ( V_s_ref_angle>= 300.0f && V_s_ref_angle<360.0f) {
                                                             *duty_A=Tcon;     
                                                             *duty_B=Taon;
                                                             *duty_C=Tbon;
                                                           }
  //failed sector
  else                                                     {
                                                             *duty_A=0.0f;     
                                                             *duty_B=0.0f;
                                                             *duty_C=0.0f;
                                                           }
}

void  DTC_SVM(void)
{

  //switching_states                        (&S_A,&S_B,&S_C);
  //V_sD    =direct_stator_voltage_V_sD     (S_A,S_B,S_C,U_d);
  //V_sQ    =quadrature_stator_voltage_V_SQ (    S_B,S_C,U_d);
  V_sD    =floating_switches_direct_stator_voltage_V_sD     (S_A_f,S_B_f,S_C_f,U_d);
  V_sQ    =floating_switches_quadrature_stator_voltage_V_SQ (      S_B_f,S_C_f,U_d);
 

  V_s     =vector_magnitude               (V_sQ,V_sD);
  cita_V_s=vector_angle                   (V_sQ,V_sD);

  i_sD    =direct_stator_current_i_sD     (i_sA);
  i_sQ    =quadrature_stator_current_i_sQ (i_sA,i_sB);
  i_s     =vector_magnitude               (i_sQ,i_sD);
  cita_i_s=vector_angle                   (i_sQ,i_sD);

  psi_sD   =direct_stator_flux_linkage_estimator_psi_sD     (TICK_PERIOD,V_sD,i_sD,R_s);
  psi_sQ   =quadrature_stator_flux_linkage_estimator_psi_sQ (TICK_PERIOD,V_sQ,i_sQ,R_s);
  psi_s    =stator_flux_linkage_magnite_psi_s               (psi_sD,psi_sQ);
  psi_alpha=stator_flux_linkage_sector_alpha                (psi_sD,psi_sQ);
  w_r=rotor_speed_w_r(psi_sD,psi_sQ,TICK_PERIOD);

  t_e      =electromagnetic_torque_estimation_t_e(psi_sD,i_sQ,psi_sQ,i_sD,pole_pairs);
  //t_e_ref=DTC_torque_reference_PI(CUR_FREQ, ref_freq);

  psi_s_ref=stator_flux_linkage_reference_psi_s_ref(psi_F,t_e_ref,L_sq,pole_pairs);


  d_psi=stator_flux_linkage_hysteresis_controller_d_psi   (psi_s_ref, psi_s,psi_delta_percentage);
  d_te =electromagnetic_torque_hysteresis_controller_d_te (t_e_ref  , t_e  ,t_e_delta_percentage);


  optimal_voltage_switching_vector_selection_table(d_psi,d_te,psi_alpha,&S_A,&S_B,&S_C);

if (dtc_on)
{
  voltage_switch_inverter_VSI(S_A,S_B,S_C,i_sA,i_sB);
  
}

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
float w_r=0;
float t_e=0.0f;

float psi_s_ref=0.0f;
//float t_e_ref=10.0f;

int   d_psi=0.0f;
int   d_te=0.0f;
float psi_delta_percentage=5.0f;
float t_e_delta_percentage=25.0f;//0.8f;


//motor parameters;
float R_s        = R_s_0;
float pole_pairs = pole_pairs_0;
float L_sq       = L_s_q_0;
float psi_F      = psi_F_0;

