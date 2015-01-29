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
#include <libopencm3/stm32/f4/spi.h>
#include <libopencm3-plus/newlib/syscall.h>
#include "resolver_test.h"
#include <libopencm3-plus/cdcacm_one_serial/cdcacm.h>
#include <stdio.h>
#include <libopencm3-plus/utils/misc.h>
#include <libopencm3-plus/stm32f4discovery/leds.h>
#include <limits.h>
#include <stdbool.h>


#define AD2S1210_WRPIN GPIO2
#define AD2S1210_WRPIN_PORT GPIOA
#define AD2S1210_SPI SPI2
#define AD2S1210_SAMPLEPIN GPIO2
#define AD2S1210_SAMPLEPIN_PORT GPIOC

#define AD2S1210_REG_FAULT 0xff
#define AD2S1210_REG_POS_H 0x80
#define AD2S1210_REG_POS_L 0x81
#define AD2S1210_REG_VEL_H 0x82
#define AD2S1210_REG_VEL_L 0x83

#define AD2S1210_WR(reg, data) {\
  gpio_clear(AD2S1210_WRPIN_PORT, AD2S1210_WRPIN);\
  spi_write(AD2S1210_SPI, reg);			  \
  spi_read(AD2S1210_SPI); \
  gpio_set(AD2S1210_WRPIN_PORT, AD2S1210_WRPIN);\
  gpio_clear(AD2S1210_WRPIN_PORT, AD2S1210_WRPIN);\
  spi_write(AD2S1210_SPI, data);			  \
  spi_read(AD2S1210_SPI); \
  gpio_set(AD2S1210_WRPIN_PORT, AD2S1210_WRPIN);\
  }

#define AD2S1210_RD(reg) ({\
  uint8_t data;\
  gpio_clear(AD2S1210_WRPIN_PORT, AD2S1210_WRPIN);\
  spi_write(AD2S1210_SPI, reg);			  \
  spi_read(AD2S1210_SPI); \
  gpio_set(AD2S1210_WRPIN_PORT, AD2S1210_WRPIN);\
  gpio_clear(AD2S1210_WRPIN_PORT, AD2S1210_WRPIN);\
  spi_write(AD2S1210_SPI, 0x00); \
  data = spi_read(AD2S1210_SPI); \
  gpio_set(AD2S1210_WRPIN_PORT, AD2S1210_WRPIN);\
  data;\
    })

#define WAIT_100NANO(ns) {\
  int i;\
  for (i = 0; i < 7*ns; i++)    /* Wait a bit. */	\
    __asm__("nop");\
  }


#define AD2S1210_SAMPLE() {\
  gpio_set(AD2S1210_SAMPLEPIN_PORT, AD2S1210_SAMPLEPIN);\
  gpio_clear(AD2S1210_SAMPLEPIN_PORT, AD2S1210_SAMPLEPIN);\
  WAIT_100NANO(2); \
  gpio_set(AD2S1210_SAMPLEPIN_PORT, AD2S1210_SAMPLEPIN);\
  WAIT_100NANO(5); \
  }

void leds_init(void) {
  rcc_peripheral_enable_clock(&RCC_AHB1ENR, RCC_AHB1ENR_IOPDEN);
  gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO12 | GPIO13 | GPIO14 | GPIO15);
}

void spi_init(void) {
  rcc_periph_clock_enable(RCC_SPI2);
  /* For spi signal pins */
  rcc_periph_clock_enable(RCC_GPIOB);

  rcc_peripheral_enable_clock(&RCC_AHB1ENR, RCC_AHB1ENR_IOPBEN);
  rcc_peripheral_enable_clock(&RCC_AHB1ENR, RCC_SPI2);


  /* Setup GPIO pins for AF5 for SPI2 signals. */
  gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE,
		  GPIO13 | GPIO14 | GPIO15);
  gpio_set_af(GPIOB, GPIO_AF5, GPIO13 | GPIO14 | GPIO15);

  //spi initialization, SPI2 working at 21MHz
  spi_init_master(SPI2, SPI_CR1_BR_FPCLK_DIV_64, SPI_CR1_CPOL_CLK_TO_0_WHEN_IDLE , SPI_CR1_CPHA_CLK_TRANSITION_1, SPI_CR1_DFF_8BIT, SPI_CR1_MSBFIRST);

  //spi_set_master_mode(SPI2);
  spi_set_baudrate_prescaler(SPI2, SPI_CR1_BR_FPCLK_DIV_16);
  spi_set_clock_polarity_0(SPI2);
  spi_set_clock_phase_1(SPI2);
  //spi_set_full_duplex_mode(SPI2);
  //spi_set_unidirectional_mode(SPI2); /* bidirectional but in 3-wire */
  //spi_set_data_size(SPI2, SPI_CR2_DS_8BIT);
  spi_enable_software_slave_management(SPI2);
  //spi_send_msb_first(SPI2);
  spi_set_nss_high(SPI2);
  //spi_enable_ss_output(SPI2);
  //spi_fifo_reception_threshold_8bit(SPI2);
  //SPI_I2SCFGR(SPI2) &= ~SPI_I2SCFGR_I2SMOD;
  spi_enable(SPI2);
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

