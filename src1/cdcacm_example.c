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

#include <libopencm3/stm32/f4/rcc.h>
#include <libopencm3/stm32/f4/gpio.h>
#include <libopencm3/stm32/f4/timer.h>
#include <libopencm3/stm32/f4/nvic.h>
#include "cdcacm_example.h"
#include "cdcacm/cdcacm.h"
#include <stdio.h>
#include "utils/mutex.h"
#include "utils/misc.h"

//pwm-related timer configuration
#define SYSFREQ     168000000 //168MHz
#define PWMFREQ        32000  //32000
#define PWMFREQ_F       ((float )(PWMFREQ)) //32000.0f
#define PRESCALE        1                                       //freq_CK_CNT=freq_CK_PSC/(PSC[15:0]+1)
#define PWM_PERIOD_ARR  SYSFREQ/( PWMFREQ*(PRESCALE+1) )
#define deadtime_percentage 0.10f   //10% 87.2us de alto a bajo, 48.8us de bajo a alto 
#define INIT_DUTY 0.5f
#define PI 3.1416f

float attenuation=0.3f;

void leds_init(void) {
  rcc_peripheral_enable_clock(&RCC_AHB1ENR, RCC_AHB1ENR_IOPDEN);
  gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO12);
  gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO13);
  gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO14);
  gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO15);
}

void hall_init(void) {
  /* Enable GPIOB and GPIOE clock. */
  rcc_peripheral_enable_clock(&RCC_AHB1ENR, RCC_AHB1ENR_IOPBEN);
  rcc_peripheral_enable_clock(&RCC_AHB1ENR, RCC_AHB1ENR_IOPEEN);
  /* Set GPIO to 'input floating'. */

  //Hall sensor 1
  gpio_mode_setup(GPIOE, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO15);
  gpio_set_af(GPIOE, GPIO_AF0, GPIO15);
}

void tim_init(void)
{
	/* Enable TIM1 clock. and Port E clock (for outputs) */
	rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_TIM1EN);
	rcc_peripheral_enable_clock(&RCC_AHB1ENR, RCC_AHB1ENR_IOPEEN);

	//Set TIM1 channel (and complementary) output to alternate function push-pull'.
	//f4 TIM1=> GIO9: CH1, GPIO11: CH2, GPIO13: CH3
	//f4 TIM1=> GIO8: CH1N, GPIO10: CH2N, GPIO12: CH3N
	gpio_mode_setup(GPIOE, GPIO_MODE_AF,GPIO_PUPD_NONE,GPIO9 | GPIO11 | GPIO13);
	gpio_set_af(GPIOE, GPIO_AF1, GPIO9 | GPIO11 | GPIO13);
	gpio_mode_setup(GPIOE, GPIO_MODE_AF,GPIO_PUPD_NONE,GPIO8 | GPIO10 | GPIO12);
	gpio_set_af(GPIOE, GPIO_AF1, GPIO8 | GPIO10 | GPIO12);

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
	timer_set_repetition_counter(TIM1, 0); //disabled
	timer_enable_preload(TIM1);
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
	timer_disable_oc_output(TIM1, TIM_OC3);
	timer_disable_oc_output(TIM1, TIM_OC3N);

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

	/* -- OC3 and OC3N configuration -- */
	/* Configure global mode of line 3. */
	timer_enable_oc_preload(TIM1, TIM_OC3);
	timer_set_oc_mode(TIM1, TIM_OC3, TIM_OCM_PWM1);
	/* Configure OC3. */
	timer_set_oc_polarity_high(TIM1, TIM_OC3);
	timer_set_oc_idle_state_unset(TIM1, TIM_OC3);
	/* Configure OC3N. */
	timer_set_oc_polarity_high(TIM1, TIM_OC3N);
	timer_set_oc_idle_state_unset(TIM1, TIM_OC3N);
	/* Set the capture compare value for OC3. */
	timer_set_oc_value(TIM1, TIM_OC3, INIT_DUTY*PWM_PERIOD_ARR);//initial_duty_cycle*pwm_period_ARR);//100);

	/* Reenable outputs. */
	timer_enable_oc_output(TIM1, TIM_OC1);
	timer_enable_oc_output(TIM1, TIM_OC1N);
	timer_enable_oc_output(TIM1, TIM_OC2);
	timer_enable_oc_output(TIM1, TIM_OC2N);
	timer_enable_oc_output(TIM1, TIM_OC3);
	timer_enable_oc_output(TIM1, TIM_OC3N);

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
  hall_init();
  cdcacm_init();
  printled(4, LRED);
  tim_init();
}

#include <limits.h>
#include <stdbool.h>
#define HALL_A() gpio_get(GPIOE, GPIO15);
int hall_a;
uint ticks;
uint period;
#define TICK_PERIOD 1.0f/PWMFREQ_F

#define MYUINT_MAX 536870912

uint temp_period;
float est_angle=0;

