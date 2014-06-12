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
#define R_SHUNT_A 0.05f//0.05527f
#define R_SHUNT_B 0.05f//(0.05f/1.04f)//0.0555149f
#define V_DIFFERENTIAL_AMPLIFIER_REFFERENCE_A 1.487f//1.48f//1.3622f//1.51458f//1.4911f//1.487f //1.5015306f
#define V_DIFFERENTIAL_AMPLIFIER_REFFERENCE_B 1.487f//1.493f//1.3256f//1.47285f//1.496176f//1.487f //1.5015306f

	//Ra=979Ohm
	//Rb=981Ohm
	//Rb/(Ra+Rb)=0.5005102
	//0.5005102*3=1.5015306
#define G_OP_AMP_A  2.0f//2.0030675f 	
	//Rf=981Ohm
	//Ri=978Ohm
	//G=1+Rf/Ri=2.0030675
#define G_OP_AMP_B  2.0f   
	//Rf=979Ohm
	//Ri=979Ohm
	//G=2

//from 3V to the real battery voltage



//Baldor------------------------------------------------
#define BATTERY_VOLTAGE_CONVERTION_FACTOR 19.05416f  
	//(R_A_Ud+R_B_Ud)/R_A_Ud
	//R_A_Ud 18k Ohm
	//R_B_Ud 997  Ohm




//Multirotor------------------------------------------------
//#define BATTERY_VOLTAGE_CONVERTION_FACTOR 4.951120f  
	//(R_A_Ud+R_B_Ud)/R_A_Ud
	//R_A_Ud 3880 Ohm
	//R_B_Ud 982  Ohm
	


void voltage_measure (uint32_t adc,uint8_t channel)
{
  uint8_t channels[16];
	            
  channels[0] = channel;
  adc_set_regular_sequence(adc, 1, channels);	
  adc_start_conversion_regular(adc);
}








