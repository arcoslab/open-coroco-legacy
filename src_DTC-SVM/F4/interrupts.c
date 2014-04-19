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








//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------






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
  //gpio_set(GPIOB, GPIO15);



if (center_aligned_state==FIRST_HALF)
{
  gpio_set(GPIOB, GPIO15);

  voltage_measure (ADC1,ADC_CHANNEL1);
}
else 
{
  DTC_SVM();
  collecting_floating_data();
  colllecting_flux_linkage();
}

}




void adc_isr(void)
{ 
  static int adc_counter=0;
  static float V_stm32_A  = 0.0f;
  static float V_stm32_B  = 0.0f;
  float V_stm32_Ud = 0.0f;
  float V_shunt_A  = 0.0f;
  float V_shunt_B  = 0.0f;
  

  if (adc_counter==0)
  {
/*
    V_stm32_A = adc_read_regular(ADC1)*(VREF/ADC_CONVERSION_FACTOR);
    V_shunt_A = (V_stm32_A-V_DIFFERENTIAL_AMPLIFIER_REFFERENCE)/G_OP_AMP_A;
    i_sA      = V_shunt_A/R_SHUNT_A;
*/
    V_stm32_A = adc_read_regular(ADC1);


    
    //oscilloscope flag: start of ADC current measuring 
    gpio_clear(GPIOB, GPIO15);

    voltage_measure (ADC1,ADC_CHANNEL2);
    adc_counter++;
  }
  else if (adc_counter==1)
  {
/*
    V_stm32_B = adc_read_regular(ADC1)*(VREF/ADC_CONVERSION_FACTOR);
    V_shunt_B = (V_stm32_B-V_DIFFERENTIAL_AMPLIFIER_REFFERENCE)/G_OP_AMP_B;
    i_sB      = V_shunt_B/R_SHUNT_B;
*/
    V_stm32_B = adc_read_regular(ADC1);
    
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

    /*
    U_d=40.0f;
    i_sA=0.0f;
    i_sB=0.0f;
    */

    V_shunt_A = (V_stm32_A*(VREF/ADC_CONVERSION_FACTOR)-V_DIFFERENTIAL_AMPLIFIER_REFFERENCE)/G_OP_AMP_A;
    i_sA      = V_shunt_A/R_SHUNT_A;

    V_shunt_B = (V_stm32_B*(VREF/ADC_CONVERSION_FACTOR)-V_DIFFERENTIAL_AMPLIFIER_REFFERENCE)/G_OP_AMP_B;
    i_sB      = V_shunt_B/R_SHUNT_B;





       

    adc_counter=0;

    //oscilloscope flag: start of DTC
    gpio_set(GPIOD, GPIO9);

    DTC_SVM();

    //collecting_data();

    
    //oscilloscope flag: end of DTC
    //gpio_clear(GPIOD, GPIO9);

  }


}
