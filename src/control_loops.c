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

#define INITIAL_STATOR_ANGLE_OPEN_LOOP	0
#define NO_HALL_UPDATE	 		1
#define FIRST_HALL_UPDATE		2
#define SECOND_HALL_UPDATE		3
#define OPEN_LOOP			4
#define CLOSE_LOOP 			5


#define MAX_AMPLITUDE 1.0f
#define AMPLITUDE_REDUCTION 90.0f 


#define OPEN_LOOP_MIN_ATTENUATION 0.7f





void open_loop (int* rotor_speed_loop_state, float* attenuation, float* sine_freq, int* frequency_change_counter,float actual_sine_freq)//, float hall_time)

{	
	 if (*sine_freq>20.0f)
	{
		//*rotor_speed_loop_state=CLOSE_LOOP;
	}
	
	else if (*frequency_change_counter>max_sinusoidal_periods)
	{
		*sine_freq=*sine_freq+0.75f;
	
		//dealing with reverse rotation during startup of the motor spin--------
		if ( actual_sine_freq>0.0f)// && (hall_time<2.5f) )
		{
			*attenuation=0.0f;
			//*attenuation=OPEN_LOOP_MIN_ATTENUATION;
			*sine_freq=sine_freq_fixed;
		}
		//----------------------------------------------------------------------


		else if (*sine_freq>50.0f)
			*attenuation=OPEN_LOOP_MIN_ATTENUATION;
		else 
			*attenuation=1.0f;

		*rotor_speed_loop_state=OPEN_LOOP;
		*frequency_change_counter=0;
	}
	
	else 
	{
		*rotor_speed_loop_state=OPEN_LOOP;
	}

	
}

void close_loop(float desired_rotor_frequency,float actual_rotor_frequency,bool update,
		int* rotor_speed_loop_state,float* sine_frequency,float* attenuation, float* offset)
{
	#define K_P 	1.0f
	#define K_I	1.0f
	#define K_D	1.0f	
	
	#define MAX_PHASE_ADVANCE 90.0f

	float
		//phase_excess=0.0f,
		close_loop_error=0.0f,
		phase_advance=0.0f,
		phase_U=0.0f; 
		
	if (update)
	{

		gpio_set(GPIOD, GPIO14);

		close_loop_error=desired_rotor_frequency-actual_rotor_frequency;

		phase_U=-close_loop_error*K_P; 


		if (phase_U>MAX_PHASE_ADVANCE)
		{
			phase_advance=MAX_PHASE_ADVANCE;
			*attenuation=OPEN_LOOP_MIN_ATTENUATION;//1.0f;
		}
		
		else if (phase_U<-MAX_PHASE_ADVANCE)
		{
			phase_advance=-MAX_PHASE_ADVANCE;
			*attenuation=OPEN_LOOP_MIN_ATTENUATION;//1.0f;
		}

		else
		{
			phase_advance=phase_U;
			*attenuation=OPEN_LOOP_MIN_ATTENUATION;//1.0f;
		}


		//phase_sum+phase_excess
		//phase_excess=phase_U-phase_advance;

		//BEMF adjustment
		//phase_sum+=close_loop_tick_error/Pki-Pkc*phase_excess;

		//phase_stator=phase_rotor+phase_advance;


		*rotor_speed_loop_state=CLOSE_LOOP;


		if (*attenuation>1.0f)
			*attenuation=1.00f;

		//*offset=phase_advance;
		*offset=-0.5f;

		if (actual_rotor_frequency<0.0f)
			*sine_frequency=-actual_rotor_frequency;

		else 
			*sine_frequency=actual_rotor_frequency;
		
		E=phase_U;
		F=phase_advance;
		G=close_loop_error;
	}


	else
	{

	}



}


int next_stator_angle_and_hall_time(bool update, float* phase_A_stator_angle, float sine_freq,int frequency_change_counter, float* new_hall_time, float* old_hall_time, float offset)
{	
	//PWM angle
	*phase_A_stator_angle=*phase_A_stator_angle+360.0f*CYCLE_TIME* sine_freq+offset;

	if (*phase_A_stator_angle>=360.0f)
	{
		*phase_A_stator_angle=*phase_A_stator_angle-360.0f;
		frequency_change_counter++;	
	}
	
	//speed time
	*new_hall_time=*new_hall_time+CYCLE_TIME;
	
	if (update)
	{
		*old_hall_time=*new_hall_time;
		*new_hall_time=0.0f;	
	}
	
	return frequency_change_counter;
}




void next_PID_loop_state(int* rotor_speed_loop_state, float* attenuation, float* sine_freq, int* frequency_change_counter,float actual_sine_freq, 
//float hall_time,
bool update, float desired_rotor_frequency, float* offset)
{
	if (*rotor_speed_loop_state==OPEN_LOOP)
		open_loop (rotor_speed_loop_state,attenuation,sine_freq,frequency_change_counter,actual_sine_freq);//,hall_time);

	else if (*rotor_speed_loop_state==CLOSE_LOOP)
		close_loop(	desired_rotor_frequency,actual_sine_freq,update,
				rotor_speed_loop_state,sine_freq,attenuation,offset);

	else
		*rotor_speed_loop_state=OPEN_LOOP;
}





void PID_control_loop(float* attenuation)
{

	static int rotor_speed_loop_state=OPEN_LOOP;
	static float old_rotor_angle=0.0f;
	static float last_read_rotor_angle=0.0f;
	static float desired_rotor_frequency=-100.0f;
	static float offset=0.0f;
	static float actual_sine_frequency=0.0f;
	static float previous_hall_time=0.0f;
	static float hall_time=0.0f;
	

	//if (previous_rotor_angle!=rotor_angle)
		//previous_rotor_angle=rotor_angle;
	
	
	rotor_angle_update(&rotor_angle,&hall1_data.hall_update);

	if (rotor_angle!=last_read_rotor_angle)
	{
		old_rotor_angle=last_read_rotor_angle;
		last_read_rotor_angle=rotor_angle;
	}

	actual_sine_frequency=rotor_frequency_Hz(hall_time,previous_hall_time,last_read_rotor_angle,old_rotor_angle);


	frequency_change_counter=
		next_stator_angle_and_hall_time(
				hall1_data.hall_update,
				&phase_A_stator_angle,
				sine_freq,
				frequency_change_counter,
				&hall_time,
				&previous_hall_time,
				offset);

	next_PID_loop_state(	&rotor_speed_loop_state,attenuation,&sine_freq,&frequency_change_counter,
				actual_sine_frequency,
				//hall_time,
				hall1_data.hall_update,desired_rotor_frequency,&offset);
	

	A=sine_freq;
	B=actual_sine_frequency;
	C=offset;
	D=*attenuation;
}







