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

#define SAMPLES 500
#define FLUX_LINKAGE_TIMER_DELAY 4000

//printing buffers
float hall_angle=0.0f;
float PID_angle[SAMPLES];
float current_data_i_sA[SAMPLES];
float current_data_i_sB[SAMPLES];
float switching_data_SA[SAMPLES];
float switching_data_SB[SAMPLES];
float switching_data_SC[SAMPLES];

bool  inductance_measure=true;

int current_counter=0;  
bool print_current=false;
bool first_movement=false;
float data_CUR_FREQ[SAMPLES];

int data_S_A[SAMPLES];
int data_S_B[SAMPLES];
int data_S_C[SAMPLES];

float data_S_A_f[SAMPLES];
float data_S_B_f[SAMPLES];
float data_S_C_f[SAMPLES];

float data_i_sA [SAMPLES];
float data_i_sB [SAMPLES];

float data_U_d  [SAMPLES];

float data_i_sD[SAMPLES];
float data_i_sQ[SAMPLES];
float data_i_s[SAMPLES];
float data_cita_i_s[SAMPLES];

float data_V_sD[SAMPLES];
float data_V_sQ[SAMPLES];
float data_V_s [SAMPLES];
float data_cita_V_s[SAMPLES];

float data_psi_sD[SAMPLES];
float data_psi_sQ[SAMPLES];
float data_psi_s [SAMPLES];
int   data_psi_alpha[SAMPLES];
float data_w_r[SAMPLES];
int timer[SAMPLES];

float data_t_e[SAMPLES];

float data_psi_s_ref[SAMPLES];
float data_t_e_ref[SAMPLES];

int   data_d_psi[SAMPLES];
int   data_d_te[SAMPLES];
float data_psi_delta_percentage[SAMPLES];
float data_t_e_delta_percentage[SAMPLES];


//motor parameters;
float data_R_s[SAMPLES];
float data_pole_pairs[SAMPLES];
float data_L_sq[SAMPLES];
float data_psi_F[SAMPLES];

int data_optimal_voltage_vector[SAMPLES];




void tim1_up_tim10_isr(void) 
{

  //oscilloscope flag: start of calculations
  gpio_set(GPIOD, GPIO9);
   
  //floating_switching_states (&switching_data_SA[current_counter],&switching_data_SB[current_counter],&switching_data_SC[current_counter]);
  
  floating_switching_states (&S_A_f,&S_B_f,&S_C_f);
  //switching_states (&S_A,&S_B,&S_C);

  //Clear the update interrupt flag
  timer_clear_flag(TIM1,  TIM_SR_UIF);

  calc_freq();
  start_up();
/*
if (CUR_FREQ>=600.0f)
{
  dtc_on=true;
  if (first_dtc==true)
  {
    first_dtc=false;
    collecting_current=true;
  }
}
*/
if (dtc_on==false)
{
  gen_pwm();
  //close_loop=true;
}
/*
else
{
  timer_set_oc_value(TIM1, TIM_OC1, 0.0f*PWM_PERIOD_ARR);
  // Set the capture compare value for OC1. 
  timer_set_oc_value(TIM1, TIM_OC2, 0.0f*PWM_PERIOD_ARR);
  // Set the capture compare value for OC1. 
  timer_set_oc_value(TIM1, TIM_OC3, 0.0f*PWM_PERIOD_ARR);
}
*/
  //oscilloscope flag: end of calculations
  gpio_clear(GPIOD, GPIO9);

  //oscilloscope flag: start of ADC current measuring
  gpio_set(GPIOB, GPIO15);

  voltage_measure (ADC1,ADC_CHANNEL1);
}




