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

void collecting_data(void)
{

    if (collecting_current==true)
    {

        if (current_counter<SAMPLES/8)
      {

        data_psi_sD[current_counter]=psi_sD;
        data_psi_sQ[current_counter]=psi_sQ;
        data_psi_s [current_counter]=psi_s;
        data_psi_alpha[current_counter]=psi_alpha;
        data_CUR_FREQ[current_counter]=CUR_FREQ;
        data_U_d  [current_counter]=U_d;
        timer[current_counter]=flux_linkage_capture_counter;
        
        //---------------------------------------
        current_data_i_sB[current_counter]=i_sB;

	data_CUR_FREQ[current_counter]=CUR_FREQ;	

        data_S_A[current_counter]=S_A;
        data_S_B[current_counter]=S_B;
        data_S_C[current_counter]=S_C;

        data_S_A_f[current_counter]=S_A_f;
        data_S_B_f[current_counter]=S_B_f;
        data_S_C_f[current_counter]=S_C_f;

        data_i_sA [current_counter]=i_sA;
        data_i_sB [current_counter]=i_sB;
        data_U_d  [current_counter]=U_d;

        data_i_sD[current_counter]=i_sD;
        data_i_sQ[current_counter]=i_sQ;
        data_i_s[current_counter]=i_s;
        data_cita_i_s[current_counter]=cita_i_s;

        data_V_sD[current_counter]=V_sD;
        data_V_sQ[current_counter]=V_sQ;
        data_V_s [current_counter]=V_s;
        data_cita_V_s[current_counter]=cita_V_s;

        data_psi_sD[current_counter]=psi_sD;
        data_psi_sQ[current_counter]=psi_sQ;
        data_psi_s [current_counter]=psi_s;
        data_psi_alpha[current_counter]=psi_alpha;

        data_t_e[current_counter]=t_e;

        data_psi_s_ref[current_counter]=psi_s_ref;
        data_t_e_ref[current_counter]=t_e_ref;

        data_d_psi[current_counter]=d_psi;
        data_d_te[current_counter]=d_te;
        data_psi_delta_percentage[current_counter]=psi_delta_percentage;
        data_t_e_delta_percentage[current_counter]=t_e_delta_percentage;

        data_R_s[current_counter]=R_s;
        data_pole_pairs[current_counter]=pole_pairs;
        data_L_sq[current_counter]=L_sq;
        data_psi_F[current_counter]=psi_F;
        data_optimal_voltage_vector[current_counter]=optimal_voltage_vector;
        PID_angle[current_counter]=cmd_angle;//angle_hall1;
        data_w_r[current_counter]=w_r;   
        //------------------------------------------

	current_counter++;
        flux_limit_counter_1=flux_linkage_capture_counter;
        flux_linkage_capture_counter++;
        
      }
      else if (flux_linkage_capture_counter<FLUX_LINKAGE_TIMER_DELAY+flux_limit_counter_1)
      {
        flux_linkage_capture_counter++;
      }
      else if(current_counter<SAMPLES/4)
      {
        data_psi_sD[current_counter]=psi_sD;
        data_psi_sQ[current_counter]=psi_sQ;
        data_psi_s [current_counter]=psi_s;
        data_psi_alpha[current_counter]=psi_alpha;
        data_CUR_FREQ[current_counter]=CUR_FREQ;
        data_U_d  [current_counter]=U_d;
        timer[current_counter]=flux_linkage_capture_counter;

        //---------------------------------------
        current_data_i_sB[current_counter]=i_sB;

	data_CUR_FREQ[current_counter]=CUR_FREQ;	

        data_S_A[current_counter]=S_A;
        data_S_B[current_counter]=S_B;
        data_S_C[current_counter]=S_C;

        data_S_A_f[current_counter]=S_A_f;
        data_S_B_f[current_counter]=S_B_f;
        data_S_C_f[current_counter]=S_C_f;

        data_i_sA [current_counter]=i_sA;
        data_i_sB [current_counter]=i_sB;
        data_U_d  [current_counter]=U_d;

        data_i_sD[current_counter]=i_sD;
        data_i_sQ[current_counter]=i_sQ;
        data_i_s[current_counter]=i_s;
        data_cita_i_s[current_counter]=cita_i_s;

        data_V_sD[current_counter]=V_sD;
        data_V_sQ[current_counter]=V_sQ;
        data_V_s [current_counter]=V_s;
        data_cita_V_s[current_counter]=cita_V_s;

        data_psi_sD[current_counter]=psi_sD;
        data_psi_sQ[current_counter]=psi_sQ;
        data_psi_s [current_counter]=psi_s;
        data_psi_alpha[current_counter]=psi_alpha;

        data_t_e[current_counter]=t_e;

        data_psi_s_ref[current_counter]=psi_s_ref;
        data_t_e_ref[current_counter]=t_e_ref;

        data_d_psi[current_counter]=d_psi;
        data_d_te[current_counter]=d_te;
        data_psi_delta_percentage[current_counter]=psi_delta_percentage;
        data_t_e_delta_percentage[current_counter]=t_e_delta_percentage;

        data_R_s[current_counter]=R_s;
        data_pole_pairs[current_counter]=pole_pairs;
        data_L_sq[current_counter]=L_sq;
        data_psi_F[current_counter]=psi_F;
        data_optimal_voltage_vector[current_counter]=optimal_voltage_vector;
        PID_angle[current_counter]=cmd_angle;//angle_hall1;   
        data_w_r[current_counter]=w_r;
        //------------------------------------------

	current_counter++;
        flux_limit_counter_2=flux_linkage_capture_counter;
        flux_linkage_capture_counter++;
      }
      else if (flux_linkage_capture_counter<FLUX_LINKAGE_TIMER_DELAY+flux_limit_counter_2)
      {
        flux_linkage_capture_counter++;
      }
      else if(current_counter<(3*SAMPLES)/8)
      {
        data_psi_sD[current_counter]=psi_sD;
        data_psi_sQ[current_counter]=psi_sQ;
        data_psi_s [current_counter]=psi_s;
        data_psi_alpha[current_counter]=psi_alpha;
        data_CUR_FREQ[current_counter]=CUR_FREQ;
        data_U_d  [current_counter]=U_d;
        timer[current_counter]=flux_linkage_capture_counter;

        //---------------------------------------
        current_data_i_sB[current_counter]=i_sB;

	data_CUR_FREQ[current_counter]=CUR_FREQ;	

        data_S_A[current_counter]=S_A;
        data_S_B[current_counter]=S_B;
        data_S_C[current_counter]=S_C;

        data_S_A_f[current_counter]=S_A_f;
        data_S_B_f[current_counter]=S_B_f;
        data_S_C_f[current_counter]=S_C_f;

        data_i_sA [current_counter]=i_sA;
        data_i_sB [current_counter]=i_sB;
        data_U_d  [current_counter]=U_d;

        data_i_sD[current_counter]=i_sD;
        data_i_sQ[current_counter]=i_sQ;
        data_i_s[current_counter]=i_s;
        data_cita_i_s[current_counter]=cita_i_s;

        data_V_sD[current_counter]=V_sD;
        data_V_sQ[current_counter]=V_sQ;
        data_V_s [current_counter]=V_s;
        data_cita_V_s[current_counter]=cita_V_s;

        data_psi_sD[current_counter]=psi_sD;
        data_psi_sQ[current_counter]=psi_sQ;
        data_psi_s [current_counter]=psi_s;
        data_psi_alpha[current_counter]=psi_alpha;

        data_t_e[current_counter]=t_e;

        data_psi_s_ref[current_counter]=psi_s_ref;
        data_t_e_ref[current_counter]=t_e_ref;

        data_d_psi[current_counter]=d_psi;
        data_d_te[current_counter]=d_te;
        data_psi_delta_percentage[current_counter]=psi_delta_percentage;
        data_t_e_delta_percentage[current_counter]=t_e_delta_percentage;

        data_R_s[current_counter]=R_s;
        data_pole_pairs[current_counter]=pole_pairs;
        data_L_sq[current_counter]=L_sq;
        data_psi_F[current_counter]=psi_F;
        data_optimal_voltage_vector[current_counter]=optimal_voltage_vector;
        PID_angle[current_counter]=cmd_angle;//angle_hall1;   
        data_w_r[current_counter]=w_r;
        //------------------------------------------

	current_counter++;
        flux_limit_counter_3=flux_linkage_capture_counter;
        flux_linkage_capture_counter++;
      }
      else if (flux_linkage_capture_counter<FLUX_LINKAGE_TIMER_DELAY+flux_limit_counter_3)
      {
        flux_linkage_capture_counter++;
      }
      else if(current_counter<SAMPLES/2)
      {
        data_psi_sD[current_counter]=psi_sD;
        data_psi_sQ[current_counter]=psi_sQ;
        data_psi_s [current_counter]=psi_s;
        data_psi_alpha[current_counter]=psi_alpha;
        data_CUR_FREQ[current_counter]=CUR_FREQ;
        data_U_d  [current_counter]=U_d;
        timer[current_counter]=flux_linkage_capture_counter;

        //---------------------------------------
        current_data_i_sB[current_counter]=i_sB;

	data_CUR_FREQ[current_counter]=CUR_FREQ;	

        data_S_A[current_counter]=S_A;
        data_S_B[current_counter]=S_B;
        data_S_C[current_counter]=S_C;

        data_S_A_f[current_counter]=S_A_f;
        data_S_B_f[current_counter]=S_B_f;
        data_S_C_f[current_counter]=S_C_f;

        data_i_sA [current_counter]=i_sA;
        data_i_sB [current_counter]=i_sB;
        data_U_d  [current_counter]=U_d;

        data_i_sD[current_counter]=i_sD;
        data_i_sQ[current_counter]=i_sQ;
        data_i_s[current_counter]=i_s;
        data_cita_i_s[current_counter]=cita_i_s;

        data_V_sD[current_counter]=V_sD;
        data_V_sQ[current_counter]=V_sQ;
        data_V_s [current_counter]=V_s;
        data_cita_V_s[current_counter]=cita_V_s;

        data_psi_sD[current_counter]=psi_sD;
        data_psi_sQ[current_counter]=psi_sQ;
        data_psi_s [current_counter]=psi_s;
        data_psi_alpha[current_counter]=psi_alpha;

        data_t_e[current_counter]=t_e;

        data_psi_s_ref[current_counter]=psi_s_ref;
        data_t_e_ref[current_counter]=t_e_ref;

        data_d_psi[current_counter]=d_psi;
        data_d_te[current_counter]=d_te;
        data_psi_delta_percentage[current_counter]=psi_delta_percentage;
        data_t_e_delta_percentage[current_counter]=t_e_delta_percentage;

        data_R_s[current_counter]=R_s;
        data_pole_pairs[current_counter]=pole_pairs;
        data_L_sq[current_counter]=L_sq;
        data_psi_F[current_counter]=psi_F;
        data_optimal_voltage_vector[current_counter]=optimal_voltage_vector;
        PID_angle[current_counter]=cmd_angle;//angle_hall1;  
        data_w_r[current_counter]=w_r; 
        //------------------------------------------

	current_counter++;
        flux_limit_counter_4=flux_linkage_capture_counter;
        flux_linkage_capture_counter++;
      }
            
      else if (flux_linkage_capture_counter<FLUX_LINKAGE_TIMER_DELAY+flux_limit_counter_4)
      {
        flux_linkage_capture_counter++;
      }
      else if(current_counter<5*SAMPLES/8)
      {
        data_psi_sD[current_counter]=psi_sD;
        data_psi_sQ[current_counter]=psi_sQ;
        data_psi_s [current_counter]=psi_s;
        data_psi_alpha[current_counter]=psi_alpha;
        data_CUR_FREQ[current_counter]=CUR_FREQ;
        data_U_d  [current_counter]=U_d;
        timer[current_counter]=flux_linkage_capture_counter;

        //---------------------------------------
        current_data_i_sB[current_counter]=i_sB;

	data_CUR_FREQ[current_counter]=CUR_FREQ;	

        data_S_A[current_counter]=S_A;
        data_S_B[current_counter]=S_B;
        data_S_C[current_counter]=S_C;

        data_S_A_f[current_counter]=S_A_f;
        data_S_B_f[current_counter]=S_B_f;
        data_S_C_f[current_counter]=S_C_f;

        data_i_sA [current_counter]=i_sA;
        data_i_sB [current_counter]=i_sB;
        data_U_d  [current_counter]=U_d;

        data_i_sD[current_counter]=i_sD;
        data_i_sQ[current_counter]=i_sQ;
        data_i_s[current_counter]=i_s;
        data_cita_i_s[current_counter]=cita_i_s;

        data_V_sD[current_counter]=V_sD;
        data_V_sQ[current_counter]=V_sQ;
        data_V_s [current_counter]=V_s;
        data_cita_V_s[current_counter]=cita_V_s;

        data_psi_sD[current_counter]=psi_sD;
        data_psi_sQ[current_counter]=psi_sQ;
        data_psi_s [current_counter]=psi_s;
        data_psi_alpha[current_counter]=psi_alpha;

        data_t_e[current_counter]=t_e;

        data_psi_s_ref[current_counter]=psi_s_ref;
        data_t_e_ref[current_counter]=t_e_ref;

        data_d_psi[current_counter]=d_psi;
        data_d_te[current_counter]=d_te;
        data_psi_delta_percentage[current_counter]=psi_delta_percentage;
        data_t_e_delta_percentage[current_counter]=t_e_delta_percentage;

        data_R_s[current_counter]=R_s;
        data_pole_pairs[current_counter]=pole_pairs;
        data_L_sq[current_counter]=L_sq;
        data_psi_F[current_counter]=psi_F;
        data_optimal_voltage_vector[current_counter]=optimal_voltage_vector;
        PID_angle[current_counter]=cmd_angle;//angle_hall1; 
        data_w_r[current_counter]=w_r;  
        //------------------------------------------

	current_counter++;
        flux_limit_counter_5=flux_linkage_capture_counter;
        flux_linkage_capture_counter++;
      }

      else if (flux_linkage_capture_counter<FLUX_LINKAGE_TIMER_DELAY+flux_limit_counter_5)
      {
        flux_linkage_capture_counter++;
      }
      else if(current_counter<3*SAMPLES/4)
      {
        data_psi_sD[current_counter]=psi_sD;
        data_psi_sQ[current_counter]=psi_sQ;
        data_psi_s [current_counter]=psi_s;
        data_psi_alpha[current_counter]=psi_alpha;
        data_CUR_FREQ[current_counter]=CUR_FREQ;
        data_U_d  [current_counter]=U_d;
        timer[current_counter]=flux_linkage_capture_counter;

        //---------------------------------------
        current_data_i_sB[current_counter]=i_sB;

	data_CUR_FREQ[current_counter]=CUR_FREQ;	

        data_S_A[current_counter]=S_A;
        data_S_B[current_counter]=S_B;
        data_S_C[current_counter]=S_C;

        data_S_A_f[current_counter]=S_A_f;
        data_S_B_f[current_counter]=S_B_f;
        data_S_C_f[current_counter]=S_C_f;

        data_i_sA [current_counter]=i_sA;
        data_i_sB [current_counter]=i_sB;
        data_U_d  [current_counter]=U_d;

        data_i_sD[current_counter]=i_sD;
        data_i_sQ[current_counter]=i_sQ;
        data_i_s[current_counter]=i_s;
        data_cita_i_s[current_counter]=cita_i_s;

        data_V_sD[current_counter]=V_sD;
        data_V_sQ[current_counter]=V_sQ;
        data_V_s [current_counter]=V_s;
        data_cita_V_s[current_counter]=cita_V_s;

        data_psi_sD[current_counter]=psi_sD;
        data_psi_sQ[current_counter]=psi_sQ;
        data_psi_s [current_counter]=psi_s;
        data_psi_alpha[current_counter]=psi_alpha;

        data_t_e[current_counter]=t_e;

        data_psi_s_ref[current_counter]=psi_s_ref;
        data_t_e_ref[current_counter]=t_e_ref;

        data_d_psi[current_counter]=d_psi;
        data_d_te[current_counter]=d_te;
        data_psi_delta_percentage[current_counter]=psi_delta_percentage;
        data_t_e_delta_percentage[current_counter]=t_e_delta_percentage;

        data_R_s[current_counter]=R_s;
        data_pole_pairs[current_counter]=pole_pairs;
        data_L_sq[current_counter]=L_sq;
        data_psi_F[current_counter]=psi_F;
        data_optimal_voltage_vector[current_counter]=optimal_voltage_vector;
        PID_angle[current_counter]=cmd_angle;//angle_hall1;   
        data_w_r[current_counter]=w_r;
        //------------------------------------------

	current_counter++;
        flux_limit_counter_6=flux_linkage_capture_counter;
        flux_linkage_capture_counter++;
      }

      else if (flux_linkage_capture_counter<FLUX_LINKAGE_TIMER_DELAY+flux_limit_counter_6)
      {
        flux_linkage_capture_counter++;
      }
      else if(current_counter<7*SAMPLES/8)
      {
        data_psi_sD[current_counter]=psi_sD;
        data_psi_sQ[current_counter]=psi_sQ;
        data_psi_s [current_counter]=psi_s;
        data_psi_alpha[current_counter]=psi_alpha;
        data_CUR_FREQ[current_counter]=CUR_FREQ;
        data_U_d  [current_counter]=U_d;
        timer[current_counter]=flux_linkage_capture_counter;
        
        //---------------------------------------
        current_data_i_sB[current_counter]=i_sB;

	data_CUR_FREQ[current_counter]=CUR_FREQ;	

        data_S_A[current_counter]=S_A;
        data_S_B[current_counter]=S_B;
        data_S_C[current_counter]=S_C;

        data_S_A_f[current_counter]=S_A_f;
        data_S_B_f[current_counter]=S_B_f;
        data_S_C_f[current_counter]=S_C_f;

        data_i_sA [current_counter]=i_sA;
        data_i_sB [current_counter]=i_sB;
        data_U_d  [current_counter]=U_d;

        data_i_sD[current_counter]=i_sD;
        data_i_sQ[current_counter]=i_sQ;
        data_i_s[current_counter]=i_s;
        data_cita_i_s[current_counter]=cita_i_s;

        data_V_sD[current_counter]=V_sD;
        data_V_sQ[current_counter]=V_sQ;
        data_V_s [current_counter]=V_s;
        data_cita_V_s[current_counter]=cita_V_s;

        data_psi_sD[current_counter]=psi_sD;
        data_psi_sQ[current_counter]=psi_sQ;
        data_psi_s [current_counter]=psi_s;
        data_psi_alpha[current_counter]=psi_alpha;

        data_t_e[current_counter]=t_e;

        data_psi_s_ref[current_counter]=psi_s_ref;
        data_t_e_ref[current_counter]=t_e_ref;

        data_d_psi[current_counter]=d_psi;
        data_d_te[current_counter]=d_te;
        data_psi_delta_percentage[current_counter]=psi_delta_percentage;
        data_t_e_delta_percentage[current_counter]=t_e_delta_percentage;

        data_R_s[current_counter]=R_s;
        data_pole_pairs[current_counter]=pole_pairs;
        data_L_sq[current_counter]=L_sq;
        data_psi_F[current_counter]=psi_F;
        data_optimal_voltage_vector[current_counter]=optimal_voltage_vector;
        PID_angle[current_counter]=cmd_angle;//angle_hall1;   
        data_w_r[current_counter]=w_r;
        //------------------------------------------

	current_counter++;
        flux_limit_counter_7=flux_linkage_capture_counter;
        flux_linkage_capture_counter++;
      }

      else if (flux_linkage_capture_counter<FLUX_LINKAGE_TIMER_DELAY+flux_limit_counter_7)
      {
        flux_linkage_capture_counter++;
      }
      else if(current_counter<SAMPLES)
      {
        data_psi_sD[current_counter]=psi_sD;
        data_psi_sQ[current_counter]=psi_sQ;
        data_psi_s [current_counter]=psi_s;
        data_psi_alpha[current_counter]=psi_alpha;
        data_CUR_FREQ[current_counter]=CUR_FREQ;
        data_U_d  [current_counter]=U_d;
        timer[current_counter]=flux_linkage_capture_counter;
        
        //---------------------------------------
        current_data_i_sB[current_counter]=i_sB;

	data_CUR_FREQ[current_counter]=CUR_FREQ;	

        data_S_A[current_counter]=S_A;
        data_S_B[current_counter]=S_B;
        data_S_C[current_counter]=S_C;

        data_S_A_f[current_counter]=S_A_f;
        data_S_B_f[current_counter]=S_B_f;
        data_S_C_f[current_counter]=S_C_f;

        data_i_sA [current_counter]=i_sA;
        data_i_sB [current_counter]=i_sB;
        data_U_d  [current_counter]=U_d;

        data_i_sD[current_counter]=i_sD;
        data_i_sQ[current_counter]=i_sQ;
        data_i_s[current_counter]=i_s;
        data_cita_i_s[current_counter]=cita_i_s;

        data_V_sD[current_counter]=V_sD;
        data_V_sQ[current_counter]=V_sQ;
        data_V_s [current_counter]=V_s;
        data_cita_V_s[current_counter]=cita_V_s;

        data_psi_sD[current_counter]=psi_sD;
        data_psi_sQ[current_counter]=psi_sQ;
        data_psi_s [current_counter]=psi_s;
        data_psi_alpha[current_counter]=psi_alpha;

        data_t_e[current_counter]=t_e;

        data_psi_s_ref[current_counter]=psi_s_ref;
        data_t_e_ref[current_counter]=t_e_ref;

        data_d_psi[current_counter]=d_psi;
        data_d_te[current_counter]=d_te;
        data_psi_delta_percentage[current_counter]=psi_delta_percentage;
        data_t_e_delta_percentage[current_counter]=t_e_delta_percentage;

        data_R_s[current_counter]=R_s;
        data_pole_pairs[current_counter]=pole_pairs;
        data_L_sq[current_counter]=L_sq;
        data_psi_F[current_counter]=psi_F;
        data_optimal_voltage_vector[current_counter]=optimal_voltage_vector;
        PID_angle[current_counter]=cmd_angle;//angle_hall1;   
        data_w_r[current_counter]=w_r;
        //------------------------------------------

	current_counter++;
        flux_limit_counter_8=flux_linkage_capture_counter;
        flux_linkage_capture_counter++;
      }

      else if (flux_linkage_capture_counter<FLUX_LINKAGE_TIMER_DELAY+flux_limit_counter_8)
      {
        flux_linkage_capture_counter++;
      }
      else if(current_counter<SAMPLES)
      { 
        data_psi_sD[current_counter]=psi_sD;
        data_psi_sQ[current_counter]=psi_sQ;
        data_psi_s [current_counter]=psi_s;
        data_psi_alpha[current_counter]=psi_alpha;
        data_CUR_FREQ[current_counter]=CUR_FREQ;
        data_U_d  [current_counter]=U_d;
        timer[current_counter]=flux_linkage_capture_counter;


        //---------------------------------------
        current_data_i_sB[current_counter]=i_sB;

	data_CUR_FREQ[current_counter]=CUR_FREQ;	

        data_S_A[current_counter]=S_A;
        data_S_B[current_counter]=S_B;
        data_S_C[current_counter]=S_C;

        data_S_A_f[current_counter]=S_A_f;
        data_S_B_f[current_counter]=S_B_f;
        data_S_C_f[current_counter]=S_C_f;

        data_i_sA [current_counter]=i_sA;
        data_i_sB [current_counter]=i_sB;
        data_U_d  [current_counter]=U_d;

        data_i_sD[current_counter]=i_sD;
        data_i_sQ[current_counter]=i_sQ;
        data_i_s[current_counter]=i_s;
        data_cita_i_s[current_counter]=cita_i_s;

        data_V_sD[current_counter]=V_sD;
        data_V_sQ[current_counter]=V_sQ;
        data_V_s [current_counter]=V_s;
        data_cita_V_s[current_counter]=cita_V_s;

        data_psi_sD[current_counter]=psi_sD;
        data_psi_sQ[current_counter]=psi_sQ;
        data_psi_s [current_counter]=psi_s;
        data_psi_alpha[current_counter]=psi_alpha;

        data_t_e[current_counter]=t_e;

        data_psi_s_ref[current_counter]=psi_s_ref;
        data_t_e_ref[current_counter]=t_e_ref;

        data_d_psi[current_counter]=d_psi;
        data_d_te[current_counter]=d_te;
        data_psi_delta_percentage[current_counter]=psi_delta_percentage;
        data_t_e_delta_percentage[current_counter]=t_e_delta_percentage;

        data_R_s[current_counter]=R_s;
        data_pole_pairs[current_counter]=pole_pairs;
        data_L_sq[current_counter]=L_sq;
        data_psi_F[current_counter]=psi_F;
        data_optimal_voltage_vector[current_counter]=optimal_voltage_vector;
        PID_angle[current_counter]=cmd_angle;//angle_hall1;   
        data_w_r[current_counter]=w_r;
        //------------------------------------------


	current_counter++;
        flux_limit_counter_9=flux_linkage_capture_counter;
        flux_linkage_capture_counter++;
      }

      else
      {
        current_counter=0;
        collecting_current=false;
        print_current=true;
        flux_linkage_capture_counter=0;
        flux_limit_counter_1=0;
        flux_limit_counter_2=0;
        flux_limit_counter_3=0;
        flux_limit_counter_4=0;
        flux_limit_counter_5=0;
        flux_limit_counter_6=0;
        flux_limit_counter_7=0;
        flux_limit_counter_8=0;
        flux_limit_counter_9=0;
      }
    }

}
