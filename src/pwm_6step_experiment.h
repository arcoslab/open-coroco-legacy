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


#ifndef PWM_6STEP_EXPERIMENT_H
#define PWM_6STEP_EXPERIMENT_H


//stm32F4 libopencm3 libraries
#include <libopencm3/stm32/f4/rcc.h>
#include <libopencm3/stm32/f4/gpio.h>
#include <libopencm3/stm32/f4/timer.h>
#include <libopencm3/stm32/f4/adc.h>
#include <libopencm3/stm32/nvic.h>
#include <libopencm3/stm32/exti.h>
#include <libopencm3/stm32/usart.h>
#include <stdio.h>
#include <math.h>


//pwm-related timer configuration
#define system_freq 	168000000 				//168MHz
#define pwm_freq 	64000					//32000 
#define pwmfreq_f 	64000.0f				//32000.0f
#define prescale 	1					//freq_CK_CNT=freq_CK_PSC/(PSC[15:0]+1)
#define pwm_period_ARR 	system_freq/( pwm_freq*(prescale+1) )	
#define repetition_counter_value 	0				
#define deadtime_percentage		0.10f	//10% 87.2us de alto a bajo, 48.8us de bajo a alto 
#define	initial_duty_cycle		0.5f


//
#define CYCLE_TIME	1.0f/pwmfreq_f

/*new motor: 
	min electrical frequency: 20Hz
	max electrical frequency: 425Hz (for full open loop demo (some))*/ 
#define max_sinusoidal_periods	5
#define sine_freq_increment	0.75f
#define max_sine_freq		20.0f
#define min_sine_freq		10.0f	
#define sine_freq_fixed	10.0f

#define attenuation_value 		1.0f
#define attenuation_max 		1.0f
#define attenuation_min 		0.25f
#define attenuation_increment 		0.1f



#define PI 				3.1415926535f 

#define FALLING 0
#define RISING 1

#define DOWN 0
#define UP 1

#define HIGH true
#define LOW false

#define YES true
#define NO  false

int
	button=0;	
	
u16 
	exti_direction = FALLING;
	
float	
	attenuation		=0.0f,
	//ticks			=0.0f,
	//hall_ticks		=0.0f,
	//previous_hall_ticks	=0.0f,
	//max_ticks		=pwmfreq_f/sine_freq_fixed,
	sine_freq		=sine_freq_fixed;


float open_loop_error=0.0f;
float open_loop_desired_sine_frequency=25.0f;
float actual_sine_frequency;

float captured_open_loop_error=0.0f;
float captured_open_loop_desired_sine_frequency=0.0f;
float captured_actual_sine_frequency=0.0f;



float close_loop_error=0.0f;
float close_loop_desired_frequency=100.0f;
//float desired_previous_hall_ticks=0.0f;
//float close_loop_tick_error=0.0f;
float phaseU=0.0f;
float phase_advance=0.0f;
float phase_excess=0.0f;
float phase_stator=0.0f;
float phase_rotor=0.0f;
float phase_sum;

float captured_close_loop_error=0.0f;
float captured_close_loop_desired_frequency=0.0f;
//float captured_desired_previous_hall_ticks=0.0f;
//float captured_close_loop_tick_error=0.0f;
float captured_phaseU=0.0f;
float captured_phase_advance=0.0f;
float captured_phase_excess=0.0f;
float captured_phase_stator=0.0f;
float captured_phase_rotor=0.0f;
float captured_phase_sum=0.0f;

float captured_attenuation=0.0f;

int frequency_change_counter=0;


float offset=0.0f;
float rotor_angle=0.0f;
float stator_angle=0.0f;
float hall_time=0.0f;
float previous_hall_time=0.0f;




float 
	V_sD=0.0f,
	V_sQ=0.0f,
	V_s_angle=0.0f,
	V_s_magnitude=0.0f;



#endif
