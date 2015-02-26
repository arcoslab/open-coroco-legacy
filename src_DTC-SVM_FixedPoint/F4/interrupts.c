/*
 * Copyright (C) 2013 ARCOS-Lab Universidad de Costa Rica
 * Author: Federico Ruiz Ugalde <memeruiz@gmail.com>
 * Modified by: Sebastian Chinchilla Gutierrez <tumacher@gmail.com>
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

//#define SAMPLES 3
#define FLUX_LINKAGE_TIMER_DELAY 0


//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------

//printing buffers
int current_counter=0;  
bool print_current=false;
bool first_movement=false;



float data_CUR_FREQ;

float data_i_sA;
float data_i_sB;
float data_i_sD;
float data_i_sQ;

float data_U_d;

float data_V_sD;
float data_V_sQ;
float data_V_s;
float data_cita_V_s;
float data_cita_V_s_relative_angle;

float data_required_V_sD;
float data_required_V_sQ;
float data_required_V_s;
float data_required_cita_V_s;


float data_psi_sD;
float data_psi_sQ;
float data_psi_s;
int   data_psi_alpha;
float data_psi_s_alpha_SVM;
float data_w_r;

float data_timer;

float data_t_e;

float data_psi_s_ref;
float data_t_e_ref;

int   data_d_psi;
int   data_d_te;
float data_psi_delta_percentage;
float data_t_e_delta_percentage;


float data_phase_advance_SVM;
float data_V_s_ref_relative_angle;

float data_pi_control_SVM;
float data_ref_freq_SVM;
int data_state_SVM;
float data_rotating_angle_SVM;

float data_strain_gauge;

//admittance controller
float data_reference_electric_angle      = 0.0f;
float data_reference_mechanical_angle    = 0.0f;
float data_reference_gear_angle          = 0.0f;

float data_electric_angle                = 0.0f;
float data_mechanical_angle        = 0.0f;
float data_gear_angle                    = 0.0f;

float data_mechanical_frequency          = 0.0f;
float data_gear_frequency                = 0.0f;

float data_hall_a=0.0f;
float data_hall_b=0.0f;
float data_previous_hall_a=0.0f;
float data_previous_hall_b=0.0f;


int a = 0;

/*
float volatile V1;
float volatile V2;
float volatile R1;
float volatile V3;
float volatile V4;
float volatile V5;

float volatile data_V1;
float volatile data_V2;
float volatile data_R1;
float volatile data_V3;
float volatile data_V4;
float volatile data_V5;
*/


#include "./../fixedptc.h"
#define FIXEDPT_WBITS 20
//fixedpt volatile Valor1, Valor2, Valor3, Valor4, Valor5, Suma, Resta, Multiplicacion, Division;//, Sqrt2;//, ONE, TWO;
//fixedpt volatile data_Valor1, data_Valor2, data_Valor3, data_Valor4, data_Valor5, data_Suma, data_Resta, data_Multiplicacion, data_Division;//, Sqrt2;//, ONE, TWO;


fixedpt volatile V1;
fixedpt volatile V2;
fixedpt volatile R1;

fixedpt volatile V3;
fixedpt volatile V4;
fixedpt volatile V5;
fixedpt volatile R2;
fixedpt volatile R3;
fixedpt volatile R4;
fixedpt volatile R5;


fixedpt volatile data_V1;
fixedpt volatile data_V2;
fixedpt volatile data_R1;

fixedpt volatile data_V3;
fixedpt volatile data_V4;
fixedpt volatile data_V5;
fixedpt volatile data_R2;
fixedpt volatile data_R3;
fixedpt volatile data_R4;
fixedpt volatile data_R5;

//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------

