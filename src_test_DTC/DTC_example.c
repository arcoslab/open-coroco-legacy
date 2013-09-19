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

                   // printf ("V_s_an: %6.2f i_s_an: %6.2f psi_s_an: %6.2f hall_angle: %6.2f freq: %6.2f U_d: %6.2f\n", cita_V_s,cita_i_s, vector_angle(psi_sQ,psi_sD), angle_hall1,CUR_FREQ,U_d); 

    //------------------------------------------------------------------------------
    if (print_current==true )
    {
      current_counter=1;
      while (current_counter<SAMPLES-2)
      {
        //printf("\n %d %6.2f   %6.2f   %6.2f %6.2f   %6.2f   %6.2f ",current_counter,current_data_i_sA[current_counter],current_data_i_sB[current_counter],-current_data_i_sA[current_counter]-current_data_i_sB[current_counter],U_d*switching_data_SA[current_counter],U_d*switching_data_SB[current_counter],U_d*switching_data_SC[current_counter]);


        if (print_selection==0)
        {
          printf ("freq: %6.2f i_sA: %6.2f i_sB: %6.2f i_sD: %6.2f i_sQ: %6.2f i_s: %6.2f cita_i_s: %6.2f U_d: %6.2f \n", data_CUR_FREQ[current_counter],data_i_sA[current_counter],data_i_sB[current_counter],data_i_sD[current_counter],data_i_sQ[current_counter],data_i_s[current_counter],data_cita_i_s[current_counter],data_U_d[current_counter]);
        }
        else if (print_selection==1)
        {
          printf ("freq: %6.2f V_sA: %6.2f V_sB: %6.2f VsC: %6.2f V_sD: %6.2f V_sQ: %6.2f V_s: %6.2f cita_V_s: %6.2f U_d: %6.2f \n", data_CUR_FREQ[current_counter],data_S_A_f[current_counter]*data_U_d[current_counter],data_S_B_f[current_counter]*data_U_d[current_counter],data_S_C_f[current_counter]*data_U_d[current_counter],data_V_sD[current_counter],data_V_sQ[current_counter],data_V_s[current_counter],data_cita_V_s[current_counter],data_U_d[current_counter]);
        }
        else if (print_selection==2)
        {
          printf ("freq: %6.2f psi_sD: %9.6f psi_sQ: %9.6f psi_s: %9.6f psi_alpha: %2d U_d: %8.2f timer: %8d\n", data_CUR_FREQ[current_counter],data_psi_sD[current_counter],data_psi_sQ[current_counter],data_psi_s[current_counter],data_psi_alpha[current_counter],data_U_d[current_counter],timer[current_counter]);
        }
        else if (print_selection==3)
        {
          //printf ("freq: %6.2f t_e: %8.2f psi_s_ref: %6.2f t_e_ref: %6.2f d_psi: %4d d_te: %4d psi_delta: %6.2f t_e_delta: %6.2f time: %8d \n", data_CUR_FREQ[current_counter],data_t_e[current_counter],data_psi_s_ref[current_counter],data_t_e_ref[current_counter],data_d_psi[current_counter],data_d_te[current_counter],data_psi_delta_percentage[current_counter],data_t_e_delta_percentage[current_counter],timer[current_counter]);
                    printf ("te: %5.2f psi_s_ref: %10.8f te_ref: %6.2f dpsi: %2d dte: %2d psi_s: %6.4f psi_a: %1d t: %6d SA: %2d SB: %2d SC: %1d v: %2d \n",data_t_e[current_counter],data_psi_s_ref[current_counter],data_t_e_ref[current_counter],data_d_psi[current_counter],data_d_te[current_counter],data_psi_s[current_counter],data_psi_alpha[current_counter],timer[current_counter],data_S_A[current_counter],data_S_B[current_counter],
data_S_C[current_counter],data_optimal_voltage_vector[current_counter]);


        }
 else if (print_selection==4)
        {

while (PID_angle[current_counter]>(2.0f*PI) )
  PID_angle[current_counter]=PID_angle[current_counter]-2.0*PI;
/*
if (PID_angle[current_counter]<0.0f)
  PID_angle[current_counter]=PID_angle[current_counter]+2.0f*PI;
*/
float ang_PID=(180.0f/PI)*PID_angle[current_counter];
if (ang_PID<0.0f)
  ang_PID=ang_PID+360.0f;

                    printf ("V_s_an: %6.2f i_s_an: %6.2f psi_s_an: %6.2f hall_angle: %8.2f freq: %6.2f U_d: %6.2f timer: %8d\n", data_cita_V_s[current_counter],data_cita_i_s[current_counter], vector_angle(data_psi_sQ[current_counter],data_psi_sD[current_counter]), ang_PID,data_CUR_FREQ[current_counter],data_U_d  [current_counter],timer[current_counter]); 

        }


//else 
if (print_selection==5 )//&& data_V_s[current_counter] > 0.0f)
        {
/*
          float angle_delay=0.0f;
	  angle_delay=data_cita_i_s[current_counter]-data_cita_V_s[current_counter];
          if (angle_delay>=360.0f)
            angle_delay=angle_delay-360.0f;
	  else if (angle_delay<0.0f)
            angle_delay=angle_delay+360.0f;

          printf ("V_sD: %6.2f V_sQ: %6.2f V_s: %6.2f cita_V_s: %6.2f i_sD: %6.2f i_sQ: %6.2f i_s: %6.2f cita_i_s: %6.2f  diff: %6.2f\n", data_V_sD[current_counter],data_V_sQ[current_counter],data_V_s[current_counter],data_cita_V_s[current_counter],data_i_sD[current_counter],data_i_sQ[current_counter],data_i_s[current_counter],data_cita_i_s[current_counter],angle_delay);
*/

/*
          printf ("V_sD: %6.2f V_sQ: %6.2f i_sD: %6.2f i_sQ: %6.2f psi_sD: %f psi_sQ: %f psi_s: %6.5f psi_alpha: %2d R_s: %6.3f\n", data_V_sD[current_counter],data_V_sQ[current_counter],data_i_sD[current_counter],data_i_sQ[current_counter],data_psi_sD[current_counter],data_psi_sQ[current_counter],data_psi_s[current_counter],data_psi_alpha[current_counter],data_R_s[current_counter]);
*/
printf ("psi_sD: %9.6f psi_sQ: %9.6f psi_s: %6.5f angle: %6.2f psi_alpha: %2d timer: %8d freq: %6.2f U_d: %6.2f\n", data_psi_sD[current_counter],data_psi_sQ[current_counter],data_psi_s[current_counter],vector_angle(data_psi_sQ[current_counter],data_psi_sD[current_counter]),data_psi_alpha[current_counter], timer[current_counter], data_CUR_FREQ[current_counter],data_U_d  [current_counter]);

        }

/*
        data_S_A[current_counter]=S_A;
        data_S_B[current_counter]=S_B;
        data_S_C[current_counter]=S_C;

        data_S_A_f[current_counter]=S_A_f;
        data_S_B_f[current_counter]=S_B_f;
        data_S_C_f[current_counter]=S_C_f;

        data_i_sA [current_counter]=i_sA;
        data_i_sB [current_counter]=i_sB;
        data_U_d  [current_counter]=U_d;

        data_i_sD[current_counter]=i_sD;
        data_i_sQ[current_counter]=i_sQ;
        data_i_s[current_counter]=i_s;
        data_cita_i_s[current_counter]=cita_i_s;

        data_V_sD[current_counter];
        data_V_sQ[current_counter];
        data_V_s [current_counter];
        data_cita_V_s[current_counter];

        data_psi_sD[current_counter];
        data_psi_sQ[current_counter];
        data_psi_s [current_counter];
        data_psi_alpha[current_counter];

        data_t_e[current_counter];

        data_psi_s_ref[current_counter];
        data_t_e_ref[current_counter];

        data_d_psi[current_counter];
        data_d_te[current_counter];
        data_psi_delta_percentage[current_counter];
        data_t_e_delta_percentage[current_counter];

        data_R_s[current_counter];
        data_pole_pairs[current_counter;
        data_L_sq[current_counter];
        data_psi_F[current_counter];

*/







        current_counter++;
      }
    
      print_current=false;
      current_counter=0;
    }


