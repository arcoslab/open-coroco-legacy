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



//min hall voltaje1.49
//max hall voltaje 1.67
//2.97/4096 = 0.00072509765625 (accuracy?)

//final threshold
	//Both hall outputs for every are directly connected to the the stm32. No resistance is set between the V2 and ground.
	//This configuration is the must suttable since it allowed to light all leds in order. Voltage is now easier to read
	//since it changes from +0.05V to -0.05V, so the threshold is just 0V. However the third sensor is unstable and does not provide 
	//a steady led ligth. A Finite state machine must be created in order to handle the change of stages between one and another light. 


#define HALL_1_THRESHOLD 0.0f	//0.70f //0.11f	
				//oscilloscope: differential=> max     99mV min:   95mV Dif: 4mV;   to ground=>max: 855mV min: 823mV dif=32mV
				//tester: differential=> max: 101.1mV  min:   98.2mV Dif: 1.9mV;   to ground=>max: 867mV min: 839mV dif=28mV
#define HALL_2_THRESHOLD 0.0f	//0.86f //0.950f//0.25f	
				//oscilloscope: differential=>max:287mV min:217mV Dif:0.07V; to ground=>max: 0.987V min: VDif= 0.927V Dif: 0.06mV
				//tester: differential=> max: 284.8mV  min:   226.1mV Dif: 58.7mV;   to ground=>max: 976mV min: 934mV dif=42mV
#define HALL_3_THRESHOLD 0.0f	//1.59f//0.496f	
				//oscilloscope: differential=> max: 0.503mV min:0.286mV Dif: 0.212; to ground=>max: 1.70V  min: 1.54V Dif= 0.16V
				//tester: differential=> max: 496.8mV min:285.9mV Dif: 211.1mV;   to ground=>max: 1.701V min: 1.554mV dif=147mV
#define ADC_CONVERSION_FACTOR 4096.0f


//#define HALL_1_UPPER_BAND 2.800f
//#define HALL_1_LOWER_BAND 0.200f


#define HALL_1_UPPER_BAND 0.800f
#define HALL_1_LOWER_BAND 0.200f


//rotor positions
/*
#define POSITION_0	 30.0f*PI/180.0f	//h1
#define POSITION_1	 90.0f*PI/180.0f	//h2
#define POSITION_2	120.0f*PI/180.0f	//h3
#define POSITION_3	150.0f*PI/180.0f	//h1h2
#define POSITION_4	210.0f*PI/180.0f	//h2h3
#define POSITION_5	270.0f*PI/180.0f	//h3h1
#define POSITION_6	330.0f*PI/180.0f	//
#define POSITION_7	  0.0f*PI/180.0f	//
#define POSITION_8	  0.0f*PI/180.0f
#define POSITION_9	180.0f*PI/180.0f
*/

#define POSITION_0	  0.0f	//H1
#define POSITION_1	 60.0f	//H1H2
#define POSITION_2	120.0f	//H2
#define POSITION_3	180.0f	//H2H3
#define POSITION_4	240.0f	//H3
#define POSITION_5	300.0f	//H3H1
#define POSITION_6	360.0f	//
#define POSITION_7	  0.0f	//
#define POSITION_8	  0.0f
#define POSITION_9	180.0f



//finite-state-machine states
#define HALL_INITIAL	0
#define HALL_DELAY	1
#define HALL_UPDATED	2
#define NO_HALL_DELAY	3
#define NO_HALL_UPDATED 4

#define hall_counter_max 100//max_ticks/50.0f
#define VREF 2.966f	//ADC conversion reference voltage

typedef struct 
{
	int 	hall_state;
	int	hall_counter;
	bool	hall_update;
	bool	hall_corrected;	
} hall_finite_state_machine_data;

hall_finite_state_machine_data
	hall1_data,
	hall2_data,
	hall3_data;
	


float hall_angle=0.0f;
/*
//Digital Voltage (High/Low)
bool hall_1=LOW;
bool hall_2=LOW;
bool hall_3=LOW;
float hall_angle;
*/



/*
bool hall_sensor_voltage_evaluation (float V1, float V2,float threshold)
{
	bool hall;

	if ( (V1-V2) >threshold)
		hall=HIGH;		
	else
		hall=LOW;
		
	return hall;
}
*/



