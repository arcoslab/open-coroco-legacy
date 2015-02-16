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
//#include "/home/tumacher/local/src/repositories/arcoslab/experiment/open-coroco/libopencm3/lib/stm32/f4/vector_chipset.c"

//usb serial cdcacm
#include <libopencm3-plus/newlib/syscall.h>
#include "example.h"
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

#include "admittance_control/admittance_control.h"
#include "admittance_control/admittance_control.c"

#include "PID/PID.h"
#include "PID/PID.c"

#include "Filtering/filters.h"
#include "Filtering/filters.c"
#include "F4/initial_setup.h"
#include "F4/initial_setup.c"

#include "Shunt/shunt.h"
#include "Shunt/shunt.c"

#include "PID/sensorless_PID.h"
#include "PID/sensorless_PID.c"

#include "DTC/DTC.h"
#include "DTC/DTC.c"

#include "SVM/initial_rotor_position.h"
#include "SVM/initial_rotor_position.c"
#include "SVM/SVM.h"
#include "SVM/SVM.c"

#include "Print/collecting_data.h"
#include "F4/interrupts.c"

#include "Print/collecting_data.c"

#include "Parameter_Identification/parameter_identification.h"
#include "Parameter_Identification/parameter_identification.c"

 #include "fixedptc.h"
 //#define FIXEDPT_WBITS 12

 float prueba;

int main(void)
{
  //The fixedpt_fromint() function only can be used if the argument is an integer, 
  //otherwise the fixedpt_rconst() function should be used.
  //ONE = fixedpt_fromint(1);
  //TWO = fixedpt_fromint(2); 
  
  /*
  ONE = fixedpt_rconst(1.013014015);
  TWO = fixedpt_rconst(2.013014015); 
  Valor1 = ONE;
  Valor2 = TWO;
  V1 = 1.013014015f;
  V2 = 2.013014015f;
  a = 0;
  */

/*
  V1    = 1.013014015f;
  V2    = 2.013014015f;
  V3    = 3.013014015f;
  V4    = 4.013014015f;
  V5    = 5.013014015f;
*/
  
  V1 = fixedpt_rconst(1.013014015);
  V2 = fixedpt_rconst(2.013014015); 
  //V3 = fixedpt_rconst(3.013014015); 
  //V4 = fixedpt_rconst(4.013014015); 
  //V5 = fixedpt_rconst(5.013014015); 


  //V1 = fixedpt_fromint(-2);
  //V2 = fixedpt_fromint(-1);
  //prueba = 2.0f;

  system_init();
   
  printf ("\n\n****************************************************************************************************************\n");
  printf ("\n\n*************************************DTC-SVM example************************************************************\n");
  printf ("\n\n****************************************************************************************************************\n\n");



//Prueba Para verificar el funcionamiento correcto de la nueva opcion de configuracion implementada p20 con float.
/*
  printf("X");
  printf("4")   ; print_float_as_bytes(V1);
  printf("5")   ; print_float_as_bytes(V2);
  R1 = V1 + V2;
  //R1 = fixedpt_add(V1,V2);    
  printf("6")   ; print_float_as_bytes(R1);
  printf("m");
*/

//Prueba Para verificar el funcionamiento correcto de la nueva opcion de configuracion implementada p20 con fixed.



 /*
  printf("X");
  printf("4")   ; print_fixedpt_as_bytes(V1);
  printf("5")   ; print_fixedpt_as_bytes(V2);
  R1 = fixedpt_add(V1,V2);    
  printf("6")   ; print_fixedpt_as_bytes(R1);
  printf("m");
 */
  

  while (1)
  {


  printf("X");
  //printf("4_: ")   ; fixedpt_print(V1); //MINICOM
  //printf("5_: ")   ; fixedpt_print(V2); //MINICOM
  R1 = fixedpt_add(V1,V2);    
  //R1 = fixedpt_sub(V2,V1);
  //R3 = fixedpt_mul(V1,V2);
  //R4 = fixedpt_div(V1,V2);
  //printf("6_:  ")   ; fixedpt_print(R1); //MINICOM
  printf("m");
  //  printf("X");
    //printf("P")   ; print_float_as_bytes(V1);
    //R1 = V1 + V2;
    //R1 = R1 + V3;
    //R1 = R1 + V4;
    //R1 = R1 + V5;

  
  printf("Estas son pruebas de fixedpt_print\n");
  printf("V1: ");  fixedpt_print(V1); printf("\n");
  printf("V2: ");  fixedpt_print(V2); printf("\n");
  printf("R1: ");  fixedpt_print(R1); printf("\n");
/*
  printf("Estas son pruebas de float\n");
  printf("float number: "); printf("%f", prueba); printf("\n");
  */
  

    /*
      gpio_set(GPIOD, GPIO12);
  while(a<100000)
  {
    R1 = V1 + V2;
    R1 = V1 - V2;
    R1 = V1*V2;
    R1 = V1/V2;
    //R1 = sqrt(V2);
    a = a+1;
  }
  gpio_clear(GPIOD, GPIO12);
      */

    
      
      /*
      R1 = V1 + V2;
      R1 = V1 - V2;
      R1 = V1*V2;
      R1 = V1/V2;
      //R1 = sqrt(V2);
      */
      /*
      Suma = fixedpt_add(Valor1,Valor2);
      Resta = fixedpt_sub(Valor1,Valor2);
      Multiplicacion = fixedpt_mul(Valor1,Valor2);
      Division = fixedpt_div(Valor1,Valor2);
      Sqrt2 = fixedpt_sqrt(Valor2);
      */
      



    if (transmitting_to_python==true)
    {
        if (print_selection==9) { full_print_regular_data();  }
        else                  {      print_regular_data();  }
        transmitting_to_python=false;
        collecting_sensorless_data=true; 
    }

//    print_selection=20;
//    print_regular_data();

    frequency_input();
   //printf("m");
  }
}
