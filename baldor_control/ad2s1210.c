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
#include <libopencm3/stm32/spi.h>
#include "ad2s1210.h"

void ad2s1210_init(void) {
  //PC1=RES0, PC0=RES1
  rcc_periph_clock_enable(RCC_GPIOC);
  gpio_mode_setup(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO0 | GPIO1 | GPIO3);
  gpio_clear(GPIOC, GPIO0); //RES0
  gpio_clear(GPIOC, GPIO1); //RES1

  //PE15=RD
  rcc_periph_clock_enable(RCC_GPIOE);
  gpio_mode_setup(GPIOE, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO15);
  gpio_set(GPIOE, GPIO15); //RD

  //PB11=A0
  //PB12=A1
  rcc_periph_clock_enable(RCC_GPIOB);
  gpio_mode_setup(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO11 | GPIO12);
  gpio_set(GPIOB, GPIO11); //A0
  gpio_set(GPIOB, GPIO12); //A1

  //PA3=WR
  rcc_periph_clock_enable(RCC_GPIOA);
  gpio_mode_setup(AD2S1210_WRPIN_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, AD2S1210_WRPIN);
  gpio_set(AD2S1210_WRPIN_PORT, AD2S1210_WRPIN); //WR

  /* Reset pin */
  rcc_periph_clock_enable(RCC_GPIOD);
  /* Setup GPIOD8 pin for gpio output */
  gpio_mode_setup(AD2S1210_RESETPIN_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, AD2S1210_RESETPIN);
  /* Reset and then release */
  gpio_clear(AD2S1210_RESETPIN_PORT, AD2S1210_RESETPIN);
  WAIT_100NANO(50);
  gpio_set(AD2S1210_RESETPIN_PORT, AD2S1210_RESETPIN);
  WAIT_100NANO(50);

  /* For spi mode select on the ad2s1210 */
  //rcc_periph_clock_enable(RCC_GPIOA);
  rcc_periph_clock_enable(RCC_GPIOE); //brian test
  /* Setup GPIOA2 pin for spi mode ad2s1210 select. */
  gpio_mode_setup(AD2S1210_CSPIN_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, AD2S1210_CSPIN);
  gpio_clear(AD2S1210_CSPIN_PORT, AD2S1210_CSPIN);
  // Pin PC2 for sample generation
  rcc_periph_clock_enable(RCC_GPIOC);
  /* Setup GPIOC2 pin for ad2s1210 sample. */
  gpio_mode_setup(AD2S1210_SAMPLEPIN_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, AD2S1210_SAMPLEPIN);
  /* Start with sample high, data register update is done in high to low edge */
  gpio_set(AD2S1210_SAMPLEPIN_PORT, AD2S1210_SAMPLEPIN);
}

void ad2s1210_conf(void) {
    WAIT_100NANO(50);
    //soft reset
    AD2S1210_WR(AD2S1210_REG_SOFT_RESET, 0x00);
    //Setting excitation frequency to 4.5KHz
    AD2S1210_WR(AD2S1210_REG_EXC_FREQ, 0x12);
}

