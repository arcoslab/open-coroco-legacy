/*
 *This file is part of the open-coroco project.
 *
 *  Copyright (C) 2013  Sebastian Chinchilla Gutierrez <tumacher@gmail.com>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

void TIM4_setup(void)
{
	/* Enable TIM4 clock. */
	rcc_peripheral_enable_clock(&RCC_APB1ENR, RCC_APB1ENR_TIM4EN);  //F4 clock for TIM 4 is APB1. TIM4 port is E
	
	/*Enable AF clock*/
	rcc_peripheral_enable_clock(&RCC_AHB1ENR, RCC_AHB1ENR_IOPEEN);

	/*
	 * Set TIM4 channel input pins to
	 *  'input open-drain'. */
	gpio_mode_setup(GPIOD, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO12 | GPIO13 | GPIO14);		

	
	/*TIM4 CH1, CH2 and CH3 as alternate functions*/
	gpio_set_af(GPIOD, GPIO_AF2, GPIO12 | GPIO13 | GPIO14);	//f4 AF2 means TIM4/TIM4.


	/* Enable TIM4 commutation interrupt. */
	nvic_enable_irq(NVIC_TIM4_IRQ);	//f4



	/* Reset TIM4 peripheral. */
	timer_reset(TIM4);

	/* Timer global mode:
	 * - No divider
	 * - Alignment edge
	 * - Direction up
	 */
	timer_set_mode(TIM4, TIM_CR1_CKD_CK_INT,
		       TIM_CR1_CMS_CENTER_3,	//TIM_CR1_CMS_EDGE 
						//TIM_CR1_CMS_CENTER_1 
						//TIM_CR1_CMS_CENTER_2 
						//TIM_CR1_CMS_CENTER_3 la frequencia del pwm se divide a la mitad.
			 TIM_CR1_DIR_UP);

	/* Reset prescaler value. */
	timer_set_prescaler(TIM4, prescale);

	/* Reset repetition counter value. */
	timer_set_repetition_counter(TIM4, repetition_counter_value);

	/* Enable preload. */
	timer_enable_preload(TIM4);

	/* Continuous mode. */
	timer_continuous_mode(TIM4);

	/* Period (32kHz). */
	timer_set_period(TIM4,pwm_period_ARR);


	/* Configure break and deadtime. */
	timer_set_deadtime(TIM4, deadtime_percentage*pwm_period_ARR);
	timer_set_enabled_off_state_in_idle_mode(TIM4);
	timer_set_enabled_off_state_in_run_mode(TIM4);
	timer_disable_break(TIM4);
	timer_set_break_polarity_high(TIM4);
	timer_disable_break_automatic_output(TIM4);
	timer_set_break_lock(TIM4, TIM_BDTR_LOCK_OFF);

	/* -- OC1 and OC1N configuration -- */

	/* Disable outputs. */
	timer_disable_oc_output(TIM4, TIM_OC1);
	timer_disable_oc_output(TIM4, TIM_OC1N);

	/* Configure global mode of line 1. */
	timer_disable_oc_clear(TIM4, TIM_OC1);
	timer_enable_oc_preload(TIM4, TIM_OC1);
	timer_set_oc_slow_mode(TIM4, TIM_OC1);
	timer_set_oc_mode(TIM4, TIM_OC1, TIM_OCM_PWM1);

	/* Configure OC1. */
	timer_set_oc_polarity_high(TIM4, TIM_OC1);
	timer_set_oc_idle_state_set(TIM4, TIM_OC1);

	/* Configure OC1N. */
	timer_set_oc_polarity_high(TIM4, TIM_OC1N);
	timer_set_oc_idle_state_set(TIM4, TIM_OC1N);

	/* Set the capture compare value for OC1. */
	timer_set_oc_value(TIM4, TIM_OC1, 1000);//initial_duty_cycle*pwm_period_ARR);

	/* Reenable outputs. */
	timer_enable_oc_output(TIM4, TIM_OC1);
	timer_enable_oc_output(TIM4, TIM_OC1N);

	/* -- OC2 and OC2N configuration -- */

	/* Disable outputs. */
	timer_disable_oc_output(TIM4, TIM_OC2);
	timer_disable_oc_output(TIM4, TIM_OC2N);

	/* Configure global mode of line 2. */
	timer_disable_oc_clear(TIM4, TIM_OC2);
	timer_enable_oc_preload(TIM4, TIM_OC2);
	timer_set_oc_slow_mode(TIM4, TIM_OC2);
	timer_set_oc_mode(TIM4, TIM_OC2, TIM_OCM_PWM1);

	/* Configure OC2. */
	timer_set_oc_polarity_high(TIM4, TIM_OC2);
	timer_set_oc_idle_state_set(TIM4, TIM_OC2);

	/* Configure OC2N. */
	timer_set_oc_polarity_high(TIM4, TIM_OC2N);
	timer_set_oc_idle_state_set(TIM4, TIM_OC2N);

	/* Set the capture compare value for OC2. */
	timer_set_oc_value(TIM4, TIM_OC2, 1000);//initial_duty_cycle*pwm_period_ARR);

	/* Reenable outputs. */
	timer_enable_oc_output(TIM4, TIM_OC2);
	timer_enable_oc_output(TIM4, TIM_OC2N);

	/* -- OC3 and OC3N configuration -- */

	/* Disable outputs. */
	timer_disable_oc_output(TIM4, TIM_OC3);
	timer_disable_oc_output(TIM4, TIM_OC3N);

	/* Configure global mode of line 3. */
	timer_disable_oc_clear(TIM4, TIM_OC3);
	timer_enable_oc_preload(TIM4, TIM_OC3);
	timer_set_oc_slow_mode(TIM4, TIM_OC3);
	timer_set_oc_mode(TIM4, TIM_OC3, TIM_OCM_PWM1);

	/* Configure OC3. */
	timer_set_oc_polarity_high(TIM4, TIM_OC3);
	timer_set_oc_idle_state_set(TIM4, TIM_OC3);

	/* Configure OC3N. */
	timer_set_oc_polarity_high(TIM4, TIM_OC3N);
	timer_set_oc_idle_state_set(TIM4, TIM_OC3N);

	/* Set the capture compare value for OC3. */
	timer_set_oc_value(TIM4, TIM_OC3, 1000);//initial_duty_cycle*pwm_period_ARR);//100);

	/* Reenable outputs. */
	timer_enable_oc_output(TIM4, TIM_OC3);
	timer_enable_oc_output(TIM4, TIM_OC3N);

	/* ---- */

	/* ARR reload enable. */
	timer_enable_preload(TIM4);

	/*
	 * Enable preload of complementary channel configurations and
	 * update on COM event.
	 */
	//timer_enable_preload_complementry_enable_bits(TIM4);
	timer_disable_preload_complementry_enable_bits(TIM4);

	/* Enable outputs in the break subsystem. */
	timer_enable_break_main_output(TIM4);

	/* Counter enable. */
	timer_enable_counter(TIM4);

	/* Enable commutation interrupt. */
	timer_enable_irq(TIM4, TIM_DIER_COMIE);



	



	/*********/
	/*Capture compare interrupt*/
	
	//enable capture compare interrupt 
	//timer_enable_update_event(TIM4);

	/* Enable commutation interrupt. */
	//timer_enable_irq(TIM4, TIM_DIER_CC1IE);	//Capture/compare 1 interrupt enable
	/* Enable commutation interrupt. */
	//timer_enable_irq(TIM4, TIM_DIER_CC1IE);
	//nvic_enable_irq(NVIC_TIM4_CC_IRQ);

	timer_set_ti1_ch123_xor(TIM4);

}

