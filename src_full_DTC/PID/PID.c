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




/*void calc_attenuation(void) {
  if (CUR_FREQ < START_ATTENUATION_FREQ) { //freq at which attenuation starts to increase
    attenuation = MIN_ATTENUATION;
  } else {
    attenuation=MIN_ATTENUATION+(CUR_FREQ-START_ATTENUATION_FREQ)/(MAX_MOTOR_FREQ/(MAX_ATTENUATION-MIN_ATTENUATION)); //500 top freq of motor
    if (attenuation > MAX_ATTENUATION ) {
      attenuation = MAX_ATTENUATION;
    }
  }
}
*/
float angle_hall1=0.0f;
float t_e_ref=-1.5f;//-1.6f;
int  print_selection              = 3;
bool flux_linkage_capture         = false;
int  flux_linkage_capture_counter = 0;
int  flux_linkage_capture_timer    = 0;
int        flux_limit_counter_1=0;
int        flux_limit_counter_2=0;
int        flux_limit_counter_3=0;
int        flux_limit_counter_4=0;
int        flux_limit_counter_5=0;
int        flux_limit_counter_6=0;
int        flux_limit_counter_7=0;
int        flux_limit_counter_8=0;
int        flux_limit_counter_9=0;

float attenuation=MIN_ATTENUATION;
	
uint ticks;
uint period;
uint temp_period;
float est_angle=0;
float duty_a=0.0f;
float duty_b=0.0f;
float duty_c=0.0f;
float ref_freq=0.0f;//1;

//printf("D1: %6.2f S1: %6.2f D2: %6.2f S2: %6.2f D3: %6.2f S3: %6.2f\n",divisor_voltage,source_voltage,divisor_voltage_2,source_voltage_2,divisor_voltage_3,source_voltage_3);

float cur_angle=0.0f;
float final_ref_freq=40;
float error, p_error;
float i_error=0;
float cmd_angle;
float pi_control;
int close_loop=false;
int first_closed=false;
int motor_off=true;//false;


//colecting current command
bool collecting_current=false;

void calc_freq(void) 
{
  static bool first=true;
  static int  hall_a = 0;
  static int  hall_a_last=0;
  static uint last_fall_hall_a_ticks=0;

  hall_a=HALL_A();

  if (first) 
  {
    hall_a_last=hall_a;
    ticks=0;
    period=UINT_MAX;
    last_fall_hall_a_ticks=ticks;
    first=false;
  } 

  else 
  {
    //one more period wating for hall change
    ticks++;

    //ticks overflow
    if (ticks == UINT_MAX) 
    {
      ticks=0;
    }

    
    if ((hall_a_last > 0) && (hall_a == 0)) 
    {
      //hall falling edge: new cycle
      //new rotor position measurement
      est_angle=0;
      angle_hall1=0.0f;  //***


      if (ticks > last_fall_hall_a_ticks) 
      { //updating period
	period=ticks-last_fall_hall_a_ticks;
      } 
      else 
      {
	period=UINT_MAX-last_fall_hall_a_ticks+ticks;
      }
      last_fall_hall_a_ticks=ticks;
    } 

    else 
    {
      //we update period only if is bigger than before
      if (ticks > last_fall_hall_a_ticks) 
      {
	temp_period=ticks-last_fall_hall_a_ticks;
      } 
      else 
      {
	temp_period=UINT_MAX-last_fall_hall_a_ticks+ticks;
      }

      if (temp_period > period) 
      {
	period=temp_period;
      }
     
      //update estimated current angle
      //est_angle+=2.0f*PI*TICK_PERIOD/(period/TICK_PERIOD);

      angle_hall1+=360.0f*TICK_PERIOD*CUR_FREQ;//***
      if (angle_hall1>=360.0f)
        { angle_hall1=0.0f; }


      est_angle+=2.0f*PI/period;///TICK_PERIOD);
      if (est_angle > 2.0f*PI) 
      {
	est_angle=0.0f;
      }
    }
  }
  hall_a_last=hall_a;


   
      
}



void start_up(void) 
{
  if (CUR_FREQ < MIN_CLOSE_LOOP_FREQ) 
  {
    //printf("Open loop\n");
    ref_freq=START_UP_REF_FREQ;
    close_loop=false;
    first_closed=true;
  } 
  else 
  {
    close_loop=true;
  }

  if (close_loop && first_closed) 
  {
    first_closed=false;
    ref_freq=FIRST_CLOSE_LOOP_REF_FREQ;
  }
}



