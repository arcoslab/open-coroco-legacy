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


void SVM_starting_open_loop(
                            bool  open_loop,
                            float * VsD, 
                            float *VsQ, 
                            float U_d,
                            float maximum_open_loop_speed,
                            float frequency,
                            float reference_frequency
                           );

float SVM_speed_close_loop_of_voltage_frequency(
                                                float reference_frequency, 
                                                float frequency,
                                                bool close_loop_active, 
                                                float* VsD, 
                                                float* VsQ,
                                                float Ud,
                                                bool shutdown
                                               );

void SVM_speed_close_loop(float reference_frequency, float frequency,bool close_loop_active, float* VsD, float*     VsQ);
void SVM_torque_close_loop(float reference_torque, float torque,bool close_loop_active, float* VsD, float* VsQ);    
void SVM_loop_control(float frequency,float maximum_open_loop_frequency,float te_ref, float freq_ref, bool* open_loop, bool* close_loop);



float SVM_V_s_ref_D               (float psi_s_ref, float psi_s, float psi_s_angle, float phase_advance,float i_sD, float R_s,float T_s);
float SVM_V_s_ref_Q               (float psi_s_ref, float psi_s, float psi_s_angle, float phase_advance,float i_sQ, float R_s,float T_s);

void  SVM_Maximum_allowed_V_s_ref (float* V_sD,float* V_sQ  ,float* V_s_ref,float U_d,bool* increase);
float SVM_V_s_relative_angle      (float V_s_angle);
float SVM_T1                      (float T_s,float U_s, float U1, float V_s_ref_relative_angle);
float SVM_T2                      (float T_s,float U_s, float U2, float V_s_ref_relative_angle);
/*
float SVM_Taon(float T_s, float T1, float T2);
float SVM_Tbon(float Taon, float T1);
float SVM_Tcon(float Tbon, float T2);
*/
float SVM_T_min_on (float T_s, float T1, float T2);
float SVM_T_med_on (float T_min_on, float T1,float T2,float V_s_ref_angle);
float SVM_T_max_on (float T_med_on, float T1,float T2,float V_s_ref_angle);

void  SVM_phase_duty_cycles           (float *duty_A, float *duty_B, float *duty_C,float V_s_ref_angle,float Taon, float Tbon,float Tcon);
void  shutdown_SVM_speed (float reference_frequency,float actual_frequency,bool* shutdown);
void shutdown_SVM_position (float reference_frequency,float reference_position_change,float reference_position,float actual_position,bool* shutdown);
void shutdown_SVM_torque (float torque_reference,float actual_torque,bool* shutdown);
void shutdown_admittance_speed (float reference_frequency,float actual_frequency,bool* shutdown);
void simple_shutdown(float control_signal,bool* shutdown);
void  SVM_voltage_switch_inverter_VSI (float duty_A,float duty_B,float duty_C,bool shutdown);
void  DTC_SVM                         (void);


void speed_PID_no_SVM(void);

void catching_NaNs (void);
void catching_NaNs_data (float data_4, float data_5, float data_6);
void catched_values (void);
void catched_values_data (float data_4, float data_5, float data_6);
