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
#include "/home/tumacher/local/src/repositories/arcoslab/experiment/open-coroco/libopencm3/lib/stm32/f4/vector_chipset.c"

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
#include "Trigonometric_Functions/Trigonometric_Functions.h"
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
    if (transmitting_to_python==true)
    {
        print_regular_data();
        transmitting_to_python=false;
        collecting_sensorless_data=true; 
    }
    //else 
      //  print_regular_data();

    frequency_input();
/*
    if (regular_print==true)
    {
      print_regular_data();
    }
    


    else if (print_current==true )
    {
      current_counter=0;
      printf ("\n\n**************************************************************************************************************\n");
      printf("t:t:freq_ref:freq_ref:freq:freq:hall_freq:hall_freq:Vs:Vs");
      printf(":phase_adv:phase_adv:isA:isA:isB:isB:isC:isC:isD:isD:isQ:isQ:is:is:iscita:iscita:psisD:psisD:psisQ:psisQ:psis:psis:psisa:psisa:psiref: psiref:te:te:VsD:VsD:VsQ:VsQ:Vs:Vs:Vscita:Vscita:Vscitar:Vscitar:T1:T1:T2:T2:Tmin:Tmin:Tmed:Tmed:Tmax:Tmax:Ud:Ud:pi:pi:maxpi:maxpi\n");

      if (SAMPLES==1)
        print_captured_data();


      while (current_counter<SAMPLES-1)
      {

        if (print_selection== 7)// && data_ref_freq_SVM [current_counter] !=0.0f )
        {
          print_captured_data();
        }

        current_counter++;
      }  
    
      print_current=false;
      current_counter=0;
    }
*/

  }
}
