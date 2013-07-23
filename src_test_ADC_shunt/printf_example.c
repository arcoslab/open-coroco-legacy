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
#include <libopencm3/stm32/f4/adc.h>

#include <libopencm3/stm32/f4/rcc.h>
#include <libopencm3/stm32/f4/gpio.h>
#include <libopencm3/stm32/f4/timer.h>
#include <libopencm3/stm32/f4/nvic.h>
#include <libopencm3-plus/newlib/syscall.h>
#include <printf_example.h>
#include <libopencm3-plus/cdcacm_one_serial/cdcacm.h>
#include <stdio.h>
#include <libopencm3-plus/utils/misc.h>
#include <libopencm3-plus/stm32f4discovery/leds.h>
#include <stdbool.h>
#include <string.h>

#define ADC_CONVERSION_FACTOR 4096.0f
#define VREF 2.966f	//ADC conversion reference voltage


  float divisor_voltage=0.0f;
  float source_voltage=0.0f;

  float divisor_voltage_2=0.0f;
  float source_voltage_2=0.0f;

  float divisor_voltage_3=0.0f;
  float source_voltage_3=0.0f;


void adc_isr(void)
{ 
  static int counter=0;
    
  if (counter==0)
  {
    divisor_voltage=adc_read_regular(ADC1)*(VREF/ADC_CONVERSION_FACTOR);
    voltage_measure (ADC1,ADC_CHANNEL2);
    counter++;
  }
  else if (counter==1)
  {
    divisor_voltage_2=adc_read_regular(ADC1)*(VREF/ADC_CONVERSION_FACTOR);
    voltage_measure (ADC1,ADC_CHANNEL3);
    counter++; 
  }
  else
  {
    divisor_voltage_3=adc_read_regular(ADC1)*(VREF/ADC_CONVERSION_FACTOR);
    counter=0;
  }
}


void adc_init (void)
{
  rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_ADC1EN);
  rcc_peripheral_enable_clock(&RCC_AHB1ENR, RCC_AHB1ENR_IOPAEN);

  gpio_mode_setup(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO1);	//PA1
  gpio_mode_setup(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO2);	//PA2
  gpio_mode_setup(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO3);	//PA3

  adc_set_clk_prescale(ADC_CCR_ADCPRE_BY2);
  adc_disable_scan_mode(ADC1);
  adc_set_single_conversion_mode(ADC1);

  adc_set_sample_time(ADC1, ADC_CHANNEL1, ADC_SMPR_SMP_3CYC);
  adc_set_sample_time(ADC1, ADC_CHANNEL2, ADC_SMPR_SMP_3CYC);
  adc_set_sample_time(ADC1, ADC_CHANNEL3, ADC_SMPR_SMP_3CYC);

  adc_set_multi_mode(ADC_CCR_MULTI_INDEPENDENT);
  adc_power_on(ADC1);

  nvic_enable_irq(NVIC_ADC_IRQ);
  adc_enable_eoc_interrupt(ADC1);
  //adc_disable_eoc_interrupt(ADC1);
}

/*
//float voltage_measure_ADC1 (u8 channel)
float voltage_measure_ADC1 (uint8_t channel)
{
//	u8 channels[16];
        uint8_t channels[16];
	//uint8_t channels[] = ADC_CHANNEL0;
	float voltage;
        
        //channels=ADC_CHANNEL0; 
       
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
	//adc_set_regular_sequence(ADC1, 8, channels);
	//ADC_SQR1(adc)=channel;


       	adc_start_conversion_regular(ADC1);
	//while (!adc_eoc(ADC1));
	voltage=adc_read_regular(ADC1)*(VREF/ADC_CONVERSION_FACTOR);
	return voltage;
}
*/

void voltage_measure (uint32_t adc,uint8_t channel)
{
  uint8_t channels[16];
	            
  channels[0] = channel;
  adc_set_regular_sequence(adc, 1, channels);	
  adc_start_conversion_regular(adc);
}


void leds_init(void) 
{
  rcc_peripheral_enable_clock(&RCC_AHB1ENR, RCC_AHB1ENR_IOPDEN);
  gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO12);
  gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO13);
  gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO14);
  gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO15);
}

void system_init(void) 
{
  rcc_clock_setup_hse_3v3(&hse_8mhz_3v3[CLOCK_3V3_168MHZ]);
  leds_init();
  cdcacm_init();
  adc_init();
  printled(4, LRED);
}




//----------------------

void clean_stdin(void)
{
  while ( poll(stdin)>0) 
  {
    printf("Cleaning stdin\n");
    getc(stdin);
  }
}

void stdin_init(void)
{
  setvbuf(stdin,NULL,_IONBF,0);  // Sets stdin  in unbuffered mode (normal for usart com)
  setvbuf(stdout,NULL,_IONBF,0); // Sets stdout in unbuffered mode (normal for usart com)
  clean_stdin();
}   

bool receive_a_string(char* cmd_s)
{
  char c=0;
  int i=0;
	
  if (poll(stdin) > 0) 
  {
    while (c!='\r') 
    {
      c=getc(stdin);
      cmd_s[i]=c;
      i++;
      putc(c, stdout);
    }
    
    cmd_s[i]='\0';
    return true;
  }
  else
    return false;
    
}

#define R2		5580.0f
#define R_divisor_sense 1000.0f






int main(void)
{

  system_init();
  stdin_init();
  
  char user_input[50]="";
  char user_string[25]="";
  float user_value=0.0f;


printf("\n**********************************\n");
 

  while (1)
  {  

      voltage_measure (ADC1,ADC_CHANNEL1);

      source_voltage    = divisor_voltage  *(R_divisor_sense+R2)/R_divisor_sense;
      source_voltage_2  = divisor_voltage_2*(R_divisor_sense+R2)/R_divisor_sense;	 
      source_voltage_3  = divisor_voltage_3*(R_divisor_sense+R2)/R_divisor_sense;

      if (user_string!='f')
      {
        printf("D1: %6.2f S1: %6.2f D2: %6.2f S2: %6.2f D3: %6.2f S3: %6.2f\n",divisor_voltage,source_voltage,divisor_voltage_2,source_voltage_2,divisor_voltage_3,source_voltage_3);     
      }

      if ( receive_a_string(user_input) )
      {  
        printf("%s", user_input);
        sscanf(user_input, "%s %f",user_string, &user_value);
	
        //printf(" You typed: %s \nString: %s \n number: %f\n", user_input,user_string,user_value);
        
        if (strcmp(user_string, "f") == 0)
        { 
	
	
        }
            
      }
      
      else
      {
 
      }
    }
   
    
}
