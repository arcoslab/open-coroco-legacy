/*
 * Copyright (C) 2013 ARCOS-Lab Universidad de Costa Rica
 * Author: Federico Ruiz Ugalde <memeruiz@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

//stm32f4 
#include <libopencm3/stm32/f4/rcc.h>
#include <libopencm3/stm32/f4/gpio.h>
#include <libopencm3/stm32/f4/timer.h>
#include <libopencm3/stm32/f4/nvic.h>
#include <libopencm3/stm32/f4/adc.h>


//usb serial cdcacm
#include <libopencm3-plus/newlib/syscall.h>
#include <DTC_example.h>
#include <libopencm3-plus/cdcacm_one_serial/cdcacm.h>
#include <libopencm3-plus/utils/misc.h>
#include <libopencm3-plus/stm32f4discovery/leds.h>

//external libraries
#include <stdio.h>
#include <limits.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

//parameters
#include "Configuration/motor.h"
#include "Configuration/pwm-related_timer_configuration.h"
//functions
#include "Print/Print.h"
#include "Print/Print.c"
#include "PID/PID.h"
#include "PID/PID.c"
#include "F4/initial_setup.h"
#include "F4/initial_setup.c"
#include "DTC/DTC.h"
#include "DTC/DTC.c"
#include "Shunt/shunt.h"
#include "Shunt/shunt.c"
#include "F4/interrupts.c"

int main(void)
{
  system_init();
   
  printf ("\n\n****************************************************************************************************************\n");

  while (1){
   
   frequency_input();


   // printf(" e: %7.2f, e_p %6.2f, e_i: %6.2f, adv: %6.2f, c_f: %6.2f, r_f: %6.2f, att: %6.2f, counter %d, eof %d, buf: %s, v: %f, e_a: %f, cmd_a: %f\n", error, p_error, i_error, pi_control*180.0f/PI, 1.0f/(period/TICK_PERIOD), ref_freq, attenuation, counter, eof, cmd, value,est_angle*180.0f/PI,cmd_angle*180.0f/PI);

    //printf(" e: %7.2f, e_p %6.2f, e_i: %6.2f, adv: %6.2f, c_f: %6.2f, r_f: %6.2f, att: %6.2f, counter  %d, buf: %s, v: %f, e_a: %f, cmd_a: %f\n", error, p_error, i_error, pi_control*180.0f/PI, 1.0f/(period/TICK_PERIOD), ref_freq, attenuation, counter, cmd, value,est_angle*180.0f/PI,cmd_angle*180.0f/PI);


int S_A=0;
int S_B=0;
int S_C=0;



float i_sD=0.0f;
float i_sQ=0.0f;
float i_s=0.0f;
float cita_i_s=0.0f;



float V_sD=0.0f;
float V_sQ=0.0f;
float V_s =0.0f;
float cita_V_s=0.0f;

float psi_sD=0.0f;
float psi_sQ=0.0f;
float psi_s=0.0f;
float psi_alpha=0.0f;

float R_s=0.0f;


switching_states (&S_A,&S_B,&S_C);
V_sD=direct_stator_voltage_V_sD     (S_A,S_B,S_C,U_d);
V_sQ=quadrature_stator_voltage_V_SQ (S_B,S_C,U_d);
V_s =vector_magnitude(V_sQ,V_sD);
cita_V_s=vector_angle    (V_sQ,V_sD);

i_sD=direct_stator_current_i_sD     (i_sA);
i_sQ=quadrature_stator_current_i_sQ (i_sA,i_sB);
i_s =vector_magnitude(i_sQ,i_sD);
cita_i_s=vector_angle(i_sQ,i_sD);

psi_sD   =direct_stator_flux_linkage_estimator_psi_sD     (TICK_PERIOD,V_sD,i_sD,R_s);
psi_sQ   =quadrature_stator_flux_linkage_estimator_psi_sQ (TICK_PERIOD,V_sQ,i_sQ,R_s);
psi_s    =stator_flux_linkage_magnite_psi_s               (psi_sD,psi_sQ);
psi_alpha=stator_flux_linkage_sector_alpha                (psi_sD,psi_sQ);


//if(S_A!=2 || S_B!=2 || S_C!=2)
if(S_A!=2 && S_B!=2 && S_C!=2)
{
/*
  if (S_A==2) S_A=0;
  if (S_B==2) S_B=0;
  if (S_C==2) S_C=0;
*/
  //printf ("S_A: %4d S_1: %4d S_4: %4d S_B: %4d S_3: %4d S_6: %4d S_C: %4d S_5: %4d S_2: %4d V_sD: %6.2f V_sQ: %6.2f V_s: %6.2f cita: %6.2f\n", S_A,S1,S4,S_B,S3,S6,S_C,S5,S2,V_sD,V_sQ,V_s,cita_V_s);
  //printf ("S_A: %4d S_B: %4d S_C: %4d V_sD: %6.2f V_sQ: %6.2f V_s: %6.2f cita: %6.2f i_sD: %6.2f i_sQ: %6.2f i_s: %6.2f cita: %6.2f\n", S_A,S_B,S_C,V_sD,V_sQ,V_s,cita_V_s,i_sD,i_sQ,i_s,cita_i_s);
  printf ("U_d: %6.2f V_sD: %6.2f V_sQ: %6.2f V_s: %6.2f cita: %6.2f i_sA: %6.2f i_sB: %6.2f i_sD: %6.2f i_sQ: %6.2f i_s: %6.2f cita: %6.2f freq: %6.2f\n", U_d,V_sD,V_sQ,V_s,cita_V_s,i_sA,i_sB,i_sD,i_sQ,i_s,cita_i_s,CUR_FREQ);
}

  }
}
