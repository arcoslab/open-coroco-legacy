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


//-----------------voltage and current space vectors---------------------


float direct_stator_current_i_sD     (float i_sA)
{
  return i_sA;
}
float quadrature_stator_current_i_sQ (float i_sA,float i_sB)
{
  return (1.0f/sqrt(3.0f))*(i_sA+2.0f*i_sB);
}

/*
  int S2;
  int S3;
  int S4;
  int S5;
  int S6;
  int S1;
*/

void  switching_states (int* S_A, int* S_B, int* S_C)
{
  int S1,S2,S3,S4,S5,S6;
  
  S1=gpio_get(GPIOE, GPIO9 );
  S4=gpio_get(GPIOE, GPIO8 );
  S3=gpio_get(GPIOE, GPIO11);
  S6=gpio_get(GPIOE, GPIO10);
  S5=gpio_get(GPIOE, GPIO13);
  S2=gpio_get(GPIOE, GPIO12);

  if      ( S1 && ~S4) *S_A=1;
  else if (~S1 &&  S4) *S_A=0;
  else                 *S_A=2;

  if      ( S3 && ~S6) *S_B=1;
  else if (~S3 &&  S6) *S_B=0;
  else                 *S_B=2;

  if      ( S5 && ~S2) *S_C=1;
  else if (~S5 &&  S2) *S_C=0;
  else                 *S_C=2;
  
  

}

void  floating_switching_states (float* S_A, float* S_B, float* S_C)
{
  int S1,S2,S3,S4,S5,S6;
  
  S1=gpio_get(GPIOE, GPIO9 );
  S4=gpio_get(GPIOE, GPIO8 );
  S3=gpio_get(GPIOE, GPIO11);
  S6=gpio_get(GPIOE, GPIO10);
  S5=gpio_get(GPIOE, GPIO13);
  S2=gpio_get(GPIOE, GPIO12);

  if      ( S1 && ~S4) *S_A=1.0f;
  else if (~S1 &&  S4) *S_A=0.0f;
  else                 *S_A=0.5f;//0.5f;

  if      ( S3 && ~S6) *S_B=1.0f;
  else if (~S3 &&  S6) *S_B=0.0f;
  else                 *S_B=0.5f;//0.5f;

  if      ( S5 && ~S2) *S_C=1.0f;
  else if (~S5 &&  S2) *S_C=0.0f;
  else                 *S_C=0.5f;//0.5f;
  
  

}


float direct_stator_voltage_V_sD     (int S_A, int S_B, int S_C,float U_d)
{
  return (2.0f/3.0f)*U_d*(S_A-0.5f*S_B-0.5*S_C);
}

float quadrature_stator_voltage_V_SQ (int S_B, int S_C,float U_d)
{ 
  return (1.0f/sqrt(3))*U_d*(S_B-S_C);
}


float floating_switches_direct_stator_voltage_V_sD     (float S_A, float S_B, float S_C,float U_d)
{
  return (2.0f/3.0f)*U_d*(S_A-0.5f*S_B-0.5*S_C);
}

float floating_switches_quadrature_stator_voltage_V_SQ (float S_B, float S_C,float U_d)
{ 
  return (1.0f/sqrt(3))*U_d*(S_B-S_C);
}

//---------------------stator flux-linkage space vector estimation-------------------------------
#define W_CUTOFF 200.0f

//#define a_sD (-0.000008721f+0.00000905405538680536f)*2.0f
//#define b_sQ (-0.00000227445533769063f)*2.0f
#define a_sD 0.0f
#define b_sQ 0.0f


float direct_stator_flux_linkage_estimator_psi_sD     (float T,float V_sD,float i_sD,float R_s)
{
  static float previous_psi_sD=0.0f;
  previous_psi_sD = ( previous_psi_sD+T*(V_sD-i_sD*R_s) )/(1.0f+T*W_CUTOFF);

  //float psi_increment_sD=0.0f;
  
  //previous_psi_sD = ( previous_psi_sD+T*(V_sD) )/(1.0f+T*W_CUTOFF);
  //previous_psi_sD = ( T*(V_sD) );
/*
  psi_increment_sD = T*(V_sD-i_sD*R_s); 
  if (psi_increment_sD==0)  {  previous_psi_sD = ( previous_psi_sD+psi_increment_sD      ); }///(1.0f+T*W_CUTOFF);  }
  else                      {  previous_psi_sD = ( previous_psi_sD+psi_increment_sD-a_sD ); }///(1.0f+T*W_CUTOFF);  }
*/
  //---------------------
  //canceling the flux-linkage when the motor is off
  //if (motor_off==true) {previous_psi_sD=0.0f;};
  //--------------------- 

  return previous_psi_sD;
}

