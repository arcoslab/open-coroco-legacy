/*
 * Copyright (C) 2013 ARCOS-Lab Universidad de Costa Rica
 * Author: Federico Ruiz Ugalde <memeruiz@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#define SAMPLES 400
#define FLUX_LINKAGE_TIMER_DELAY 0

//printing buffers
float hall_angle=0.0f;
float PID_angle[SAMPLES];
float current_data_i_sA[SAMPLES];
float current_data_i_sB[SAMPLES];
float switching_data_SA[SAMPLES];
float switching_data_SB[SAMPLES];
float switching_data_SC[SAMPLES];

bool  inductance_measure=true;

int current_counter=0;  
bool print_current=false;
bool first_movement=false;
float data_CUR_FREQ[SAMPLES];
/*
int data_S_A[SAMPLES];
int data_S_B[SAMPLES];
int data_S_C[SAMPLES];

float data_S_A_f[SAMPLES];
float data_S_B_f[SAMPLES];
float data_S_C_f[SAMPLES];
*/
float data_i_sA [SAMPLES];
float data_i_sB [SAMPLES];

float data_U_d  [SAMPLES];

float data_i_sD[SAMPLES];
float data_i_sQ[SAMPLES];
float data_i_s[SAMPLES];
float data_cita_i_s[SAMPLES];

float data_V_sD[SAMPLES];
float data_V_sQ[SAMPLES];
float data_V_s [SAMPLES];
float data_cita_V_s[SAMPLES];
float data_cita_V_s_relative_angle[SAMPLES];

float data_psi_sD[SAMPLES];
float data_psi_sQ[SAMPLES];
float data_psi_s [SAMPLES];
int   data_psi_alpha[SAMPLES];
float data_psi_s_alpha_SVM[SAMPLES];
float data_w_r[SAMPLES];
int timer[SAMPLES];

float data_t_e[SAMPLES];

float data_psi_s_ref[SAMPLES];
float data_t_e_ref[SAMPLES];

int   data_d_psi[SAMPLES];
int   data_d_te[SAMPLES];
float data_psi_delta_percentage[SAMPLES];
float data_t_e_delta_percentage[SAMPLES];


//motor parameters;
/*
float data_R_s[SAMPLES];
float data_pole_pairs[SAMPLES];
float data_L_sq[SAMPLES];
float data_psi_F[SAMPLES];

int data_optimal_voltage_vector[SAMPLES];
*/
float data_duty_a[SAMPLES];
float data_duty_b[SAMPLES];
float data_duty_c[SAMPLES];

float data_phase_advance_SVM[SAMPLES];
float data_V_s_ref_relative_angle[SAMPLES];
float data_T1[SAMPLES];
float data_T2[SAMPLES];
float data_T_min_on[SAMPLES];
float data_T_med_on[SAMPLES];
float data_T_max_on[SAMPLES];

float data_attenuation[SAMPLES];
float data_pi_control_SVM[SAMPLES];
float data_ref_freq_SVM[SAMPLES];
int data_state_SVM[SAMPLES];
float data_rotating_angle_SVM[SAMPLES];


void tim1_up_tim10_isr(void) 
{

  //oscilloscope flag: start of calculations
  gpio_set(GPIOD, GPIO9);
   
  //Clear the update interrupt flag
  timer_clear_flag(TIM1,  TIM_SR_UIF);

  calc_freq();

  //oscilloscope flag: end of calculations
  gpio_clear(GPIOD, GPIO9);

  //oscilloscope flag: start of ADC current measuring
  gpio_set(GPIOB, GPIO15);

  voltage_measure (ADC1,ADC_CHANNEL1);

}




void adc_isr(void)
{ 
  static int adc_counter=0;
  float V_stm32_A  = 0.0f;
  float V_stm32_B  = 0.0f;
  float V_stm32_Ud = 0.0f;
  float V_shunt_A  = 0.0f;
  float V_shunt_B  = 0.0f;
  

  if (adc_counter==0)
  {
    V_stm32_A = adc_read_regular(ADC1)*(VREF/ADC_CONVERSION_FACTOR);
    V_shunt_A = (V_stm32_A-V_DIFFERENTIAL_AMPLIFIER_REFFERENCE)/G_OP_AMP_A;
    i_sA      = V_shunt_A/R_SHUNT_A;
    
    //oscilloscope flag: start of ADC current measuring 
    gpio_clear(GPIOB, GPIO15);

    voltage_measure (ADC1,ADC_CHANNEL2);
    adc_counter++;
  }
  else if (adc_counter==1)
  {
    V_stm32_B = adc_read_regular(ADC1)*(VREF/ADC_CONVERSION_FACTOR);
    V_shunt_B = (V_stm32_B-V_DIFFERENTIAL_AMPLIFIER_REFFERENCE)/G_OP_AMP_B;
    i_sB      = V_shunt_B/R_SHUNT_B;
    
    //oscilloscope flag: start of ADC Voltage measuring
    gpio_set(GPIOB, GPIO15);

    voltage_measure (ADC1,ADC_CHANNEL3);
    adc_counter++; 
  }
  else
  {
    //oscilloscope flag: end of ADC current measuring
    gpio_clear(GPIOB, GPIO15);

    V_stm32_Ud = adc_read_regular(ADC1)*(VREF/ADC_CONVERSION_FACTOR);
    U_d        = V_stm32_Ud*BATTERY_VOLTAGE_CONVERTION_FACTOR;    
    adc_counter=0;

    //oscilloscope flag: start of DTC
    //gpio_set(GPIOD, GPIO9);

    DTC_SVM();

    collecting_data();

    collecting_floating_data();

    
    //oscilloscope flag: end of DTC
    //gpio_clear(GPIOD, GPIO9);

  }


}
