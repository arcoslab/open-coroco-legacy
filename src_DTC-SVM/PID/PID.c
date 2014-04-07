/*
 * Copyright (C) 2013 ARCOS-Lab Universidad de Costa Rica
 * Author:      Federico Ruiz Ugalde            <memeruiz@gmail.com>
 * Modified by: Sebastian Chinchilla Gutierrez  <tumacher@gmail.com>

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

int pi_mode=0;
bool collected_permission=false;
bool initial_rotor_position_start=false;
/*
float P_SENSORLESS      = 0.00001f; 
float P_DOWN_SENSORLESS = 0.00001f; 
float fake_P_SENSORLESS = 0.0f;
*/
/*float I_SENSORLESS      = 0.0000000000000001f;//0.0000000000001f//0.000000000000001f //0.000000000001f
float I_DOWN_SENSORLESS = 0.0000000000000001f;//0.0000000000001f//0.000000000000001f //0.000000000001f
float fake_I_SENSORLESS = 0.0f;
*/
float P_SENSORLESS      = 0.0100f;//0.00001f; 
float P_DOWN_SENSORLESS = 0.0100f;//0.00001f; 
float fake_P_SENSORLESS = 0.0100f;



float I_SENSORLESS      = 0.00001f;//0.0000000000000001f;//0.0000000000001f//0.000000000000001f //0.000000000001f
float I_DOWN_SENSORLESS = 0.00001f;//0.0000000000000001f;//0.0000000000001f//0.000000000000001f //0.000000000001f
float fake_I_SENSORLESS = 0.00001f;


/*
float P_SENSORLESS_TORQUE      = 0.00001f; //0.000001f
float P_DOWN_SENSORLESS_TORQUE = 0.00001f; //0.000001f
float fake_P_SENSORLESS_TORQUE = 0.0f;

float I_SENSORLESS_TORQUE      = 0.0000000000000001f;//0.0000000000001f//0.000000000000001f //0.000000000001f
float I_DOWN_SENSORLESS_TORQUE = 0.0000000000000001f;//0.0000000000001f//0.000000000000001f //0.000000000001f
float fake_I_SENSORLESS_TORQUE = 0.0f;
*/
/*
float P_SENSORLESS_TORQUE      = 1.1f;//0.0005f;//0.00001f; //0.000001f
float P_DOWN_SENSORLESS_TORQUE = 1.1f;//0.0005f;//0.00001f; //0.000001f
float fake_P_SENSORLESS_TORQUE = 0.f;

float I_SENSORLESS_TORQUE      = 0.0f;//0.0000000000000001f;//0.0000000000001f//0.000000000000001f //0.000000000001f
float I_DOWN_SENSORLESS_TORQUE = 0.0f;//0.0000000000000001f;//0.0000000000001f//0.000000000000001f //0.000000000001f
float fake_I_SENSORLESS_TORQUE = 0.0f;
*/

float P_SENSORLESS_TORQUE      = 4.0f;//0.0005f;//0.00001f; //0.000001f
float P_DOWN_SENSORLESS_TORQUE = 4.0f;//0.0005f;//0.00001f; //0.000001f
float fake_P_SENSORLESS_TORQUE = 0.f;

float I_SENSORLESS_TORQUE      = 0.0f;//0.0000000000000001f;//0.0000000000001f//0.000000000000001f //0.000000000001f
float I_DOWN_SENSORLESS_TORQUE = 0.0f;//0.0000000000000001f;//0.0000000000001f//0.000000000000001f //0.000000000001f
float fake_I_SENSORLESS_TORQUE = 0.0f;



float timer=0.0f;

float   ref_freq_SVM=0.0f;

float   angle_hall1=0.0f;
float   t_e_ref=0.0f;//-1.5;//-0.9f;//-0.15;//-1.6f;
int     print_selection              = 0;
bool    flux_linkage_capture         = false;
int     flux_linkage_capture_counter = 0;
int     flux_linkage_capture_timer   = 0;
int     flux_limit_counter_1=0;
int     flux_limit_counter_2=0;
int     flux_limit_counter_3=0;
int     flux_limit_counter_4=0;
int     flux_limit_counter_5=0;
int     flux_limit_counter_6=0;
int     flux_limit_counter_7=0;
int     flux_limit_counter_8=0;
int     flux_limit_counter_9=0;

float attenuation=MIN_ATTENUATION;
	
uint ticks;
uint period;
uint temp_period;
float est_angle=0;
float duty_a=0.0f;
float duty_b=0.0f;
float duty_c=0.0f;
float ref_freq=0.0f;//1;

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
bool collecting_speed  =false;
bool regular_print=true;

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