void calc_freq(void) {
  static first=true;
  static int hall_a_last=0;
  static uint last_fall_hall_a_ticks=0;
  hall_a=HALL_A();
  if (first) {
    hall_a_last=hall_a;
    ticks=0;
    period=UINT_MAX;
    last_fall_hall_a_ticks=ticks;
    first=false;
  } else {
    ticks++;
    if (ticks == UINT_MAX) {
      ticks=0;
    }
    if ((hall_a_last > 0) && (hall_a == 0)) {
      //hall falling edge: new cycle
      //new rotor position measurement
      est_angle=0;
      gpio_toggle(LGREEN);
      if (ticks > last_fall_hall_a_ticks) { //updating period
	period=ticks-last_fall_hall_a_ticks;
      } else {
	period=UINT_MAX-last_fall_hall_a_ticks+ticks;
      }
      last_fall_hall_a_ticks=ticks;
    } else {
      //we update period only if is bigger than before
      if (ticks > last_fall_hall_a_ticks) {
	temp_period=ticks-last_fall_hall_a_ticks;
      } else {
	temp_period=UINT_MAX-last_fall_hall_a_ticks+ticks;
      }
      if (temp_period > period) {
	period=temp_period;
      }
      //update estimated current angle
      est_angle+=2.0f*PI*TICK_PERIOD/(period/TICK_PERIOD);
      if (est_angle > 2.0f*PI) {
	est_angle=0;
      }
    }
  }
  hall_a_last=hall_a;
}

float duty_a=0.0f;
float duty_b=0;
float duty_c=0;
float ref_freq=1;
float cur_angle=0;
#define t ticks/TICK_PERIOD
int close_loop=false;
#define P 0.01
#define P_DOWN 0.0001
#define I 0.000001
#define I_DOWN 0.00000001
#define I_MAX 80*PI/180.0f
#define P_MAX 80*PI/180.0f
#define PI_MAX 89*PI/180.0f
#define PI_MIN -10*PI/180.0f
float final_ref_freq=40;
float error, p_error;
float i_error=0;
float cmd_angle;
float pi_control;

void pi_controller(void) {
  error=ref_freq-1.0f/(period/TICK_PERIOD); // ref_freq-cur_freq
  if (error > 0.0f) {
    p_error=P*error;
  } else {
    p_error=P_DOWN*error;
  }
  if (error > 0.0f) {
    i_error+=I*error;
  } else {
    i_error+=I_DOWN*error;
  }
  if (i_error > I_MAX){
    i_error=I_MAX;
  }
  if (i_error < -I_MAX) {
    i_error=-I_MAX;
  }
  if (p_error > P_MAX) {
    p_error=P_MAX;
  }
  if (p_error < -P_MAX) {
    p_error= -P_MAX;
  }
  pi_control=p_error+i_error;
  if (pi_control > PI_MAX) {
    pi_control = PI_MAX;
  }
  if (pi_control < -PI_MAX) {
    pi_control = -PI_MAX;
  }
  cmd_angle+=pi_control;
}

#define CUR_FREQ 1.0f/(period/TICK_PERIOD)

void calc_attenuation(void) {
  if (CUR_FREQ < 10.0f) {
    attenuation = 0.3f;
  } else {
    //attenuation=0.3f+(CUR_FREQ-10.0f)/(30.0f/0.7f);
  }
}

