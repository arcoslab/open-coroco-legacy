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




float direct_park_transformation(float i_sA, float i_sB, float i_sC)
{
	float i_sD;
	i_sD=(2.0f/3.0f) * (i_sA-(1.0f/2.0f)*i_sB-(1.0f/2.0f)*i_sC);
	return i_sD;
}

float quadrature_park_transformation(float i_sB, float i_sC)
{
	float i_sQ;
	i_sQ=(2.0f/3.0f)*(sqrtf(3.0f)/2.0f) * (i_sB-i_sC);
	return i_sQ;
}


float vector_magnitude(float quadrature_component, float direct_component)
{
	float magnitude;
	magnitude=sqrtf( (quadrature_component*quadrature_component+direct_component*direct_component) );
	return magnitude;
}

float vector_angle(float quadrature_component, float direct_component)
{
	float angle;
	angle=180.0f/PI*atanf(quadrature_component/direct_component);
	
	if (angle<0.0f)
		angle=angle+360.0f;

	return angle;
}


//void pwm(float ticks, float attenuation,float max_ticks)
void pwm(float a_phase_angle_degree)

{

	static float
		duty_a=0.0f, 
		duty_b=0.0f, 
		duty_c=0.0f;
	float
		angle_radians;


  	//angle=(2.0f*PI*ticks/max_ticks)+offset;
	angle_radians=a_phase_angle_degree*PI/180.0f;

	
  	duty_a=sinf(angle_radians);
 	duty_b=sinf(angle_radians+2.0f*PI/3.0f);
  	duty_c=sinf(angle_radians+4.0f*PI/3.0f);


	//park_transformation
	V_sD		=direct_park_transformation	(duty_a,duty_b,duty_c);
	V_sQ		=quadrature_park_transformation	(duty_b,duty_c);
	V_s_angle	=vector_angle			(V_sQ,V_sD);
	V_s_magnitude	=vector_magnitude		(V_sQ,V_sD);		



  
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
