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

void leds_init(void) {
  rcc_periph_clock_enable(RCC_GPIOD);
  gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO12 | GPIO13 | GPIO14 | GPIO15);
}

void ad2s1210_init(void) {
  /* For spi mode select on the ad2s1210 */
  rcc_periph_clock_enable(RCC_GPIOA);
  /* Setup GPIOA2 pin for spi mode ad2s1210 select. */
  gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO2);
  /* Start with spi communication disabled */
  gpio_set(GPIOA, GPIO2);
  // Pin PC2 for sample generation
  rcc_periph_clock_enable(RCC_GPIOC);
  /* Setup GPIOC2 pin for ad2s1210 sample. */
  gpio_mode_setup(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO2);
  /* Start with sample high, data register update is done in high to low edge */
  gpio_set(GPIOC, GPIO2);
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

  /* Reenable outputs. */
  timer_enable_oc_output(TIM1, TIM_OC1);
  timer_enable_oc_output(TIM1, TIM_OC1N);
  timer_enable_oc_output(TIM1, TIM_OC2);
  timer_enable_oc_output(TIM1, TIM_OC2N);
  timer_enable_oc_output(TIM1, TIM_OC3);
  timer_enable_oc_output(TIM1, TIM_OC3N);

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

void system_init(void) {
  rcc_clock_setup_hse_3v3(&hse_8mhz_3v3[CLOCK_3V3_168MHZ]);
  leds_init();
  cdcacm_init();
  ad2s1210_init();
  spi_init();
  tim_init();
}


int main(void)
{
  system_init();
  return(0);
}
