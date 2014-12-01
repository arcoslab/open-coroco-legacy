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
#include <libopencm3-plus/cdcacm_one_serial/cdcacm.h>
#include <libopencm3/stm32/f4/spi.h>
#include <libopencm3/stm32/f4/timer.h>
#include <libopencm3/stm32/f4/nvic.h>
#include "baldor_control.h"
#include <libopencm3-plus/newlib/syscall.h>
#include <libopencm3-plus/utils/misc.h>
#include <libopencm3-plus/stm32f4discovery/leds.h>
#include "ad2s1210.h"
#include <stdlib.h>

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

void serial_conf(void) {
  setvbuf(stdin,NULL,_IONBF,0); // Sets stdin in unbuffered mode (normal for usart com)
  setvbuf(stdout,NULL,_IONBF,0); // Sets stdin in unbuffered mode (normal for usart com)
  while (poll(stdin) > 0) {
    printf("Cleaning stdin\n");
    getc(stdin);
  }
}

bool ad_ready=false;

void system_init(void) {
  rcc_clock_setup_hse_3v3(&hse_8mhz_3v3[CLOCK_3V3_168MHZ]);
  leds_init();
  cdcacm_init(); //default 921600bps
  ad2s1210_init();
  spi_init();
  tim_init();
  serial_conf();
  ad2s1210_conf();
  ad_ready=true;
}

inline int avg_filter(int in) {
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

void tim1_up_tim10_isr(void) {
  // Clear the update interrupt flag
  timer_clear_flag(TIM1,  TIM_SR_UIF);
  update_est_freq();
  //calc_freq();
  //start_up();
  //gen_pwm();
}

int main(void)
{
  system_init();


  while(true) {
    printf("ad2s_fault: 0x%02X, raw_pos: %05d, raw_pos_last: %05d, diff_pos: %05d, est_freq: %010.5f\n", ad2s1210_fault, raw_pos, raw_pos_last, diff_pos, est_freq);
  }

  return(0);
}
