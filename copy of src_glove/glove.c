/*
 * Copyright (C) 2013 ARCOS-Lab Universidad de Costa Rica
 * Author: Sebastian Chinchilla Gutierrez <tumacher@gmail.com>
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
//#include <libopencm3/stm32/f4/adc.h>
#include <libopencm3/stm32/f4/timer.h>
#include <libopencm3/stm32/f4/nvic.h>

/**********Serial port********************/
//usb serial cdcacm
#include <libopencm3-plus/newlib/syscall.h>
//#include "example.h"
#include <libopencm3-plus/cdcacm_one_serial/cdcacm.h>
#include <libopencm3-plus/utils/misc.h>
#include <libopencm3-plus/stm32f4discovery/leds.h>
/*******************************************/

//header
#include <glove.h>

//external libraries
#include <stdio.h>
//#include <limits.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>



//pwm-related timer configuration
#define SYSFREQ                              168000000 //168MHz
#define PWMFREQ                                  2000 //58000 //32000
#define PWMFREQ_F                  ((float )(PWMFREQ)) //32000.0f
#define PWMFREQ_CENTER_ALIGNED_F   (PWMFREQ_F/2.0f)            


#define PRESCALE                                                   1 //freq_CK_CNT=freq_CK_PSC/(PSC[15:0]+1)
#define PWM_PERIOD_ARR              SYSFREQ/( PWMFREQ*(PRESCALE+1) )
#define INIT_DUTY                                               0.5f


/*
#define ADC_CONVERSION_FACTOR 4096.0f
#define VREF 2.966f	//ADC conversion reference voltage

#define CONVERSION_FACTOR_JOINT_0 1.0f
#define CONVERSION_FACTOR_JOINT_1 1.0f
#define CONVERSION_FACTOR_JOINT_2 1.0f
*/
void tim1_up_tim10_isr(void) 
{

  //Clear the update interrupt flag
  timer_clear_flag(TIM1,  TIM_SR_UIF);

  static int counter=0;
/*
  if (counter==0)
  {
      gpio_set(GPIOD,GPIO12);
       counter+=1;
  }
  else 
  {
      gpio_clear(GPIOD,GPIO12);
      counter=0;
  }
*/

        int button=0;

        button=gpio_get(GPIOA,GPIO0);




    counter+=1;
    if (counter>=1000)
    {
    gpio_toggle(GPIOD,GPIO12);
    counter=0;

    }

/*
        if (button)
        {
        gpio_toggle(GPIOD,GPIO12);
        gpio_toggle(GPIOD,GPIO13);
        gpio_toggle(GPIOD,GPIO14);
        gpio_toggle(GPIOD,GPIO15);
        }
*/
/*
        if (button)
        {
        gpio_set(GPIOD,GPIO12);
        gpio_set(GPIOD,GPIO13);
        gpio_set(GPIOD,GPIO14);
        gpio_set(GPIOD,GPIO15);
        }

        else
        {
        gpio_clear(GPIOD,GPIO12);
        gpio_clear(GPIOD,GPIO13);
        gpio_clear(GPIOD,GPIO14);
        gpio_clear(GPIOD,GPIO15);
        }
*/
}

