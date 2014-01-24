/*
 * Copyright (C) 2013 ARCOS-Lab Universidad de Costa Rica
 * Author: Federico Ruiz Ugalde <memeruiz@gmail.com>
 * Modified by Carolina Arauz Villegas <caro16av@gmail.com> ARCOS-Lab
 * Universidad de Costa Rica.
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

#include <libopencm3/stm32/f4/rcc.h>
#include <libopencm3/stm32/f4/gpio.h>
#include <libopencm3/stm32/f4/timer.h>
#include <libopencm3/stm32/f4/nvic.h>
#include <libopencm3-plus/newlib/syscall.h>
#include <cdcacm_example.h>
#include <libopencm3-plus/cdcacm_one_serial/cdcacm.h>
#include <stdio.h>
#include <libopencm3-plus/utils/misc.h>
#include <libopencm3-plus/stm32f4discovery/leds.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <time.h>


//pwm-related timer configuration
#define SYSFREQ     168000000 //168MHz
#define PWMFREQ        32000  //32000
#define PWMFREQ_F       ((float )(PWMFREQ)) //32000.0f
#define PRESCALE        1                                       //freq_CK_CNT=freq_CK_PSC/(PSC[15:0]+1)
#define PWM_PERIOD_ARR  SYSFREQ/( PWMFREQ*(PRESCALE+1) )
#define INIT_DUTY 0.0f //init duty

//encoder min and max values
#define ENCODER_MAX 65535
#define ENCODER_MIN 0

float duty_r=0.0f;
float duty_l=0.0f;

float ref_freqR=0;
float ref_freqL=0;
int32_t totalencoderL=0;
int32_t totalencoderR=0;

//to calculate speed
int32_t old_totalencoderL=0;
int32_t old_totalencoderR=0;

//to update totalencoder counters
int lpos=0;
int lneg=0;
int rpos=0;
int rneg=0;

//encoder counters
uint32_t encoderL=0;
uint32_t encoderR=0;

//encoder speed
float encoder_speed_r=0;
float encoder_speed_l=0;

//disable LGREEN and LORGANGE, because those pins will be connected to the  right encoders inputs
void leds_init(void) {
  rcc_peripheral_enable_clock(&RCC_AHB1ENR, RCC_AHB1ENR_IOPDEN);
  //gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO12);
  //gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO13);
  gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO14);
  gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO15);
}

/*Difference between configurations of TIM1 vs TIM3 and TIM4 (encoder interface mode*/
	/*Encoder mode
	1)Set Encoder Interface mode in the TIMx_SMCR register:
		
		void timer_slave_set_mode(uint32_t  timer_peripheral, mode)
				 
				 timer_peripheral: TIMx
	
			 	mode:   TIM_SMCR_SMS_EM1 = SMS=001 TI2 edges
								TIM_SMCR_SMS_EM2 = SMS=010 TI1 edges
								TIM_SMCR_SMS_EM1 = SMS=011 TI1 and TI2 edges														
															
	2)Set TI1 and TI2 polarity in the CC1P and CC2P bits in TIMx_CCER register:
	
	 void timer_set_oc_polarity(uint32_t timer_peripheral, enum tim_oc_id oc_id)
				
				timer_peripheral: TIMx
			
				oc_id:  TIM_OC1 = TIM_CCER_CC1P
								TIM_OC2 = TIM_CCER_CC2P
	  				
  3)Enable Timer Input Capture... (CCER register) :	  				
	
	void timer_ic_enable(uint32_t timer_peripheral, enum tim_ic_id ic)  				
	
				timer_peripheral: TIMx
	
				channel designtar tim_ic_id:  TIM_IC1 
																		  TIM_IC2 
		   
  4)Map TI1FP1 to TI1 and TI2FP2 to TI2
 
		 CC1S=’01’ (TIMx_CCMR1 register, TI1FP1 mapped on TI1).
		 CC2S=’01’ (TIMx_CCMR2 register, TI1FP2 mapped on TI2).

	void timer_ic_set_input(uint32_t timer_peripheral, enum tim_ic_id ic, enum tim_ic_input in)
	
				timer_peripheral: TIMx
				
				channel designtar tim_ic_id:  TIM_IC1 
																		  TIM_IC2 

        tim_ic_input	TIM_IC_IN_TI1 = 1

	5)Enable counter CEN=’1’ (TIMx_CR1 register):
	void timer_enable_counter(uint32_t timer_peripheral)
		timer_peripheral: TIMx
	*/

