/*
 * Copyright (C) 2016 ARCOS-Lab Universidad de Costa Rica
 * Author: Sebastian Chinchilla Gutierrez <tumacher@gmail.com>
 * Modified by: Ariel Mora Jimenez <ariel.moraj@gmail.com>
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

//usb serial cdcacm
#include <libopencm3-plus/newlib/syscall.h>
#include <libopencm3-plus/cdcacm_one_serial/cdcacm.h>
#include <libopencm3-plus/utils/misc.h>
#include <libopencm3-plus/stm32f4discovery/leds.h>

//header
#include <torque_sensing.h>

//external libraries
#include <stdio.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>

//constants
#define ADC_CONVERSION_FACTOR 4096.0f
#define VREF 2.966f	//ADC conversion reference voltage

void leds_init(void){
  rcc_peripheral_enable_clock(&RCC_AHB1ENR, RCC_AHB1ENR_IOPDEN);
  gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO12);
  gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO13);
  gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO14);
  gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO15);
}

void adc_init (void){
  rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_ADC1EN);
  rcc_peripheral_enable_clock(&RCC_AHB1ENR, RCC_AHB1ENR_IOPAEN);
  rcc_peripheral_enable_clock(&RCC_AHB1ENR, RCC_AHB1ENR_IOPCEN);

  gpio_mode_setup(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO1);	//PA1   joint_1
  gpio_mode_setup(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO2);	//PA2   joint_2
  gpio_mode_setup(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO3);	//PA3   joint_3
  gpio_mode_setup(GPIOC, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO1);	//PC1   joint_4
  gpio_mode_setup(GPIOC, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO2);	//PC2   joint_5
  gpio_mode_setup(GPIOC, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO5);	//PC5   joint_6

  adc_set_clk_prescale(ADC_CCR_ADCPRE_BY2);
  adc_disable_scan_mode(ADC1);
  adc_set_single_conversion_mode(ADC1);

  adc_set_sample_time(ADC1, ADC_CHANNEL1, ADC_SMPR_SMP_3CYC);		//joint_1
  adc_set_sample_time(ADC1, ADC_CHANNEL2, ADC_SMPR_SMP_3CYC);		//joint_2
  adc_set_sample_time(ADC1, ADC_CHANNEL3, ADC_SMPR_SMP_3CYC);		//joint_3
  adc_set_sample_time(ADC1, ADC_CHANNEL11, ADC_SMPR_SMP_3CYC);		//joint_4
  adc_set_sample_time(ADC1, ADC_CHANNEL12, ADC_SMPR_SMP_3CYC);		//joint_5
  adc_set_sample_time(ADC1, ADC_CHANNEL15, ADC_SMPR_SMP_3CYC);		//joint_6

  adc_set_multi_mode(ADC_CCR_MULTI_INDEPENDENT);
  adc_power_on(ADC1);

  /*
  nvic_enable_irq(NVIC_ADC_IRQ);
  adc_enable_eoc_interrupt(ADC1);
  adc_disable_eoc_interrupt(ADC1);
  */
}

void system_init(void){
  rcc_clock_setup_hse_3v3(&hse_8mhz_3v3[CLOCK_3V3_168MHZ]);
  leds_init();
  adc_init();
  cdcacm_init();
  printled(4, LRED);
}

float voltage_measure (uint32_t adc,uint8_t channel){
    uint8_t channels[16];
    float voltage;

    channels[0] = channel;
    adc_set_regular_sequence(adc, 1, channels);
    adc_start_conversion_regular(adc);
    gpio_toggle (GPIOD,GPIO15);
    while (!adc_eoc(adc)){
	//waiting for adc
    }
    voltage=adc_read_regular(adc)*(VREF/ADC_CONVERSION_FACTOR);
    return voltage;
}

int main(void){
    system_init();

    float voltage_joint_0 = 0.0f;
    //float voltage_joint_1 = 0.0f;
    //float voltage_joint_2 = 0.0f;

    while (1){
        voltage_joint_0	= voltage_measure (ADC1,ADC_CHANNEL1);
        //voltage_joint_1 = voltage_measure (ADC1,ADC_CHANNEL2);
        //voltage_joint_2 = voltage_measure (ADC1,ADC_CHANNEL3);

	printf("%6.2f\n", voltage_joint_0);
    }

}
