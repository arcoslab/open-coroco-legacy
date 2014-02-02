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




//#define P_SENSORLESS             0.000001f//0.00001f
//#define P_DOWN_SENSORLESS        0.000001f//0.00001f 
                                                   
//#define I_SENSORLESS             0.0000000000001f//0.000000000000001f //0.000000000001f
//#define I_DOWN_SENSORLESS        0.0000000000001f//0.000000000000001f //0.000000000001f

/*----------Original--------------------------------------
*/
/*
#define I_MAX_SENSORLESS              (90.0f*400.0f/interrupt_frequency) 
#define P_MAX_SENSORLESS              (90.0f*400.0f/interrupt_frequency) 
#define PI_MAX_SENSORLESS             (90.0f*400.0f/interrupt_frequency) 
#define PI_MIN_SENSORLESS            -(90.0f*400.0f/interrupt_frequency) 
*/
/*
#define I_MAX_SENSORLESS              (90.0f*frequency/interrupt_frequency) 
#define P_MAX_SENSORLESS              (90.0f*frequency/interrupt_frequency) 
#define PI_MAX_SENSORLESS             (90.0f*frequency/interrupt_frequency) 
#define PI_MIN_SENSORLESS            -(90.0f*frequency/interrupt_frequency) 
*/

#define I_MAX_SENSORLESS              0.0005f//(90.0f*frequency/interrupt_frequency) 
#define P_MAX_SENSORLESS              0.0005f//(90.0f*frequency/interrupt_frequency) 
#define PI_MAX_SENSORLESS             0.0005f//(90.0f*frequency/interrupt_frequency) 
#define PI_MIN_SENSORLESS            -0.0005f//-(90.0f*frequency/interrupt_frequency) 


/*
#define I_MAX_SENSORLESS              0.05f//(90.0f*frequency/interrupt_frequency) 
#define P_MAX_SENSORLESS              0.05f//(90.0f*frequency/interrupt_frequency) 
#define PI_MAX_SENSORLESS             0.05f//(90.0f*frequency/interrupt_frequency) 
#define PI_MIN_SENSORLESS            -0.05f//-(90.0f*frequency/interrupt_frequency) 
*/



float SVM_pi_control=0.0f;
float psi_rotating_angle_SVM=0.0f;
float phase_advance_SVM=0.0f;
float pi_max=0.0f;
void sensorless_speed_pi_controller(
                           float reference_frequency, float frequency,float interrupt_frequency, float* rotating_angle) 
{
  float        sensorless_error         = 0.0f;
  float        p_sensorless_error       = 0.0f;
  static float i_sensorless_error       = 0.0f;
  float        pi_control_sensorless    = 0.0f;

  sensorless_error=reference_frequency-frequency;

  if (sensorless_error > 0.0f) {  p_sensorless_error  = P_SENSORLESS      * sensorless_error;
                                  i_sensorless_error += I_SENSORLESS      * sensorless_error; } 
  else                         {  p_sensorless_error  = P_DOWN_SENSORLESS * sensorless_error;
                                  i_sensorless_error += I_DOWN_SENSORLESS * sensorless_error; }

  if      (i_sensorless_error >  I_MAX) { i_sensorless_error =  I_MAX; }
  else if (i_sensorless_error < -I_MAX) { i_sensorless_error = -I_MAX; }

  if      (p_sensorless_error >  P_MAX_SENSORLESS) { p_sensorless_error =  P_MAX_SENSORLESS; }
  else if (p_sensorless_error < -P_MAX_SENSORLESS) { p_sensorless_error = -P_MAX_SENSORLESS; }

  pi_control_sensorless=p_sensorless_error+i_sensorless_error;

  if      (pi_control_sensorless > PI_MAX_SENSORLESS) { pi_control_sensorless = PI_MAX_SENSORLESS; }
  else if (pi_control_sensorless < PI_MIN_SENSORLESS) { pi_control_sensorless = PI_MIN_SENSORLESS; }

    /*
    if (reference_frequency!=0.0f){pi_control_sensorless=0.001f;}
    //pi control: 0.0005f arranca
    //pi control: 
    else {pi_control_sensorless =0.0f;}
    */
    *rotating_angle=*rotating_angle+pi_control_sensorless;


  SVM_pi_control=pi_control_sensorless;

  phase_advance_SVM=pi_control_sensorless;//*rotating_angle;//pi_control_sensorless;
  pi_max=P_MAX_SENSORLESS;//*interrupt_frequency;
}


void sensorless_open_loop(
     float *reference_frequency, float* sensorless_attenuation,float interrupt_frequency,float max_frequency,float frequency_increment)
{
  static int cicle_counter=0.0f;

  *sensorless_attenuation=MIN_ATTENUATION;


  if (cicle_counter >= interrupt_frequency/10.0f && *reference_frequency <= max_frequency)
  {
    cicle_counter=0;
    *reference_frequency=*reference_frequency+frequency_increment;
  } 

   cicle_counter=cicle_counter+1;

}


float psi_advance_calculator(float reference_frequency, float interrupt_frequency)
{
  return 360.0f*reference_frequency/interrupt_frequency;
}

