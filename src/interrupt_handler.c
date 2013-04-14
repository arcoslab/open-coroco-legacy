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



