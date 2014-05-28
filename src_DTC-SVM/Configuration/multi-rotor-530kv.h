//Multirotor_parameters


//motor parameters
#define R_s_0          0.225f   //Ohms
#define L_s_d_0        0.0001f  //0.000038671875f  //H   0.000089f H
#define L_s_q_0	       0.0001f  //0.00003515625f  //(54uH)H   0.000089f H
#define psi_F_0        0.002f//0.0016f//0.005f    //0.0016f  //Wb-t (weber-turn) (kg m2 s-2 A-1)
#define pole_pairs_0   7.0f	//seven pole pairs (seven d-axis)


//40-> 0.005
//60->0.01
//70->0.03
//80->0.04

//PID parameters

float P_SENSORLESS      = 1.0f; 
float P_DOWN_SENSORLESS = 1.0f; 
float fake_P_SENSORLESS = 1.0f;
float I_SENSORLESS      = 0.0f;
float I_DOWN_SENSORLESS = 0.0f;
float fake_I_SENSORLESS = 0.0f;

float P_SENSORLESS_TORQUE      = 0.0f;
float P_DOWN_SENSORLESS_TORQUE = 0.0f;
float fake_P_SENSORLESS_TORQUE = 0.0f;
float I_SENSORLESS_TORQUE      = 0.0f;
float I_DOWN_SENSORLESS_TORQUE = 0.0f;
float fake_I_SENSORLESS_TORQUE = 0.0f;



//maximum load angle 70
#define I_MAX_SENSORLESS            60.0f//  0.0005f//(90.0f*frequency/interrupt_frequency) 
#define P_MAX_SENSORLESS            60.0f//  0.0005f//(90.0f*frequency/interrupt_frequency) 
#define PI_MAX_SENSORLESS           60.0f//  0.0005f//(90.0f*frequency/interrupt_frequency) 
#define PI_MIN_SENSORLESS          -60.0f// -0.0005f//-(90.0f*frequency/interrupt_frequency) 

#define I_MAX_SENSORLESS_TORQUE            90.0f//  0.005f//(90.0f*frequency/interrupt_frequency) 
#define P_MAX_SENSORLESS_TORQUE            90.0f//  0.005f//(90.0f*frequency/interrupt_frequency) 
#define PI_MAX_SENSORLESS_TORQUE           90.0f//  0.005f//(90.0f*frequency/interrupt_frequency) 
#define PI_MIN_SENSORLESS_TORQUE          -90.0f//  -0.005f//-(90.0f*frequency/interrupt_frequency) 

#define FIXED_LOAD_ANGLE 80.0f


//Battey
#define UD_PERCENTAGE   0.8f    //0.8f
#define MAXIMUM_OPEN_LOOP_SPEED 100.0f
#define MAXIMUM_OPEN_LOOP_ANGLE_INCREASE 0.0001f//0.0005f

//Hall sensor parameters
#define HALL_FACTOR 1.0f
