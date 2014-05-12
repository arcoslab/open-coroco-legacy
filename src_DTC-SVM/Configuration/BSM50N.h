//Baldor BSM50N-233AX



//motor parameters
#define R_s_0          2.25f    //Ohms
#define L_s_d_0        0.0001f  //H
#define L_s_q_0	       0.0001f  //H
#define psi_F_0        0.085f   //Wb-t (weber-turn) (kg m2 s-2 A-1)
#define pole_pairs_0   2.0f	    //two pole pairs (two d-axis)



//PID parameters
#define I_MAX_SENSORLESS            45.0f//  0.0005f//(90.0f*frequency/interrupt_frequency) 
#define P_MAX_SENSORLESS            45.0f//  0.0005f//(90.0f*frequency/interrupt_frequency) 
#define PI_MAX_SENSORLESS           45.0f//  0.0005f//(90.0f*frequency/interrupt_frequency) 
#define PI_MIN_SENSORLESS          -45.0f// -0.0005f//-(90.0f*frequency/interrupt_frequency) 

#define I_MAX_SENSORLESS_TORQUE            90.0f//  0.005f//(90.0f*frequency/interrupt_frequency) 
#define P_MAX_SENSORLESS_TORQUE            90.0f//  0.005f//(90.0f*frequency/interrupt_frequency) 
#define PI_MAX_SENSORLESS_TORQUE           90.0f//  0.005f//(90.0f*frequency/interrupt_frequency) 
#define PI_MIN_SENSORLESS_TORQUE          -90.0f//  -0.005f//-(90.0f*frequency/interrupt_frequency) 

//Battey
#define UD_PERCENTAGE   0.5f
#define MAXIMUM_OPEN_LOOP_SPEED 5.0f