void tim4_init(void) {

	/* Enable TIM4 clock. and Port D clock */
	rcc_peripheral_enable_clock(&RCC_APB1ENR, RCC_APB1ENR_TIM4EN);
	rcc_peripheral_enable_clock(&RCC_AHB1ENR, RCC_AHB1ENR_IOPDEN);

	//Set TIM4 channel output to alternate function push-pull'.
	//f4 TIM4=> GIO12: CH1, GPIO13: CH2, 
  //	GPIO12 and GPIO13 are also connected to GREEN and ORANGE Leds, so we disabled those before
	gpio_mode_setup(GPIOD, GPIO_MODE_AF,GPIO_PUPD_NONE,GPIO12 | GPIO13);
	gpio_set_af(GPIOD, GPIO_AF2, GPIO12 | GPIO13); //set as AF2

	/* Reset TIM4 peripheral. */
	timer_reset(TIM4);

	timer_set_mode(TIM4, TIM_CR1_CKD_CK_INT, //For dead time and filter sampling, not important for now.
		       TIM_CR1_CMS_EDGE,	//TIM_CR1_CMS_EDGE
						//TIM_CR1_CMS_CENTER_1
						//TIM_CR1_CMS_CENTER_2
						//TIM_CR1_CMS_CENTER_3 la frequencia del pwm se divide a la mitad.
			 TIM_CR1_DIR_UP);

	timer_set_prescaler(TIM4, 0); //0 = disabled (max speed)
	timer_set_repetition_counter(TIM4, 0); //disabled, only valid for advanced timers (TIM1 and TIM8)
	timer_enable_preload(TIM4); //load from its auto-reload register only at the next update event
	timer_continuous_mode(TIM4); //enable the timer to run continuosly

/* Encoder Interface mode configurations*/
	timer_slave_set_mode(TIM4, TIM_SMCR_SMS_EM3); 
	timer_set_oc_polarity_high(TIM4, TIM_OC1);
	timer_set_oc_polarity_high(TIM4, TIM_OC2);
	
	timer_ic_enable(TIM4, TIM_IC1);
	timer_ic_enable(TIM4, TIM_IC2);
	timer_ic_set_input(TIM4, TIM_IC1, TIM_IC_IN_TI1);
	timer_ic_set_input(TIM4, TIM_IC2, TIM_IC_IN_TI1);

	/* Period (32kHz). */
	/* Configure TIMx_ARR register, the counter will count continuosly between 0 and the auto-reload value in the TIMx_ARR*/
	timer_set_period(TIM4, ENCODER_MAX); //ARR (value compared against main counter to reload counter aka: period of counter)


	/* Disable outputs. */ 
	timer_disable_oc_output(TIM4, TIM_OC1);
	timer_disable_oc_output(TIM4, TIM_OC2);
	timer_disable_oc_output(TIM4, TIM_OC3);


	/* ARR reload enable. */ 
	//timer_enable_preload(TIM1);

	/*
	 * Disable preload of complementary channel configurations and
	 * update on COM event.
	 */
	timer_disable_preload_complementry_enable_bits(TIM4);


	/* Counter enable. */
	timer_enable_counter(TIM4);

	/*********/
	/*Capture compare interrupt*/

	//enable capture compare interrupt
	timer_enable_update_event(TIM4);

	/* Enable interrupt. */
	timer_enable_irq(TIM4, TIM_DIER_UIE);
	nvic_enable_irq(NVIC_TIM4_IRQ);

}