bool first_dtc=true;
bool dtc_on=true;

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
          first_dtc=true;
        }
         motor_off=true;
	close_loop=false;
	//ref_freq=START_UP_REF_FREQ; //before 1.0f
      }

    }


     if (receive_a_string(cmd_s) )
     {
      printf("%s", cmd_s);
      sscanf(cmd_s, "%s %f", cmd, &value);
      
      if (strcmp(cmd, "d") == 0)
      {

        collected_permission=true;

        //print_selection=0;
        regular_print=true;
        dtc_on=true;
        ref_freq_SVM=value;
        motor_off=false;

        if (ref_freq_SVM==0.0f) 
        { 
          dtc_on=true;
        }
        
        collecting_speed=true;
        timer=0;

        pi_mode=0;  //speed pi controller

      }	 

      if (strcmp(cmd, "Q") == 0)
      {
        collected_permission=true;
        timer=0;
        //print_selection=0;
        regular_print=true;
        dtc_on=true;
        t_e_ref=value;
        motor_off=false;

        
        collecting_speed=true;
        timer=0;

        pi_mode=1;   //torque pi controller
      }	 




      if (strcmp(cmd, "c") == 0)
      {
        timer=0;
      }	 

      if (strcmp(cmd, "t") == 0)
      {
        timer=0;
      }	 
      if (strcmp(cmd, "P") == 0)
      {
        fake_P_SENSORLESS =value;
        P_SENSORLESS      = value/10000000.0f;
        P_DOWN_SENSORLESS = value/10000000.0f;
        print_selection   = 8;
      }	 

      if (strcmp(cmd, "#") == 0)
      {
        fake_P_SENSORLESS_TORQUE =value;
        P_SENSORLESS_TORQUE      = value/1000000000.0f;
        P_DOWN_SENSORLESS_TORQUE = value/1000000000.0f;
        print_selection   = 10;
      }	       



      if (strcmp(cmd, "I") == 0)
      {
        fake_I_SENSORLESS =value;
        I_SENSORLESS      = value/10000000000000000000.0f;
        I_DOWN_SENSORLESS = value/10000000000000000000.0f;
        print_selection   = 8;
      }

      if (strcmp(cmd, "T") == 0)
      {
        fake_I_SENSORLESS_TORQUE =value;
        I_SENSORLESS_TORQUE      = value/10000000000000000000.0f;
        I_DOWN_SENSORLESS_TORQUE = value/10000000000000000000.0f;
        print_selection   = 10;
      }


      if (strcmp(cmd, "p") == 0)
      {
        /*
        if      (value== 0)   {   print_selection= 0;  }   //print frequencies
        else if (value== 1)   {   print_selection= 1;  }   //print three-phase_currents
        else if (value== 2)   {   print_selection= 2;  }   //print quadrature and direct currents 
        else if (value== 3)   {   print_selection= 3;  }   //print quadrature and direct voltage 
        else if (value== 4)   {   print_selection= 4;  }   //print voltages and voltage angles
        else if (value== 5)   {   print_selection= 5;  }   //print quadrature, direct and reference flux-linkage 
        else if (value== 6)   {   print_selection= 6;  }   //print flux-linkage magnitude and  angle
        else if (value== 7)   {   print_selection= 7;  }   //print electric torque
        else if (value== 8)   {   print_selection= 8;  }   //print Ud
        else if (value== 9)   {   print_selection= 9;  }   //print pi control and pi_max
        else if (value==10)   {   print_selection=10;  }   //print frequencies
        */
        print_selection=value;
     }



      if (strcmp(cmd, "s") == 0)
      {
        print_selection=7;
        regular_print=false;
        dtc_on=true;
        ref_freq_SVM=value;
        motor_off=false;
        
        if (ref_freq_SVM==0.0f) 
        { 
          dtc_on=true;
        }
        
        collecting_speed=true;
      }	
      /*
      else if (strcmp(cmd, "t") == 0)
      {
        dtc_on=true;
        t_e_ref=value;
        motor_off=false;

        if (t_e_ref==0.0f) 
        { 
          dtc_on=true;
        }
        
        collecting_speed=true;

      }	
       */ 

      else if (strcmp(cmd, "z") == 0)
      {
        dtc_on    = true;
        t_e_ref   = value;
        motor_off = false;

        if (t_e_ref==0.0f) 
        { 
          dtc_on=true;
        }
        
        //collecting_speed=true;

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

  if (motor_off) {
    duty_a=0.0f;
    duty_b=0.0f;
    duty_c=0.0f;
    //attenuation=0.0f;
  }

  else 
  { 
    /*
    duty_a=0.8f;//1.0f;
    duty_b=0.8f;//1.0f;
    duty_c=0.8f;//1.0f;
    attenuation=1.0f;
    */
/*    
    duty_a=1.0f;
    duty_b=1.0f;
    duty_c=1.0f;
    attenuation=1.0f;
*/  
/*  
    duty_a=0.0f;
    duty_b=0.0f;
    duty_c=0.0f;
*/
    //attenuation=1.0f;

  }

     


  /* Set the capture compare value for OC1. */
  timer_set_oc_value(TIM1, TIM_OC1, duty_a*attenuation*PWM_PERIOD_ARR);
  /* Set the capture compare value for OC1. */
  timer_set_oc_value(TIM1, TIM_OC2, duty_b*attenuation*PWM_PERIOD_ARR);
  /* Set the capture compare value for OC1. */
  timer_set_oc_value(TIM1, TIM_OC3, duty_c*attenuation*PWM_PERIOD_ARR);
  //tim_force_update_event(TIM1);
}




