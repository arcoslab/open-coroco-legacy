/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2011 Piotr Esden-Tempski <piotr@esden.net>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

float 
	captured_max_ticks,
	captured_previous_hall_ticks,
	captured_sine_freq,


	captured_V_hall_1_V1,
	captured_V_hall_1_V2,
	captured_V_hall_2_V1,
	captured_V_hall_2_V2,
	captured_V_hall_3_V1,
	captured_V_hall_3_V2;


#include "pwm_6step_experiment.h"
//impedance control libraries
#include "set_up.c"
#include "usart_functions.c"
#include "timer_1.c"
#include "hall_sensors.c"
#include "control_loops.c"
#include "interrupt_handler.c"


int main(void)
{
	stm32_setup();
	
	// hall data initialization
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

	float 
	captured_ticks=0.0f,
	II_captured_max_ticks=0.0f,
	II_captured_previous_hall_ticks=0.0f,
	II_captured_sine_freq=0.0f;
	//captured_hall_ticks=0.0f;
	//bool
	//captured_corrected=false;
	
	//int usart_counter=0;

	while (1)
	{
		
		
		II_captured_max_ticks=max_ticks;
		II_captured_previous_hall_ticks=previous_hall_ticks;
		II_captured_sine_freq=sine_freq;
		//captured_hall_ticks=hall_ticks;
		//captured_corrected=hall1_data.hall_corrected;

		captured_open_loop_error=open_loop_error;
		captured_open_loop_desired_sine_frequency=open_loop_desired_sine_frequency;
		captured_actual_sine_frequency=actual_sine_frequency;

		/*II_captured_V_hall_1_V1=captured_V_hall_1_V1;
		II_captured_V_hall_1_V2=captured_V_hall_1_V2;
		II_captured_V_hall_2_V1=captured_V_hall_2_V1;
		II_captured_V_hall_2_V2=captured_V_hall_2_V2;
		II_captured_V_hall_3_V1=captured_V_hall_3_V1;
		II_captured_V_hall_3_V2=captured_V_hall_3_V2;*/

		captured_close_loop_error		=close_loop_error;
		captured_close_loop_desired_frequency	=close_loop_desired_frequency;
		captured_desired_previous_hall_ticks	=desired_previous_hall_ticks;
		captured_close_loop_tick_error		=close_loop_tick_error;
		captured_phaseU				=phaseU;
		captured_phase_advance			=phase_advance;
		captured_phase_excess			=phase_excess;
		captured_phase_stator			=phase_stator;
		captured_phase_rotor			=phase_rotor;
		captured_phase_sum			=phase_sum;

		captured_ticks				=ticks;
		captured_attenuation			=attenuation;

			usart_send_blocking(USART2, 'm');
			usart_transmit_a_floating_number(II_captured_max_ticks);
			usart_send_blocking(USART2, 'p');
			usart_transmit_a_floating_number(II_captured_previous_hall_ticks);
			//usart_send_blocking(USART2, 'h');
			//usart_transmit_an_integer_number(captured_hall_ticks);
			usart_send_blocking(USART2, 't');
			usart_transmit_a_floating_number(captured_ticks*360.0f/II_captured_max_ticks);
			
			//usart_send_blocking(USART2, 'e');
			//usart_transmit_a_floating_number(II_captured_sine_freq);
			usart_send_blocking(USART2, 'a');
			usart_transmit_a_floating_number(captured_actual_sine_frequency);
			//usart_send_blocking(USART2, 'd');
			//usart_transmit_a_floating_number(captured_desired_sine_frequency);
			//usart_send_blocking(USART2, 'E');
			//usart_transmit_a_floating_number(open_loop_error);

			usart_send_blocking(USART2, 'A');
			usart_transmit_a_floating_number(captured_attenuation*100.0f);
			usart_send_blocking(USART2, 'E');
			usart_transmit_a_floating_number(captured_close_loop_error);
			usart_send_blocking(USART2, 'D');
			usart_transmit_a_floating_number(captured_close_loop_desired_frequency);
			usart_send_blocking(USART2, 'P');
			//usart_transmit_a_floating_number(captured_desired_previous_hall_ticks);
			//usart_send_blocking(USART2, 'T');
			usart_transmit_a_floating_number(captured_close_loop_tick_error);
			usart_send_blocking(USART2, 'U');
			usart_transmit_a_floating_number(captured_phaseU);
			usart_send_blocking(USART2, 'A');
			usart_transmit_a_floating_number(captured_phase_advance);
			//usart_send_blocking(USART2, 'X');
			//usart_transmit_a_floating_number(captured_phase_excess);
			//usart_send_blocking(USART2, 'S');
			//usart_transmit_a_floating_number(captured_phase_stator);
			//usart_send_blocking(USART2, 'R');
			//usart_transmit_a_floating_number(captured_phase_rotor);
			//usart_send_blocking(USART2, 'M');
			//usart_transmit_a_floating_number(captured_phase_sum);

		
		
			
			
/*
			usart_send_blocking(USART2, 'V');
			usart_transmit_a_floating_number(captured_V_hall_1_V1*1000);

			usart_send_blocking(USART2, 'V');
			usart_transmit_a_floating_number(captured_V_hall_2_V1*1000);

			usart_send_blocking(USART2, 'V');
			usart_transmit_a_floating_number(captured_V_hall_3_V1*1000);
*/
			usart_send_blocking(USART2, 'D');
			if (hall1_data.hall_corrected==HIGH)
			{

				usart_send_blocking(USART2, '1');
			}
			else
			{
				usart_send_blocking(USART2, '0');
			}

			usart_send_blocking(USART2, '\r');
			usart_send_blocking(USART2, '\n');


			//usart_counter=0;
		
		
		hall_sensors();

	}	
		return 0;
}