//TIM3 configuration is the same that TIM4, only changes are the clock and the GPIO
void tim3_init(void) {
	/* Enable TIM3 clock. and Port B clock */
	rcc_peripheral_enable_clock(&RCC_APB1ENR, RCC_APB1ENR_TIM3EN);
	rcc_peripheral_enable_clock(&RCC_AHB1ENR, RCC_AHB1ENR_IOPBEN);

	//Set TIM3 channel output to alternate function push-pull'.
	//f4 TIM3=> GIO4: CH1, GPIO5: CH2, 
	gpio_mode_setup(GPIOB, GPIO_MODE_AF,GPIO_PUPD_NONE,GPIO4 | GPIO5);
	gpio_set_af(GPIOB, GPIO_AF2, GPIO4 | GPIO5); //set as AF2


	/* Reset TIM4 peripheral. */
	timer_reset(TIM3);


	timer_set_mode(TIM3, TIM_CR1_CKD_CK_INT, //For dead time and filter sampling, not important for now.
		       TIM_CR1_CMS_EDGE,	//TIM_CR1_CMS_EDGE
						//TIM_CR1_CMS_CENTER_1
						//TIM_CR1_CMS_CENTER_2
						//TIM_CR1_CMS_CENTER_3 la frequencia del pwm se divide a la mitad.
			 TIM_CR1_DIR_UP);

	timer_set_prescaler(TIM3, 0); //0 = disabled (max speed)
	timer_set_repetition_counter(TIM3, 0); //disabled
	timer_enable_preload(TIM3);
	timer_continuous_mode(TIM3);
	timer_slave_set_mode(TIM3, TIM_SMCR_SMS_EM3);
	timer_set_oc_polarity_high(TIM3, TIM_OC1);
	timer_set_oc_polarity_high(TIM3, TIM_OC2);
	timer_ic_enable(TIM3, TIM_IC1);
	timer_ic_enable(TIM3, TIM_IC2);
	timer_ic_set_input(TIM3, TIM_IC1, TIM_IC_IN_TI1);
	timer_ic_set_input(TIM3, TIM_IC2, TIM_IC_IN_TI1);

	/* Period (32kHz). */ //Max encoder value is 65535 (16 bits)
	timer_set_period(TIM3, ENCODER_MAX); //ARR (value compared against main counter to reload counter aka: period of counter)


	/* Disable outputs. */
	timer_disable_oc_output(TIM3, TIM_OC1);
	//timer_disable_oc_output(TIM1, TIM_OC1N);
	timer_disable_oc_output(TIM3, TIM_OC2);
	//timer_disable_oc_output(TIM1, TIM_OC2N);
	timer_disable_oc_output(TIM3, TIM_OC3);
	//timer_disable_oc_output(TIM1, TIM_OC3N);


	/*
	 * Disable preload of complementary channel configurations and
	 * update on COM event.
	 */
	timer_disable_preload_complementry_enable_bits(TIM3);

	/* Counter enable. */
	timer_enable_counter(TIM3);

	/*********/
	/*Capture compare interrupt*/

	//enable capture compare interrupt
	timer_enable_update_event(TIM3);

	/* Enable commutation interrupt. */
	//timer_enable_irq(TIM1, TIM_DIER_CC1IE);	//Capture/compare 1 interrupt enable
	/* Enable commutation interrupt. */
	//timer_enable_irq(TIM1, TIM_DIER_CC1IE);
	timer_enable_irq(TIM3, TIM_DIER_UIE);
	nvic_enable_irq(NVIC_TIM3_IRQ);

}


void tim1_init(void)
{
	/* Enable TIM1 clock. and Port E clock (for outputs) */
	rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_TIM1EN);//F4 clock for TIM 1 is APB2. TIM1 port is E

	/*Enable AF clock*/
	rcc_peripheral_enable_clock(&RCC_AHB1ENR, RCC_AHB1ENR_IOPEEN);


	//Set TIM1 channel (and complementary) output to alternate function push-pull.
	//f4 TIM1=> GIO9: CH1, GPIO11: CH2
	//f4 TIM1=> GIO8: CH1N, GPIO10: CH2N
	gpio_mode_setup(GPIOE, GPIO_MODE_AF,GPIO_PUPD_NONE,GPIO9 | GPIO11);
	gpio_set_af(GPIOE, GPIO_AF1, GPIO9 | GPIO11);
	gpio_mode_setup(GPIOE, GPIO_MODE_AF,GPIO_PUPD_NONE,GPIO8 | GPIO10 );
	gpio_set_af(GPIOE, GPIO_AF1, GPIO8 | GPIO10);

	/* Enable TIM1 commutation interrupt. */
	//nvic_enable_irq(NVIC_TIM1_TRG_COM_TIM11_IRQ);	//f4

	/* Reset TIM1 peripheral. */
	timer_reset(TIM1);

	/* Timer global mode:
	 * - No divider
	 * - Alignment edge
	 * - Direction up
	 */
	timer_set_mode(TIM1, TIM_CR1_CKD_CK_INT, //For dead time and filter sampling, not important for now.
		       TIM_CR1_CMS_CENTER_3,	//TIM_CR1_CMS_EDGE
						//TIM_CR1_CMS_CENTER_1
						//TIM_CR1_CMS_CENTER_2
						//TIM_CR1_CMS_CENTER_3 la frequencia del pwm se divide a la mitad.
			 TIM_CR1_DIR_UP);

	timer_set_prescaler(TIM1, PRESCALE); //1 = disabled (max speed)