float quadrature_stator_flux_linkage_estimator_psi_sQ (float T,float V_sQ,float i_sQ,float R_s)
{
  static float previous_psi_sQ=0.0f;

  previous_psi_sQ = ( previous_psi_sQ+T*(V_sQ-i_sQ*R_s) )/(1.0f+T*W_CUTOFF);
  //previous_psi_sQ = ( previous_psi_sQ+T*(V_sQ) )/(1.0f+T*W_CUTOFF);
  //previous_psi_sQ = ( T*(V_sQ) );

/*
  float psi_increment_sQ=0.0f;
  psi_increment_sQ=T*(V_sQ-i_sQ*R_s);
  if (psi_increment_sQ==0.0f)  {  previous_psi_sQ = ( previous_psi_sQ+psi_increment_sQ      ); }///(1.0f+T*W_CUTOFF);  }
  else                         {  previous_psi_sQ = ( previous_psi_sQ+psi_increment_sQ-b_sQ ); }///(1.0f+T*W_CUTOFF);  }
*/  

  //------------------------------------
  //canceling the flux-linkage when the motor is off
  //if (motor_off==true) {previous_psi_sQ=0.0f;}
  //-------------------------------------
  return previous_psi_sQ;
}

float stator_flux_linkage_magnite_psi_s               (float psi_sD,float psi_sQ)
{
  return sqrtf( (psi_sQ*psi_sQ+psi_sD*psi_sD) );
}

int stator_flux_linkage_sector_alpha                (float psi_sD, float psi_sQ)
{
  float psi_angle=0.0f;
  int psi_alpha=0;
  psi_angle=vector_angle(psi_sQ,psi_sD);
  
  //sector selection
  if      ( ( (psi_angle>=330.0f) && (psi_angle<=360.0f) ) ||
            ( (psi_angle>=  0.0f) && (psi_angle<  30.0f) ) )
  {   
    psi_alpha=1;
  }
  else if   ( (psi_angle>= 30.0f) && (psi_angle<  90.0f) ) 
  {   
    psi_alpha=2;
  } 
  else if   ( (psi_angle>= 90.0f) && (psi_angle< 150.0f) ) 
  {   
    psi_alpha=3;
  } 
  else if   ( (psi_angle>=150.0f) && (psi_angle< 210.0f) ) 
  {   
    psi_alpha=4;
  } 
  else if   ( (psi_angle>=210.0f) && (psi_angle< 270.0f) ) 
  {   
    psi_alpha=5;
  } 
  else if   ( (psi_angle>=270.0f) && (psi_angle< 330.0f) ) 
  {   
    psi_alpha=6;
  } 
  else 
  {
    psi_alpha=0;
  }
  
  return psi_alpha;
  
}







//electromagnetic torque estimation
float electromagnetic_torque_estimation_t_e(float psi_sD,float i_sQ, float psi_sQ,float i_sD,float pole_pairs)
{
  return (3.0f/2.0f)*pole_pairs*(psi_sD*i_sQ-psi_sQ*i_sD);
}



//stator flux-linkage reference
float stator_flux_linkage_reference_psi_s_ref(float psi_F,float te_ref,float L_sq,float pole_pairs)
{
  return sqrt ( psi_F*psi_F+L_sq*L_sq*(2.0f*te_ref/(3.0f*pole_pairs*psi_F))*(2.0f*te_ref/(3.0f*pole_pairs*psi_F)) );
}

//quadrature rotor inductance
float quadrature_rotor_inductance_L_sq (float psi_s,float psi_F,float t_e,float pole_pairs)
{
  return sqrt(psi_s*psi_s-psi_F*psi_F)/(2.0f*t_e/(3.0f*pole_pairs*psi_F));
}

//hysteresis windows
int stator_flux_linkage_hysteresis_controller_d_psi(float psi_s_ref, float psi_s,float psi_delta_percentage)
{
  static int d_psi=0; 
  if      (psi_s<=psi_s_ref*(1.0f-psi_delta_percentage/100.0f) )
  {
    d_psi=1;
  }
  else if (psi_s>=psi_s_ref*(1.0f+psi_delta_percentage/100.0f) )
  {
    d_psi=0;
  }
  else
  {
    d_psi=d_psi;
  }
  return d_psi;
}