void pi_controller(void) {
  error=ref_freq-CUR_FREQ; // ref_freq-cur_freq
  if (error > 0.0f) {
    p_error=P*error;
  } else {
    p_error=P_DOWN*error;
  }
  if (error > 0.0f) {
    i_error+=I*error;
  } else {
    i_error+=I_DOWN*error;
  }
  if (i_error > I_MAX){
    i_error=I_MAX;
  }
  if (i_error < -I_MAX) {
    i_error=-I_MAX;
  }
  if (p_error > P_MAX) {
    p_error=P_MAX;
  }
  if (p_error < -P_MAX) {
    p_error= -P_MAX;
  }
  pi_control=p_error+i_error;
  if (pi_control > PI_MAX) {
    pi_control = PI_MAX;
  }
  if (pi_control < PI_MIN) {
    pi_control = PI_MIN;
  }
  cmd_angle+=pi_control;
  if (pi_control >= 0.0f) {
    attenuation=MIN_ATTENUATION+pi_control/(PI_MAX/(MAX_ATTENUATION-MIN_ATTENUATION));
  } else {
    attenuation=MIN_ATTENUATION-pi_control/(PI_MAX/(MAX_ATTENUATION-MIN_ATTENUATION));
  }
}



    char cmd_s[50]="";
    char cmd[10]="";
    float value=FIRST_CLOSE_LOOP_REF_FREQ;
    int motor_stop=true;
    int counter=0;
    //motor_off=false;

void frequency_input(void)
{

    counter++;
    static int counter_stop=0;

    if (motor_stop) {
      if (CUR_FREQ < FREQ_TO_STOP_MOTOR) {

        
        if (counter_stop==0)
        {
	  printf("Motor Off now\n");
	  counter_stop=1;

        }
         motor_off=true;
	close_loop=false;
	ref_freq=START_UP_REF_FREQ; //before 1.0f
      }

    }


     if (receive_a_string(cmd_s) ){
      printf("%s", cmd_s);
      sscanf(cmd_s, "%s %f", cmd, &value);
      
      if (strcmp(cmd, "f") == 0)
      { //set ref freq
	printf("New reference frequency: %f. Confirm? (Press \"y\")\n", value);
	ref_freq=value;
        //t_e_ref=value;
	if (value == 0.0f) {
	  motor_stop=true;
	} else {
	  printf("Motor on\n");
	  motor_stop=false;
	  motor_off=false;
          counter_stop=0;

          collecting_current=true;
          //print_selection=5;
	}
      
      }

      //collecting current command
      else if (strcmp(cmd, "s") == 0)
      {
        collecting_current=true;
        
        //flux_linkage_capture=true;
        flux_linkage_capture_counter=0;
        flux_limit_counter_1=0;
        flux_limit_counter_2=0;
        flux_limit_counter_3=0;
        flux_limit_counter_4=0;
        flux_limit_counter_5=0;
        flux_limit_counter_6=0;
        flux_limit_counter_7=0;
        flux_limit_counter_8=0;
        flux_limit_counter_9=0;
        
      }	 
      //print current command
      else if (strcmp(cmd, "i") == 0)
      {
        print_selection=0;
      }	 
      //print flux-linkage command
      else if (strcmp(cmd, "v") == 0)
      {
        print_selection=1;
      }	 
      //print torque command
      else if (strcmp(cmd, "m") == 0)
      {
        print_selection=2;
      }	 
      else if (strcmp(cmd, "t") == 0)
      {
        print_selection=3;
      }	 
      else if (strcmp(cmd, "n") == 0)
      {
        print_selection=4;
      }	 
            else if (strcmp(cmd, "a") == 0)
      {
        print_selection=5;
      }	 
    }
    if (!close_loop) {
      while (poll(stdin) > 0) {
	getc(stdin);
      }
    } else {
      ref_freq=value;

      //printf("Close loop\n");
    }
}

void gen_pwm(void) {
  //calc_attenuation();

  cur_angle+=2.0f*PI*TICK_PERIOD*ref_freq;
  //converting big angles into something between 0 and 2pi
  if (cur_angle >= (2.0f*PI)) {
    cur_angle=cur_angle-(2.0f*PI);
  }

  if ((cur_angle >= 89.0f*PI/180.0f) && (cur_angle <= 91.0f*PI/180.0f)) {
    //gpio_toggle(LBLUE); //To indicate start of electric cycle
  }

  cmd_angle=est_angle+HALL_CAL_OFFSET*PI/180.0f;
/*  while (cmd_angle>=2.0f*PI)
  {
    cmd_angle=cmd_angle-2.0f*PI;
  }*/


  if (!close_loop) {
    duty_a=sinf(cur_angle);
    duty_b=sinf(cur_angle+2.0f*PI/3.0f);
    duty_c=sinf(cur_angle+4.0f*PI/3.0f);
  } else {
    pi_controller();
    duty_a=sinf(cmd_angle);
    duty_b=sinf(cmd_angle+2.0f*PI/3.0f);
    duty_c=sinf(cmd_angle+4.0f*PI/3.0f);
  }

  if (motor_off) {
    duty_a=0.0f;
    duty_b=0.0f;
    duty_c=0.0f;
  }

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
/*
  if (~motor_off) {
    duty_a=1.0f;
    duty_b=1.0f;
    duty_c=1.0f;
  }
*/
  /* Set the capture compare value for OC1. */
  timer_set_oc_value(TIM1, TIM_OC1, duty_a*attenuation*PWM_PERIOD_ARR);
  /* Set the capture compare value for OC1. */
  timer_set_oc_value(TIM1, TIM_OC2, duty_b*attenuation*PWM_PERIOD_ARR);
  /* Set the capture compare value for OC1. */
  timer_set_oc_value(TIM1, TIM_OC3, duty_c*attenuation*PWM_PERIOD_ARR);
  //tim_force_update_event(TIM1);
}




