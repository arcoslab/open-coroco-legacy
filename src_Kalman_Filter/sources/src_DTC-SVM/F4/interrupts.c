/*
 * Copyright (C) 2013 ARCOS-Lab Universidad de Costa Rica
 * Author: Federico Ruiz Ugalde <memeruiz@gmail.com>
 * Modified by: Sebastian Chinchilla Gutierrez <tumacher@gmail.com>
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

//#define SAMPLES 3
#define FLUX_LINKAGE_TIMER_DELAY 0


//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------

//printing buffers
int current_counter=0;  
bool print_current=false;
bool first_movement=false;



float data_CUR_FREQ;

float data_i_sA;
float data_i_sB;
float data_i_sD;
float data_i_sQ;

float data_U_d;

float data_V_sD;
float data_V_sQ;
float data_V_s;
float data_cita_V_s;
float data_cita_V_s_relative_angle;

float data_required_V_sD;
float data_required_V_sQ;
float data_required_V_s;
float data_required_cita_V_s;


float data_psi_sD;
float data_psi_sQ;
float data_psi_s;
int   data_psi_alpha;
float data_psi_s_alpha_SVM;
float data_w_r;

float data_timer;

float data_t_e;

float data_psi_s_ref;
float data_t_e_ref;

int   data_d_psi;
int   data_d_te;
float data_psi_delta_percentage;
float data_t_e_delta_percentage;


float data_phase_advance_SVM;
float data_V_s_ref_relative_angle;

float data_pi_control_SVM;
float data_ref_freq_SVM;
int data_state_SVM;
float data_rotating_angle_SVM;

float data_strain_gauge;

//admittance controller
float data_reference_electric_angle      = 0.0f;
float data_reference_mechanical_angle    = 0.0f;
float data_reference_gear_angle          = 0.0f;

float data_electric_angle                = 0.0f;
float data_mechanical_angle        = 0.0f;
float data_gear_angle                    = 0.0f;

float data_mechanical_frequency          = 0.0f;
float data_gear_frequency                = 0.0f;

float data_hall_a=0.0f;
float data_hall_b=0.0f;
float data_previous_hall_a=0.0f;
float data_previous_hall_b=0.0f;


//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------






void tim1_up_tim10_isr(void) 
{
  //oscilloscope flag: start of interrupt
  gpio_set(GPIOD, GPIO11);
   
  //Clear the update interrupt flag
  timer_clear_flag(TIM1,  TIM_SR_UIF);

  calc_freq();


if (center_aligned_state==FIRST_HALF)
{
  //oscilloscope flag: start of First HALF
  gpio_set(GPIOB, GPIO15);
  
  voltage_measure (ADC1,ADC_CHANNEL1);
}
else 
{
  //oscilloscope flag: start of second half
  gpio_set(GPIOD, GPIO9);
  DTC_SVM();
  collecting_floating_data();
  colllecting_flux_linkage();
  gpio_clear(GPIOD, GPIO11);
}

  //oscilloscope flag: start of halves
  //gpio_clear(GPIOB, GPIO15);

  //oscilloscope flag: end of interrupt
  gpio_clear(GPIOD, GPIO9);
  
}




void adc_isr(void)
{


 
  static int adc_counter=0;
  static float V_stm32_A  = 0.0f;
  static float V_stm32_B  = 0.0f;
  static float V_stm32_strain_gauge = 0.0f;
  float V_stm32_Ud = 0.0f;
  float V_shunt_A  = 0.0f;
  float V_shunt_B  = 0.0f;
  float V_strain_gauge =0.0f;
  

  if (adc_counter==0)
  {
    V_stm32_A = adc_read_regular(ADC1);
    voltage_measure (ADC1,ADC_CHANNEL2);
    adc_counter++;
  }
  else if (adc_counter==1)
  {
    V_stm32_B = adc_read_regular(ADC1);
    voltage_measure (ADC1,ADC_CHANNEL15);
    adc_counter++; 
  }

  else if (adc_counter==2)
  {
    V_stm32_strain_gauge = adc_read_regular(ADC1);
    voltage_measure (ADC1,ADC_CHANNEL3);
    adc_counter++; 
  }


  else
  {
    V_stm32_Ud = adc_read_regular(ADC1)*(VREF/ADC_CONVERSION_FACTOR);
    U_d        =    V_stm32_Ud*BATTERY_VOLTAGE_CONVERTION_FACTOR; 

    V_shunt_A = (V_stm32_A*(VREF/ADC_CONVERSION_FACTOR)-V_DIFFERENTIAL_AMPLIFIER_REFFERENCE_A)/G_OP_AMP_A;
    i_sA      = V_shunt_A/R_SHUNT_A;

    V_shunt_B = (V_stm32_B*(VREF/ADC_CONVERSION_FACTOR)-V_DIFFERENTIAL_AMPLIFIER_REFFERENCE_B)/G_OP_AMP_B;
    i_sB      = V_shunt_B/R_SHUNT_B;

    //V_strain_gauge=V_stm32_strain_gauge*(VREF/ADC_CONVERSION_FACTOR);
    //strain_gauge = V_strain_gauge-strain_gauge_reference;

    V_strain_gauge=V_stm32_strain_gauge*(VREF/ADC_CONVERSION_FACTOR)-strain_gauge_reference;
    V_strain_gauge=te_moving_average_filter(V_strain_gauge);

    strain_gauge = V_strain_gauge*STRAIN_GAUGE_CONVERSION_FACTOR;
    //strain_gauge =te_moving_average_filter(strain_gauge);
    if (reset_strain_gauge_reference==true)
    {
/*
      strain_gauge_reference=strain_gauge_reference+strain_gauge;
      strain_gauge=0.0f;
      reset_strain_gauge_reference=false;
*/

      strain_gauge_reference=strain_gauge_reference+V_strain_gauge;
      V_strain_gauge=0.0f;
      reset_strain_gauge_reference=false;
    }
    //filtering currents

    //i_sA = isA_moving_average_filter(i_sA);
    //i_sB = isB_moving_average_filter(i_sB);
       
    adc_counter=0;

    DTC_SVM();

    //collecting_data();

      //oscilloscope flag: start of halves
  gpio_clear(GPIOB, GPIO15);

  //oscilloscope flag: end of interrupt
  gpio_clear(GPIOD, GPIO9);
  //gpio_clear(GPIOD, GPIO11);
  }


}
