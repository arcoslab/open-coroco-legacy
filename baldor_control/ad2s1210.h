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

void ad2s1210_init(void);
void ad2s1210_conf(void);

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
#define AD2S1210_CSPIN GPIO8 //Brian test
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



/* #define AD2S1210_WR(reg, data) {\ */
/*   gpio_clear(AD2S1210_WRPIN_PORT, AD2S1210_WRPIN);\ */
/*   spi_write(AD2S1210_SPI, reg);			  \ */
/*   spi_read(AD2S1210_SPI); \ */
/*   gpio_set(AD2S1210_WRPIN_PORT, AD2S1210_WRPIN);\ */
/*   gpio_clear(AD2S1210_WRPIN_PORT, AD2S1210_WRPIN);\ */
/*   spi_write(AD2S1210_SPI, data);			  \ */
/*   spi_read(AD2S1210_SPI); \ */
/*   gpio_set(AD2S1210_WRPIN_PORT, AD2S1210_WRPIN);\ */
/*   } */

/* #define AD2S1210_RD(reg) ({\ */
/*   uint8_t data;\ */
/*   gpio_clear(AD2S1210_WRPIN_PORT, AD2S1210_WRPIN);\ */
/*   spi_write(AD2S1210_SPI, reg);			  \ */
/*   spi_read(AD2S1210_SPI); \ */
/*   gpio_set(AD2S1210_WRPIN_PORT, AD2S1210_WRPIN);\ */
/*   gpio_clear(AD2S1210_WRPIN_PORT, AD2S1210_WRPIN);\ */
/*   spi_write(AD2S1210_SPI, 0x00); \ */
/*   data = spi_read(AD2S1210_SPI); \ */
/*   gpio_set(AD2S1210_WRPIN_PORT, AD2S1210_WRPIN);\ */
/*   data;\ */
/*     }) */

/* /\* #define AD2S1210_SAMPLE() {\ *\/ */
/* /\*   gpio_set(AD2S1210_SAMPLEPIN_PORT, AD2S1210_SAMPLEPIN);\ *\/ */
/* /\*   gpio_clear(AD2S1210_SAMPLEPIN_PORT, AD2S1210_SAMPLEPIN);\ *\/ */
/* /\*   WAIT_100NANO(2); \ *\/ */
/* /\*   gpio_set(AD2S1210_SAMPLEPIN_PORT, AD2S1210_SAMPLEPIN);\ *\/ */
/* /\*   WAIT_100NANO(5); \ *\/ */
/* /\*   } *\/ */

/* #define AD2S1210_SAMPLE() {\ */
/*   gpio_set(AD2S1210_SAMPLEPIN_PORT, AD2S1210_SAMPLEPIN);\ */
/*   gpio_clear(AD2S1210_SAMPLEPIN_PORT, AD2S1210_SAMPLEPIN);\ */
/*   WAIT_100NANO(4); \ */
/*   gpio_set(AD2S1210_SAMPLEPIN_PORT, AD2S1210_SAMPLEPIN);\ */
/*   WAIT_100NANO(8); \ */
/*   } */

