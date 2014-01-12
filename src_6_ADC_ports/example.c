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

//stm32f4 
#include <libopencm3/stm32/f4/rcc.h>
#include <libopencm3/stm32/f4/gpio.h>
#include <libopencm3/stm32/f4/adc.h>


#include <example.h>

//external libraries
#include <stdio.h>
#include <limits.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

#define ADC_CONVERSION_FACTOR 4096.0f
#define VREF 2.966f	//ADC conversion reference voltage

void leds_init(void) 
{
  rcc_peripheral_enable_clock(&RCC_AHB1ENR, RCC_AHB1ENR_IOPDEN);
  gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO12);
  gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO13);
  gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO14);
  gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO15);
}

void adc_init (void)
{
  rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_ADC1EN);
  rcc_peripheral_enable_clock(&RCC_AHB1ENR, RCC_AHB1ENR_IOPAEN);
  rcc_peripheral_enable_clock(&RCC_AHB1ENR, RCC_AHB1ENR_IOPCEN);

  gpio_mode_setup(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO1);	//PA1
  gpio_mode_setup(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO2);	//PA2
  gpio_mode_setup(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO3);	//PA3
  gpio_mode_setup(GPIOC, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO1);	//PC1
  gpio_mode_setup(GPIOC, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO2);	//PC2

  adc_set_clk_prescale(ADC_CCR_ADCPRE_BY2);
  adc_disable_scan_mode(ADC1);
  adc_set_single_conversion_mode(ADC1);

  adc_set_sample_time(ADC1, ADC_CHANNEL1, ADC_SMPR_SMP_3CYC);
  adc_set_sample_time(ADC1, ADC_CHANNEL2, ADC_SMPR_SMP_3CYC);
  adc_set_sample_time(ADC1, ADC_CHANNEL3, ADC_SMPR_SMP_3CYC);
  adc_set_sample_time(ADC1, ADC_CHANNEL11, ADC_SMPR_SMP_3CYC);
  adc_set_sample_time(ADC1, ADC_CHANNEL12, ADC_SMPR_SMP_3CYC);

  adc_set_multi_mode(ADC_CCR_MULTI_INDEPENDENT);
  adc_power_on(ADC1);

  //nvic_enable_irq(NVIC_ADC_IRQ);
  //adc_enable_eoc_interrupt(ADC1);
  //adc_disable_eoc_interrupt(ADC1);
}

void system_init(void) 
{
  rcc_clock_setup_hse_3v3(&hse_8mhz_3v3[CLOCK_3V3_168MHZ]);
  leds_init();
  adc_init();

}


float voltage_measure_ADC1 (uint8_t channel)
{
	uint8_t channels[16];
	float hall_voltage;

	channels[0] = channel;
	channels[1] = channel;
	channels[2] = channel;
	channels[3] = channel;
	channels[4] = channel;
	channels[5] = channel;
	channels[6] = channel;
	channels[7] = channel;
	channels[8] = channel;
	channels[9] = channel;
	channels[10] = channel;
	channels[11] = channel;
	channels[12] = channel;
	channels[13] = channel;
	channels[14] = channel;
	channels[15] = channel;

	adc_set_regular_sequence(ADC1, 1, channels);
	//ADC_SQR1(adc)=channel;


       	adc_start_conversion_regular(ADC1);
	while (!adc_eoc(ADC1));
	hall_voltage=adc_read_regular(ADC1)*(VREF/ADC_CONVERSION_FACTOR);
	return hall_voltage;
}




int main(void)
{
  system_init();
   
  /*printf ("\n\n****************************************************************************************************************\n");
  printf ("\n\n*************************************ADC 6 ports example********************************************************\n");
  printf ("\n\n****************************************************************************************************************\n\n");
   */
  float voltage=0.0f;
/*
gpio_set(GPIOD, GPIO12);
gpio_set(GPIOD, GPIO13);
gpio_set(GPIOD, GPIO14);
gpio_set(GPIOD, GPIO15);
*/

  while (1)
  {
/*
        voltage	=voltage_measure_ADC1 (ADC_CHANNEL1);
        if (voltage >2.0f )
            gpio_set(GPIOD, GPIO12);    
        else 
            gpio_clear(GPIOD, GPIO12);    
        
        voltage	=voltage_measure_ADC1 (ADC_CHANNEL2);
        if (voltage >2.0f )
            gpio_set(GPIOD, GPIO13);    
        else 
            gpio_clear(GPIOD, GPIO13);    
        
        voltage	=voltage_measure_ADC1 (ADC_CHANNEL3);
        if (voltage >2.0f )
            gpio_set(GPIOD, GPIO14);    
        else 
            gpio_clear(GPIOD, GPIO14);    
*/

        voltage	=voltage_measure_ADC1 (ADC_CHANNEL11);
        if (voltage >2.0f )
            gpio_set(GPIOD, GPIO12);    
        else 
            gpio_clear(GPIOD, GPIO12);    
        
        voltage	=voltage_measure_ADC1 (ADC_CHANNEL12);
        if (voltage >2.0f )
            gpio_set(GPIOD, GPIO13);    
        else 
            gpio_clear(GPIOD, GPIO13);    
/*        
        voltage	=voltage_measure_ADC1 (ADC_CHANNEL3);
        if (voltage >2.0f )
            gpio_set(GPIOD, GPIO14);    
        else 
            gpio_clear(GPIOD, GPIO14);            
*/

  }


}
