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
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/f4/nvic.h>
#include <libopencm3-plus/newlib/syscall.h>
#include <libopencm3-plus/cdcacm_one_serial/cdcacm.h>
#include <libopencm3/stm32/spi.h>

#include <stdio.h>
#include <libopencm3-plus/utils/misc.h>
#include <libopencm3-plus/stm32f4discovery/leds.h>
#include <limits.h>
#include <stdbool.h>

#include "resolver_test.h"
/*

  PortD:
  12,13,14,15: out, leds
  8: out, ad2s1210 (reset)

  PortC:
  0,1,3: out, ad2s1210 (res0, res1, sample)

  PortE:
  7,15: out, ad2s1210 (cs , rd)

  PortB:
  11,12: out, ad2s1210 (a0, a1)
  13,14,15: spi, ad2s1210( )

  PortA:
  3:out, ad2s1210 (wr)

 */



//#define AD2S1210_WRPIN GPIO2
//#define AD2S1210_WRPIN_PORT GPIOA
#define AD2S1210_WRPIN GPIO3 //For Brian test
#define AD2S1210_WRPIN_PORT GPIOA //For brian test
#define AD2S1210_SPI SPI2
//#define AD2S1210_SAMPLEPIN GPIO2
//#define AD2S1210_SAMPLEPIN_PORT GPIOC
#define AD2S1210_SAMPLEPIN GPIO3 //Brian test
#define AD2S1210_SAMPLEPIN_PORT GPIOC //Brian test
#define AD2S1210_RESETPIN GPIO8
#define AD2S1210_RESETPIN_PORT GPIOD
//#define AD2S1210_CSPIN GPIO2
//#define AD2S1210_CSPIN_PORT GPIOA
#define AD2S1210_CSPIN GPIO7 //Brian test
#define AD2S1210_CSPIN_PORT GPIOE //Brian test

#define AD2S1210_REG_FAULT 0xff
#define AD2S1210_REG_POS_H 0x80
#define AD2S1210_REG_POS_L 0x81
#define AD2S1210_REG_VEL_H 0x82
#define AD2S1210_REG_VEL_L 0x83
#define AD2S1210_REG_LOS_THRES 0x88
#define AD2S1210_REG_DOS_OVER_THRES 0x89
#define AD2S1210_REG_DOS_MIS_THRES 0x8A
#define AD2S1210_REG_LOT_HIGH_THRES 0x8D
#define AD2S1210_REG_LOT_LOW_THRES 0x8E
#define AD2S1210_REG_EXC_FREQ 0x91
#define AD2S1210_REG_SOFT_RESET 0xF0
#define AD2S1210_REG_DOS_RESET_MAX_THRES 0x8B
#define AD2S1210_REG_DOS_RESET_MIN_THRES 0x8C



#define CLEAR_WAIT 10

#define WAIT_100NANO(ns) {                      \
  for (int wait_i = 0; wait_i < 7*ns; wait_i++)    /* Wait a bit. */	\
    __asm__("nop");\
  }

#define AD2S1210_WR(reg, data) {\
  gpio_clear(AD2S1210_WRPIN_PORT, AD2S1210_WRPIN);\
  WAIT_100NANO(CLEAR_WAIT);\
  spi_write(AD2S1210_SPI, reg);			  \
  spi_read(AD2S1210_SPI); \
  WAIT_100NANO(CLEAR_WAIT);\
  gpio_set(AD2S1210_WRPIN_PORT, AD2S1210_WRPIN);\
  gpio_clear(AD2S1210_WRPIN_PORT, AD2S1210_WRPIN);\
  WAIT_100NANO(CLEAR_WAIT);\
  spi_write(AD2S1210_SPI, data);			  \
  spi_read(AD2S1210_SPI); \
  WAIT_100NANO(CLEAR_WAIT);\
  gpio_set(AD2S1210_WRPIN_PORT, AD2S1210_WRPIN);\
  }

#define AD2S1210_RD(reg) ({\
  uint8_t data;\
  gpio_clear(AD2S1210_WRPIN_PORT, AD2S1210_WRPIN);\
  WAIT_100NANO(CLEAR_WAIT);\
  spi_write(AD2S1210_SPI, reg);			  \
  spi_read(AD2S1210_SPI); \
  WAIT_100NANO(CLEAR_WAIT);\
  gpio_set(AD2S1210_WRPIN_PORT, AD2S1210_WRPIN);\
  gpio_clear(AD2S1210_WRPIN_PORT, AD2S1210_WRPIN);\
  WAIT_100NANO(CLEAR_WAIT);\
  spi_write(AD2S1210_SPI, 0x00); \
  data = spi_read(AD2S1210_SPI); \
  WAIT_100NANO(CLEAR_WAIT);\
  gpio_set(AD2S1210_WRPIN_PORT, AD2S1210_WRPIN);\
  data;\
    })



#define AD2S1210_SAMPLE() {\
  gpio_set(AD2S1210_SAMPLEPIN_PORT, AD2S1210_SAMPLEPIN);\
  gpio_clear(AD2S1210_SAMPLEPIN_PORT, AD2S1210_SAMPLEPIN);\
  WAIT_100NANO(2); \
  gpio_set(AD2S1210_SAMPLEPIN_PORT, AD2S1210_SAMPLEPIN);\
  WAIT_100NANO(5); \
  }