void DTC_SVM_tim_init(void)
{
	/* Enable TIM1 clock. and Port E clock (for outputs) */
	rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_TIM1EN);
	rcc_peripheral_enable_clock(&RCC_AHB1ENR, RCC_AHB1ENR_IOPEEN);

	//Set TIM1 channel (and complementary) output to alternate function push-pull'.
	//f4 TIM1=> GIO9: CH1, GPIO11: CH2, GPIO13: CH3
	//f4 TIM1=> GIO8: CH1N, GPIO10: CH2N, GPIO12: CH3N
	gpio_mode_setup(GPIOE, GPIO_MODE_AF,GPIO_PUPD_NONE,GPIO9 | GPIO11 | GPIO13);
	gpio_set_af(GPIOE, GPIO_AF1, GPIO9 | GPIO11 | GPIO13);
	gpio_mode_setup(GPIOE, GPIO_MODE_AF,GPIO_PUPD_NONE,GPIO8 | GPIO10 | GPIO12);
	gpio_set_af(GPIOE, GPIO_AF1, GPIO8 | GPIO10 | GPIO12);

	/* Enable TIM1 commutation interrupt. */
	//nvic_enable_irq(NVIC_TIM1_TRG_COM_TIM11_IRQ);	//f4

	/* Reset TIM1 peripheral. */
	timer_reset(TIM1);

	/* Timer global mode:
	 * - No divider
	 * - Alignment edge
	 * - Direction up
	 */
	timer_set_mode(TIM1, TIM_CR1_CKD_CK_INT, //For dead time and filter sampling, not important for now.
		       TIM_CR1_CMS_EDGE,//TIM_CR1_CMS_EDGE,//TIM_CR1_CMS_CENTER_3,	//
						//TIM_CR1_CMS_CENTER_1
						//TIM_CR1_CMS_CENTER_2
						//TIM_CR1_CMS_CENTER_3 la frequencia del pwm se divide a la mitad. (frecuencia senoidal)
			 TIM_CR1_DIR_UP);

	timer_set_prescaler(TIM1, PRESCALE); //1 = disabled (max speed)
	timer_set_repetition_counter(TIM1, 0); //disabled
	timer_enable_preload(TIM1);
	timer_continuous_mode(TIM1);

	/* Period (32kHz). */
	timer_set_period(TIM1, PWM_PERIOD_ARR); //ARR (value compared against main counter to reload counter aka: period of counter)

	/* Configure break and deadtime. */
	//timer_set_deadtime(TIM1, deadtime_percentage*pwm_period_ARR);
    timer_set_deadtime(TIM1, 0.25f*PWM_PERIOD_ARR);
    //timer_set_deadtime(TIM1, 500);
    //timer_set_deadtime(TIM1, DEAD_TIME_PERCENTAGE*PWM_PERIOD_ARR);
	timer_set_enabled_off_state_in_idle_mode(TIM1);
	timer_set_enabled_off_state_in_run_mode(TIM1);
	timer_disable_break(TIM1);
    
	timer_set_break_polarity_high(TIM1);
	timer_disable_break_automatic_output(TIM1);
    
	timer_set_break_lock(TIM1, TIM_BDTR_LOCK_OFF);

	/* Disable outputs. */
	timer_disable_oc_output(TIM1, TIM_OC1);
	timer_disable_oc_output(TIM1, TIM_OC1N);
	timer_disable_oc_output(TIM1, TIM_OC2);
	timer_disable_oc_output(TIM1, TIM_OC2N);
	timer_disable_oc_output(TIM1, TIM_OC3);
	timer_disable_oc_output(TIM1, TIM_OC3N);

	/* -- OC1 and OC1N configuration -- */
	/* Configure global mode of line 1. */
	timer_enable_oc_preload(TIM1, TIM_OC1);
	timer_set_oc_mode(TIM1, TIM_OC1, TIM_OCM_PWM1);
	/* Configure OC1. */
	timer_set_oc_polarity_high(TIM1, TIM_OC1);
	timer_set_oc_idle_state_unset(TIM1, TIM_OC1); //When idle (braked) put 0 on output
	/* Configure OC1N. */
	timer_set_oc_polarity_high(TIM1, TIM_OC1N);
	timer_set_oc_idle_state_unset(TIM1, TIM_OC1N);
	/* Set the capture compare value for OC1. */
	timer_set_oc_value(TIM1, TIM_OC1, INIT_DUTY*PWM_PERIOD_ARR);//initial_duty_cycle*pwm_period_ARR);

	/* -- OC2 and OC2N configuration -- */
	/* Configure global mode of line 2. */
	timer_enable_oc_preload(TIM1, TIM_OC2);
	timer_set_oc_mode(TIM1, TIM_OC2, TIM_OCM_PWM1);
	/* Configure OC2. */
	timer_set_oc_polarity_high(TIM1, TIM_OC2);
	timer_set_oc_idle_state_unset(TIM1, TIM_OC2);
	/* Configure OC2N. */
	timer_set_oc_polarity_high(TIM1, TIM_OC2N);
	timer_set_oc_idle_state_unset(TIM1, TIM_OC2N);
	/* Set the capture compare value for OC2. */
	timer_set_oc_value(TIM1, TIM_OC2, INIT_DUTY*PWM_PERIOD_ARR);//initial_duty_cycle*pwm_period_ARR);

	/* -- OC3 and OC3N configuration -- */
	/* Configure global mode of line 3. */
	timer_enable_oc_preload(TIM1, TIM_OC3);
	timer_set_oc_mode(TIM1, TIM_OC3, TIM_OCM_PWM1);
	/* Configure OC3. */
	timer_set_oc_polarity_high(TIM1, TIM_OC3);
	timer_set_oc_idle_state_unset(TIM1, TIM_OC3);
	/* Configure OC3N. */
	timer_set_oc_polarity_high(TIM1, TIM_OC3N);
	timer_set_oc_idle_state_unset(TIM1, TIM_OC3N);
	/* Set the capture compare value for OC3. */
	timer_set_oc_value(TIM1, TIM_OC3, INIT_DUTY*PWM_PERIOD_ARR);//initial_duty_cycle*pwm_period_ARR);//100);

	/* Reenable outputs. */
	timer_enable_oc_output(TIM1, TIM_OC1);
	timer_enable_oc_output(TIM1, TIM_OC1N);
	timer_enable_oc_output(TIM1, TIM_OC2);
	timer_enable_oc_output(TIM1, TIM_OC2N);
	timer_enable_oc_output(TIM1, TIM_OC3);
	timer_enable_oc_output(TIM1, TIM_OC3N);

	/* ---- */

	/* ARR reload enable. */
	timer_enable_preload(TIM1);

	/*
	 * Enable preload of complementary channel configurations and
	 * update on COM event.
	 */
	//timer_enable_preload_complementry_enable_bits(TIM1);
	timer_disable_preload_complementry_enable_bits(TIM1);

	/* Enable outputs in the break subsystem. */
	timer_enable_break_main_output(TIM1);

	/* Generate update event to reload all registers before starting*/
	timer_generate_event(TIM1, TIM_EGR_UG);

	/* Counter enable. */
	timer_enable_counter(TIM1);

	/* Enable commutation interrupt. */
	//timer_enable_irq(TIM1, TIM_DIER_COMIE);

	/*********/
	/*Capture compare interrupt*/

	//enable capture compare interrupt
	timer_enable_update_event(TIM1);

	/* Enable commutation interrupt. */
	//timer_enable_irq(TIM1, TIM_DIER_CC1IE);	//Capture/compare 1 interrupt enable
	/* Enable commutation interrupt. */
	//timer_enable_irq(TIM1, TIM_DIER_CC1IE);
	timer_enable_irq(TIM1, TIM_DIER_UIE);
	nvic_enable_irq(NVIC_TIM1_UP_TIM10_IRQ);
}


