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


#define COLLECTING_SPEED_DELAY 0	

void collecting_floating_data(void)
{

  static int countaa=0;
  static int delay  =0;
  static int absotule_time=0;
  
  if (collecting_speed==true)
  {

    if (countaa<SAMPLES)
    {

      if (delay>COLLECTING_SPEED_DELAY || (absotule_time==0) ) 
      {
/*
        data_V_s          [countaa] = V_s;
        data_w_r          [countaa] = w_r;
        data_ref_freq_SVM [countaa] = ref_freq_SVM;
        
        data_state_SVM    [countaa] = state;
        data_CUR_FREQ     [countaa] = CUR_FREQ;
*/
        
        collecting_samples(countaa);
        timer[countaa] = absotule_time;
        countaa++;
        delay=0;
      }
      delay++;
    }

    else 
    {
      collecting_speed=false;
      print_current=true;
    }
    absotule_time++;
  }
  
  else 
  {
    absotule_time=0;
    countaa=0;
    delay=0;    
  }
  

}
/*
void collecting_speed_data(void)
{
  bool fin;
  fin=collecting_floating_data(true);

  if (collecting_speed==true)
  {
    if (fin)==false)
    {

    }
    else 
    {
      collecting_floating_data(false);
      collecting_speed = false;
      print_current      = true;
    }
  }

}
*/
void collecting_samples(int sample_counter)
{

        data_V_s          [sample_counter] = V_s;
        data_w_r          [sample_counter] = w_r;
        data_ref_freq_SVM [sample_counter] = ref_freq_SVM;
        //timer             [sample_counter] = absotule_time;
        data_state_SVM    [sample_counter] = state;
        data_CUR_FREQ     [sample_counter] = CUR_FREQ;

        data_psi_sD    [sample_counter]=psi_sD;
        data_psi_sQ    [sample_counter]=psi_sQ;
        data_psi_s     [sample_counter]=psi_s;
        data_psi_alpha [sample_counter]=psi_alpha;
        data_CUR_FREQ  [sample_counter]=CUR_FREQ;
        data_U_d       [sample_counter]=U_d;
        timer          [sample_counter]=flux_linkage_capture_counter;
        
        //---------------------------------------
        current_data_i_sB[sample_counter]=i_sB;

	data_CUR_FREQ[sample_counter]=CUR_FREQ;	
/*
        data_S_A[sample_counter]=S_A;
        data_S_B[sample_counter]=S_B;
        data_S_C[sample_counter]=S_C;

        data_S_A_f[sample_counter]=S_A_f;
        data_S_B_f[sample_counter]=S_B_f;
        data_S_C_f[sample_counter]=S_C_f;
*/
        data_i_sA [sample_counter]=i_sA;
        data_i_sB [sample_counter]=i_sB;
        data_U_d  [sample_counter]=U_d;

        data_i_sD[sample_counter]=i_sD;
        data_i_sQ[sample_counter]=i_sQ;
        data_i_s[sample_counter]=i_s;
        data_cita_i_s[sample_counter]=cita_i_s;

        data_V_sD[sample_counter]=V_sD;
        data_V_sQ[sample_counter]=V_sQ;
        data_V_s [sample_counter]=V_s;
        data_cita_V_s[sample_counter]=cita_V_s;
        data_cita_V_s_relative_angle[sample_counter]=cita_V_s;

        data_psi_sD[sample_counter]=psi_sD;
        data_psi_sQ[sample_counter]=psi_sQ;
        data_psi_s [sample_counter]=psi_s;
        data_psi_alpha[sample_counter]=psi_alpha;
        data_psi_s_alpha_SVM[sample_counter]=psi_s_alpha_SVM;

        data_t_e[sample_counter]=t_e;

        data_psi_s_ref[sample_counter]=psi_s_ref;
        data_t_e_ref[sample_counter]=t_e_ref;

        data_d_psi[sample_counter]=d_psi;
        data_d_te[sample_counter]=d_te;
        data_psi_delta_percentage[sample_counter]=psi_delta_percentage;
        data_t_e_delta_percentage[sample_counter]=t_e_delta_percentage;
/*
        data_R_s[sample_counter]=R_s;
        data_pole_pairs[sample_counter]=pole_pairs;
        data_L_sq[sample_counter]=L_sq;
        data_psi_F[sample_counter]=psi_F;
        data_optimal_voltage_vector[sample_counter]=optimal_voltage_vector;
*/
        PID_angle[sample_counter]=cmd_angle;//angle_hall1;
        data_w_r[sample_counter]=w_r;   

        data_duty_a[sample_counter]=duty_a;
        data_duty_b[sample_counter]=duty_b;
        data_duty_c[sample_counter]=duty_c;

        data_phase_advance_SVM[sample_counter]=phase_advance_SVM;
        data_V_s_ref_relative_angle[sample_counter]=V_s_ref_relative_angle;
        data_T1[sample_counter]=T1;
        data_T2[sample_counter]=T2;
        data_T_min_on[sample_counter]=T_min_on;
        data_T_med_on[sample_counter]=T_med_on;
        data_T_max_on[sample_counter]=T_max_on;
        data_attenuation[sample_counter]=attenuation;
        data_pi_control_SVM[sample_counter]	=SVM_pi_control;
        data_rotating_angle_SVM[sample_counter] =psi_rotating_angle_SVM;
}


void collecting_data(void)
{

    if (collecting_current==true)
    {

        if (current_counter<SAMPLES/8)
      {

        collecting_samples(current_counter);

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
        collecting_samples(current_counter);

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
        collecting_samples(current_counter);

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
        collecting_samples(current_counter);

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
        collecting_samples(current_counter);

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
        collecting_samples(current_counter);

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
        collecting_samples(current_counter);

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
        collecting_samples(current_counter);

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
        collecting_samples(current_counter);

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


//-------------------------------------------------------------------------
/*
void collecting_data_old(void)
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
*/
