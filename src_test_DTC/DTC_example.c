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

#include "Shunt/shunt.h"
#include "Shunt/shunt.c"
#include "DTC/DTC.h"
#include "DTC/DTC.c"
#include "F4/interrupts.c"


#include "Parameter_Identification/parameter_identification.h"
#include "Parameter_Identification/parameter_identification.c"

int main(void)
{
  system_init();
   
  printf ("\n\n****************************************************************************************************************\n");





  while (1)
  {
   
    frequency_input();

    if (print_current==true )
    {
      current_counter=0;
      while (current_counter<998)
      {
        printf("\n %d %6.2f   %6.2f   %6.2f %6.2f   %6.2f   %6.2f ",current_counter,current_data_i_sA[current_counter],current_data_i_sB[current_counter],-current_data_i_sA[current_counter]-current_data_i_sB[current_counter],U_d*switching_data_SA[current_counter],U_d*switching_data_SB[current_counter],U_d*switching_data_SC[current_counter]);
        current_counter++;
      }
    
      print_current=false;
      current_counter=0;
    }




   // printf(" e: %7.2f, e_p %6.2f, e_i: %6.2f, adv: %6.2f, c_f: %6.2f, r_f: %6.2f, att: %6.2f, counter %d, eof %d, buf: %s, v: %f, e_a: %f, cmd_a: %f\n", error, p_error, i_error, pi_control*180.0f/PI, 1.0f/(period/TICK_PERIOD), ref_freq, attenuation, counter, eof, cmd, value,est_angle*180.0f/PI,cmd_angle*180.0f/PI);

    //printf(" e: %7.2f, e_p %6.2f, e_i: %6.2f, adv: %6.2f, c_f: %6.2f, r_f: %6.2f, att: %6.2f, counter  %d, buf: %s, v: %f, e_a: %f, cmd_a: %f\n", error, p_error, i_error, pi_control*180.0f/PI, 1.0f/(period/TICK_PERIOD), ref_freq, attenuation, counter, cmd, value,est_angle*180.0f/PI,cmd_angle*180.0f/PI);



if (motor_stop==false)
{
  //printf ("freq: %6.2f i_sA: %6.2f i_sB: %6.2f U_d: %6.2f\n", CUR_FREQ,i_sA,i_sB,U_d);
  //printf ("%6.2f \n",i_sA);
}







  //printf ("S_A: %4d S_1: %4d S_4: %4d S_B: %4d S_3: %4d S_6: %4d S_C: %4d S_5: %4d S_2: %4d V_sD: %6.2f V_sQ: %6.2f V_s: %6.2f cita: %6.2f\n", S_A,S1,S4,S_B,S3,S6,S_C,S5,S2,V_sD,V_sQ,V_s,cita_V_s);
  //printf ("S_A: %4d S_B: %4d S_C: %4d V_sD: %6.2f V_sQ: %6.2f V_s: %6.2f cita: %6.2f i_sD: %6.2f i_sQ: %6.2f i_s: %6.2f cita: %6.2f\n", S_A,S_B,S_C,V_sD,V_sQ,V_s,cita_V_s,i_sD,i_sQ,i_s,cita_i_s);

if(S_A!=2 && S_B!=2 && S_C!=2)
{
  //printf ("U_d: %6.2f V_sD: %6.2f V_sQ: %6.2f V_s: %6.2f cita: %6.2f i_sA: %6.2f i_sB: %6.2f i_sD: %6.2f i_sQ: %6.2f i_s: %6.2f cita: %6.2f freq: %6.2f\n", U_d,V_sD,V_sQ,V_s,cita_V_s,i_sA,i_sB,i_sD,i_sQ,i_s,cita_i_s,CUR_FREQ);
//  printf ("i_sD: %6.2f i_sQ: %6.2f i_s: %6.2f cita: %6.2f freq: %6.2f psi_sD: %6.2f psi_sQ: %6.2f psi_s: %6.2f psi_alpha: %6.2f   \n",i_sD,i_sQ,i_s,cita_i_s,CUR_FREQ,psi_sD,psi_sQ,psi_s,psi_alpha);
//printf ("freq: %6.2f psi_sD: %6.2f psi_sQ: %6.2f psi_s: %6.2f psi_alpha: %6.2f   \n", CUR_FREQ,psi_sD,psi_sQ,psi_s,psi_alpha);
//printf ("freq: %6.2f i_sA: %6.2f i_sB: %6.2f U_d: %6.2f\n", CUR_FREQ,i_sA,i_sB,U_d);


}

  }
}