void leds_init(void) 
{
  rcc_peripheral_enable_clock(&RCC_AHB1ENR, RCC_AHB1ENR_IOPDEN);
  gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO12);
  gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO13);
  gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO14);
  gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO15);
}

void button_init(void)
{

  rcc_peripheral_enable_clock(&RCC_AHB1ENR, RCC_AHB1ENR_IOPAEN);
  gpio_mode_setup(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO0);
  gpio_set_af(GPIOA, GPIO_AF0, GPIO0);

}
/*
void adc_init (void)
{
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

  adc_set_sample_time(ADC1, ADC_CHANNEL1, ADC_SMPR_SMP_3CYC);   //joint_1
  adc_set_sample_time(ADC1, ADC_CHANNEL2, ADC_SMPR_SMP_3CYC);   //joint_2
  adc_set_sample_time(ADC1, ADC_CHANNEL3, ADC_SMPR_SMP_3CYC);   //joint_3
  adc_set_sample_time(ADC1, ADC_CHANNEL11, ADC_SMPR_SMP_3CYC);  //joint_4
  adc_set_sample_time(ADC1, ADC_CHANNEL12, ADC_SMPR_SMP_3CYC);  //joint_5
  adc_set_sample_time(ADC1, ADC_CHANNEL15, ADC_SMPR_SMP_3CYC);  //joint_6

  adc_set_multi_mode(ADC_CCR_MULTI_INDEPENDENT);
  adc_power_on(ADC1);

  //nvic_enable_irq(NVIC_ADC_IRQ);
  //adc_enable_eoc_interrupt(ADC1);
  //adc_disable_eoc_interrupt(ADC1);
}
*/
void system_init(void) 
{
  rcc_clock_setup_hse_3v3(&hse_8mhz_3v3[CLOCK_3V3_168MHZ]);
  leds_init();
  button_init();
  DTC_SVM_tim_init();
  //adc_init();
  //cdcacm_init();
  //printled(4, LRED);

}

/*
float voltage_measure (uint32_t adc,uint8_t channel)
{
    uint8_t channels[16];
    float voltage;
            
    channels[0] = channel;
    adc_set_regular_sequence(adc, 1, channels);	
    adc_start_conversion_regular(adc);
    gpio_toggle (GPIOD,GPIO15);

    while (!adc_eoc(adc));
    voltage=adc_read_regular(adc)*(VREF/ADC_CONVERSION_FACTOR);
    return voltage;
}
*/



int main(void)
{
    system_init();
/*
    float voltage_joint_0 = 0.0f;      
    float voltage_joint_1 = 0.0f;
    float voltage_joint_2 = 0.0f;

    float joint_0_angle = 0.0f;
    float joint_1_angle = 0.0f;
    float joint_2_angle = 0.0f;   
*/
    while (1)
    {


        //función(grupo_funcional,pin/registro);


/*
        voltage_joint_0	= voltage_measure (ADC1,ADC_CHANNEL1);      
        voltage_joint_1	= voltage_measure (ADC1,ADC_CHANNEL2);
        voltage_joint_2 = voltage_measure (ADC1,ADC_CHANNEL3);

        joint_0_angle = CONVERSION_FACTOR_JOINT_0*voltage_joint_0;
        joint_1_angle = CONVERSION_FACTOR_JOINT_1*voltage_joint_1;
        joint_2_angle = CONVERSION_FACTOR_JOINT_2*voltage_joint_2;   

        printf("1 %6.2f %6.2f %6.2f \n",joint_0_angle,joint_1_angle,joint_2_angle);    
*/ 
   }


}