void spi_init(void) {
  rcc_periph_clock_enable(RCC_SPI2);
  /* For spi signal pins */
  rcc_periph_clock_enable(RCC_GPIOB);

  rcc_peripheral_enable_clock(&RCC_AHB1ENR, RCC_AHB1ENR_IOPBEN);
  rcc_peripheral_enable_clock(&RCC_AHB1ENR, RCC_SPI2);


  /* Setup GPIO pins for AF5 for SPI2 signals. */
  //PB13: clk, PB14: MISO, PB15: MOSI
  gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE,
		  GPIO13 | GPIO14 | GPIO15);
  gpio_set_af(GPIOB, GPIO_AF5, GPIO13 | GPIO14 | GPIO15);

  //spi initialization, SPI2 working at 21MHz
  spi_init_master(SPI2, SPI_CR1_BR_FPCLK_DIV_64, SPI_CR1_CPOL_CLK_TO_0_WHEN_IDLE , SPI_CR1_CPHA_CLK_TRANSITION_1, SPI_CR1_DFF_8BIT, SPI_CR1_MSBFIRST);

  //spi_set_master_mode(SPI2);
  spi_set_baudrate_prescaler(SPI2, SPI_CR1_BR_FPCLK_DIV_16); // or 32
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
  /* Start with spi communication disabled */
  gpio_clear(AD2S1210_CSPIN_PORT, AD2S1210_CSPIN);
  // Pin PC2 for sample generation
  rcc_periph_clock_enable(RCC_GPIOC);
  /* Setup GPIOC2 pin for ad2s1210 sample. */
  gpio_mode_setup(AD2S1210_SAMPLEPIN_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, AD2S1210_SAMPLEPIN);
  /* Start with sample high, data register update is done in high to low edge */
  gpio_set(AD2S1210_SAMPLEPIN_PORT, AD2S1210_SAMPLEPIN);
}

void leds_init(void) {
  rcc_peripheral_enable_clock(&RCC_AHB1ENR, RCC_AHB1ENR_IOPDEN);
  gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO12 | GPIO13 | GPIO14 | GPIO15);
}

void system_init(void) {
  rcc_clock_setup_hse_3v3(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_168MHZ]);
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

    WAIT_100NANO(50);
    AD2S1210_WR(AD2S1210_REG_SOFT_RESET, 0x00);
    //configure
    AD2S1210_WR(AD2S1210_REG_EXC_FREQ, 0x12);

    /* gpio_clear(GPIOA, GPIO2); */
    /* WAIT_100NANO(CLEAR_WAIT); */
    /* spi_write(SPI2, 0x91); */
    /* spi_data1=spi_read(SPI2); */
    /* WAIT_100NANO(CLEAR_WAIT); */
    /* gpio_set(GPIOA, GPIO2); */
    /* //gpio_clear(GPIOA, GPIO2); */
    /* //gpio_set(GPIOA, GPIO2); */
    /* gpio_clear(GPIOA, GPIO2); */
    /* WAIT_100NANO(CLEAR_WAIT); */
    /* //if (counter>20) { */
    /* //  spi_write(SPI2, 0x28); // 10KHz */
    /* //} else { */
    /*   spi_write(SPI2, 0x12); // 4.5KHz */
      //}
    /* if (counter>40) { */
    /*   counter=0; */
    /* } */
    /* spi_data2=spi_read(SPI2); */
    /* WAIT_100NANO(CLEAR_WAIT); */
    /* gpio_set(GPIOA, GPIO2); */
    //gpio_clear(GPIOA, GPIO2);
    //gpio_set(GPIOA, GPIO2);

    //AD2S1210_WR(AD2S1210_REG_DOS_OVER_THRES, 4800/38);
    //AD2S1210_WR(AD2S1210_REG_DOS_MIS_THRES, 880/38);
    //AD2S1210_WR(AD2S1210_REG_DOS_RESET_MIN_THRES, 4390/38);
    //AD2S1210_WR(AD2S1210_REG_DOS_RESET_MAX_THRES, 80/38);
    //AD2S1210_WR(AD2S1210_REG_LOS_THRES, 1200/38);
    //AD2S1210_WR(AD2S1210_REG_LOT_HIGH_THRES, 100);
    //AD2S1210_WR(AD2S1210_REG_LOT_LOW_THRES, 50);
    spi_data1=AD2S1210_RD(AD2S1210_REG_FAULT);
    AD2S1210_SAMPLE();
    uint16_t pos;
    pos=((AD2S1210_RD(AD2S1210_REG_POS_H) << 8) | (AD2S1210_RD(AD2S1210_REG_POS_L)));
    int16_t vel;
    vel=((AD2S1210_RD(AD2S1210_REG_VEL_H) << 8) | (AD2S1210_RD(AD2S1210_REG_VEL_L)));

    WAIT_100NANO(50);
    AD2S1210_SAMPLE();
    spi_data2=AD2S1210_RD(AD2S1210_REG_FAULT);
    WAIT_100NANO(50);
    AD2S1210_SAMPLE();


  while (1){
    counter+=1;
    WAIT_100NANO(50);
    //fault register
    //AD2S1210_SAMPLE();
    spi_data1=AD2S1210_RD(AD2S1210_REG_FAULT);
    WAIT_100NANO(5);
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
    /* AD2S1210_SAMPLE(); */
    /* AD2S1210_SAMPLE(); */

    if (spi_data1>0) {
    AD2S1210_SAMPLE();
    spi_data2=AD2S1210_RD(AD2S1210_REG_FAULT);
    WAIT_100NANO(5);
    AD2S1210_SAMPLE();
    } else { spi_data2=0xFF;}
    
    AD2S1210_SAMPLE();

    pos=((AD2S1210_RD(AD2S1210_REG_POS_H) << 8) | (AD2S1210_RD(AD2S1210_REG_POS_L)));
    vel=((AD2S1210_RD(AD2S1210_REG_VEL_H) << 8) | (AD2S1210_RD(AD2S1210_REG_VEL_L)));

    printf("testo: fault reg: 0x%02X, 0x%02X, pos: 0x%04X %04d, vel: 0x%04X %04d\n", spi_data1, spi_data2, pos, pos, vel, vel);
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