int electromagnetic_torque_hysteresis_controller_d_te(float t_e_ref, float t_e, float t_e_delta_percentage)
{
  int d_te=0;
  
  //forward rotation  
  if (t_e_ref>=0.0f)
  {
    if      (t_e<=t_e_ref*(1.0f-t_e_delta_percentage))
    {
      d_te=1;
    }
    else if (t_e>=t_e_ref*(1.0f+t_e_delta_percentage))
    {
      d_te=-1;
    }
    else
    {
      d_te=0;
    }
  }
  //clockwise rotation
  else
  {
    if (t_e_ref<=t_e_ref*(1.0f+t_e_delta_percentage))
    {
      d_te=1;
    }
    else if (t_e>=t_e_ref*(1.0f-t_e_delta_percentage))
    {
      d_te=-1;
    }
    else 
    {
      d_te=0;
    }
  }  
  
  return d_te;
}



//output voltages

//vector voltages definition
#define V_1  *S_A=1;*S_B=0;*S_C=0; //100
#define V_2  *S_A=1;*S_B=1;*S_C=0; //110
#define V_3  *S_A=0;*S_B=1;*S_C=0; //010
#define V_4  *S_A=0;*S_B=1;*S_C=1; //011
#define V_5  *S_A=0;*S_B=0;*S_C=1; //001
#define V_6  *S_A=1;*S_B=0;*S_C=1; //101
#define V_7  *S_A=1;*S_B=1;*S_C=1; //111
#define V_8  *S_A=0;*S_B=0;*S_C=0; //000


void optimal_voltage_switching_vector_selection_table(int d_psi,int d_te,int alpha,int* S_A, int* S_B, int* S_C)
{

  if (d_psi==1) { if      (d_te==1) { if           (alpha==1) { V_2 }
                                      else if      (alpha==2) { V_3 }
                                      else if      (alpha==3) { V_4 }
                                      else if      (alpha==4) { V_5 }
                                      else if      (alpha==5) { V_6 }
                                      else if      (alpha==6) { V_1 }
                                      else                    {     }                      
                                    }
                  else if (d_te==0) { if           (alpha==1) { V_7 }
                                      else if      (alpha==2) { V_8 }
                                      else if      (alpha==3) { V_7 }
                                      else if      (alpha==4) { V_8 }
                                      else if      (alpha==5) { V_7 }
                                      else if      (alpha==6) { V_8 }
                                      else                    {     }                      
                                    }
                  else if (d_te==-1){ if           (alpha==1) { V_6 }
                                      else if      (alpha==2) { V_1 }
                                      else if      (alpha==3) { V_2 }
                                      else if      (alpha==4) { V_3 }
                                      else if      (alpha==5) { V_4 }
                                      else if      (alpha==6) { V_5 }
                                      else                    {     }                      
                                    }
                }

  else if (d_psi==1) { 
                  if      (d_te==1) { if           (alpha==1) { V_3 }
                                      else if      (alpha==2) { V_4 }
                                      else if      (alpha==3) { V_5 }
                                      else if      (alpha==4) { V_6 }
                                      else if      (alpha==5) { V_1 }
                                      else if      (alpha==6) { V_2 }
                                      else                    {     }                      
                                    }
                  else if (d_te==0) { if           (alpha==1) { V_8 }
                                      else if      (alpha==2) { V_7 }
                                      else if      (alpha==3) { V_8 }
                                      else if      (alpha==4) { V_7 }
                                      else if      (alpha==5) { V_8 }
                                      else if      (alpha==6) { V_7 }
                                      else                    {     }                      
                                    }
                  else if (d_te==-1){ if           (alpha==1) { V_5 }
                                      else if      (alpha==2) { V_6 }
                                      else if      (alpha==3) { V_1 }
                                      else if      (alpha==4) { V_2 }
                                      else if      (alpha==5) { V_3 }
                                      else if      (alpha==6) { V_4 }
                                      else                    {     }                      
                                    }
                }

}

