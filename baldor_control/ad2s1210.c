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
#include <libopencm3/stm32/f4/spi.h>
#include "ad2s1210.h"

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

void ad2s1210_conf(void) {
  //Setting excitation frequency to 4.5KHz
  gpio_clear(GPIOA, GPIO2);
  spi_write(SPI2, AD2S1210_REG_EXC_FREQ);
  spi_read(SPI2);
  gpio_set(GPIOA, GPIO2);
  gpio_clear(GPIOA, GPIO2);
  spi_write(SPI2, 0x12); // 4.5KHz
  spi_read(SPI2);
  gpio_set(GPIOA, GPIO2);

  //Setting resolution to 10bits
  gpio_clear(GPIOA, GPIO2);
  spi_write(SPI2, AD2S1210_REG_EXC_FREQ);
  spi_read(SPI2);
  gpio_set(GPIOA, GPIO2);
  gpio_clear(GPIOA, GPIO2);
  spi_write(SPI2, 0x12); // 4.5KHz
  spi_read(SPI2);
  gpio_set(GPIOA, GPIO2);

}