void adc_isr(void)
{ 
  static int adc_counter=0;
  float V_stm32_A  = 0.0f;
  float V_stm32_B  = 0.0f;
  float V_stm32_Ud = 0.0f;
  float V_shunt_A  = 0.0f;
  float V_shunt_B  = 0.0f;
  

  if (adc_counter==0)
  {
    V_stm32_A = adc_read_regular(ADC1)*(VREF/ADC_CONVERSION_FACTOR);
    V_shunt_A = (V_stm32_A-V_DIFFERENTIAL_AMPLIFIER_REFFERENCE)/G_OP_AMP_A;
    i_sA      = V_shunt_A/R_SHUNT_A;
    
    //oscilloscope flag: start of ADC current measuring 
    gpio_clear(GPIOB, GPIO15);

    voltage_measure (ADC1,ADC_CHANNEL2);
    adc_counter++;
    
/*
     //parameter identification code: current measurement for Lq and Ld
     if ( (i_sA>=0.5f) &&  (inductance_measure==true) )
     {
     collecting_current=true;
     inductance_measure=false;
     }
 */

/*
    //------------------------------------------
    //capturing current when user inputs s 100
    if (collecting_current==true)
    {
      //printf ("\nCollecting current!");
      if (current_counter<SAMPLES)
      {
        current_data_i_sA[current_counter]=i_sA;
        
	//current_counter++;
      }
      else
      {
        current_counter=0;
        collecting_current=false;
        print_current=true;
      }
      //------------------------------------------
    }
*/
  }
  else if (adc_counter==1)
  {

    

    V_stm32_B = adc_read_regular(ADC1)*(VREF/ADC_CONVERSION_FACTOR);
    V_shunt_B = (V_stm32_B-V_DIFFERENTIAL_AMPLIFIER_REFFERENCE)/G_OP_AMP_B;
    i_sB      = V_shunt_B/R_SHUNT_B;
    
    //oscilloscope flag: start of ADC Voltage measuring
    gpio_set(GPIOB, GPIO15);

    voltage_measure (ADC1,ADC_CHANNEL3);
    adc_counter++; 


/*
    //--------------------------------------------
    //capturing current when user inputs s 100
    if (collecting_current==true)
    {
      if (current_counter<SAMPLES)
      {
        current_data_i_sB[current_counter]=i_sB;
	current_counter++;
      }
      else
      {
        current_counter=0;
        collecting_current=false;
        print_current=true;
      }
    }
    //--------------------------------------------
*/
  }
  else
  {
    //oscilloscope flag: end of ADC current measuring
    gpio_clear(GPIOB, GPIO15);

    V_stm32_Ud = adc_read_regular(ADC1)*(VREF/ADC_CONVERSION_FACTOR);
    
    //U_d=12.0f;
    U_d        = V_stm32_Ud*BATTERY_VOLTAGE_CONVERTION_FACTOR;    
    adc_counter=0;

    //oscilloscope flag: start of DTC
    gpio_set(GPIOD, GPIO9);


/*
    if (i_sA<0.2f && i_sA>-0.2f)
      i_sA=0.0f;
    if (i_sB<0.2f && i_sB>-0.2f)
      i_sB=0.0f;
*/    

    //DTC();
  
/*
    if (flux_linkage_capture==true)
    {
      if (flux_linkage_capture_counter==0 && collecting_current==false)
      { 
        //collecting_current=true;
        flux_linkage_capture_counter=1;
        flux_linkage_capture_timer=0;
        printf("\nStating data gathering 1\n");
      }
      else if (flux_linkage_capture_counter==1 && collecting_current==false && flux_linkage_capture_timer<2000)
      { 
        collecting_current=false;
        //flux_linkage_counter++;
        flux_linkage_capture_timer++;
        printf("\nStarting delay time 1\n");
      }

      else if (flux_linkage_capture_counter==1 && collecting_current==false)
      { 
        //collecting_current=true;
        flux_linkage_capture_counter++;
        flux_linkage_capture_timer=0;
        printf("\nStating data gathering 2\n");
      }
      

      else if (flux_linkage_capture_counter==2 && collecting_current==false && flux_linkage_capture_timer<2000)
      { 
        collecting_current=false;
        //flux_linkage_counter++;
        flux_linkage_capture_timer++;
        printf("\nStarting dely time 2\n");
      }

      else if (flux_linkage_capture_counter==2 && collecting_current==false)
      { 
        //collecting_current=true;
        flux_linkage_capture_counter++;
        flux_linkage_capture_timer=0;
        printf("\nStating data gathering 3\n");
      }

      else
      {
        flux_linkage_capture=false;
        printf("\nEnd of data collecting\n");
      }

    }
*/




    //--------------------------------------------
    //capturing current when user inputs s 100


    //angle_hall1+=2*PI*CUR_FREQ*TIME_CITA
    //PID_angle[current_counter]=angle_hall1;   

    if (collecting_current==true)
    {
      
      //printf ("psi_sD: %f psi_sQ: %f psi_s: %6.5f psi_alpha: %2d psi_D_diff: %10.6f psi_Q_diff: %10.6f timer: %d\n", data_psi_sD[current_counter],data_psi_sQ[current_counter],data_psi_s[current_counter],data_psi_alpha[current_counter],data_psi_sD[current_counter]-data_psi_sD[current_counter-1],data_psi_sQ[current_counter]-data_psi_sQ[current_counter-1], timer[current_counter]);      

      if (current_counter<SAMPLES/8)
      {

        data_psi_sD[current_counter]=psi_sD;
        data_psi_sQ[current_counter]=psi_sQ;
        data_psi_s [current_counter]=psi_s;
        data_psi_alpha[current_counter]=psi_alpha;
        data_CUR_FREQ[current_counter]=CUR_FREQ;
        data_U_d  [current_counter]=U_d;
        timer[current_counter]=flux_linkage_capture_counter;
        
        //---------------------------------------
        current_data_i_sB[current_counter]=i_sB;

	data_CUR_FREQ[current_counter]=CUR_FREQ;	

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

        data_V_sD[current_counter]=V_sD;
        data_V_sQ[current_counter]=V_sQ;
        data_V_s [current_counter]=V_s;
        data_cita_V_s[current_counter]=cita_V_s;

        data_psi_sD[current_counter]=psi_sD;
        data_psi_sQ[current_counter]=psi_sQ;
        data_psi_s [current_counter]=psi_s;
        data_psi_alpha[current_counter]=psi_alpha;

        data_t_e[current_counter]=t_e;

        data_psi_s_ref[current_counter]=psi_s_ref;
        data_t_e_ref[current_counter]=t_e_ref;

        data_d_psi[current_counter]=d_psi;
        data_d_te[current_counter]=d_te;
        data_psi_delta_percentage[current_counter]=psi_delta_percentage;
        data_t_e_delta_percentage[current_counter]=t_e_delta_percentage;

        data_R_s[current_counter]=R_s;
        data_pole_pairs[current_counter]=pole_pairs;
        data_L_sq[current_counter]=L_sq;
        data_psi_F[current_counter]=psi_F;
        data_optimal_voltage_vector[current_counter]=optimal_voltage_vector;
        PID_angle[current_counter]=cmd_angle;//angle_hall1;
        data_w_r[current_counter]=w_r;   
        //------------------------------------------

	current_counter++;
        flux_limit_counter_1=flux_linkage_capture_counter;
        flux_linkage_capture_counter++;
        
      }
      else if (flux_linkage_capture_counter<FLUX_LINKAGE_TIMER_DELAY+flux_limit_counter_1)
      {
        flux_linkage_capture_counter++;
//printf("\ncurrent counter: %d flux_limit_counter: %d flux linkage capture counter: %d \n", current_counter, flux_limit_counter_1,flux_linkage_capture_counter);   
      }
      else if(current_counter<SAMPLES/4)
      {
        data_psi_sD[current_counter]=psi_sD;
        data_psi_sQ[current_counter]=psi_sQ;
        data_psi_s [current_counter]=psi_s;
        data_psi_alpha[current_counter]=psi_alpha;
        data_CUR_FREQ[current_counter]=CUR_FREQ;
        data_U_d  [current_counter]=U_d;
        timer[current_counter]=flux_linkage_capture_counter;

        //---------------------------------------
        current_data_i_sB[current_counter]=i_sB;

	data_CUR_FREQ[current_counter]=CUR_FREQ;	

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

        data_V_sD[current_counter]=V_sD;
        data_V_sQ[current_counter]=V_sQ;
        data_V_s [current_counter]=V_s;
        data_cita_V_s[current_counter]=cita_V_s;

        data_psi_sD[current_counter]=psi_sD;
        data_psi_sQ[current_counter]=psi_sQ;
        data_psi_s [current_counter]=psi_s;
        data_psi_alpha[current_counter]=psi_alpha;

        data_t_e[current_counter]=t_e;

        data_psi_s_ref[current_counter]=psi_s_ref;
        data_t_e_ref[current_counter]=t_e_ref;

        data_d_psi[current_counter]=d_psi;
        data_d_te[current_counter]=d_te;
        data_psi_delta_percentage[current_counter]=psi_delta_percentage;
        data_t_e_delta_percentage[current_counter]=t_e_delta_percentage;

        data_R_s[current_counter]=R_s;
        data_pole_pairs[current_counter]=pole_pairs;
        data_L_sq[current_counter]=L_sq;
        data_psi_F[current_counter]=psi_F;
        data_optimal_voltage_vector[current_counter]=optimal_voltage_vector;
        PID_angle[current_counter]=cmd_angle;//angle_hall1;   
        data_w_r[current_counter]=w_r;
        //------------------------------------------

	current_counter++;
        flux_limit_counter_2=flux_linkage_capture_counter;
        flux_linkage_capture_counter++;
      }
      else if (flux_linkage_capture_counter<FLUX_LINKAGE_TIMER_DELAY+flux_limit_counter_2)
      {
        flux_linkage_capture_counter++;
      }
      else if(current_counter<(3*SAMPLES)/8)
      {
        data_psi_sD[current_counter]=psi_sD;
        data_psi_sQ[current_counter]=psi_sQ;
        data_psi_s [current_counter]=psi_s;
        data_psi_alpha[current_counter]=psi_alpha;
        data_CUR_FREQ[current_counter]=CUR_FREQ;
        data_U_d  [current_counter]=U_d;
        timer[current_counter]=flux_linkage_capture_counter;

        //---------------------------------------
        current_data_i_sB[current_counter]=i_sB;

	data_CUR_FREQ[current_counter]=CUR_FREQ;	

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

        data_V_sD[current_counter]=V_sD;
        data_V_sQ[current_counter]=V_sQ;
        data_V_s [current_counter]=V_s;
        data_cita_V_s[current_counter]=cita_V_s;

        data_psi_sD[current_counter]=psi_sD;
        data_psi_sQ[current_counter]=psi_sQ;
        data_psi_s [current_counter]=psi_s;
        data_psi_alpha[current_counter]=psi_alpha;

        data_t_e[current_counter]=t_e;

        data_psi_s_ref[current_counter]=psi_s_ref;
        data_t_e_ref[current_counter]=t_e_ref;

        data_d_psi[current_counter]=d_psi;
        data_d_te[current_counter]=d_te;
        data_psi_delta_percentage[current_counter]=psi_delta_percentage;
        data_t_e_delta_percentage[current_counter]=t_e_delta_percentage;

        data_R_s[current_counter]=R_s;
        data_pole_pairs[current_counter]=pole_pairs;
        data_L_sq[current_counter]=L_sq;
        data_psi_F[current_counter]=psi_F;
        data_optimal_voltage_vector[current_counter]=optimal_voltage_vector;
        PID_angle[current_counter]=cmd_angle;//angle_hall1;   
        data_w_r[current_counter]=w_r;
        //------------------------------------------

	current_counter++;
        flux_limit_counter_3=flux_linkage_capture_counter;
        flux_linkage_capture_counter++;
      }
      else if (flux_linkage_capture_counter<FLUX_LINKAGE_TIMER_DELAY+flux_limit_counter_3)
      {
        flux_linkage_capture_counter++;
      }
      else if(current_counter<SAMPLES/2)
      {
        data_psi_sD[current_counter]=psi_sD;
        data_psi_sQ[current_counter]=psi_sQ;
        data_psi_s [current_counter]=psi_s;
        data_psi_alpha[current_counter]=psi_alpha;
        data_CUR_FREQ[current_counter]=CUR_FREQ;
        data_U_d  [current_counter]=U_d;
        timer[current_counter]=flux_linkage_capture_counter;

        //---------------------------------------
        current_data_i_sB[current_counter]=i_sB;

	data_CUR_FREQ[current_counter]=CUR_FREQ;	

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

        data_V_sD[current_counter]=V_sD;
        data_V_sQ[current_counter]=V_sQ;
        data_V_s [current_counter]=V_s;
        data_cita_V_s[current_counter]=cita_V_s;

        data_psi_sD[current_counter]=psi_sD;
        data_psi_sQ[current_counter]=psi_sQ;
        data_psi_s [current_counter]=psi_s;
        data_psi_alpha[current_counter]=psi_alpha;

        data_t_e[current_counter]=t_e;

        data_psi_s_ref[current_counter]=psi_s_ref;
        data_t_e_ref[current_counter]=t_e_ref;

        data_d_psi[current_counter]=d_psi;
        data_d_te[current_counter]=d_te;
        data_psi_delta_percentage[current_counter]=psi_delta_percentage;
        data_t_e_delta_percentage[current_counter]=t_e_delta_percentage;

        data_R_s[current_counter]=R_s;
        data_pole_pairs[current_counter]=pole_pairs;
        data_L_sq[current_counter]=L_sq;
        data_psi_F[current_counter]=psi_F;
        data_optimal_voltage_vector[current_counter]=optimal_voltage_vector;
        PID_angle[current_counter]=cmd_angle;//angle_hall1;  
        data_w_r[current_counter]=w_r; 
        //------------------------------------------

	current_counter++;
        flux_limit_counter_4=flux_linkage_capture_counter;
        flux_linkage_capture_counter++;
      }
            
      else if (flux_linkage_capture_counter<FLUX_LINKAGE_TIMER_DELAY+flux_limit_counter_4)
      {
        flux_linkage_capture_counter++;
      }
      else if(current_counter<5*SAMPLES/8)
      {
        data_psi_sD[current_counter]=psi_sD;
        data_psi_sQ[current_counter]=psi_sQ;
        data_psi_s [current_counter]=psi_s;
        data_psi_alpha[current_counter]=psi_alpha;
        data_CUR_FREQ[current_counter]=CUR_FREQ;
        data_U_d  [current_counter]=U_d;
        timer[current_counter]=flux_linkage_capture_counter;

        //---------------------------------------
        current_data_i_sB[current_counter]=i_sB;

	data_CUR_FREQ[current_counter]=CUR_FREQ;	

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

        data_V_sD[current_counter]=V_sD;
        data_V_sQ[current_counter]=V_sQ;
        data_V_s [current_counter]=V_s;
        data_cita_V_s[current_counter]=cita_V_s;

        data_psi_sD[current_counter]=psi_sD;
        data_psi_sQ[current_counter]=psi_sQ;
        data_psi_s [current_counter]=psi_s;
        data_psi_alpha[current_counter]=psi_alpha;

        data_t_e[current_counter]=t_e;

        data_psi_s_ref[current_counter]=psi_s_ref;
        data_t_e_ref[current_counter]=t_e_ref;

        data_d_psi[current_counter]=d_psi;
        data_d_te[current_counter]=d_te;
        data_psi_delta_percentage[current_counter]=psi_delta_percentage;
        data_t_e_delta_percentage[current_counter]=t_e_delta_percentage;

        data_R_s[current_counter]=R_s;
        data_pole_pairs[current_counter]=pole_pairs;
        data_L_sq[current_counter]=L_sq;
        data_psi_F[current_counter]=psi_F;
        data_optimal_voltage_vector[current_counter]=optimal_voltage_vector;
        PID_angle[current_counter]=cmd_angle;//angle_hall1; 
        data_w_r[current_counter]=w_r;  
        //------------------------------------------

	current_counter++;
        flux_limit_counter_5=flux_linkage_capture_counter;
        flux_linkage_capture_counter++;
      }

      else if (flux_linkage_capture_counter<FLUX_LINKAGE_TIMER_DELAY+flux_limit_counter_5)
      {
        flux_linkage_capture_counter++;
      }
      else if(current_counter<3*SAMPLES/4)
      {
        data_psi_sD[current_counter]=psi_sD;
        data_psi_sQ[current_counter]=psi_sQ;
        data_psi_s [current_counter]=psi_s;
        data_psi_alpha[current_counter]=psi_alpha;
        data_CUR_FREQ[current_counter]=CUR_FREQ;
        data_U_d  [current_counter]=U_d;
        timer[current_counter]=flux_linkage_capture_counter;

        //---------------------------------------
        current_data_i_sB[current_counter]=i_sB;

	data_CUR_FREQ[current_counter]=CUR_FREQ;	

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

        data_V_sD[current_counter]=V_sD;
        data_V_sQ[current_counter]=V_sQ;
        data_V_s [current_counter]=V_s;
        data_cita_V_s[current_counter]=cita_V_s;

        data_psi_sD[current_counter]=psi_sD;
        data_psi_sQ[current_counter]=psi_sQ;
        data_psi_s [current_counter]=psi_s;
        data_psi_alpha[current_counter]=psi_alpha;

        data_t_e[current_counter]=t_e;

        data_psi_s_ref[current_counter]=psi_s_ref;
        data_t_e_ref[current_counter]=t_e_ref;

        data_d_psi[current_counter]=d_psi;
        data_d_te[current_counter]=d_te;
        data_psi_delta_percentage[current_counter]=psi_delta_percentage;
        data_t_e_delta_percentage[current_counter]=t_e_delta_percentage;

        data_R_s[current_counter]=R_s;
        data_pole_pairs[current_counter]=pole_pairs;
        data_L_sq[current_counter]=L_sq;
        data_psi_F[current_counter]=psi_F;
        data_optimal_voltage_vector[current_counter]=optimal_voltage_vector;
        PID_angle[current_counter]=cmd_angle;//angle_hall1;   
        data_w_r[current_counter]=w_r;
        //------------------------------------------

	current_counter++;
        flux_limit_counter_6=flux_linkage_capture_counter;
        flux_linkage_capture_counter++;
      }

      else if (flux_linkage_capture_counter<FLUX_LINKAGE_TIMER_DELAY+flux_limit_counter_6)
      {
        flux_linkage_capture_counter++;
      }
      else if(current_counter<7*SAMPLES/8)
      {
        data_psi_sD[current_counter]=psi_sD;
        data_psi_sQ[current_counter]=psi_sQ;
        data_psi_s [current_counter]=psi_s;
        data_psi_alpha[current_counter]=psi_alpha;
        data_CUR_FREQ[current_counter]=CUR_FREQ;
        data_U_d  [current_counter]=U_d;
        timer[current_counter]=flux_linkage_capture_counter;
        
        //---------------------------------------
        current_data_i_sB[current_counter]=i_sB;

	data_CUR_FREQ[current_counter]=CUR_FREQ;	

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

        data_V_sD[current_counter]=V_sD;
        data_V_sQ[current_counter]=V_sQ;
        data_V_s [current_counter]=V_s;
        data_cita_V_s[current_counter]=cita_V_s;

        data_psi_sD[current_counter]=psi_sD;
        data_psi_sQ[current_counter]=psi_sQ;
        data_psi_s [current_counter]=psi_s;
        data_psi_alpha[current_counter]=psi_alpha;

        data_t_e[current_counter]=t_e;

        data_psi_s_ref[current_counter]=psi_s_ref;
        data_t_e_ref[current_counter]=t_e_ref;

        data_d_psi[current_counter]=d_psi;
        data_d_te[current_counter]=d_te;
        data_psi_delta_percentage[current_counter]=psi_delta_percentage;
        data_t_e_delta_percentage[current_counter]=t_e_delta_percentage;

        data_R_s[current_counter]=R_s;
        data_pole_pairs[current_counter]=pole_pairs;
        data_L_sq[current_counter]=L_sq;
        data_psi_F[current_counter]=psi_F;
        data_optimal_voltage_vector[current_counter]=optimal_voltage_vector;
        PID_angle[current_counter]=cmd_angle;//angle_hall1;   
        data_w_r[current_counter]=w_r;
        //------------------------------------------

	current_counter++;
        flux_limit_counter_7=flux_linkage_capture_counter;
        flux_linkage_capture_counter++;
      }

      else if (flux_linkage_capture_counter<FLUX_LINKAGE_TIMER_DELAY+flux_limit_counter_7)
      {
        flux_linkage_capture_counter++;
      }
      else if(current_counter<SAMPLES)
      {
        data_psi_sD[current_counter]=psi_sD;
        data_psi_sQ[current_counter]=psi_sQ;
        data_psi_s [current_counter]=psi_s;
        data_psi_alpha[current_counter]=psi_alpha;
        data_CUR_FREQ[current_counter]=CUR_FREQ;
        data_U_d  [current_counter]=U_d;
        timer[current_counter]=flux_linkage_capture_counter;
        
        //---------------------------------------
        current_data_i_sB[current_counter]=i_sB;

	data_CUR_FREQ[current_counter]=CUR_FREQ;	

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

        data_V_sD[current_counter]=V_sD;
        data_V_sQ[current_counter]=V_sQ;
        data_V_s [current_counter]=V_s;
        data_cita_V_s[current_counter]=cita_V_s;

        data_psi_sD[current_counter]=psi_sD;
        data_psi_sQ[current_counter]=psi_sQ;
        data_psi_s [current_counter]=psi_s;
        data_psi_alpha[current_counter]=psi_alpha;

        data_t_e[current_counter]=t_e;

        data_psi_s_ref[current_counter]=psi_s_ref;
        data_t_e_ref[current_counter]=t_e_ref;

        data_d_psi[current_counter]=d_psi;
        data_d_te[current_counter]=d_te;
        data_psi_delta_percentage[current_counter]=psi_delta_percentage;
        data_t_e_delta_percentage[current_counter]=t_e_delta_percentage;

        data_R_s[current_counter]=R_s;
        data_pole_pairs[current_counter]=pole_pairs;
        data_L_sq[current_counter]=L_sq;
        data_psi_F[current_counter]=psi_F;
        data_optimal_voltage_vector[current_counter]=optimal_voltage_vector;
        PID_angle[current_counter]=cmd_angle;//angle_hall1;   
        data_w_r[current_counter]=w_r;
        //------------------------------------------

	current_counter++;
        flux_limit_counter_8=flux_linkage_capture_counter;
        flux_linkage_capture_counter++;
      }

      else if (flux_linkage_capture_counter<FLUX_LINKAGE_TIMER_DELAY+flux_limit_counter_8)
      {
        flux_linkage_capture_counter++;
      }
      else if(current_counter<SAMPLES)
      { 
        data_psi_sD[current_counter]=psi_sD;
        data_psi_sQ[current_counter]=psi_sQ;
        data_psi_s [current_counter]=psi_s;
        data_psi_alpha[current_counter]=psi_alpha;
        data_CUR_FREQ[current_counter]=CUR_FREQ;
        data_U_d  [current_counter]=U_d;
        timer[current_counter]=flux_linkage_capture_counter;


        //---------------------------------------
        current_data_i_sB[current_counter]=i_sB;

	data_CUR_FREQ[current_counter]=CUR_FREQ;	

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

        data_V_sD[current_counter]=V_sD;
        data_V_sQ[current_counter]=V_sQ;
        data_V_s [current_counter]=V_s;
        data_cita_V_s[current_counter]=cita_V_s;

        data_psi_sD[current_counter]=psi_sD;
        data_psi_sQ[current_counter]=psi_sQ;
        data_psi_s [current_counter]=psi_s;
        data_psi_alpha[current_counter]=psi_alpha;

        data_t_e[current_counter]=t_e;

        data_psi_s_ref[current_counter]=psi_s_ref;
        data_t_e_ref[current_counter]=t_e_ref;

        data_d_psi[current_counter]=d_psi;
        data_d_te[current_counter]=d_te;
        data_psi_delta_percentage[current_counter]=psi_delta_percentage;
        data_t_e_delta_percentage[current_counter]=t_e_delta_percentage;

        data_R_s[current_counter]=R_s;
        data_pole_pairs[current_counter]=pole_pairs;
        data_L_sq[current_counter]=L_sq;
        data_psi_F[current_counter]=psi_F;
        data_optimal_voltage_vector[current_counter]=optimal_voltage_vector;
        PID_angle[current_counter]=cmd_angle;//angle_hall1;   
        data_w_r[current_counter]=w_r;
        //------------------------------------------


	current_counter++;
        flux_limit_counter_9=flux_linkage_capture_counter;
        flux_linkage_capture_counter++;
      }


      else
      {
        current_counter=0;
        collecting_current=false;
        print_current=true;
        flux_linkage_capture_counter=0;
        flux_limit_counter_1=0;
        flux_limit_counter_2=0;
        flux_limit_counter_3=0;
        flux_limit_counter_4=0;
        flux_limit_counter_5=0;
        flux_limit_counter_6=0;
        flux_limit_counter_7=0;
        flux_limit_counter_8=0;
        flux_limit_counter_9=0;
      }
    }





    //--------------------------------------------
    //capturing current when user inputs s 100
/*    if (collecting_current==true)
    {


      if (current_counter<SAMPLES)
      {

        current_data_i_sB[current_counter]=i_sB;

	data_CUR_FREQ[current_counter]=CUR_FREQ;	

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

        data_V_sD[current_counter]=V_sD;
        data_V_sQ[current_counter]=V_sQ;
        data_V_s [current_counter]=V_s;
        data_cita_V_s[current_counter]=cita_V_s;

        data_psi_sD[current_counter]=psi_sD;
        data_psi_sQ[current_counter]=psi_sQ;
        data_psi_s [current_counter]=psi_s;
        data_psi_alpha[current_counter]=psi_alpha;

        data_t_e[current_counter]=t_e;

        data_psi_s_ref[current_counter]=psi_s_ref;
        data_t_e_ref[current_counter]=t_e_ref;

        data_d_psi[current_counter]=d_psi;
        data_d_te[current_counter]=d_te;
        data_psi_delta_percentage[current_counter]=psi_delta_percentage;
        data_t_e_delta_percentage[current_counter]=t_e_delta_percentage;

        data_R_s[current_counter]=R_s;
        data_pole_pairs[current_counter]=pole_pairs;
        data_L_sq[current_counter]=L_sq;
        data_psi_F[current_counter]=psi_F;

        
	current_counter++;
      }
      else
      {
        current_counter=0;
        collecting_current=false;
        print_current=true;
      }
    }
*/




    //--------------------------------------------
  
    



    //oscilloscope flag: end of DTC
    gpio_clear(GPIOD, GPIO9);

  }


}