#define OPEN_LOOP   0
#define CLOSE_LOOP  1
/*
#define START_UP_SVM_FREQUENCY        25.0f
#define FINAL_OPEN_LOOP_SVM_FREQUENCY 50.0f
#define OPEN_LOOP_FREQUENCY_INCREMENT  0.5f
*/
int   state=OPEN_LOOP;

void psi_finitite_state_machine (float reference_frequency, float actual_frequency, float* rotating_angle)
{
   //static int   state=OPEN_LOOP;
   //static float open_loop_frequency=0.0f;//=START_UP_SVM_FREQUENCY;//START_UP_SVM_FREQUENCY;


/**
   //counter-clock start up
   if      ( state==OPEN_LOOP && reference_frequency>0.0 && FINAL_OPEN_LOOP_SVM_FREQUENCY>=actual_frequency )
   {
      sensorless_open_loop(&open_loop_frequency, &attenuation,PWMFREQ_F,FINAL_OPEN_LOOP_SVM_FREQUENCY,OPEN_LOOP_FREQUENCY_INCREMENT);
      *rotating_angle = psi_advance_calculator(open_loop_frequency,PWMFREQ_F);
      state=OPEN_LOOP;
      attenuation=0.7f;//0.1f;
   }
   //clock-wise start up
   else if ( state==OPEN_LOOP && reference_frequency<0.0 && -FINAL_OPEN_LOOP_SVM_FREQUENCY<=actual_frequency )
   {
      sensorless_open_loop(&open_loop_frequency, &attenuation,PWMFREQ_F,-FINAL_OPEN_LOOP_SVM_FREQUENCY,-OPEN_LOOP_FREQUENCY_INCREMENT);
      *rotating_angle = psi_advance_calculator(open_loop_frequency,PWMFREQ_F);
      state=OPEN_LOOP;
      attenuation=1.0f;//0.1f;
   }
   else if (state==OPEN_LOOP)
   {
     *rotating_angle = psi_advance_calculator(open_loop_frequency,PWMFREQ_F); 
     state=CLOSE_LOOP;
     attenuation=1.0f;//0.2f;
   }
   else if (state==CLOSE_LOOP )
   {
     sensorless_pi_controller(reference_frequency,actual_frequency,PWMFREQ_F,&attenuation,rotating_angle);
     //state=CLOSE_LOOP;
     attenuation=1.0f;
   }   
   else
   {
     state=OPEN_LOOP;
   }
*/
     sensorless_speed_pi_controller(reference_frequency,actual_frequency,PWMFREQ_F,rotating_angle);
     //state=CLOSE_LOOP;
     attenuation=1.0f; 
   
}

#define P_SENSORLESS_TORQUE             0.001f
#define P_DOWN_SENSORLESS_TORQUE        0.001f 

#define I_SENSORLESS_TORQUE             0.0f
#define I_DOWN_SENSORLESS_TORQUE        0.0f

#define I_MAX_SENSORLESS_TORQUE              (9.0f*400.0f/switching_frequency) 
#define P_MAX_SENSORLESS_TORQUE              (9.0f*400.0f/switching_frequency) 
#define PI_MAX_SENSORLESS_TORQUE             (9.0f*400.0f/switching_frequency) 
#define PI_MIN_SENSORLESS_TORQUE            -(9.0f*400.0f/switching_frequency) 


void sensorless_torque_pi_controller(
                           float reference_torque, float torque,float switching_frequency, float* rotating_angle) 
{
  float        sensorless_error         = 0.0f;
  float        p_sensorless_error       = 0.0f;
  static float i_sensorless_error       = 0.0f;
  float        pi_control_sensorless    = 0.0f;

  sensorless_error=reference_torque-torque;

  if (sensorless_error > 0.0f) {  p_sensorless_error  = P_SENSORLESS_TORQUE      * sensorless_error;
                                  i_sensorless_error += I_SENSORLESS_TORQUE      * sensorless_error; } 
  else                         {  p_sensorless_error  = P_DOWN_SENSORLESS_TORQUE * sensorless_error;
                                  i_sensorless_error += I_DOWN_SENSORLESS_TORQUE * sensorless_error; }

  if      (i_sensorless_error >  I_MAX) { i_sensorless_error =  I_MAX; }
  else if (i_sensorless_error < -I_MAX) { i_sensorless_error = -I_MAX; }

  if      (p_sensorless_error >  P_MAX_SENSORLESS_TORQUE) { p_sensorless_error =  P_MAX_SENSORLESS_TORQUE; }
  else if (p_sensorless_error < -P_MAX_SENSORLESS_TORQUE) { p_sensorless_error = -P_MAX_SENSORLESS_TORQUE; }

  pi_control_sensorless=p_sensorless_error+i_sensorless_error;

  if      (pi_control_sensorless > PI_MAX_SENSORLESS_TORQUE) { pi_control_sensorless = PI_MAX_SENSORLESS_TORQUE; }
  else if (pi_control_sensorless < PI_MIN_SENSORLESS_TORQUE) { pi_control_sensorless = PI_MIN_SENSORLESS_TORQUE; }


  *rotating_angle=*rotating_angle+pi_control_sensorless;


  SVM_pi_control=pi_control_sensorless;
  phase_advance_SVM=pi_control_sensorless;
  pi_max=P_MAX_SENSORLESS_TORQUE;
}

