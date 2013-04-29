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

#define ADC_CONVERSION_FACTOR 4096.0f
#define VREF 2.966f	//ADC conversion reference voltage


float voltage_measure_ADC1 (u8 channel)
{
	u8 channels[16];
	float hall_voltage;

	channels[0] = channel;
	channels[1] = channel;
	channels[2] = channel;
	channels[3] = channel;
	channels[4] = channel;
	channels[5] = channel;
	channels[6] = channel;
	channels[7] = channel;
	channels[8] = channel;
	channels[9] = channel;
	channels[10] = channel;
	channels[11] = channel;
	channels[12] = channel;
	channels[13] = channel;
	channels[14] = channel;
	channels[15] = channel;

	adc_set_regular_sequence(ADC1, 1, channels);
	//ADC_SQR1(adc)=channel;


       	adc_start_conversion_regular(ADC1);
	while (!adc_eoc(ADC1));
	hall_voltage=adc_read_regular(ADC1)*(VREF/ADC_CONVERSION_FACTOR);
	return hall_voltage;
}

void hall_sensors (void)
{		
	/*float V_hall_1_V1=0.0f;
	float V_hall_1_V2=0.0f;
	float V_hall_2_V1=0.0f;
	float V_hall_2_V2=0.0f;
	float V_hall_3_V1=0.0f;
	float V_hall_3_V2=0.0f;*/
/*
	hall1_data.hall_state=HALL_INITIAL;
	hall1_data.hall_counter=0;
	hall1_data.hall_update=false;
	hall1_data.hall_corrected=LOW;

	hall2_data.hall_state=HALL_INITIAL;
	hall2_data.hall_counter=0;
	hall2_data.hall_update=false;
	hall2_data.hall_corrected=LOW;

	hall3_data.hall_state=HALL_INITIAL;
	hall3_data.hall_counter=0;
	hall3_data.hall_update=false;
	hall3_data.hall_corrected=LOW;
*/
//while(1)
//{
	//Hall-Sensor 1
	V_hall_1_V1	=hall_sensor_voltage_measure_ADC1 (ADC_CHANNEL1);
	//hall_hysteresis_window(V_hall_1_V1,HALL_1_UPPER_BAND,HALL_1_LOWER_BAND,&hall1_data);


	//Hall-Sensor 2
	//+++++++++++++++++++++++V_hall_2_V1	=hall_sensor_voltage_measure_ADC1 (ADC_CHANNEL3);
	//V_hall_2_V2	=hall_sensor_voltage_measure_ADC1 (ADC_CHANNEL4);
	/*hall_2		=hall_sensor_voltage_evaluation (	V_hall_2_V1,
								V_hall_2_V2,
								HALL_2_THRESHOLD);*/
	//Hall-Sensor 3
	//+++++++++++++++++++++++V_hall_3_V1	=hall_sensor_voltage_measure_ADC1 (ADC_CHANNEL5);
	//V_hall_3_V2	=hall_sensor_voltage_measure_ADC1 (ADC_CHANNEL6);
	/*hall_3		=hall_sensor_voltage_evaluation (	V_hall_3_V1,
								V_hall_3_V2,
								HALL_2_THRESHOLD);*/

	//Captured Values
	captured_V_hall_1_V1=V_hall_1_V1;
	captured_V_hall_1_V2=V_hall_1_V2;
	captured_V_hall_2_V1=V_hall_2_V1;
	captured_V_hall_2_V2=V_hall_2_V2;
	captured_V_hall_3_V1=V_hall_3_V1;
	captured_V_hall_3_V2=V_hall_3_V2;


	//position detection
	//hall_angle=one_hall_sensor_position_detection (hall1_data.hall_corrected);

	
//}
}
