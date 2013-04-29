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

/************************************************************/
/**************************Interrupts************************/
/************************************************************/
void exti0_isr(void)
{
	exti_reset_request(EXTI0);

	button=1;
	
/*
	if (exti_direction == FALLING) {
		 //gpio_toggle(GPIOD, GPIO13);
		exti_direction = RISING;
		exti_set_trigger(EXTI0, EXTI_TRIGGER_RISING);
	} else {
		 //gpio_toggle(GPIOD, GPIO13);
		
		exti_direction = FALLING;
		exti_set_trigger(EXTI0, EXTI_TRIGGER_FALLING);
		//timer_generate_event(TIM1, TIM_EGR_COMG);


		//capture compare event
		//timer_generate_event(TIM1, TIM_EGR_UG);

	}*/
}


void tim1_trg_com_tim11_isr(void)
{
	static int step = 0;

	/* Clear the COM trigger interrupt flag. */
	timer_clear_flag(TIM1, TIM_SR_COMIF);

	/*
	 * A simplified and inefficient implementation of PWM On
	 * scheme. Look at the implementation in Open-BLDC on
	 * http://open-bldc.org for the proper implementation. This
	 * one only serves as an example.
	 *
	 * Table of the PWM scheme zone configurations when driving:
	 * @verbatim
	 *  | 1| 2| 3| 4| 5| 6|
	 * -+--+--+--+--+--+--+
	 * A|p+|++|  |p-|--|  |
	 * -+--+--+--+--+--+--+
	 * B|  |p-|--|  |p+|++|
	 * -+--+--+--+--+--+--+
	 * C|--|  |p+|++|  |p-|
	 * -+--+--+--+--+--+--+
	 *  |  |  |  |  |  |  '- 360 Deg
	 *  |  |  |  |  |  '---- 300 Deg
	 *  |  |  |  |  '------- 240 Deg
	 *  |  |  |  '---------- 180 Deg
	 *  |  |  '------------- 120 Deg
	 *  |  '----------------  60 Deg
	 *  '-------------------   0 Deg
	 *
	 * Legend:
	 * p+: PWM on the high side
	 * p-: PWM on the low side
	 * --: Low side on
	 * ++: High side on
	 *   : Floating/NC
	 * @endverbatim
	 */

	//gpio_port_write(GPIOD, GPIO13);
	//GPIOD_BSRR = GPIO13;		/* LED on */
	gpio_port_write(GPIOD, GPIO13);
	switch (step) {
	case 0: /* A PWM HIGH, B OFF, C LOW */
		timer_set_oc_mode(TIM1, TIM_OC1, TIM_OCM_PWM1);
		timer_set_oc_mode(TIM1, TIM_OC2, TIM_OCM_FROZEN);
		timer_set_oc_mode(TIM1, TIM_OC3, TIM_OCM_FORCE_LOW);

		timer_enable_oc_output(TIM1, TIM_OC1);
		timer_disable_oc_output(TIM1, TIM_OC1N);

		timer_disable_oc_output(TIM1, TIM_OC2);
		timer_disable_oc_output(TIM1, TIM_OC2N);

		timer_enable_oc_output(TIM1, TIM_OC3);
		timer_enable_oc_output(TIM1, TIM_OC3N);

		step++;
		//gpio_port_write(GPIOD, GPIO12);
		break;
	case 1: /* A HIGH, B PWM LOW, C OFF */
		timer_set_oc_mode(TIM1, TIM_OC1, TIM_OCM_FORCE_HIGH);
		timer_set_oc_mode(TIM1, TIM_OC2, TIM_OCM_PWM1);
		timer_set_oc_mode(TIM1, TIM_OC3, TIM_OCM_FROZEN);

		timer_enable_oc_output(TIM1, TIM_OC1);
		timer_enable_oc_output(TIM1, TIM_OC1N);

		timer_disable_oc_output(TIM1, TIM_OC2);
		timer_enable_oc_output(TIM1, TIM_OC2N);

		timer_disable_oc_output(TIM1, TIM_OC3);
		timer_disable_oc_output(TIM1, TIM_OC3N);

		step++;
		//gpio_port_write(GPIOD, GPIO13);
		break;
	case 2: /* A OFF, B LOW, C PWM HIGH */
		timer_set_oc_mode(TIM1, TIM_OC1, TIM_OCM_FROZEN);
		timer_set_oc_mode(TIM1, TIM_OC2, TIM_OCM_FORCE_LOW);
		timer_set_oc_mode(TIM1, TIM_OC3, TIM_OCM_PWM1);

		timer_disable_oc_output(TIM1, TIM_OC1);
		timer_disable_oc_output(TIM1, TIM_OC1N);

		timer_enable_oc_output(TIM1, TIM_OC2);
		timer_enable_oc_output(TIM1, TIM_OC2N);

		timer_enable_oc_output(TIM1, TIM_OC3);
		timer_disable_oc_output(TIM1, TIM_OC3N);

		step++;
		//gpio_port_write(GPIOD, GPIO14);
		break;
	case 3: /* A PWM LOW, B OFF, C HIGH */
		timer_set_oc_mode(TIM1, TIM_OC1, TIM_OCM_PWM1);
		timer_set_oc_mode(TIM1, TIM_OC2, TIM_OCM_FROZEN);
		timer_set_oc_mode(TIM1, TIM_OC3, TIM_OCM_FORCE_HIGH);

		timer_disable_oc_output(TIM1, TIM_OC1);
		timer_enable_oc_output(TIM1, TIM_OC1N);

		timer_disable_oc_output(TIM1, TIM_OC2);
		timer_disable_oc_output(TIM1, TIM_OC2N);

		timer_enable_oc_output(TIM1, TIM_OC3);
		timer_enable_oc_output(TIM1, TIM_OC3N);

		step++;
		//gpio_port_write(GPIOD, GPIO15);
		break;
	case 4: /* A LOW, B PWM HIGH, C OFF */
		timer_set_oc_mode(TIM1, TIM_OC1, TIM_OCM_FORCE_LOW);
		timer_set_oc_mode(TIM1, TIM_OC2, TIM_OCM_PWM1);
		timer_set_oc_mode(TIM1, TIM_OC3, TIM_OCM_FROZEN);

		timer_enable_oc_output(TIM1, TIM_OC1);
		timer_enable_oc_output(TIM1, TIM_OC1N);

		timer_enable_oc_output(TIM1, TIM_OC2);
		timer_disable_oc_output(TIM1, TIM_OC2N);

		timer_disable_oc_output(TIM1, TIM_OC3);
		timer_disable_oc_output(TIM1, TIM_OC3N);

		step++;
		//gpio_port_write(GPIOD, GPIO15);
		break;
	case 5: /* A OFF, B HIGH, C PWM LOW */
		timer_set_oc_mode(TIM1, TIM_OC1, TIM_OCM_FROZEN);
		timer_set_oc_mode(TIM1, TIM_OC2, TIM_OCM_FORCE_HIGH);
		timer_set_oc_mode(TIM1, TIM_OC3, TIM_OCM_PWM1);

		timer_disable_oc_output(TIM1, TIM_OC1);
		timer_disable_oc_output(TIM1, TIM_OC1N);

		timer_enable_oc_output(TIM1, TIM_OC2);
		timer_enable_oc_output(TIM1, TIM_OC2N);

		timer_disable_oc_output(TIM1, TIM_OC3);
		timer_enable_oc_output(TIM1, TIM_OC3N);

		step = 0;
		//gpio_port_write(GPIOD, GPIO15);
		break;
	}

}


/************************************************************/
/**************************PID states************************/
/************************************************************/

void initial_stator_angle_open_loop(int* rotor_speed_loop_state)
{
	attenuation = OPEN_LOOP_MIN_ATTENUATION;
	sine_freq=sine_freq_fixed;
	*rotor_speed_loop_state=NO_HALL_UPDATE;
	
	//V_s_angle=rotor_angle+180.0f;//rotor_angle+180.0f;
	//phase_A_stator_angle=stator_angle_to_phase_A(V_s_angle);
	//phase_A_stator_angle=rotor_angle+240.0f;//200.0f;
/*
	if (rotor_angle==0.0f)
		phase_A_stator_angle=rotor_angle-180.0f;//200.0f;
	else
		phase_A_stator_angle=rotor_angle-120.0f;//200.0f;
*/	
}


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
	
}
/*
void second_hall_update (int* rotor_speed_loop_state)
{
	
}
*/
