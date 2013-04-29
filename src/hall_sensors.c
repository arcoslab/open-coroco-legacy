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


#define HALL_UPPER_BAND 0.800f
#define HALL_LOWER_BAND 0.200f


//rotor positions
/*
#define POSITION_0	  0.0f	//H1
#define POSITION_1	 60.0f	//H1H2	*
#define POSITION_2	120.0f	//H2	*
#define POSITION_3	180.0f	//H2H3	*
#define POSITION_4	240.0f	//H3
#define POSITION_5	300.0f	//H3H1	
#define POSITION_6	360.0f	//	*
#define POSITION_7	  0.0f	//
#define POSITION_8	  0.0f
#define POSITION_9	180.0f
*/

#define POSITION_0	  0.0f			//H1
#define POSITION_1	94.57f	// 60.0f	//H1H2	*
#define POSITION_2	154.039f//120.0f	//H2	*
#define POSITION_3	189.37f	//180.0f	//H2H3	*
#define POSITION_4	261.40f	//240.0f	//H3
#define POSITION_5	327.0f	//300.0f	//H3H1	
#define POSITION_6	360.0f	//	*
#define POSITION_7	  0.0f	//
#define POSITION_8	  0.0f
#define POSITION_9	180.0f

//finite-state-machine states
#define HALL_INITIAL	0
#define HALL_DELAY	1
#define HALL_UPDATED	2
#define NO_HALL_DELAY	3
#define NO_HALL_UPDATED 4

 
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

	float V_hall_1_V1=0.0f;
	float V_hall_1_V2=0.0f;
	float V_hall_2_V1=0.0f;
	float V_hall_2_V2=0.0f;
	float V_hall_3_V1=0.0f;
	float V_hall_3_V2=0.0f;

//float three_hall_sensor_position_detection (bool hall_1, bool hall_2, bool hall_3)
float three_hall_sensor_position_detection (float hall_1, float hall_2, float hall_3)
{
	if 	( (hall_1>HALL_LOWER_BAND ) && (hall_2<HALL_LOWER_BAND ) && (hall_3<HALL_LOWER_BAND) )//100	first
	{
		hall_angle=POSITION_0;

	}
	else if ( (hall_1>HALL_LOWER_BAND) && (hall_2>HALL_LOWER_BAND) && (hall_3<HALL_LOWER_BAND) )//110 fourth 
	{
		hall_angle=POSITION_1;
	}
	else if ( (hall_1<HALL_LOWER_BAND) && (hall_2>HALL_LOWER_BAND) && (hall_3<HALL_LOWER_BAND) )//010	second
	{
		hall_angle=POSITION_2;
	}
	else if ( (hall_1<HALL_LOWER_BAND) && (hall_2>HALL_LOWER_BAND) && (hall_3>HALL_LOWER_BAND) )//011	third 
	{
		hall_angle=POSITION_3;
	}
	else if ( (hall_1<HALL_LOWER_BAND) && (hall_2<HALL_LOWER_BAND) && (hall_3>HALL_LOWER_BAND) )//001
	{
		hall_angle=POSITION_4;
	}
	else if ( (hall_1>HALL_LOWER_BAND) && (hall_2<HALL_LOWER_BAND) && (hall_3>HALL_LOWER_BAND) )//101 fifth
	{
		hall_angle=POSITION_5;
	}
	else if ( (hall_1<HALL_LOWER_BAND) && (hall_2<HALL_LOWER_BAND) && (hall_3<HALL_LOWER_BAND) )//000
	{
		hall_angle=POSITION_6;
	}
	else
	{
		hall_angle=POSITION_6;
	}
	return hall_angle;
}


bool hall_hysteresis_window(float voltage, float upper_band,float lower_band,bool* previous_hall)//hall_finite_state_machine_data * hallx_data)
{	
	bool update;

	if (*previous_hall==HIGH)
	{
		if (voltage<lower_band)
		{
			update=true;
			*previous_hall=LOW;
			
		}
		else 
		{
			update=false;
			*previous_hall=HIGH;
		}
	}

	else if (*previous_hall==LOW)
	{
		if (voltage>upper_band)
		{
			update=true;
			*previous_hall=HIGH;
			
		}
		else 
		{
			update=false;
			*previous_hall=LOW;
		}
	}


	//A=previous_hall;
	//B=update;
	return update;
}




float rotor_frequency_Hz(float hall_time,float previous_hall_time,float rotor_angle,float previous_rotor_angle, float update)
{
	//static float previous_rotor_angle=0.0f;
	float rotor_frequency;
	float advance;

	advance=rotor_angle-previous_rotor_angle;
	
	if (advance>=300.0f)
		advance=advance-360.0f;

	else if (advance<=-300.0f)
		advance=advance+360.0f;

	//rotor_frequency=(advance)/(360.0f*previous_hall_time);


	//solo con un hall sensor-------------------------------
	if (advance>0.0f)
		rotor_frequency=1.0f/(2.0f*previous_hall_time);
	else
		rotor_frequency=-1.0f/(2.0f*previous_hall_time);
	//------------------------------------------------------

	if (hall_time>=3.0f)
		rotor_frequency=0.0f;

	return rotor_frequency;
}



void hall_sensor_measure(float *hall_1,float *hall_2, float *hall_3)
{
	*hall_1=gpio_get(GPIOE, GPIO15);	//32768	10000000 00000000
	*hall_2=gpio_get(GPIOB, GPIO11);	//2048	00001000 00000000
	*hall_3=gpio_get(GPIOB, GPIO13);	//8192	00100000 00000000
}

void rotor_angle_update(float *rotor_angle, bool *update)
{
	float hall_1,hall_2,hall_3;
	static bool previous_hall_1=false,previous_hall_2=false,previous_hall_3=false;
	bool updated_hall_1,updated_hall_2,updated_hall_3;

	hall_sensor_measure(&hall_1,&hall_2,&hall_3);

	updated_hall_1=hall_hysteresis_window(hall_1,HALL_UPPER_BAND,HALL_LOWER_BAND,&previous_hall_1);
	updated_hall_2=hall_hysteresis_window(hall_2,HALL_UPPER_BAND,HALL_LOWER_BAND,&previous_hall_2);
	updated_hall_3=hall_hysteresis_window(hall_3,HALL_UPPER_BAND,HALL_LOWER_BAND,&previous_hall_3);

	if (updated_hall_1)// || updated_hall_2 || updated_hall_3)
		*update=true;

	else
		*update=false;



	*rotor_angle=three_hall_sensor_position_detection (hall_1,hall_2,hall_3);
	C=*rotor_angle;
	
}





















