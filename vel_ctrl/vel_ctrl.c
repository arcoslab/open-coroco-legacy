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

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3-plus/cdcacm_one_serial/cdcacm.h>
#include <libopencm3/stm32/spi.h>
#include <libopencm3/stm32/adc.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/f4/nvic.h>
#include "vel_ctrl.h"
#include <libopencm3-plus/newlib/syscall.h>
#include <libopencm3-plus/utils/misc.h>
#include <libopencm3-plus/stm32f4discovery/leds.h>
#include "ad2s1210.h"
#include <stdlib.h>
#include <string.h>

void leds_init(void) {
  rcc_periph_clock_enable(RCC_GPIOD);
  gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO12 | GPIO13 | GPIO14 | GPIO15);
}

void spi_init(void) {
  /* For spi signal pins */
  rcc_periph_clock_enable(RCC_SPI2);
  rcc_periph_clock_enable(RCC_GPIOB);

  /* Setup GPIO pins for AF5 for SPI2 signals. */
  gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE,
		  GPIO13 | GPIO14 | GPIO15);
  gpio_set_af(GPIOB, GPIO_AF5, GPIO13 | GPIO14 | GPIO15);

  //spi initialization, SPI2 working at 21MHz
  spi_init_master(SPI2, SPI_CR1_BR_FPCLK_DIV_64, SPI_CR1_CPOL_CLK_TO_0_WHEN_IDLE , SPI_CR1_CPHA_CLK_TRANSITION_1, SPI_CR1_DFF_8BIT, SPI_CR1_MSBFIRST);

  spi_set_baudrate_prescaler(SPI2, SPI_CR1_BR_FPCLK_DIV_16);
  spi_set_clock_polarity_0(SPI2);
  spi_set_clock_phase_1(SPI2);
  spi_enable_software_slave_management(SPI2);
  spi_set_nss_high(SPI2);
  spi_enable(SPI2);
}

void tim_init(void)
{
  /* Enable TIM1 clock. and Port E clock (for outputs) */
  rcc_periph_clock_enable(RCC_TIM1);
  rcc_periph_clock_enable(RCC_GPIOE);

  //Set TIM1 channel (and complementary) output to alternate function push-pull'.
  //f4 TIM1=> GIO9: CH1, GPIO11: CH2, GPIO13: CH3
  //f4 TIM1=> GIO8: CH1N, GPIO10: CH2N, GPIO12: CH3N
  gpio_mode_setup(GPIOE, GPIO_MODE_AF,GPIO_PUPD_NONE,GPIO9 | GPIO11 | GPIO13);
  gpio_set_af(GPIOE, GPIO_AF1, GPIO9 | GPIO11 | GPIO13);
  gpio_mode_setup(GPIOE, GPIO_MODE_AF,GPIO_PUPD_NONE,GPIO8 | GPIO10 | GPIO12);
  gpio_set_af(GPIOE, GPIO_AF1, GPIO8 | GPIO10 | GPIO12);

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

  /* ARR reload enable. */
  timer_enable_preload(TIM1);

  /*
   * Enable preload of complementary channel configurations and
   * update on COM event.
   */
  timer_disable_preload_complementry_enable_bits(TIM1);

  /* Enable outputs in the break subsystem. */
  timer_enable_break_main_output(TIM1);

  /* Generate update event to reload all registers before starting*/
  timer_generate_event(TIM1, TIM_EGR_UG);

  /* Counter enable. */
  timer_enable_counter(TIM1);

  //enable capture compare interrupt
  timer_enable_update_event(TIM1);

  timer_enable_irq(TIM1, TIM_DIER_UIE);
  nvic_enable_irq(NVIC_TIM1_UP_TIM10_IRQ);
}

void shutdown_on() {
  gpio_set(GPIOD, GPIO3);
}

void shutdown_off() {
  gpio_clear(GPIOD, GPIO3);
}

void shutdown_init(){
  rcc_periph_clock_enable(RCC_GPIOD);
  gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO3);
  shutdown_on();
}

void serial_conf(void) {
  setvbuf(stdin,NULL,_IONBF,0); // Sets stdin in unbuffered mode (normal for usart com)
  setvbuf(stdout,NULL,_IONBF,0); // Sets stdin in unbuffered mode (normal for usart com)
  while (poll(stdin) > 0) {
    //printf("Cleaning stdin\n");
    getc(stdin);
  }
}

bool ad_ready=false; //resolver circuit ad2s

static void adc_setup(void)
{
  rcc_periph_clock_enable(RCC_ADC1);
  rcc_periph_clock_enable(RCC_GPIOA);
  rcc_periph_clock_enable(RCC_GPIOC);
	gpio_mode_setup(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO1);
	gpio_mode_setup(GPIOC, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO1);

	adc_power_off(ADC1);
	adc_disable_scan_mode(ADC1);
	adc_set_sample_time_on_all_channels(ADC1, ADC_SMPR_SMP_3CYC);

	adc_power_on(ADC1);

}

