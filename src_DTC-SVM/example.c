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
#include <example.h>
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

#include "PID/sensorless_PID.h"
#include "PID/sensorless_PID.c"

#include "DTC/DTC.h"
#include "DTC/DTC.c"

#include "SVM/SVM.h"
#include "SVM/SVM.c"

#include "Print/collecting_data.h"
#include "F4/interrupts.c"

#include "Print/collecting_data.c"

#include "Parameter_Identification/parameter_identification.h"
#include "Parameter_Identification/parameter_identification.c"

int main(void)
{
  system_init();
   
  printf ("\n\n****************************************************************************************************************\n");
  printf ("\n\n*************************************DTC-SVM example************************************************************\n");
  printf ("\n\n****************************************************************************************************************\n\n");

  while (1)
  {
    printf("freq_ref:%6.2f :freq: %6.2f hall: %6.2f :Ud: %6.2f\n",ref_freq_SVM,w_r,CUR_FREQ,U_d);
   
    frequency_input();


    if (print_current==true )
    {
      current_counter=0;
      //printf ("\n\n****************************************************************************************************************\n");

      while (current_counter<SAMPLES-2)
      {
        if (print_selection==0)
        {
          printf (":t:%5d:freq: %6.2f :i_sA: %6.2f :i_sB: %6.2f :i_sD: %6.2f :i_sQ: %6.2f :i_s: %6.2f :cita_i_s: %6.2f :U_d: %6.2f \n",timer[current_counter], data_CUR_FREQ[current_counter],data_i_sA[current_counter],data_i_sB[current_counter],data_i_sD[current_counter],data_i_sQ[current_counter],data_i_s[current_counter],data_cita_i_s[current_counter],data_U_d[current_counter]);
        }
        else if (print_selection==1)
        {
          //printf ("freq: %6.2f V_sA: %6.2f V_sB: %6.2f VsC: %6.2f V_sD: %6.2f V_sQ: %6.2f V_s: %6.2f cita_V_s: %6.2f U_d: %6.2f \n", data_CUR_FREQ[current_counter],data_S_A_f[current_counter]*data_U_d[current_counter],data_S_B_f[current_counter]*data_U_d[current_counter],data_S_C_f[current_counter]*data_U_d[current_counter],data_V_sD[current_counter],data_V_sQ[current_counter],data_V_s[current_counter],data_cita_V_s[current_counter],data_U_d[current_counter]);
        }
        else if (print_selection==2)
        {
printf ("t:%5d:f:%6.2f:psi_sD:%9.6f:i_sQ:%6.2f:psi_sQ:%9.6f:i_sD:%6.2f:c_is:%6.2f:psi_an:%6.2f:te:%8.5f:U_d: %5.2f\n", 
timer[current_counter],
data_CUR_FREQ[current_counter],
data_psi_sD[current_counter],
data_i_sQ[current_counter],
data_psi_sQ[current_counter],
data_i_sD[current_counter],
data_cita_i_s[current_counter],
vector_angle(data_psi_sQ[current_counter],data_psi_sD[current_counter]),
data_t_e[current_counter],
data_U_d[current_counter]);
        }
        else if (print_selection==3)
        {
          printf (":iA:%6.2f:B:%6.2f:D:%6.2f:Q:%6.2f:s:%6.2f:c:%6.2f:U:%6.2f",
data_i_sA[current_counter],data_i_sB[current_counter],data_i_sD[current_counter],data_i_sQ[current_counter],data_i_s[current_counter],data_cita_i_s[current_counter],data_U_d[current_counter]);
        printf (":pD:%9.6f:pQ:%9.6f\n", 
data_psi_sD[current_counter],
data_psi_sQ[current_counter]
);
}

 else if (print_selection==4)
        {

          while (PID_angle[current_counter]>(2.0f*PI) )
            PID_angle[current_counter]=PID_angle[current_counter]-2.0*PI;
          float ang_PID=(180.0f/PI)*PID_angle[current_counter];
          if (ang_PID<0.0f)
            ang_PID=ang_PID+360.0f;
          float actual_hall_angle;
          actual_hall_angle=ang_PID*-1.0f;
          if (actual_hall_angle<0.0f)
            actual_hall_angle=actual_hall_angle+360.0f;
          if (actual_hall_angle>360.0f)
            actual_hall_angle=actual_hall_angle-360.0f;
          float diff;
          diff=actual_hall_angle-vector_angle(data_psi_sQ[current_counter],data_psi_sD[current_counter]);
          if (diff<0.0f)
            diff=diff+360.0f;
          if (actual_hall_angle>360.0f)
            diff=diff-360.0f;          

          printf ("Vsa: %6.2f isa: %6.2f psisan: %6.2f ha: %8.2f f: %6.2f Ud: %6.2f t: %5d w_r: %9.2f a_h: %6.2f diff: %6.2f\n", data_cita_V_s[current_counter],data_cita_i_s[current_counter], flux_linkage_angle_psi_s_angle(PID_angle[current_counter]), ang_PID,data_CUR_FREQ[current_counter],data_U_d  [current_counter],timer[current_counter],data_w_r[current_counter]/(2.0f*PI),actual_hall_angle,diff); 

        }



else if (print_selection==5 )
        {
printf (":psi_sD:%9.6f:psi_sQ:%9.6f:psi_s:%6.5f:angle:%6.2f:psi_alpha:%2d:timer:%8d:freq: %6.2f:U_d: %6.2f\n", data_psi_sD[current_counter],data_psi_sQ[current_counter],data_psi_s[current_counter],vector_angle(data_psi_sQ[current_counter],data_psi_sD[current_counter]),data_psi_alpha[current_counter], timer[current_counter], data_CUR_FREQ[current_counter],data_U_d  [current_counter]);
        }

else if (print_selection==6)
{
printf ("t:%5d:pa:%6.4f:iA:%6.2f:B:%6.2f:C:%6.2f:D:%6.2f:Q:%6.2f:S:%6.2f:c:%6.2f:pD:%7.4f:Q:%7.4f:s:%6.4f:a:%6.2f:r:%6.4f:w:%6.2f:te:%6.2f:VD:%6.2f:Q:%6.2f:s:%6.2f:c:%6.2f:r:%6.2f:T1:%4.2f:2:%4.2f:n:%5.2f:d:%4.2f:x:%4.2f:da:%4.2f:b:%4.2f:c:%4.2f:f:%6.2f:U:%6.2f:at:%10.8f:pi:%10.8f\n",
timer[current_counter],

data_phase_advance_SVM[current_counter],

data_i_sA [current_counter],
data_i_sB [current_counter],
-data_i_sA[current_counter]-data_i_sB[current_counter],
data_i_sD[current_counter],
data_i_sQ[current_counter],
data_i_s[current_counter],
data_cita_i_s[current_counter],

data_psi_sD[current_counter],
data_psi_sQ[current_counter],
data_psi_s[current_counter],
data_psi_s_alpha_SVM[current_counter],
data_psi_s_ref[current_counter],
data_w_r[current_counter],

data_t_e[current_counter],

data_V_sD                    [current_counter],
data_V_sQ                    [current_counter],
data_V_s                     [current_counter],
data_cita_V_s                [current_counter],
data_cita_V_s_relative_angle [current_counter],

data_T1[current_counter],
data_T2[current_counter],
data_T_min_on[current_counter],
data_T_med_on[current_counter],
data_T_max_on[current_counter],

data_duty_a[current_counter],
data_duty_b[current_counter],
data_duty_c[current_counter],

data_CUR_FREQ[current_counter],
data_U_d[current_counter],
data_attenuation[current_counter],
data_pi_control_SVM[current_counter]
);
}

else if (print_selection==77)// && data_ref_freq_SVM [current_counter] !=0.0f )
{


printf("t:%10d :freq_ref:%6.2f :freq: %6.2f :hall_freq:%6.2f :Vs: %6.2f ",
timer             [current_counter],
data_ref_freq_SVM [current_counter],
data_w_r          [current_counter],
data_CUR_FREQ[current_counter],
data_V_s          [current_counter]
);




printf (":phase_adv: %12.8f :isA: %6.2f :isB: %6.2f :isC: %6.2f :isD: %6.2f :isQ: %6.2f :is: %6.2f :iscita: %6.2f :psisD: %7.4f :psisQ: %7.4f :psis: %6.4f :psisa: %6.2f :psiref: %6.4f :te: %6.2f :VsD: %8.2f :VsQ: %8.2f :Vs: %6.2f :Vscita: %6.2f :Vscitar: %6.2f :T1: %4.2f :T2: %4.2f :Tmin: %5.2f :Tmed: %4.2f :Tmax: %4.2f :duta: %4.2f :dutb: %4.2f :dutc: %4.2f :Ud: %6.2f :pi: %12.8f :maxpi: %12.8f\n",

data_phase_advance_SVM[current_counter],

data_i_sA [current_counter],
data_i_sB [current_counter],
-data_i_sA[current_counter]-data_i_sB[current_counter],
data_i_sD[current_counter],
data_i_sQ[current_counter],
data_i_s[current_counter],
data_cita_i_s[current_counter],

data_psi_sD[current_counter],
data_psi_sQ[current_counter],
data_psi_s[current_counter],
data_psi_s_alpha_SVM[current_counter],
data_psi_s_ref[current_counter],

data_t_e[current_counter],

data_V_sD                    [current_counter],
data_V_sQ                    [current_counter],
data_V_s                     [current_counter],
data_cita_V_s                [current_counter],
data_cita_V_s_relative_angle [current_counter],

data_T1[current_counter],
data_T2[current_counter],
data_T_min_on[current_counter],
data_T_med_on[current_counter],
data_T_max_on[current_counter],

data_duty_a[current_counter],
data_duty_b[current_counter],
data_duty_c[current_counter],

data_U_d[current_counter],
data_pi_control_SVM[current_counter],
pi_max
);





}

        current_counter++;
      }
    
      print_current=false;
      current_counter=0;
    }


  }
}