//float three_hall_sensor_position_detection (bool hall_1, bool hall_2, bool hall_3)
float three_hall_sensor_position_detection (float hall_1, float hall_2, float hall_3)
{
	if 	( (hall_1>HALL_1_LOWER_BAND ) && (hall_2<HALL_1_LOWER_BAND ) && (hall_3<HALL_1_LOWER_BAND) )//100	first
	{
		hall_angle=POSITION_0;

	}
	else if ( (hall_1>HALL_1_LOWER_BAND) && (hall_2>HALL_1_LOWER_BAND) && (hall_3<HALL_1_LOWER_BAND) )//110 fourth 
	{
		hall_angle=POSITION_1;
	}
	else if ( (hall_1<HALL_1_LOWER_BAND) && (hall_2>HALL_1_LOWER_BAND) && (hall_3<HALL_1_LOWER_BAND) )//010	second
	{
		hall_angle=POSITION_2;
	}
	else if ( (hall_1<HALL_1_LOWER_BAND) && (hall_2>HALL_1_LOWER_BAND) && (hall_3>HALL_1_LOWER_BAND) )//011	third 
	{
		hall_angle=POSITION_3;
	}
	else if ( (hall_1<HALL_1_LOWER_BAND) && (hall_2<HALL_1_LOWER_BAND) && (hall_3>HALL_1_LOWER_BAND) )//001
	{
		hall_angle=POSITION_4;
	}
	else if ( (hall_1>HALL_1_LOWER_BAND) && (hall_2<HALL_1_LOWER_BAND) && (hall_3>HALL_1_LOWER_BAND) )//101 fifth
	{
		hall_angle=POSITION_5;
	}
	else if ( (hall_1<HALL_1_LOWER_BAND) && (hall_2<HALL_1_LOWER_BAND) && (hall_3<HALL_1_LOWER_BAND) )//000
	{
		hall_angle=POSITION_6;
	}
	else
	{
		hall_angle=POSITION_6;
	}
	return hall_angle;
}

/*
float two_hall_sensor_position_detection (bool hall_1, bool hall_2)
{
	if 	(hall_1 && ~hall_2)//100	
	{
		hall_angle=POSITION_0;
		//gpio_port_write(GPIOD, GPIO12);
	}
	else if (hall_1 & hall_2)//110
	{
		hall_angle=POSITION_1;
		//gpio_port_write(GPIOD, GPIO13);
	}
	else if (~hall_1 & hall_2)//010
	{
		hall_angle=POSITION_2;
		//gpio_port_write(GPIOD, GPIO14);
	}
	else if (~hall_1 & ~hall_2)//001
	{
		hall_angle=POSITION_4;
		//gpio_port_write(GPIOD, GPIO15);
	}
	else
	{
		hall_angle=POSITION_7;
		gpio_clear(GPIOD, GPIO12);
		gpio_clear(GPIOD, GPIO13);
		gpio_clear(GPIOD, GPIO14);
		gpio_clear(GPIOD, GPIO15);
	}
	
	return hall_angle;
}

float one_hall_sensor_position_detection (bool hall1_corrected)
{
	if (hall1_corrected==HIGH)
	{
		hall_angle=POSITION_8;
	}
	else
	{       
		hall_angle=POSITION_9;
	}
	
	return hall_angle;
}
*/



//gpio_toggle(GPIOD, GPIO13);

void hall_hysteresis_window(float voltage, float upper_band,float lower_band,hall_finite_state_machine_data * hallx_data)
{	
	

	if (hallx_data->hall_corrected==HIGH)
	{
		if (voltage<lower_band)
		{
			hallx_data->hall_update=true;
			hallx_data->hall_corrected=LOW;
			
		}
		else 
		{
			hallx_data->hall_update=false;
			hallx_data->hall_corrected=HIGH;
		}
	}

	else if (hallx_data->hall_corrected==LOW)
	{
		if (voltage>upper_band)
		{
			hallx_data->hall_update=true;
			hallx_data->hall_corrected=HIGH;
			
		}
		else 
		{
			hallx_data->hall_update=false;
			hallx_data->hall_corrected=LOW;
		}
	}
	
	
}


	float V_hall_1_V1=0.0f;
	float V_hall_1_V2=0.0f;
	float V_hall_2_V1=0.0f;
	float V_hall_2_V2=0.0f;
	float V_hall_3_V1=0.0f;
	float V_hall_3_V2=0.0f;






/*
void one_hall_sensor_finite_state_machine (void)
{

one_hall_sensor_finite_state_machine_state_evaluation (&hall1_data,hall_1);
one_hall_sensor_finite_state_machine_state_evaluation (&hall2_data,hall_2);
one_hall_sensor_finite_state_machine_state_evaluation (&hall3_data,hall_3);

}
*/














