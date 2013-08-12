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

float i_sA=0;
float i_sB=0;
float U_d;

void tim1_up_tim10_isr(void) 
{
  // Clear the update interrupt flag
  timer_clear_flag(TIM1,  TIM_SR_UIF);

  calc_freq();
  start_up();
  gen_pwm();

  voltage_measure (ADC1,ADC_CHANNEL1);
}




void adc_isr(void)
{ 
  static int adc_counter=0;
    
  if (adc_counter==0)
  {
    i_sA=adc_read_regular(ADC1)*(VREF/ADC_CONVERSION_FACTOR)*SHUNT_A_CONVERTION_FACTOR;
    voltage_measure (ADC1,ADC_CHANNEL2);
    adc_counter++;
    //printf ("\nFirst Convertion");
  }
  else if (adc_counter==1)
  {
    i_sB=adc_read_regular(ADC1)*(VREF/ADC_CONVERSION_FACTOR)*SHUNT_B_CONVERTION_FACTOR;
    voltage_measure (ADC1,ADC_CHANNEL3);
    adc_counter++; 
    //printf ("\nSecond Convertion");
  }
  else
  {
    U_d=adc_read_regular(ADC1)*(VREF/ADC_CONVERSION_FACTOR)*BATTERY_VOLTAGE_CONVERTION_FACTOR;
    adc_counter=0;
    //printf ("\nThird Convertion");
  }
}
