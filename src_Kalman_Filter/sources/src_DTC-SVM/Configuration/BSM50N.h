//Baldor BSM50N-233AX



//motor parameters
#define R_s_0          2.25f    //Ohms
#define L_s_d_0        0.0001f  //H
#define L_s_q_0	       0.0001f  //H
#define psi_F_0        0.08f   //Wb-t (weber-turn) (kg m2 s-2 A-1)
#define pole_pairs_0   2.0f	    //two pole pairs (two d-axis)



//PID parameters

float P_SENSORLESS      = 0.0f; 
float P_DOWN_SENSORLESS = 0.0f; 
float fake_P_SENSORLESS = 0.0f;
float I_SENSORLESS      = 0.0f;
float I_DOWN_SENSORLESS = 0.0f;
float fake_I_SENSORLESS = 0.0f;

float P_SENSORLESS_TORQUE      = 0.0f;
float P_DOWN_SENSORLESS_TORQUE = 0.0f;
float fake_P_SENSORLESS_TORQUE = 0.0f;
float I_SENSORLESS_TORQUE      = 0.0f;
float I_DOWN_SENSORLESS_TORQUE = 0.0f;
float fake_I_SENSORLESS_TORQUE = 0.0f;

#define I_MAX_SENSORLESS            45.0f//  0.0005f//(90.0f*frequency/interrupt_frequency) 
#define P_MAX_SENSORLESS            45.0f//  0.0005f//(90.0f*frequency/interrupt_frequency) 
#define PI_MAX_SENSORLESS           45.0f//  0.0005f//(90.0f*frequency/interrupt_frequency) 
#define PI_MIN_SENSORLESS          -45.0f// -0.0005f//-(90.0f*frequency/interrupt_frequency) 

#define I_MAX_SENSORLESS_TORQUE            90.0f//  0.005f//(90.0f*frequency/interrupt_frequency) 
#define P_MAX_SENSORLESS_TORQUE            90.0f//  0.005f//(90.0f*frequency/interrupt_frequency) 
#define PI_MAX_SENSORLESS_TORQUE           90.0f//  0.005f//(90.0f*frequency/interrupt_frequency) 
#define PI_MIN_SENSORLESS_TORQUE          -90.0f//  -0.005f//-(90.0f*frequency/interrupt_frequency) 

//Battey
#define UD_PERCENTAGE   0.8f
#define MAXIMUM_OPEN_LOOP_SPEED 20.0f
#define MAXIMUM_OPEN_LOOP_ANGLE_INCREASE 0.00005f

//Hall sensor parameters
#define HALL_FACTOR pole_pairs_0
