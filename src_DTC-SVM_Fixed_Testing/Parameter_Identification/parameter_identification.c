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

void pwm_parameter_identification(void)
{
 /*
  float duty_a=1.0f;
  float duty_b=1.0f;
  float duty_c=1.0f;
  float attenuation =1.0f;
  */
  duty_a=1.0f;
  duty_b=1.0f;
  duty_c=1.0f;
  attenuation =1.0f;

/*      //PWM mode
	TIM_OCM_FROZEN,
	TIM_OCM_ACTIVE,
	TIM_OCM_INACTIVE,
	TIM_OCM_TOGGLE,
	TIM_OCM_FORCE_LOW,
	TIM_OCM_FORCE_HIGH,
	TIM_OCM_PWM1,
	TIM_OCM_PWM2,
*/

  //----------------SA: S1 and S4---------------------------------

      timer_set_oc_mode       (TIM1, TIM_OC1, TIM_OCM_FORCE_HIGH);
      timer_enable_oc_output  (TIM1, TIM_OC1 );  //S1 on
      timer_disable_oc_output (TIM1, TIM_OC1N);  //S4 off

  //-------------SB: S3 and S6------------------------------------

      timer_set_oc_mode(TIM1, TIM_OC2, TIM_OCM_FORCE_HIGH);
      timer_disable_oc_output(TIM1, TIM_OC2 );  //S3 off
      timer_enable_oc_output (TIM1, TIM_OC2N);  //S6 on

  //-----------SC: S5 and S2--------------------------------------

      timer_set_oc_mode(TIM1, TIM_OC3, TIM_OCM_FORCE_HIGH);
      timer_disable_oc_output(TIM1, TIM_OC3 );  //S5 off
      timer_enable_oc_output (TIM1, TIM_OC3N);  //S2 on



  /* Set the capture compare value for OC1. */
  timer_set_oc_value(TIM1, TIM_OC1, duty_a*attenuation*PWM_PERIOD_ARR);
  /* Set the capture compare value for OC1. */
  timer_set_oc_value(TIM1, TIM_OC2, duty_b*attenuation*PWM_PERIOD_ARR);
  /* Set the capture compare value for OC1. */
  timer_set_oc_value(TIM1, TIM_OC3, duty_c*attenuation*PWM_PERIOD_ARR);
  //tim_force_update_event(TIM1);
}

