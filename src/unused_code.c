void exti0_isr(void)
{
	exti_reset_request(EXTI0);

	button=1;
	
/*
	if (exti_direction == FALLING) {
		 //gpio_toggle(GPIOD, GPIO13);
		exti_direction = RISING;
		exti_set_trigger(EXTI0, EXTI_TRIGGER_RISING);
	} else {
		 //gpio_toggle(GPIOD, GPIO13);
		
		exti_direction = FALLING;
		exti_set_trigger(EXTI0, EXTI_TRIGGER_FALLING);
		//timer_generate_event(TIM1, TIM_EGR_COMG);


		//capture compare event
		//timer_generate_event(TIM1, TIM_EGR_UG);

	}*/
}


void tim1_trg_com_tim11_isr(void)
{
	static int step = 0;

	/* Clear the COM trigger interrupt flag. */
	timer_clear_flag(TIM1, TIM_SR_COMIF);

	/*
	 * A simplified and inefficient implementation of PWM On
	 * scheme. Look at the implementation in Open-BLDC on
	 * http://open-bldc.org for the proper implementation. This
	 * one only serves as an example.
	 *
	 * Table of the PWM scheme zone configurations when driving:
	 * @verbatim
	 *  | 1| 2| 3| 4| 5| 6|
	 * -+--+--+--+--+--+--+
	 * A|p+|++|  |p-|--|  |
	 * -+--+--+--+--+--+--+
	 * B|  |p-|--|  |p+|++|
	 * -+--+--+--+--+--+--+
	 * C|--|  |p+|++|  |p-|
	 * -+--+--+--+--+--+--+
	 *  |  |  |  |  |  |  '- 360 Deg
	 *  |  |  |  |  |  '---- 300 Deg
	 *  |  |  |  |  '------- 240 Deg
	 *  |  |  |  '---------- 180 Deg
	 *  |  |  '------------- 120 Deg
	 *  |  '----------------  60 Deg
	 *  '-------------------   0 Deg
	 *
	 * Legend:
	 * p+: PWM on the high side
	 * p-: PWM on the low side
	 * --: Low side on
	 * ++: High side on
	 *   : Floating/NC
	 * @endverbatim
	 */

	//gpio_port_write(GPIOD, GPIO13);
	//GPIOD_BSRR = GPIO13;		/* LED on */
	gpio_port_write(GPIOD, GPIO13);
	switch (step) {
	case 0: /* A PWM HIGH, B OFF, C LOW */
		timer_set_oc_mode(TIM1, TIM_OC1, TIM_OCM_PWM1);
		timer_set_oc_mode(TIM1, TIM_OC2, TIM_OCM_FROZEN);
		timer_set_oc_mode(TIM1, TIM_OC3, TIM_OCM_FORCE_LOW);

		timer_enable_oc_output(TIM1, TIM_OC1);
		timer_disable_oc_output(TIM1, TIM_OC1N);

		timer_disable_oc_output(TIM1, TIM_OC2);
		timer_disable_oc_output(TIM1, TIM_OC2N);

		timer_enable_oc_output(TIM1, TIM_OC3);
		timer_enable_oc_output(TIM1, TIM_OC3N);

		step++;
		//gpio_port_write(GPIOD, GPIO12);
		break;
	case 1: /* A HIGH, B PWM LOW, C OFF */
		timer_set_oc_mode(TIM1, TIM_OC1, TIM_OCM_FORCE_HIGH);
		timer_set_oc_mode(TIM1, TIM_OC2, TIM_OCM_PWM1);
		timer_set_oc_mode(TIM1, TIM_OC3, TIM_OCM_FROZEN);

		timer_enable_oc_output(TIM1, TIM_OC1);
		timer_enable_oc_output(TIM1, TIM_OC1N);

		timer_disable_oc_output(TIM1, TIM_OC2);
		timer_enable_oc_output(TIM1, TIM_OC2N);

		timer_disable_oc_output(TIM1, TIM_OC3);
		timer_disable_oc_output(TIM1, TIM_OC3N);

		step++;
		//gpio_port_write(GPIOD, GPIO13);
		break;
	case 2: /* A OFF, B LOW, C PWM HIGH */
		timer_set_oc_mode(TIM1, TIM_OC1, TIM_OCM_FROZEN);
		timer_set_oc_mode(TIM1, TIM_OC2, TIM_OCM_FORCE_LOW);
		timer_set_oc_mode(TIM1, TIM_OC3, TIM_OCM_PWM1);

		timer_disable_oc_output(TIM1, TIM_OC1);
		timer_disable_oc_output(TIM1, TIM_OC1N);

		timer_enable_oc_output(TIM1, TIM_OC2);
		timer_enable_oc_output(TIM1, TIM_OC2N);

		timer_enable_oc_output(TIM1, TIM_OC3);
		timer_disable_oc_output(TIM1, TIM_OC3N);

		step++;
		//gpio_port_write(GPIOD, GPIO14);
		break;
	case 3: /* A PWM LOW, B OFF, C HIGH */
		timer_set_oc_mode(TIM1, TIM_OC1, TIM_OCM_PWM1);
		timer_set_oc_mode(TIM1, TIM_OC2, TIM_OCM_FROZEN);
		timer_set_oc_mode(TIM1, TIM_OC3, TIM_OCM_FORCE_HIGH);

		timer_disable_oc_output(TIM1, TIM_OC1);
		timer_enable_oc_output(TIM1, TIM_OC1N);

		timer_disable_oc_output(TIM1, TIM_OC2);
		timer_disable_oc_output(TIM1, TIM_OC2N);

		timer_enable_oc_output(TIM1, TIM_OC3);
		timer_enable_oc_output(TIM1, TIM_OC3N);

		step++;
		//gpio_port_write(GPIOD, GPIO15);
		break;
	case 4: /* A LOW, B PWM HIGH, C OFF */
		timer_set_oc_mode(TIM1, TIM_OC1, TIM_OCM_FORCE_LOW);
		timer_set_oc_mode(TIM1, TIM_OC2, TIM_OCM_PWM1);
		timer_set_oc_mode(TIM1, TIM_OC3, TIM_OCM_FROZEN);

		timer_enable_oc_output(TIM1, TIM_OC1);
		timer_enable_oc_output(TIM1, TIM_OC1N);

		timer_enable_oc_output(TIM1, TIM_OC2);
		timer_disable_oc_output(TIM1, TIM_OC2N);

		timer_disable_oc_output(TIM1, TIM_OC3);
		timer_disable_oc_output(TIM1, TIM_OC3N);

		step++;
		//gpio_port_write(GPIOD, GPIO15);
		break;
	case 5: /* A OFF, B HIGH, C PWM LOW */
		timer_set_oc_mode(TIM1, TIM_OC1, TIM_OCM_FROZEN);
		timer_set_oc_mode(TIM1, TIM_OC2, TIM_OCM_FORCE_HIGH);
		timer_set_oc_mode(TIM1, TIM_OC3, TIM_OCM_PWM1);

		timer_disable_oc_output(TIM1, TIM_OC1);
		timer_disable_oc_output(TIM1, TIM_OC1N);

		timer_enable_oc_output(TIM1, TIM_OC2);
		timer_enable_oc_output(TIM1, TIM_OC2N);

		timer_disable_oc_output(TIM1, TIM_OC3);
		timer_enable_oc_output(TIM1, TIM_OC3N);

		step = 0;
		//gpio_port_write(GPIOD, GPIO15);
		break;
	}

}