/* Disable repetition counter value for TIM1. */
	timer_set_repetition_counter(TIM1, 0); 

/* Enable preload on TIM1. */
	timer_enable_preload(TIM1);

/* Sets TIM1 Continuous mode. */
	timer_continuous_mode(TIM1);

	/* Period (32kHz). */
	timer_set_period(TIM1, PWM_PERIOD_ARR); //ARR (value compared against main counter to reload counter aka: period of counter)

	/* Configure break and deadtime. */
	//timer_set_deadtime(TIM1, deadtime_percentage*pwm_period_ARR);
	timer_set_enabled_off_state_in_idle_mode(TIM1);
	timer_set_enabled_off_state_in_run_mode(TIM1);
	timer_disable_break(TIM1);
	timer_set_break_polarity_high(TIM1);
	timer_disable_break_automatic_output(TIM1);
	timer_set_break_lock(TIM1, TIM_BDTR_LOCK_OFF);

	/* Disable outputs. */
	timer_disable_oc_output(TIM1, TIM_OC1);
	timer_disable_oc_output(TIM1, TIM_OC1N);
	timer_disable_oc_output(TIM1, TIM_OC2);
	timer_disable_oc_output(TIM1, TIM_OC2N);
	//timer_disable_oc_output(TIM1, TIM_OC3);
	//timer_disable_oc_output(TIM1, TIM_OC3N);

	/* -- OC1 and OC1N configuration -- */
	/* Configure global mode of line 1. */
	timer_enable_oc_preload(TIM1, TIM_OC1);
	timer_set_oc_mode(TIM1, TIM_OC1, TIM_OCM_PWM1);
	/* Configure OC1. */
	timer_set_oc_polarity_high(TIM1, TIM_OC1);
	timer_set_oc_idle_state_unset(TIM1, TIM_OC1); //When idle (braked) put 0 on output
	/* Configure OC1N. */
	timer_set_oc_polarity_high(TIM1, TIM_OC1N);
	timer_set_oc_idle_state_unset(TIM1, TIM_OC1N);
	/* Set the capture compare value for OC1. */
	timer_set_oc_value(TIM1, TIM_OC1, INIT_DUTY*PWM_PERIOD_ARR);//initial_duty_cycle*pwm_period_ARR);

	/* -- OC2 and OC2N configuration -- */
	/* Configure global mode of line 2. */
	timer_enable_oc_preload(TIM1, TIM_OC2);
	timer_set_oc_mode(TIM1, TIM_OC2, TIM_OCM_PWM1);
	/* Configure OC2. */
	timer_set_oc_polarity_high(TIM1, TIM_OC2);
	timer_set_oc_idle_state_unset(TIM1, TIM_OC2);
	/* Configure OC2N. */
	timer_set_oc_polarity_high(TIM1, TIM_OC2N);
	timer_set_oc_idle_state_unset(TIM1, TIM_OC2N);
	/* Set the capture compare value for OC2. */
	timer_set_oc_value(TIM1, TIM_OC2, INIT_DUTY*PWM_PERIOD_ARR);//initial_duty_cycle*pwm_period_ARR);

	/* Reenable outputs. */
	timer_enable_oc_output(TIM1, TIM_OC1);
	timer_enable_oc_output(TIM1, TIM_OC1N);
	timer_enable_oc_output(TIM1, TIM_OC2);
	timer_enable_oc_output(TIM1, TIM_OC2N);
	
	/* ---- */

	/* ARR reload enable. */
	timer_enable_preload(TIM1);

	/*
	 * Enable preload of complementary channel configurations and
	 * update on COM event.
	 */
	//timer_enable_preload_complementry_enable_bits(TIM1);
	timer_disable_preload_complementry_enable_bits(TIM1);

	/* Enable outputs in the break subsystem. */
	timer_enable_break_main_output(TIM1);

	/* Generate update event to reload all registers before starting*/
	timer_generate_event(TIM1, TIM_EGR_UG);

	/* Counter enable. */
	timer_enable_counter(TIM1);

	/* Enable commutation interrupt. */
	//timer_enable_irq(TIM1, TIM_DIER_COMIE);

	/*********/
	/*Capture compare interrupt*/

	//enable capture compare interrupt
	timer_enable_update_event(TIM1);

	/* Enable commutation interrupt. */
	//timer_enable_irq(TIM1, TIM_DIER_CC1IE);	//Capture/compare 1 interrupt enable
	/* Enable commutation interrupt. */
	//timer_enable_irq(TIM1, TIM_DIER_CC1IE);
	timer_enable_irq(TIM1, TIM_DIER_UIE);
	nvic_enable_irq(NVIC_TIM1_UP_TIM10_IRQ);
}