void voltage_switch_inverter_VSI(int S_A, int S_B, int S_C)
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
  if (S_A==1)
    {
      timer_set_oc_mode       (TIM1, TIM_OC1, TIM_OCM_FORCE_HIGH);
      timer_enable_oc_output  (TIM1, TIM_OC1 );  //S1 on
      timer_disable_oc_output (TIM1, TIM_OC1N);  //S4 off
    }

  else
    {
      timer_set_oc_mode       (TIM1, TIM_OC1, TIM_OCM_FORCE_HIGH);
      timer_disable_oc_output (TIM1, TIM_OC1);  //S1 off
      timer_enable_oc_output  (TIM1, TIM_OC1N); //S4 on
    }
  //-------------SB: S3 and S6------------------------------------
  if (S_B==1)
    {
      timer_set_oc_mode(TIM1, TIM_OC2, TIM_OCM_FORCE_HIGH);
      timer_enable_oc_output(TIM1, TIM_OC2 );    //S3 on
      timer_disable_oc_output (TIM1, TIM_OC2N);  //S6 off
    }


  else
    {
      timer_set_oc_mode(TIM1, TIM_OC2, TIM_OCM_FORCE_HIGH);
      timer_disable_oc_output(TIM1, TIM_OC2 );  //S3 off
      timer_enable_oc_output (TIM1, TIM_OC2N);  //S6 on
    }
  //-----------SC: S5 and S2--------------------------------------
  if (S_C==1)
    {
      timer_set_oc_mode(TIM1, TIM_OC3, TIM_OCM_FORCE_HIGH);
      timer_enable_oc_output(TIM1, TIM_OC3 );   //S5 on
      timer_disable_oc_output (TIM1, TIM_OC3N); //S2 off
    }
  else
    {
      timer_set_oc_mode(TIM1, TIM_OC3, TIM_OCM_FORCE_HIGH);
      timer_disable_oc_output(TIM1, TIM_OC3 );  //S5 off
      timer_enable_oc_output (TIM1, TIM_OC3N);  //S2 on
    }


  /* Set the capture compare value for OC1. */
  timer_set_oc_value(TIM1, TIM_OC1, duty_a*attenuation*PWM_PERIOD_ARR);
  /* Set the capture compare value for OC1. */
  timer_set_oc_value(TIM1, TIM_OC2, duty_b*attenuation*PWM_PERIOD_ARR);
  /* Set the capture compare value for OC1. */
  timer_set_oc_value(TIM1, TIM_OC3, duty_c*attenuation*PWM_PERIOD_ARR);
  //tim_force_update_event(TIM1);
}


/*
float direct_clark_transformation(float i_sA, float i_sB, float i_sC)
{
	float i_sD;
	i_sD=(2.0f/3.0f) * (i_sA-(1.0f/2.0f)*i_sB-(1.0f/2.0f)*i_sC);
	//i_sD=i_sA;
	return i_sD;
}

float quadrature_clark_transformation(float i_sA,float i_sB,float i_sC)
{
	float i_sQ;
	i_sQ=(2.0f/3.0f)*(sqrtf(3.0f)/2.0f) * (i_sB-i_sC);
	//i_sQ=(i_sA+2.0f*i_sB)/sqrtf(3.0f);		
	return i_sQ;
}
*/

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
	
	if (angle>=360.0f)
		angle=angle-360.0f;
	else if (angle<0.0f)
		angle=angle+360.0f;

	if ( (quadrature_component<=0.0f) && (direct_component<0.0f) )
		angle=angle+180.0f;

	else if ( (quadrature_component>0.0f)&&(direct_component<0.0f))
		angle=angle-180.0f;

	return angle;
}


float A_inverse_clark_transformation(float V_sD)//(float V_sD,V_sQ)
{
	//return V_sQ;
	return V_sD;
}

float B_inverse_clark_transformation(float V_sQ, float V_sD)
{
	float V_sB;
	//V_sB=(-V_sQ+sqrtf(3.0f)*V_sD)/2.0f;
	V_sB=(-V_sD+sqrtf(3.0f)*V_sQ)/2.0f;
	return V_sB;
}

float C_inverse_clark_transformation(float V_sQ, float V_sD)
{
	float V_sC;
	//V_sC=(-V_sQ-sqrtf(3.0f)*V_sD)/2.0f;
	V_sC=(-V_sD-sqrtf(3.0f)*V_sQ)/2.0f;
	return V_sC;
}
//-------------------------------------------------------------------------------
//necesita refinarse porque no considera el cuadrante donde se encuentra el vector absoluto del estator, 
//ni tampoco que la diferencia entre A, B y C es de 120°
float duty_cycle_to_angle(float duty_cycle)
{
	float angle;
	angle=180.0f/PI*asinf(duty_cycle);

	if (angle>=360.0f)
		angle=angle-360.0f;
	else if (angle<0.0f)
		angle=angle+360.0f;

	return angle;
}
//-------------------------------------------------------------------------------

float phase_A_angle_to_stator_angle(float phase_A_angle)
{
	float stator_angle;
	stator_angle=451.0f-phase_A_angle;

	if (stator_angle>=360.0f)
		stator_angle=stator_angle-360.0f;
	else if (stator_angle<0.0f)
		stator_angle=stator_angle+360.0f;
	
	return stator_angle;	
}

float stator_angle_to_phase_A(float stator_angle)
{
	float phase_A;
	phase_A=450-stator_angle;
	
	if (phase_A>=360.0f)
		phase_A=phase_A-360.0f;
	else if (phase_A<0.0f)
		phase_A=phase_A+360.0f;

	return phase_A;	
}