void tim1_up_tim10_isr(void) 
{
  //oscilloscope flag: start of interrupt
  gpio_set(GPIOD, GPIO11);
   
  //Clear the update interrupt flag
  timer_clear_flag(TIM1,  TIM_SR_UIF);
 
  //la duracion de mas se puede deber a que la asignacion de los valores se realiza dentro de la funcion para el fixed y para float no es asi.


  //gpio_set(GPIOD, GPIO12); //Desactivado temporalmente para las pruebas
  
  /*
  V1    = 1.013014015f;
  V2    = 2.013014015f;
  V3    = 3.013014015f;
  V4    = 4.013014015f;
  V5    = 5.013014015f;
  */

  //V1    = 1.0f;
  //V2    = 2.0f;
  /*
  V1    = 1;
  V2    = 2;
  V3    = 3;
  V4    = 4;
  V5    = 5;
  */
  /*
  Valor1 = fixedpt_rconst(1.013014015);
  Valor2 = fixedpt_rconst(2.013014015); 
  Valor3 = fixedpt_rconst(3.013014015); 
  Valor4 = fixedpt_rconst(4.013014015); 
  Valor5 = fixedpt_rconst(5.013014015); 
  */

  /*
  Valor1 = fixedpt_fromint(1);
  Valor2 = fixedpt_fromint(2); 
  Valor3 = fixedpt_fromint(3); 
  Valor4 = fixedpt_fromint(4); 
  Valor5 = fixedpt_fromint(5); 
  */
  //Valor1 = fixedpt_rconst(1.0);
  //Valor2 = fixedpt_rconst(2.0);
  //while(a<100000)
  //{
    
    //Con float utiliza 186ns
    //Con fixed tarda 188ns
    //Esto es haciendo operaciones con numeros de cantidades fracionales.

    //Con float utiliza 162ns
    //Con fixed tarda 164ns
    //Esto haciendo operaciones con numeros enteros, 1 y 2.

    //Con float utiliza 168ns
    //Con fixed tarda 154ns
    //Esto haciendo operaciones con numeros enteros, 1.0 y 2.

    //Calculo de la raiz cuadrada 
    //Con float se tarda 142ns usando sqrt()
    //Con float se tarda 140ns usando sqrtf()
    //Con fixed se tarda 140ns
    //La diferencia entre sqrtf() y sqrt() radica en que la primera realiza el calculo por hardware, utilizando parte de la ALU dedicada.

    //Operacion Suma, realizada 100000 veces.
    //Con float se tarda 176ns
    //Con punto fijo se tarda 142ns

    //Operacion Division, realizada 100000veces
    //Con float se tardo 174ns
    //Con punto fijo 152ns

    //Sumas en cascada con valores dependientes
    //Para valores fraccionales
    //float se tardo 250ns
    //fixed se tardo 180ns
    ///Para valores enteros
    //fixed se tardo 182ns
    //float se tardo 222ns

    //multiplicacion en cascada con valores dependientes
    //Para valores fraccionales
    //float se tardo 252ns
    //fixed se tardo 204ns
    //Para valores enteros
    //float se tardo 234ns
    //fixed se tardo 204ns

    //division en cascada con valores dependientes
    //Para valores fraccionales
    //float se tardo 252ns
    //fixed se tardo 204ns
    //Para valores enteros
    //float se tardo 264ns
    //fixed se tardo 206ns
    //Cuando puse 8 divisiones en vez de cuatro, con fraccionales y float duro 252ns

/*
    R1 = V1 + V2;  
    R1 = V1 - V2;
    R1 = V1*V2;
    R1 = V1/V2;
*/
  /*
fixedpt_print(R1);
printf("\n");
fixedpt_print(V1);
printf("\n");
fixedpt_print(V2);data_timer
printf("\n");
*/

//    R1 = V1 + V2;
 //   R1 = R1 + V3;
  //  R1 = R1 + V4;
   // R1 = R1 + V5;
  

/*
    R1 = V1*V2;
    R1 = R1*V3;
    R1 = R1*V4;
    R1 = R1*V5;
*/
/*
      R1 = V1/V2;
      R1 = R1/V3;
      R1 = R1/V4;
      R1 = R1/V5;
 */
      //R1 = V3/V2;
      //R1 = V4/V3;
      //R1 = V5/V4;
      //R1 = V2/V5;

  //  R1 = sqrtf(V2);

/*
    Suma = fixedpt_add(Valor1,Valor2);    
    Resta = fixedpt_sub(Valor1,Valor2);
    Multiplicacion = fixedpt_mul(Valor1,Valor2);
    Division = fixedpt_div(Valor1,Valor2);
  //Sqrt2 = fixedpt_sqrt(Valor2);
*/
/*
    Suma = fixedpt_add(Valor1,Valor2);
    Suma = fixedpt_add(Valor3,Suma);
    Suma = fixedpt_add(Valor4,Suma);
    Suma = fixedpt_add(Valor5,Suma);
*/

/*
    Multiplicacion = fixedpt_mul(Valor1,Valor2);
    Multiplicacion = fixedpt_mul(Multiplicacion,Valor3);
    Multiplicacion = fixedpt_mul(Multiplicacion,Valor4);
    Multiplicacion = fixedpt_mul(Multiplicacion,Valor5);
*/
/*
    Division = fixedpt_div(Valor1,Valor2);
    Division = fixedpt_div(Division,Valor3);
    Division = fixedpt_div(Division,Valor4);
    Division = fixedpt_div(Division,Valor5);
*/

    //a = a+1;
  //}
  
  //gpio_clear(GPIOD, GPIO12); //Desactivado temporalmente por las pruebas de fixedpoint.

///*
    //Desactivado temporalmente por las pruebas de fixedpoint
    /*
    if(a<1000)
      {
        gpio_set(GPIOD, GPIO12);
        a = a+1;
      }
      else if (a<2000)
      {
        gpio_clear(GPIOD, GPIO12);
        a = a+1;
      }
      else{
        a=0;
      }
    */

//*/

  calc_freq();

///*
if (center_aligned_state==FIRST_HALF)
{
  //oscilloscope flag: start of First HALF
  gpio_set(GPIOB, GPIO15);
  
  voltage_measure (ADC1,ADC_CHANNEL1);
}
else 
{
  //oscilloscope flag: start of second half
  gpio_set(GPIOD, GPIO9);
  DTC_SVM();
  collecting_floating_data();
  colllecting_flux_linkage();
  gpio_clear(GPIOD, GPIO11);
}

  //oscilloscope flag: start of halves
  //gpio_clear(GPIOB, GPIO15);

  //oscilloscope flag: end of interrupt
  gpio_clear(GPIOD, GPIO9);
//*/  
  gpio_clear(GPIOD, GPIO11);
}

