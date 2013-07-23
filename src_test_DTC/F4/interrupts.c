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


void tim1_up_tim10_isr(void) 
{
  // Clear the update interrupt flag
  timer_clear_flag(TIM1,  TIM_SR_UIF);

  calc_freq();
  start_up();
  gen_pwm();

  voltage_measure (ADC1,ADC_CHANNEL2);
}


  float divisor_voltage=0.0f;
  float source_voltage=0.0f;

  float divisor_voltage_2=0.0f;
  float source_voltage_2=0.0f;

  float divisor_voltage_3=0.0f;
  float source_voltage_3=0.0f;

void adc_isr(void)
{ 
  static int adc_counter=0;
    
  if (adc_counter==0)
  {
    divisor_voltage=adc_read_regular(ADC1)*(VREF/ADC_CONVERSION_FACTOR);
    voltage_measure (ADC1,ADC_CHANNEL2);
    counter++;
  }
  else if (adc_counter==1)
  {
    divisor_voltage_2=adc_read_regular(ADC1)*(VREF/ADC_CONVERSION_FACTOR);
    voltage_measure (ADC1,ADC_CHANNEL3);
    counter++; 
  }
  else
  {
    divisor_voltage_3=adc_read_regular(ADC1)*(VREF/ADC_CONVERSION_FACTOR);
    counter=0;
  }
}