void system_init(void) {
  rcc_clock_setup_hse_3v3(&hse_8mhz_3v3[CLOCK_3V3_168MHZ]);
  leds_init();
  cdcacm_init();
  ad2s1210_init();
  spi_init();
  printled(4, LRED);
}

int main(void)
{
  system_init();
  char cmd_s[50]="";
  char cmd[10]="";
  char confirm[10]="";
  int i, j;
  uint8_t spi_data1, spi_data2;
  int c=0;
  int n_char=0;
  setvbuf(stdin,NULL,_IONBF,0); // Sets stdin in unbuffered mode (normal for usart com)
  setvbuf(stdout,NULL,_IONBF,0); // Sets stdin in unbuffered mode (normal for usart com)
  //printled(3, LRED);
  int eof;
  while (poll(stdin) > 0) {
    printf("Cleaning stdin\n");
    getc(stdin);
  }
  int counter=0;
  while (1){
    counter+=1;
    wait(50);
    //configure
    gpio_clear(GPIOA, GPIO2);
    spi_write(SPI2, 0x91);
    spi_data1=spi_read(SPI2);
    gpio_set(GPIOA, GPIO2);
    //gpio_clear(GPIOA, GPIO2);
    //gpio_set(GPIOA, GPIO2);
    gpio_clear(GPIOA, GPIO2);
    //if (counter>20) {
    //  spi_write(SPI2, 0x28); // 10KHz
    //} else {
      spi_write(SPI2, 0x12); // 4.5KHz
      //}
    if (counter>40) {
      counter=0;
    }
    spi_data2=spi_read(SPI2);
    gpio_set(GPIOA, GPIO2);
    //gpio_clear(GPIOA, GPIO2);
    //gpio_set(GPIOA, GPIO2);

    //fault register
    spi_data1=AD2S1210_RD(AD2S1210_REG_FAULT);
    /* gpio_clear(GPIOA, GPIO2); */
    /* spi_write(SPI2, 0xff); */
    /* spi_data1=spi_read(SPI2); */
    /* gpio_set(GPIOA, GPIO2); */
    /* //gpio_clear(GPIOA, GPIO2); */
    /* //gpio_set(GPIOA, GPIO2); */
    /* gpio_clear(GPIOA, GPIO2); */
    /* spi_write(SPI2, 0x00); */
    /* spi_data1=spi_read(SPI2); */
    /* gpio_set(GPIOA, GPIO2); */
    /* //gpio_clear(GPIOA, GPIO2); */
    /* //gpio_set(GPIOA, GPIO2); */

    AD2S1210_SAMPLE();

    uint16_t pos;
    pos=((AD2S1210_RD(AD2S1210_REG_POS_H) << 8) | (AD2S1210_RD(AD2S1210_REG_POS_L)));
    int16_t vel;
    vel=((AD2S1210_RD(AD2S1210_REG_VEL_H) << 8) | (AD2S1210_RD(AD2S1210_REG_VEL_L)));

    printf("testo: fault reg: 0x%02X, pos: 0x%04X %04d, vel: 0x%04X %04d\n", spi_data1, pos, pos, vel, vel);
    //printled(1, LRED);
    if ((poll(stdin) > 0)) {
      i=0;
      if (poll(stdin) > 0) {
	c=0;
	while (c!='\r') {
	  c=getc(stdin);
	  cmd_s[i]=c;
	  i++;
	  putc(c, stdout);
	//fflush(stdout);
	}
	cmd_s[i]='\0';
      }
      printf("%s", cmd_s);
    }
  }
}
