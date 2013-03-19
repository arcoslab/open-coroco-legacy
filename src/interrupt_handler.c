void exti0_isr(void)
{
	exti_reset_request(EXTI0);

	button=1;

}

void tim1_cc_isr (void)
{
/*
	static float
		attenuation		=0;
*/
	// Clear the update interrupt flag
	timer_clear_flag(TIM1,  TIM_SR_CC1IF);	

	//pwm duty cycle calculation
	pwm(ticks,attenuation,max_ticks);

	//hall sensors 
	//one_hall_sensor_finite_state_machine_state_evaluation (&hall1_data,hall_1);
	

if (button==1)
{

	
/*
  	if (ticks<max_ticks)
	{
		ticks+=1.0f;
	}
	else
	{ 
		ticks=0.0f;
		frequency_change_counter++;
	}

	if ( (frequency_change_counter>=max_sinusoidal_periods) && (otor_speed_loop_state==OPEN_LOOP) )
	{
		frequency_change_counter=0;
		sine_freq+=sine_freq_increment;
	}

	hall_ticks+=1.0f;
*/

	PID_control_loop();
}

}