void system_init(void) {
  rcc_clock_setup_hse_3v3(&hse_8mhz_3v3[CLOCK_3V3_168MHZ]);
  leds_init();
  cdcacm_init();
  printled(4, LRED);
  tim1_init();
  tim4_init();
  tim3_init();
}
//Definiendo variables para los controladores PI de los motores
#define PWM_ENC_CYCLES 800
/*#define P 0.0005f*/
/*#define I 0.00000002f*/

#define P 0.00005f
#define I 0.00000001f
#define I_MAX 90000000
#define PI_MAX 1.0f
#define PI_MIN -1.0f

float ref_enc_r=0;
float pid_error_r=0;
float i_pid_error_r=0;
float pi_control_r=0;

float ref_enc_l=0;
float pid_error_l=0;
float i_pid_error_l=0;
float pi_control_l=0;

void pi_controller(void) {
//PID right motor
  pid_error_r=ref_enc_r-encoder_speed_r; // ref_freq-cur_freq ... ref_enc_r must be duty_r (send from the ros-python interface 
  i_pid_error_r+=pid_error_r;

  if (i_pid_error_r > I_MAX){
    i_pid_error_r=I_MAX;
  }
  if (i_pid_error_r < -I_MAX) {
    i_pid_error_r=-I_MAX;
  }

  pi_control_r=P*pid_error_r+I*i_pid_error_r;
  if (pi_control_r > PI_MAX) {
    pi_control_r = PI_MAX;
  }
  if (pi_control_r < PI_MIN) {
    pi_control_r = PI_MIN;
  }
  duty_r=pi_control_r;

//PID left motor
  pid_error_l=ref_enc_l-encoder_speed_l; // ref_freq-cur_freq ... ref_enc_l must be duty_l (send from the ros-python interface
  i_pid_error_l+=pid_error_l;

  if (i_pid_error_l > I_MAX){
    i_pid_error_l=I_MAX;
  }
  if (i_pid_error_l < -I_MAX) {
    i_pid_error_l=-I_MAX;
  }

  pi_control_l=P*pid_error_l+I*i_pid_error_l;
  if (pi_control_l > PI_MAX) {
    pi_control_l = PI_MAX;
  }
  if (pi_control_l < PI_MIN) {
    pi_control_l = PI_MIN;
  }
  duty_l=pi_control_l;
}


void gen_pwm(void) {
	//duty is calculated in the pi_controller rutine...
  pi_controller();
  //duty_r=ref_freqR/10.0f;
  //duty_l=ref_freqL/10.0f;
  static uint32_t pwm_enc_count=0;
  pwm_enc_count++;
  if (pwm_enc_count>PWM_ENC_CYCLES) {
    //update current encoder count
    encoderL=timer_get_counter(TIM4); //left encoder
    encoderR=timer_get_counter(TIM3); //right encoder  
    
    old_totalencoderL=totalencoderL;
    old_totalencoderR=totalencoderR;
    totalencoderL = 1.0*(((int32_t) encoderL) + (lpos-lneg)*ENCODER_MAX);
    totalencoderR = 1.0*(((int32_t) encoderR) + (rpos-rneg)*ENCODER_MAX);
    
    //update current speed
    encoder_speed_r=((float) (totalencoderR-old_totalencoderR))*((float) (PWMFREQ/PWM_ENC_CYCLES));
    encoder_speed_l=((float) (totalencoderL-old_totalencoderL))*((float) (PWMFREQ/PWM_ENC_CYCLES));
    pwm_enc_count=0;
  }


  if (duty_l < 0.0f)
    {
      timer_set_oc_mode(TIM1, TIM_OC1, TIM_OCM_PWM1);
      timer_disable_oc_output(TIM1,TIM_OC1);
      timer_enable_oc_output (TIM1, TIM_OC1N);
      duty_l=-duty_l;
    }
  else
    {
      timer_set_oc_mode(TIM1, TIM_OC1, TIM_OCM_PWM1);
      timer_enable_oc_output(TIM1, TIM_OC1 );
      timer_disable_oc_output (TIM1, TIM_OC1N);
    }
  if (duty_r < 0.0f)
    {
      timer_set_oc_mode(TIM1, TIM_OC2, TIM_OCM_PWM1);
      timer_disable_oc_output(TIM1, TIM_OC2 );
      timer_enable_oc_output (TIM1, TIM_OC2N);
      duty_r=-duty_r;
    }
  else
    {
      timer_set_oc_mode(TIM1, TIM_OC2, TIM_OCM_PWM1);
      timer_enable_oc_output(TIM1, TIM_OC2 );
      timer_disable_oc_output (TIM1, TIM_OC2N);
    }

  /* Set the capture compare value for OC1. */
  timer_set_oc_value(TIM1, TIM_OC1, duty_l*PWM_PERIOD_ARR);
  /* Set the capture compare value for OC1. */
  timer_set_oc_value(TIM1, TIM_OC2, duty_r*PWM_PERIOD_ARR);

}

