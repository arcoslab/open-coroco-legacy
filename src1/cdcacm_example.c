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
#include "cdcacm_example.h"
#include "cdcacm/cdcacm.h"
#include <stdio.h>
#include "utils/mutex.h"
#include "utils/misc.h"

void leds_init(void) {
  rcc_peripheral_enable_clock(&RCC_AHB1ENR, RCC_AHB1ENR_IOPDEN);
  gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO12);
  gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO13);
  gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO14);
  gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO15);
}

void system_init(void) {
  rcc_clock_setup_hse_3v3(&hse_8mhz_3v3[CLOCK_3V3_168MHZ]);
  leds_init();
  cdcacm_init();
}

int main(void)
{
  system_init();
  char buf[50]="hola_orig";
  int i, j;
  int c;
  int n_char=0;
  setvbuf(stdin,NULL,_IONBF,0); // Sets stdin in unbuffered mode (normal for usart com)
  //printled(3, LRED);
  while (1){
    //printled(1, LRED);
    /*
    //For reading until enter:
    n_char=scanf("%[^\r]", buf); // minicom generates \r for enters
    c=getc(stdin); //To remove the \r from the buffer. If not,
                   //the next time scanf is called will always
                   //read \r not print it and printing nothing as a result. Getting "stuck"
    */

    //For reading until white-spaces and non-words:
    n_char=scanf("%s", buf);

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
    printf("%s %d\n", buf, n_char);
    printf("Hola  mundo\n");
  }
}