void gen_pwm(void) {
  //calc_attenuation();

  static float pi_times;
  cur_angle+=2.0f*PI*TICK_PERIOD*ref_freq;
  //converting big angles into something between 0 and 2pi
  if (cur_angle >= (2.0f*PI)) {
    cur_angle=cur_angle-(2.0f*PI);
  }

  if ((cur_angle >= 89.0f*PI/180.0f) && (cur_angle <= 91.0f*PI/180.0f)) {
    gpio_toggle(LBLUE);
  }

  cmd_angle=est_angle+10.0f*PI/180.0f;

  if (!close_loop) {
    duty_a=sinf(cur_angle);
    duty_b=sinf(cur_angle+2.0f*PI/3.0f);
    duty_c=sinf(cur_angle+4.0f*PI/3.0f);
  } else {
    pi_controller();
    duty_a=sinf(cmd_angle);
    duty_b=sinf(cmd_angle+2.0f*PI/3.0f);
    duty_c=sinf(cmd_angle+4.0f*PI/3.0f);
  }

  if (duty_a < 0.0f)
    {
      timer_set_oc_mode(TIM1, TIM_OC1, TIM_OCM_PWM1);
      timer_disable_oc_output(TIM1,TIM_OC1);
      timer_enable_oc_output (TIM1, TIM_OC1N);
      duty_a=-duty_a;
    }
  else
    {
      timer_set_oc_mode(TIM1, TIM_OC1, TIM_OCM_PWM1);
      timer_enable_oc_output(TIM1, TIM_OC1 );
      timer_disable_oc_output (TIM1, TIM_OC1N);
    }
  if (duty_b < 0.0f)
    {
      timer_set_oc_mode(TIM1, TIM_OC2, TIM_OCM_PWM1);
      timer_disable_oc_output(TIM1, TIM_OC2 );
      timer_enable_oc_output (TIM1, TIM_OC2N);
      duty_b=-duty_b;
    }
  else
    {
      timer_set_oc_mode(TIM1, TIM_OC2, TIM_OCM_PWM1);
      timer_enable_oc_output(TIM1, TIM_OC2 );
      timer_disable_oc_output (TIM1, TIM_OC2N);
    }
  if (duty_c < 0.0f)
    {
      timer_set_oc_mode(TIM1, TIM_OC3, TIM_OCM_PWM1);
      timer_disable_oc_output(TIM1, TIM_OC3 );
      timer_enable_oc_output (TIM1, TIM_OC3N);
      duty_c=-duty_c;
    }
  else
    {
      timer_set_oc_mode(TIM1, TIM_OC3, TIM_OCM_PWM1);
      timer_enable_oc_output(TIM1, TIM_OC3 );
      timer_disable_oc_output (TIM1, TIM_OC3N);
    }

  /* Set the capture compare value for OC1. */
  timer_set_oc_value(TIM1, TIM_OC1, duty_a*attenuation*PWM_PERIOD_ARR);
  /* Set the capture compare value for OC1. */
  timer_set_oc_value(TIM1, TIM_OC2, duty_b*attenuation*PWM_PERIOD_ARR);
  /* Set the capture compare value for OC1. */
  timer_set_oc_value(TIM1, TIM_OC3, duty_c*attenuation*PWM_PERIOD_ARR);
  //tim_force_update_event(TIM1);

}

void tim1_up_tim10_isr(void) {
  
  // Clear the update interrupt flag
  timer_clear_flag(TIM1,  TIM_SR_UIF);
  //gpio_toggle(LBLUE);
  calc_freq();
  gen_pwm();
  //printled(2,LBLUE);
  //printf("Aqui estoy\n");
}

int main(void)
{
  system_init();
  char buf[50]="hola_orig";
  int i, j;
  int c;
  int n_char=0;
  setvbuf(stdin,NULL,_IONBF,0); // Sets stdin in unbuffered mode (normal for usart com)
  setvbuf(stdout,NULL,_IONBF,0); // Sets stdin in unbuffered mode (normal for usart com)
  //printled(3, LRED);
  int counter=0;
  int new_freq=0;
  while (1){
    counter++;
    //halla=gpio_get(GPIOE, GPIO15);
    //halla=gpio_read(GPIOE);
    //printf("ticks: %7u, period %15u, halla: %6d, Period %6.2f, freq: %6.2f, counter %d\n", ticks, period, hall_a, period/TICK_PERIOD, 1.0f/(period/TICK_PERIOD), counter);
    //printf("ticks: %7u, duty_a %6.2f, angle %6.1f\n", ticks, duty_a*attenuation*PWM_PERIOD_ARR, cur_angle);
    //printf("hola\n");
    //printf("a: %6.1f, e_a: %6.1f, c_f: %6.2f, ref_f: %6.2f\n", cur_angle, est_angle, 1.0f/(period/TICK_PERIOD), ref_freq);
    //printf("cur_angle: %6.1f, est_angle: %6.1f\n", cur_angle, est_angle);
    wait(30);
    if (ref_freq < 9.0f) {
      ref_freq+=0.2f;
    } else {
      close_loop=true;
      //ref_freq=400.0f;
      //pi_controller();
    }
    printf(" e: %6.2f, e_p %6.2f, e_i: %6.2f, c_a: %6.2f, c_f: %6.2f\n", error, p_error, i_error, pi_control*180.0f/PI, 1.0f/(period/TICK_PERIOD));
    if (close_loop) {
      //ref_freq=20.0f;
      printf("close loop. Enter new frequency.\n");
      scanf("%f", &ref_freq);
      printf("New freq: %d\n", new_freq);
      //ref_freq=new_freq;
      //ref_freq+=0.2;
    }
    //printf("ticks: %u, halla: %d\n", ticks, hall_a);
    //printf("period: %u, halla: %d\n", period, hall_a);
    //wait(10);
    //printled(1, LRED);
    /*
    //For reading until enter:
    n_char=scanf("%[^\r]", buf); // minicom generates \r for enters
    c=getc(stdin); //To remove the \r from the buffer. If not,
                   //the next time scanf is called will always
                   //read \r not print it and printing nothing as a result. Getting "stuck"
    */

    //For reading until white-spaces and non-words:
    //n_char=scanf("%s", buf);

    //For reading 10 chars and then printing them:
    /**for (i=0; i<10; i++) {
      c=getc(stdin);
      buf[i]=c;
      if (c=='\r') {
	buf[++i]='\n';
      }
    }
    buf[++i]='\0'; */
    //printled(1, LRED);
    //printf("%s %d\n", buf, n_char);
  }
}
