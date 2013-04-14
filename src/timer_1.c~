void pwm(float ticks, float attenuation,float max_ticks)
{

	static float
		duty_a=0.0f, 
		duty_b=0.0f, 
		duty_c=0.0f;
	float 
		angle;

  	angle=(2.0f*PI*ticks/max_ticks);
		
  	duty_a=sinf(angle);
 	duty_b=sinf(angle+2.0f*PI/3.0f);
  	duty_c=sinf(angle+4.0f*PI/3.0f);
  
  	if (duty_a < 0.0f)
	{
		timer_set_oc_mode(TIM1, TIM_OC1, TIM_OCM_PWM1);
		timer_disable_oc_output(TIM1,TIM_OC1);
		timer_enable_oc_output (TIM1, TIM_OC1N);
		duty_a=-duty_a;
	
	} 
	else 
	{
		timer_set_oc_mode(TIM1, TIM_OC1, TIM_OCM_PWM1);
		timer_enable_oc_output(TIM1, TIM_OC1 );
		timer_disable_oc_output (TIM1, TIM_OC1N);

  	}
  	if (duty_b < 0.0f)
	{
		timer_set_oc_mode(TIM1, TIM_OC2, TIM_OCM_PWM1);
		timer_disable_oc_output(TIM1, TIM_OC2 );
		timer_enable_oc_output (TIM1, TIM_OC2N);
    		duty_b=-duty_b;
  	} 
	else 
	{
		timer_set_oc_mode(TIM1, TIM_OC2, TIM_OCM_PWM1);
		timer_enable_oc_output(TIM1, TIM_OC2 );
		timer_disable_oc_output (TIM1, TIM_OC2N);
  	}
  	if (duty_c < 0.0f)
	{
		timer_set_oc_mode(TIM1, TIM_OC3, TIM_OCM_PWM1);
		timer_disable_oc_output(TIM1, TIM_OC3 );
		timer_enable_oc_output (TIM1, TIM_OC3N);
    		duty_c=-duty_c;
  	} 
	else 
	{
		timer_set_oc_mode(TIM1, TIM_OC3, TIM_OCM_PWM1);
		timer_enable_oc_output(TIM1, TIM_OC3 );
		timer_disable_oc_output (TIM1, TIM_OC3N);	
  	}

	/* Set the capture compare value for OC1. */
	timer_set_oc_value(TIM1, TIM_OC1, duty_a*attenuation*pwm_period_ARR);
	/* Set the capture compare value for OC1. */
	timer_set_oc_value(TIM1, TIM_OC2, duty_b*attenuation*pwm_period_ARR);
	/* Set the capture compare value for OC1. */
	timer_set_oc_value(TIM1, TIM_OC3, duty_c*attenuation*pwm_period_ARR);
  	//tim_force_update_event(TIM1);
	
	
}