void adc_isr(void)
{
 
  static int adc_counter=0;
  static float V_stm32_A  = 0.0f;
  static float V_stm32_B  = 0.0f;
  static float V_stm32_strain_gauge = 0.0f;
  float V_stm32_Ud = 0.0f;
  float V_shunt_A  = 0.0f;
  float V_shunt_B  = 0.0f;
  float V_strain_gauge =0.0f;
  

  if (adc_counter==0)
  {
    V_stm32_A = adc_read_regular(ADC1);
    voltage_measure (ADC1,ADC_CHANNEL2);
    adc_counter++;
  }
  else if (adc_counter==1)
  {
    V_stm32_B = adc_read_regular(ADC1);
    voltage_measure (ADC1,ADC_CHANNEL15);
    adc_counter++; 
  }

  else if (adc_counter==2)
  {
    V_stm32_strain_gauge = adc_read_regular(ADC1);
    voltage_measure (ADC1,ADC_CHANNEL3);
    adc_counter++; 
  }


  else
  {
    V_stm32_Ud = adc_read_regular(ADC1)*(VREF/ADC_CONVERSION_FACTOR);
    U_d        =    V_stm32_Ud*BATTERY_VOLTAGE_CONVERTION_FACTOR; 

    V_shunt_A = (V_stm32_A*(VREF/ADC_CONVERSION_FACTOR)-V_DIFFERENTIAL_AMPLIFIER_REFFERENCE_A)/G_OP_AMP_A;
    i_sA      = V_shunt_A/R_SHUNT_A;

    V_shunt_B = (V_stm32_B*(VREF/ADC_CONVERSION_FACTOR)-V_DIFFERENTIAL_AMPLIFIER_REFFERENCE_B)/G_OP_AMP_B;
    i_sB      = V_shunt_B/R_SHUNT_B;

    //V_strain_gauge=V_stm32_strain_gauge*(VREF/ADC_CONVERSION_FACTOR);
    //strain_gauge = V_strain_gauge-strain_gauge_reference;

    V_strain_gauge=V_stm32_strain_gauge*(VREF/ADC_CONVERSION_FACTOR)-strain_gauge_reference;
    V_strain_gauge=te_moving_average_filter(V_strain_gauge);

    strain_gauge = V_strain_gauge*STRAIN_GAUGE_CONVERSION_FACTOR;
    //strain_gauge =te_moving_average_filter(strain_gauge);
    if (reset_strain_gauge_reference==true)
    {
/*
      strain_gauge_reference=strain_gauge_reference+strain_gauge;
      strain_gauge=0.0f;
      reset_strain_gauge_reference=false;
*/

      strain_gauge_reference=strain_gauge_reference+V_strain_gauge;
      V_strain_gauge=0.0f;
      reset_strain_gauge_reference=false;
    }
    //filtering currents

    //i_sA = isA_moving_average_filter(i_sA);
    //i_sB = isB_moving_average_filter(i_sB);
       
    adc_counter=0;

    DTC_SVM();

    //collecting_data();

      //oscilloscope flag: start of halves
  gpio_clear(GPIOB, GPIO15);

  //oscilloscope flag: end of interrupt
  gpio_clear(GPIOD, GPIO9);
  //gpio_clear(GPIOD, GPIO11);
  }


}