static uint16_t read_adc_naiive(uint8_t channel)
{
	uint8_t channel_array[16];
	channel_array[0] = channel;
	adc_set_regular_sequence(ADC1, 1, channel_array);
	adc_start_conversion_regular(ADC1);
	while (!adc_eoc(ADC1));
	uint16_t reg16 = adc_read_regular(ADC1);
	return reg16;
}

void system_init(void) {
  rcc_clock_setup_hse_3v3(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_168MHZ]);
  leds_init();
  shutdown_init();
  shutdown_on();
  cdcacm_init(); //default 921600bps
  ad2s1210_init();
  spi_init();
  tim_init();
  shutdown_off();
  serial_conf();
  ad2s1210_conf();
  ad_ready=true;
  adc_setup();
}

int avg_filter(int in) {
  static int window[]={0,0,0,0,0,0,0,0,0,0};
  static uint8_t pw=0;
  window[pw]=in;
  pw++;
  if (pw==10){
    pw=0;
  }
  return((window[0]+window[1]+window[2]+window[3]+window[4]+window[5]+window[6]+window[7]+window[8]+window[9])/10);
}

#define FILTER_LEN 100
inline int avg_filter2(int in) {
  static int filter_window[FILTER_LEN];
  static uint8_t pw=0;
  static bool first=true;
  int out=0;
  int i;
  if (first) {
    for (i=0;i<FILTER_LEN; i++) {
      filter_window[i]=0;
    }
    first=false;
  }
  filter_window[pw]=in;
  pw++;
  if (pw==FILTER_LEN){
    pw=0;
  }
  for (i=0;i<FILTER_LEN;i++) {
    out=out+filter_window[i];
  }
  return(out/FILTER_LEN);
}


float est_freq=0.0f;
int raw_pos=0;
int raw_pos_last=0;
int diff_pos=0;
bool first_est=true;
uint8_t ad2s1210_fault=0xFF;
#define RES_CNT_TOP 20

void update_est_freq(void) {
  static int res_cnt=0;
  gpio_toggle(LBLUE);
  if (ad_ready) {
    raw_pos_last=raw_pos;
    AD2S1210_SAMPLE();
    raw_pos=(int) (((AD2S1210_RD(AD2S1210_REG_POS_H) << 8) | (AD2S1210_RD(AD2S1210_REG_POS_L))));
    ad2s1210_fault=AD2S1210_RD(AD2S1210_REG_FAULT);
    if (first_est==true) {
      raw_pos_last=raw_pos;
      first_est=false;
    }
    if ((raw_pos<4000) && (raw_pos_last>60000)){
      raw_pos_last=raw_pos_last-(1<<16);
    } else {
      if ((raw_pos>60000) && (raw_pos_last<4000)) {
	raw_pos_last=raw_pos_last+(1<<16);
      }
    }
    diff_pos+=raw_pos-raw_pos_last;
    if (res_cnt==RES_CNT_TOP) {
      est_freq=RAW_TO_RAD(avg_filter(diff_pos))/(TICK_PERIOD*RES_CNT_TOP);
      diff_pos=0;
      res_cnt=0;
    } else {
      res_cnt++;
    }
  }
  gpio_toggle(LBLUE);
}

float exc_volt=0;
float error=0;
float p_error=0;
float i_error=0;
float d_error=0;
float cmd_angle;
float pi_control;
float ref_freq=0*2*PI;
bool motor_off=false;