void tim1_up_tim10_isr(void) {
  // Clear the update interrupt flag
  timer_clear_flag(TIM1,  TIM_SR_UIF);
  gen_pwm();

}

void tim4_isr(void) {
  // Clear the update interrupt flag
  timer_clear_flag(TIM4,  TIM_SR_UIF); 
  encoderL = timer_get_counter(TIM4);
  //overflow interrupt
  if(encoderL < 100){
    lpos++;
  }
  //underflow interrupt	
  //if (encoderL > 800){
  if (encoderL > 64000){
    //		lneg = lneg+1;
    lneg++;
  }
}


void tim3_isr(void) {
	timer_clear_flag(TIM3,  TIM_SR_UIF);
	encoderR = timer_get_counter(TIM3);
	//overflow interrupt
	if (encoderR < 100){
			rpos++;
	}
	//underflow interrupt	
	if (encoderR > 64000){
	//if (encoderR > 800){
			rneg++;
	}	
}

bool receive_a_string(char* cmd_s){
  char c=0;
  int i=0;
	
  if (poll(stdin) > 0){
    while (c!='\r'){
      c=getc(stdin);
      cmd_s[i]=c;
      i++;
      putc(c, stdout); //no necesito q me devuelva lo q escribo?
    }
    
    cmd_s[i]='\0';
    return true;
  }
  else
    return false;
}

void clean_stdin(void){
  while ( poll(stdin)>0){
    //printf("Cleaning stdin\n");
    getc(stdin);
  }
}

void stdin_init(void){
  setvbuf(stdin,NULL,_IONBF,0);  // Sets stdin  in unbuffered mode (normal for usart com)
  setvbuf(stdout,NULL,_IONBF,0); // Sets stdout in unbuffered mode (normal for usart com)
  clean_stdin();
} 


int main(void){
  system_init();
  stdin_init();

  char cmd_s[50]="";
  char cmdR[10]=""; //Right Motor
  char cmdL[10]=""; //Left Motor

  float valueR;//Right Motor speed
  float valueL;//Left Motor speed

  while (1){
		
/*			printf("a teR: %d teL: %d\n", totalencoderR,totalencoderL);*/
/*    printf("a teR: %d teL: %d EncSR: %f EncSL: %f error_r: %f i_pid_error_r: %f pi_control_r: %f error_l: %f i_pid_error_l: %f pi_control_l: %f\n", rpos, rneg, totalencoderR, encoderR, totalencoderL, encoderL, encoder_speed_r, encoder_speed_l, pid_error_r, i_pid_error_r, pi_control_r, pid_error_l, i_pid_error_l, pi_control_l);*/


/*    printf("Rpos:, %d, rneg: %d, teR: %i , eR: %d, teL: %d, eL: %d, EncSR: %f, EncSL: %f, error_r: %f, i_pid_error_r: %f, pi_control_r: %f, error_l: %f, i_pid_error_l: %f, pi_control_l: %f\n", rpos, rneg, totalencoderR, encoderR, totalencoderL, encoderL, encoder_speed_r, encoder_speed_l, pid_error_r, i_pid_error_r, pi_control_r, pid_error_l, i_pid_error_l, pi_control_l);*/
    //if (0){
       if (receive_a_string(cmd_s)){
      //printf("%s\n", cmd_s);
      sscanf(cmd_s, "%s %f %s %f", cmdR, &valueR, cmdL, &valueL);
      if( (strcmp(cmdR, "r") == 0) && (strcmp(cmdL, "l") == 0) ){ 
				//set ref freq

				ref_enc_r=valueR;
				ref_enc_l=valueL;
						
/*			ref_freqR=valueR;*/
/*			ref_freqL=valueL;*/
		/*Get encoder counters*/

						
		printf("a teR: %d teL: %d\n", totalencoderR,totalencoderL);
					
      }
    }

 }
}