/*
	//park_transformation
	V_sD		=direct_clark_transformation	(duty_a,duty_b,duty_c);
	V_sQ		=quadrature_clark_transformation(duty_a,duty_b,duty_c);
	V_s_angle	=vector_angle			(V_sQ,V_sD);
	V_s_magnitude	=vector_magnitude		(V_sQ,V_sD);		

	B=phase_A_angle_to_stator_angle(A_phase_angle_degree);
	A=stator_angle_to_phase_A(V_s_angle);
*/


	//inverse park transformation	
/*
	A=A_inverse_clark_transformation(V_sQ);
	B=B_inverse_clark_transformation(V_sD,V_sQ);
	C=C_inverse_clark_transformation(V_sD,V_sQ);
	
	A=duty_cycle_to_angle(	A_inverse_clark_transformation(V_sQ)		);
  	B=duty_cycle_to_angle(	B_inverse_clark_transformation(V_sD,V_sQ)	);
	C=duty_cycle_to_angle(	C_inverse_clark_transformation(V_sD,V_sQ)	);
*/



int S_A=0;
int S_B=0;
int S_C=0;

float S_A_f=0;
float S_B_f=0;
float S_C_f=0;

float i_sA = 0.0f;
float i_sB = 0.0f;
float U_d  = 0.0f;

float i_sD=0.0f;
float i_sQ=0.0f;
float i_s=0.0f;
float cita_i_s=0.0f;

float V_sD=0.0f;
float V_sQ=0.0f;
float V_s =0.0f;
float cita_V_s=0.0f;

float psi_sD=0.0f;
float psi_sQ=0.0f;
float psi_s =0.0f;
int psi_alpha=0;

float t_e=0.0f;

float psi_s_ref=0.0f;
float t_e_ref=0.15f;

int   d_psi=0.0f;
int   d_te=0.0f;
float psi_delta_percentage=0.1f;//10.0f;
float t_e_delta_percentage=0.1f;//10.0f;


//motor parameters;
float R_s        = R_s_0;
float pole_pairs = pole_pairs_0;
float L_sq       = L_s_q_0;
float psi_F      = psi_F_0;


//wrapper
void DTC(void)//(float i_sA,float i_sB, float U_d,float L_sq,float psi_F,float t_e_ref)
{

  //switching_states                        (&S_A,&S_B,&S_C);
  //V_sD    =direct_stator_voltage_V_sD     (S_A,S_B,S_C,U_d);
  //V_sQ    =quadrature_stator_voltage_V_SQ (S_B,S_C,U_d);
  V_sD    =floating_switches_direct_stator_voltage_V_sD     (S_A_f,S_B_f,S_C_f,U_d);
  V_sQ    =floating_switches_quadrature_stator_voltage_V_SQ (      S_B_f,S_C_f,U_d);
 

  V_s     =vector_magnitude               (V_sQ,V_sD);
  cita_V_s=vector_angle                   (V_sQ,V_sD);

  i_sD    =direct_stator_current_i_sD     (i_sA);
  i_sQ    =quadrature_stator_current_i_sQ (i_sA,i_sB);
  i_s     =vector_magnitude               (i_sQ,i_sD);
  cita_i_s=vector_angle                   (i_sQ,i_sD);

  psi_sD   =direct_stator_flux_linkage_estimator_psi_sD     (TICK_PERIOD,V_sD,i_sD,R_s);
  psi_sQ   =quadrature_stator_flux_linkage_estimator_psi_sQ (TICK_PERIOD,V_sQ,i_sQ,R_s);
  psi_s    =stator_flux_linkage_magnite_psi_s               (psi_sD,psi_sQ);
  psi_alpha=stator_flux_linkage_sector_alpha                (psi_sD,psi_sQ);

  t_e      =electromagnetic_torque_estimation_t_e(psi_sD,i_sQ,psi_sQ,i_sD,pole_pairs);
  psi_s_ref=stator_flux_linkage_reference_psi_s_ref(psi_F,t_e_ref,L_sq,pole_pairs);


  d_psi=stator_flux_linkage_hysteresis_controller_d_psi   (psi_s_ref, psi_s,psi_delta_percentage);
  d_te =electromagnetic_torque_hysteresis_controller_d_te (t_e_ref  , t_e  ,t_e_delta_percentage);


  optimal_voltage_switching_vector_selection_table(d_psi,d_te,psi_alpha,&S_A,&S_B,&S_C);
  //voltage_switch_inverter_VSI(S_A,S_B,S_C);


}

