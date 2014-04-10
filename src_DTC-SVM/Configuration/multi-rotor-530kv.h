//Multirotor_parameters



#define MIN_ATTENUATION 0.2f
#define MAX_ATTENUATION 1.0f

#define MAX_MOTOR_FREQ 1000.0f

#define P       0.01
#define P_DOWN  0.01 //To control deacceleration speed and therefore braking current
#define I       0.000001
#define I_DOWN  0.000001 //To control deacceleration speed and therefore braking current
#define I_MAX   80*PI/180.0f
#define P_MAX   80*PI/180.0f
#define PI_MAX  87*PI/180.0f //acceleration torque
#define PI_MIN -80*PI/180.0f //braking torque

#define START_ATTENUATION_FREQ      20.0f
#define START_UP_REF_FREQ            3.0f
#define MIN_CLOSE_LOOP_FREQ          3.0f
#define FIRST_CLOSE_LOOP_REF_FREQ   60.0f
#define HALL_CAL_OFFSET           -125.0f //degrees
#define FREQ_TO_STOP_MOTOR          30.0f

//motor parameters
#define R_s_0          0.225f   //Ohms
#define L_s_d_0        0.0001f  //0.000038671875f  //H   0.000089f H
#define L_s_q_0	       0.0001f  //0.00003515625f  //(54uH)H   0.000089f H
#define psi_F_0         0.0016//0.010f//0.010f//0.010000001f//0.08f//0.01f    //Medido: 0.00164f//0.0051f    //Wb-t (weber-turn) (kg m2 s-2 A-1)
#define pole_pairs_0   7.0f	//seven pole pairs (seven d-axis)
