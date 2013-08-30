/*
 * Copyright (C) 2013 ARCOS-Lab Universidad de Costa Rica
 * Author: Sebastian Chinchilla Gutierrez <tumacher@gmail.com>
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

//
#define ADC_CONVERSION_FACTOR 4096.0f
#define VREF 2.966f	//ADC conversion reference voltage

//from 3V to the real current
#define R_SHUNT_A 0.017f
#define R_SHUNT_B 0.017f
#define V_DIFFERENTIAL_AMPLIFIER_REFFERENCE 1.5f
#define G_OP_AMP_A  100.0f
#define G_OP_AMP_B  100.0f

//from 3V to the real battery voltage
#define R_A_Ud 1000.0f
#define R_B_Ud 1000.0f
#define BATTERY_VOLTAGE_CONVERTION_FACTOR (R_A_Ud+R_B_Ud)/R_A_Ud


void voltage_measure (uint32_t adc,uint8_t channel)
{
  uint8_t channels[16];
	            
  channels[0] = channel;
  adc_set_regular_sequence(adc, 1, channels);	
  adc_start_conversion_regular(adc);
}