void pid_controller(void) {
  float last_error=0;
  if (motor_off) {
    i_error=0;
  }
  last_error=error;
  error=ref_freq-est_freq; // ref_freq-est_freq
  p_error=P*error;
  i_error+=I*error;
  d_error=(d_error+(error-last_error))/2.0f;
  if (d_error > D_MAX){
    d_error=D_MAX;
  }
  if (d_error < -D_MAX) {
    d_error=-D_MAX;
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
  pi_control=p_error+i_error+D*d_error;
  if (pi_control > PID_MAX) {
    pi_control = PID_MAX;
  }
  if (pi_control < -PID_MAX) {
    pi_control = -PID_MAX;
  }
}

float duty_a=0.0f;
float duty_b=0.0f;
float duty_c=0.0f;
float test=0;

void gen_pwm(void) {
  pid_controller();
  cmd_angle=2*RAW_TO_RAD(raw_pos)+RESOLVER_STATOR_OFFSET+pi_control;
  //cmd_angle=2*RAW_TO_RAD(raw_pos)+RESOLVER_STATOR_OFFSET;

  //cmd_angle+=2.0f*PI*TICK_PERIOD; //openloop
  //converting big angles into something between 0 and 2pi
  if (cmd_angle >= (2.0f*PI)) {
    cmd_angle=cmd_angle-(2.0f*PI);
  }

  if (fabsf(est_freq)<MIN_EXC_FREQ_PERC*MAX_FREQ) {
    exc_volt=MIN_EXC_VOLT;
  } else {
    exc_volt=MIN_EXC_VOLT+(MAX_EXC_VOLT-MIN_EXC_VOLT)*(fabsf(est_freq)-MIN_EXC_FREQ_PERC*MAX_FREQ)/(MAX_FREQ-MIN_EXC_FREQ_PERC*MAX_FREQ);
  }

  exc_volt+=(MAX_EXC_VOLT-exc_volt)*(fabsf(pi_control)/PID_MAX);

  if (exc_volt>1.0f) {
    exc_volt=1.0f;
  }
  //exc_volt=0.8;

  duty_a=sinf(cmd_angle);
  duty_b=sinf(cmd_angle+2.0f*PI/3.0f);
  duty_c=sinf(cmd_angle+4.0f*PI/3.0f);

  if (motor_off) {
    duty_a=0;
    duty_b=0;
    duty_c=0;
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
  timer_set_oc_value(TIM1, TIM_OC1, duty_a*exc_volt*PWM_PERIOD_ARR);
  /* Set the capture compare value for OC1. */
  timer_set_oc_value(TIM1, TIM_OC2, duty_b*exc_volt*PWM_PERIOD_ARR);
  /* Set the capture compare value for OC1. */
  timer_set_oc_value(TIM1, TIM_OC3, duty_c*exc_volt*PWM_PERIOD_ARR);
  //tim_force_update_event(TIM1);
}

void tim1_up_tim10_isr(void) {
  // Clear the update interrupt flag
  timer_clear_flag(TIM1,  TIM_SR_UIF);
  update_est_freq();
  gen_pwm();
}

int main(void)
{
  int i;
  int c=0;
  char cmd_s[50]="";
  char cmd[10]="";
  float value=0;
  system_init();

  while(true) {
    if ((poll(stdin) > 0)) {
      i=0;
      if (poll(stdin) > 0) {
	c=0;
	while (c!='\r') {
	  c=getc(stdin);
	  cmd_s[i]=c;
	  i++;
	  //putc(c, stdout);
	}
	cmd_s[i]='\0';
      }
      //printf("%s", cmd_s);
      sscanf(cmd_s, "%s %f", cmd, &value);
      if (strcmp(cmd, "f") == 0){ //set ref freq
	//printf("New reference frequency: %f. Confirm? (Press \"y\")\n", value);
	ref_freq=value;
	//printf("ef: %010.5f ca: %05d\n", est_freq, raw_pos); //**************Remove comment and comment the line below
    //printf("ef: %010.5f ca: %05d\n", ref_freq, raw_pos);
    //printf("STM32_POSITION %010.5f ca %05d\n", ref_freq, raw_pos);

    //printf("STM32_POSITION %010.5f ca %05d\n", ref_freq, raw_pos);
    //printf("%d %010.5f ca %05d\n",STM32_POSITION, ref_freq, raw_pos);
    //printf("%010.5f ca: %05d ref: %6.2f\n", est_freq, raw_pos,ref_freq);
	if (value == 0.0f) {
	  //motor_off=true;
	} else {
	  //printf("Motor on\n");
	  motor_off=false;
	}
      }
    }

    //printf("ad2s_fault: 0x%02X, raw_pos: %05d, raw_pos_last: %05d, diff_pos: %05d, ref_freq: %010.5f, est_freq: %010.5f, exc_volt: %04.2f, p_error: %08.5f, i_error: %04.2f, pi_control: %04.2f, cmd_angle: %04.2f\n", ad2s1210_fault, raw_pos, raw_pos_last, diff_pos, ref_freq, est_freq, exc_volt, p_error, i_error, pi_control, cmd_angle*360/(2*PI));
    uint16_t input_adc1 = read_adc_naiive(1);
    uint16_t input_adc11 = read_adc_naiive(11);

    //printf("ad2s_fault: 0x%02X, cur_angle: %05d, ref_freq: %010.5f, est_freq: %010.5f, exc_volt: %04.2f, error: %05.2f, p_error: %08.5f, i_error: %04.2f, pi_control: %08.5f, cmd_angle: %06.2f, exc_volt: %04.2f, test: %04.2f\n", ad2s1210_fault, raw_pos*360/(1<<16), ref_freq/(2*PI), est_freq/(2*PI), exc_volt, error, p_error, i_error, pi_control, cmd_angle*360/(2*PI), exc_volt, test);
    printf("ad2s_fault: 0x%02X, cur_angle: %05d, ref_freq: %010.5f, est_freq: %010.5f, exc_volt: %04.2f, error: %05.2f, p_error: %08.5f, i_error: %04.2f, pi_control: %08.5f, cmd_angle: %06.2f, adc1: %d, adc11: %d\n", ad2s1210_fault, raw_pos*360/(1<<16), ref_freq/(2*PI), est_freq/(2*PI), exc_volt, error, p_error, i_error, pi_control, cmd_angle*360/(2*PI), input_adc1, input_adc11);
    //printf("ef: %010.5f ca: %6.5f\n", est_freq, ref_freq);
    //printf("ef: %010.5f ca: %05d\n", ref_freq, raw_pos);
    //printf("%d %010.5f ca %05d\n",STM32_POSITION, ref_freq, raw_pos);
  }

  return(0);
}
