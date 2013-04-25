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

float captured_A,captured_B,captured_C;

#include "pwm_6step_experiment.h"
//impedance control libraries
#include "set_up.c"
#include "usart_functions.c"
#include "timer_1.c"
#include "timer_2.c"
#include "hall_sensors.c"
#include "control_loops.c"
#include "interrupt_handler.c"


int main(void)
{
	stm32_setup();

	//galletas
	
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
	//captured_ticks=0.0f,
	//II_captured_max_ticks=0.0f,
	//II_captured_previous_hall_ticks=0.0f,
	II_captured_sine_freq=0.0f;
	//captured_hall_ticks=0.0f;
	//bool
	//captured_corrected=false;
	
	//int usart_counter=0;

	while (1)
	{

float captured_offset=0.0f;
float captured_rotor_angle=0.0f;
float captured_phase_A_stator_angle=0.0f;
//float captured_hall_time=0.0f;
float captured_previous_hall_time=0.0f;

captured_offset=offset;
captured_rotor_angle=rotor_angle;
captured_phase_A_stator_angle=phase_A_stator_angle;
//captured_hall_time=hall_time;
captured_previous_hall_time=previous_hall_time;

captured_A=A;
captured_B=B;
captured_C=C;

float 
	captured_V_sD,
	captured_V_sQ,
	captured_V_s_angle,
	captured_V_s_magnitude;



//nota: no se deben combinar muchos comandos del usart con el hall sensor, pueden interferir y hacer que se lea mal la velocidad del motor	
		
		captured_V_sD=V_sD;
		captured_V_sQ=V_sQ;
		captured_V_s_angle=V_s_angle;
		captured_V_s_magnitude=V_s_magnitude;



		//II_captured_max_ticks=max_ticks;
		//II_captured_previous_hall_ticks=previous_hall_ticks;
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
		//captured_desired_previous_hall_ticks	=desired_previous_hall_ticks;
		//captured_close_loop_tick_error		=close_loop_tick_error;
		captured_phaseU				=phaseU;
		captured_phase_advance			=phase_advance;
		captured_phase_excess			=phase_excess;
		captured_phase_stator			=phase_stator;
		captured_phase_rotor			=phase_rotor;
		captured_phase_sum			=phase_sum;

		//captured_ticks				=ticks;
		captured_attenuation			=attenuation;



			captured_V_hall_1_V1=V_hall_1_V1;
	captured_V_hall_1_V2=V_hall_1_V2;
	captured_V_hall_2_V1=V_hall_2_V1;
	captured_V_hall_2_V2=V_hall_2_V2;
	captured_V_hall_3_V1=V_hall_3_V1;
	captured_V_hall_3_V2=V_hall_3_V2;



			//usart_send_blocking(USART2, 'm');
			//usart_transmit_a_floating_number(II_captured_max_ticks);
			//usart_send_blocking(USART2, 'p');
			//usart_transmit_a_floating_number(II_captured_previous_hall_ticks);
			//usart_send_blocking(USART2, 'h');
			//usart_transmit_an_integer_number(captured_hall_ticks);
			//usart_send_blocking(USART2, 't');
			//usart_transmit_a_floating_number(captured_ticks*360.0f/II_captured_max_ticks);
/*			
			usart_send_blocking(USART2, 'e');
			usart_transmit_a_floating_number(II_captured_sine_freq);

			usart_send_blocking(USART2, 'a');
			usart_transmit_a_floating_number(captured_actual_sine_frequency);
*/			//usart_send_blocking(USART2, 'd');
			//usart_transmit_a_floating_number(captured_desired_sine_frequency);
			//usart_send_blocking(USART2, 'E');
			//usart_transmit_a_floating_number(open_loop_error);
/*
			usart_send_blocking(USART2, 'T');
			usart_transmit_a_floating_number(captured_attenuation*100.0f);
*/			//usart_send_blocking(USART2, 'E');
			//usart_transmit_a_floating_number(captured_close_loop_error);
			//usart_send_blocking(USART2, 'D');
			//usart_transmit_a_floating_number(captured_close_loop_desired_frequency);
			//usart_send_blocking(USART2, 'P');
			//usart_transmit_a_floating_number(captured_desired_previous_hall_ticks);
			//usart_send_blocking(USART2, 'T');
			//usart_transmit_a_floating_number(captured_close_loop_tick_error);
			//usart_send_blocking(USART2, 'U');
			//usart_transmit_a_floating_number(captured_phaseU);
			//usart_send_blocking(USART2, 'A');
			//usart_transmit_a_floating_number(captured_phase_advance);
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
			usart_transmit_a_floating_number(captured_V_hall_1_V1);

			usart_send_blocking(USART2, 'V');
			usart_transmit_a_floating_number(captured_V_hall_2_V1);

			usart_send_blocking(USART2, 'V');
			usart_transmit_a_floating_number(captured_V_hall_3_V1);
*/


			usart_send_blocking(USART2, 'A');
			usart_transmit_a_floating_number(captured_phase_A_stator_angle);

			usart_send_blocking(USART2, 'R');
			usart_transmit_a_floating_number(captured_rotor_angle);

/*
			usart_send_blocking(USART2, 'H');
			usart_transmit_a_floating_number(captured_previous_hall_time);

			usart_send_blocking(USART2, 'O');
			usart_transmit_a_floating_number(captured_offset);
*/
/*
	float duty_a,duty_b,duty_c;
	static float cita=0.0f;
	duty_a=sinf(cita*PI/180.0f);
	duty_b=sinf(cita*PI/180.0f+2.0f*PI/3.0f);
	duty_c=sinf(cita*PI/180.0f+4.0f*PI/3.0f);

	//park_transformation
	V_sD		=direct_clark_transformation	(duty_a,duty_b,duty_c);
	V_sQ		=quadrature_clark_transformation(duty_a,duty_b,duty_c);
	V_s_angle	=vector_angle			(V_sQ,V_sD);
	V_s_magnitude	=vector_magnitude		(V_sQ,V_sD);		

	//inverse park transformation	
	float cita_A, cita_B, cita_C;
	cita_A=duty_cycle_to_angle(	A_inverse_clark_transformation(V_sQ,V_sD)		);
  	cita_B=duty_cycle_to_angle(	B_inverse_clark_transformation(V_sQ,V_sD)	);
	cita_C=duty_cycle_to_angle(	C_inverse_clark_transformation(V_sQ,V_sD)	);

	A=A_inverse_clark_transformation(V_sQ,V_sD)		;
  	B=B_inverse_clark_transformation(V_sQ,V_sD)	;
	C=C_inverse_clark_transformation(V_sQ,V_sD)	;

			usart_send_blocking(USART2, 'C');
			usart_transmit_a_floating_number(cita);
	cita=cita+1.0f;

	if (cita>=360.0f)
		cita=cita-360.0f;
*/

/*
			usart_send_blocking(USART2, 'D');
			usart_transmit_a_floating_number(captured_V_sD*1000.0f);

			usart_send_blocking(USART2, 'Q');
			usart_transmit_a_floating_number(captured_V_sQ*1000.0f);
*/
			usart_send_blocking(USART2, 'S');
			usart_transmit_a_floating_number(captured_V_s_angle);
/*
			usart_send_blocking(USART2, 'M');
			usart_transmit_a_floating_number(captured_V_s_magnitude*1000.0f);

			usart_send_blocking(USART2, 'A');
			usart_transmit_a_floating_number(captured_A);

			usart_send_blocking(USART2, 'B');
			usart_transmit_a_floating_number(captured_B);

			usart_send_blocking(USART2, 'C');
			usart_transmit_a_floating_number(captured_C);
*/
/*
			usart_send_blocking(USART2, 'a');
			usart_transmit_a_floating_number(duty_a*1000.0f);

			usart_send_blocking(USART2, 'b');
			usart_transmit_a_floating_number(duty_b*1000.0f);

			usart_send_blocking(USART2, 'c');
			usart_transmit_a_floating_number(duty_c*1000.0f);


			usart_send_blocking(USART2, 'D');
			usart_transmit_a_floating_number(V_sD*1000.0f);

			usart_send_blocking(USART2, 'Q');
			usart_transmit_a_floating_number(V_sQ*1000.0f);
*/
/*
			usart_send_blocking(USART2, 'A');
			usart_transmit_a_floating_number(phase_A_stator_angle);

			usart_send_blocking(USART2, 'X');
			usart_transmit_a_floating_number(captured_V_s_angle);

			usart_send_blocking(USART2, 'a');
			usart_transmit_a_floating_number(captured_A);

			usart_send_blocking(USART2, 'x');
			usart_transmit_a_floating_number(captured_B);
*/
/*
			float s;
			float v;
				
			s=451.0f-cita;
			if (s>=360.0f)
				s=s-360.0f;
			else if (s<0.0f)
				s=s+360.0f;
			
			v=450-V_s_angle;
			if (v>=360.0f)
				v=v-360.0f;
			else if (v<0.0f)
				v=v+360.0f;


			usart_send_blocking(USART2, 'S');
			usart_transmit_a_floating_number(s);
			
			usart_send_blocking(USART2, 'V');
			usart_transmit_a_floating_number(v);

*/
			
/*
			usart_send_blocking(USART2, 'M');
			usart_transmit_a_floating_number(V_s_magnitude*1000.0f);

			usart_send_blocking(USART2, 'A');
			usart_transmit_a_floating_number(A*1000.0f);

			usart_send_blocking(USART2, 'B');
			usart_transmit_a_floating_number(B*1000.0f);

			usart_send_blocking(USART2, 'C');
			usart_transmit_a_floating_number(C*1000.0f);

			usart_send_blocking(USART2, 'a');
			usart_transmit_a_floating_number(cita_A);

			usart_send_blocking(USART2, 'b');
			usart_transmit_a_floating_number(cita_B);

			usart_send_blocking(USART2, 'c');
			usart_transmit_a_floating_number(cita_C);


			usart_send_blocking(USART2, 'D');
			if (hall1_data.hall_corrected==HIGH)
			{

				usart_send_blocking(USART2, '1');
			}
			else
			{
				usart_send_blocking(USART2, '0');
			}

			usart_send_blocking(USART2, 'U');
			if (hall1_data.hall_update==true)
			{

				usart_send_blocking(USART2, '1');
			}
			else
			{
				usart_send_blocking(USART2, '0');
			}
	

			float ZZ;
			ZZ=gpio_get(GPIOA, GPIO1);
			usart_send_blocking(USART2, 'Z');
			usart_transmit_a_floating_number(ZZ);
*/
			usart_send_blocking(USART2, '\r');
			usart_send_blocking(USART2, '\n');

			//usart_counter=0;
		
		
		

		//hall_sensors();

	}	
		return 0;
}
