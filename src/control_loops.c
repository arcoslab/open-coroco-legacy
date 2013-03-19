#define NO_HALL_UPDATE	 	0
#define FIRST_HALL_UPDATE	1
#define SECOND_HALL_UPDATE	2
#define OPEN_LOOP		3
#define CLOSE_LOOP 		4


#define MAX_AMPLITUDE 1.0f
#define AMPLITUDE_REDUCTION 90.0f 
//#define MAX_PHASE_ADVANCE 90.0f*PI/180.0f
//#define KP (10.0f/(max_ticks*1.0f))
//#define KI 100.0f
//#define KC 100.0f

#define OPEN_LOOP_MIN_ATTENUATION 0.75f

void no_hall_update (int* rotor_speed_loop_state)
{
	if (hall1_data.hall_update)
	{
		attenuation = OPEN_LOOP_MIN_ATTENUATION;
		sine_freq=sine_freq_fixed;
		*rotor_speed_loop_state=FIRST_HALL_UPDATE;
	}

	else
	{
		attenuation = OPEN_LOOP_MIN_ATTENUATION;
		sine_freq=sine_freq_fixed;
		*rotor_speed_loop_state=NO_HALL_UPDATE;
	}

	max_ticks=pwmfreq_f/sine_freq;
}

void first_hall_update (int* rotor_speed_loop_state)
{
	if (hall1_data.hall_update)
	{
		attenuation = OPEN_LOOP_MIN_ATTENUATION;
		sine_freq=sine_freq_fixed;
		*rotor_speed_loop_state=OPEN_LOOP;
	}

	else
	{
		attenuation = OPEN_LOOP_MIN_ATTENUATION;
		sine_freq=sine_freq_fixed;
		*rotor_speed_loop_state=FIRST_HALL_UPDATE;
	}
	max_ticks=pwmfreq_f/sine_freq;
}
/*
void second_hall_update (int* rotor_speed_loop_state)
{
	
}
*/
void open_loop (int* rotor_speed_loop_state)
{	
	if (hall1_data.hall_update)
	{
		attenuation = 0.5f;
		#define OPEN_LOOP_P 0.2f
		
		actual_sine_frequency=pwmfreq_f/(2.0f*previous_hall_ticks);


		if (actual_sine_frequency>open_loop_desired_sine_frequency*1.0f)
		{
			*rotor_speed_loop_state=CLOSE_LOOP;
			phase_sum=90.0f;
		}


		else
		{
			open_loop_error=open_loop_desired_sine_frequency-actual_sine_frequency;

			if (open_loop_error>8.0f)
				open_loop_error=8.0f;
			else if (open_loop_error<-8.0f)
				open_loop_error=-8.0f;

			sine_freq=actual_sine_frequency;//+OPEN_LOOP_P*open_loop_error;		
			*rotor_speed_loop_state=OPEN_LOOP;
		}
	}
	

	
	else 
	{			
		*rotor_speed_loop_state=OPEN_LOOP;		
	}

	max_ticks=pwmfreq_f/sine_freq;

}

void close_loop(int* rotor_speed_loop_state)
{
	#define Pkp 1.0f/12.0f
	
	#define MAX_PHASE_ADVANCE 90.0f
	float Pki=1000000.0f;
	float Pkc=1.0f/1000.0f;

if (hall1_data.hall_update)
{

	gpio_set(GPIOD, GPIO15);

	actual_sine_frequency=pwmfreq_f/(2.0f*previous_hall_ticks);
	close_loop_error=close_loop_desired_frequency-actual_sine_frequency;

	desired_previous_hall_ticks=pwmfreq_f/(2.0f*close_loop_desired_frequency);
	close_loop_tick_error= -1.0f*(desired_previous_hall_ticks-previous_hall_ticks);

	//phaseU=close_loop_tick_error*Pkp;//+phase_sum;
	phaseU=close_loop_error*1.5f;


	if (attenuation<1)
	{
		//regular phase advance
		if (phaseU>MAX_PHASE_ADVANCE)
		{
			phase_advance=MAX_PHASE_ADVANCE;
			attenuation=1.0f;//1.0f;
			Pki=1.0f/1000.0f;//8.0f;
		}
		
		else if (phaseU<-MAX_PHASE_ADVANCE)
		{
			phase_advance=-MAX_PHASE_ADVANCE;
			attenuation=1.0f;
			Pkc=8.0f;
		}

		else
		{
			phase_advance=phaseU;
			
			if (phase_advance<0)
			{
				attenuation=phase_advance/150.0f;//-phase_advance/72.0f;
			}
			else
			{
				attenuation=phase_advance/150.0f;//phase_advance/72.0f;
			}
		}

		

	}

	else 
	{
		//double phase advance
		attenuation=1.0f;

		if (phaseU>MAX_PHASE_ADVANCE*2.0f)
		{
			phase_advance=MAX_PHASE_ADVANCE*2.0f;
		} 
		else if (phaseU<-MAX_PHASE_ADVANCE*2.0f)
		{
			phase_advance=-MAX_PHASE_ADVANCE*2.0f;
		}
		else
		{
			phase_advance=phaseU;
		}
	}

	//phase_sum+phase_excess
	phase_excess=phaseU-phase_advance;

	//BEMF adjustment
	phase_sum+=close_loop_tick_error/Pki-Pkc*phase_excess;

	phase_stator=phase_rotor+phase_advance;


	*rotor_speed_loop_state=CLOSE_LOOP;


	//max_ticks=pwmfreq_f/sine_freq;
	max_ticks=2.0f*previous_hall_ticks;
	ticks=ticks+phase_advance*max_ticks/360.0f;
	//ticks=ticks+45.0f*max_ticks/360.0f;
}


else
{

}



}

void PID_control_loop(void)
{

	static int 
		//frequency_change_counter=0,
		rotor_speed_loop_state=NO_HALL_UPDATE;

	hall_hysteresis_window(V_hall_1_V1,HALL_1_UPPER_BAND,HALL_1_LOWER_BAND,&hall1_data);
	
	
  	if (ticks<max_ticks)
	{
		ticks+=1.0f;
	}
	else
	{ 
		ticks=0.0f;
		
	}

	hall_ticks+=1.0f;
	//max_ticks=pwmfreq_f/sine_freq;



	if (hall1_data.hall_update )//|| hall2_data.hall_update || hall3_data.hall_update)
	{	
		//hall-sensor time	
		previous_hall_ticks=hall_ticks;
		hall_ticks=0.0f;
	}



	//waiting for having two hall sensor measures in order to calculate the rotor speed
	if (rotor_speed_loop_state==NO_HALL_UPDATE)
		no_hall_update(&rotor_speed_loop_state);

	else if (rotor_speed_loop_state==FIRST_HALL_UPDATE)
		first_hall_update(&rotor_speed_loop_state);
	
	//else if (rotor_speed_loop_state==SECOND_HALL_UPDATE)
		//second_hall_update(&rotor_speed_loop_state);

	else if (rotor_speed_loop_state==OPEN_LOOP)
		open_loop(&rotor_speed_loop_state);

	else if (rotor_speed_loop_state==CLOSE_LOOP)
		close_loop(&rotor_speed_loop_state);
	else
		rotor_speed_loop_state=NO_HALL_UPDATE;

}