/*2
    if (motor_stop==false)
    {

      if (print_selection==0)
      {
        printf ("freq: %6.2f i_sA: %6.2f i_sB: %6.2f i_sD: %6.2f i_sQ: %6.2f i_s: %6.2f cita_i_s: %6.2f U_d: %6.2f \n", CUR_FREQ,i_sA,i_sB,i_sD,i_sQ,i_s,cita_i_s,U_d);
      }
      else if (print_selection==1)
      {
        printf ("freq: %6.2f V_sD: %6.2f V_sQ: %6.2f V_s: %6.2f cita_V_s: %6.2f U_d: %6.2f \n", CUR_FREQ,V_sD,V_sQ,V_s,cita_V_s,U_d);
      }
      else if (print_selection==2)
      {
        printf ("freq: %6.2f psi_sD: %6.2f psi_sQ: %6.2f psi_s: %6.2f psi_alpha: %4d U_d: %6.2f \n", CUR_FREQ,psi_sD,psi_sQ,psi_s,psi_alpha,U_d);
      }
      else if (print_selection==3)
      {
        printf ("freq: %6.2f t_e: %8.2f psi_s_ref: %6.2f t_e_ref: %6.2f d_psi: %4d d_te: %4d psi_delta: %6.2f t_e_delta: %6.2f \n", CUR_FREQ,t_e,psi_s_ref,t_e_ref,d_psi,d_te,psi_delta_percentage,t_e_delta_percentage);
      }
    }
*/



/*
S_A_f
S_B_f
S_C_f

i_sA
i_sB
U_d  

i_sD
i_sQ
i_s
cita_i_s

V_sD
V_sQ
V_s 
cita_V_s

psi_sD
psi_sQ
psi_s
psi_alpha

t_e

psi_s_ref
t_e_ref

d_psi
d_te
psi_delta_percentage
t_e_delta_percentage


//motor parameters;
R_s        
pole_pairs 
L_sq       
psi_F      
*/




    




    if(S_A!=2 && S_B!=2 && S_C!=2)
    {
      //printf ("freq: %6.2f i_sA: %6.2f i_sB: %6.2f U_d: %6.2f\n", CUR_FREQ,i_sA,i_sB,U_d);
    }

  }
}
