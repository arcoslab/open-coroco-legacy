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
 //Reset wait
  WAIT_100NANO(100);

  //Do reset
  rcc_periph_clock_enable(AD2S1210_RESETN_PORT_RCC);
  gpio_mode_setup(AD2S1210_RESETN_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, AD2S1210_RESETN);
  gpio_clear(AD2S1210_RESETN_PORT, AD2S1210_RESETN);

  //Reset wait
  WAIT_100NANO(100);

  //A0 configuration mode
  rcc_periph_clock_enable(AD2S1210_A0_PORT_RCC);
  gpio_mode_setup(AD2S1210_A0_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, AD2S1210_A0);
  gpio_set(AD2S1210_A0_PORT, AD2S1210_A0);

  //A1 configuration mode
  rcc_periph_clock_enable(AD2S1210_A1_PORT_RCC);
  gpio_mode_setup(AD2S1210_A1_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, AD2S1210_A1);
  gpio_set(AD2S1210_A1_PORT, AD2S1210_A1);

  //RES0 12 bits
  rcc_periph_clock_enable(AD2S1210_RES0_PORT_RCC);
  gpio_mode_setup(AD2S1210_RES0_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, AD2S1210_RES0);
  gpio_clear(AD2S1210_RES0_PORT, AD2S1210_RES0);

  //RES1 12 bits
  rcc_periph_clock_enable(AD2S1210_RES1_PORT_RCC);
  gpio_mode_setup(AD2S1210_RES1_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, AD2S1210_RES1);
  gpio_set(AD2S1210_RES1_PORT, AD2S1210_RES1);

  //Chip select
  rcc_periph_clock_enable(AD2S1210_CSN_PORT_RCC);
  gpio_mode_setup(AD2S1210_CSN_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, AD2S1210_CSN);
  gpio_clear(AD2S1210_CSN_PORT, AD2S1210_CSN);

  //Reset wait
  WAIT_100NANO(100);

  //Remove reset
  gpio_set(AD2S1210_RESETN_PORT, AD2S1210_RESETN);

  //Reset wait
  WAIT_100NANO(100);

  /* For spi enable on the ad2s1210 */
  rcc_periph_clock_enable(AD2S1210_WRPIN_PORT_RCC);
  /* Setup pin for spi enable  ad2s1210. */
  gpio_mode_setup(AD2S1210_WRPIN_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, AD2S1210_WRPIN);
  /* Start with spi disabled */
  gpio_set(AD2S1210_WRPIN_PORT, AD2S1210_WRPIN);
  // Pin for sample generation
  rcc_periph_clock_enable(AD2S1210_SAMPLEPIN_PORT_RCC);
  /* Setup pin for ad2s1210 sample. */
  gpio_mode_setup(AD2S1210_SAMPLEPIN_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, AD2S1210_SAMPLEPIN);
  /* Start with sample high, data register update is done in high to low edge */
  gpio_set(AD2S1210_SAMPLEPIN_PORT, AD2S1210_SAMPLEPIN);
}

void ad2s1210_conf(void) {
  //Setting excitation frequency to 4.5KHz
  gpio_clear(AD2S1210_WRPIN_PORT, AD2S1210_WRPIN);
  spi_write(SPI2, AD2S1210_REG_EXC_FREQ);
  spi_read(SPI2);
  gpio_set(AD2S1210_WRPIN_PORT, AD2S1210_WRPIN);
  gpio_clear(AD2S1210_WRPIN_PORT, AD2S1210_WRPIN);
  spi_write(SPI2, 0x12); // 4.5KHz
  spi_read(SPI2);
  gpio_set(AD2S1210_WRPIN_PORT, AD2S1210_WRPIN);

  AD2S1210_WR(AD2S1210_REG_DOS_OVER_THRESHOLD, 121);

}

