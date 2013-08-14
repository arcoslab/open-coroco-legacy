#define MIN_ATTENUATION 0.3f
#define MAX_ATTENUATION 1.0f
#define MAX_MOTOR_FREQ 780.0f
#define P 0.01
//#define P_DOWN 0.0001 //To control deacceleration speed and therefore braking current
#define P_DOWN 0.01 //To control deacceleration speed and therefore braking current
//#define I 0.000001
//#define I 0.0000004 //NMB motor
//#define I_DOWN 0.00000001 //To control deacceleration speed and therefore braking current
//#define I_DOWN 0.0000001 //To control deacceleration speed and therefore braking current /NMB motors
#define I 0.0000004 //NMB motor
#define I_DOWN 0.0000004 //NMB motor
#define I_MAX 80*PI/180.0f
#define P_MAX 80*PI/180.0f
#define PI_MAX 87*PI/180.0f //acceleration torque
#define PI_MIN -1*PI/180.0f //braking torque
#define START_ATTENUATION_FREQ 20.0f
#define START_UP_REF_FREQ 5.0f
#define MIN_CLOSE_LOOP_FREQ 3.0f
#define FIRST_CLOSE_LOOP_REF_FREQ 60.0f
#define HALL_CAL_OFFSET 136.0f //degrees
#define HALL_CAL_OFFSET 136.0f //degrees (H1)
#define HALL_CAL_OFFSET 140.0f //degrees (H3)
#define HALL_CAL_OFFSET 300.0f //degrees (H2)
#define HALL_CAL_OFFSET 136.0f //degrees (H1)
#define FREQ_TO_STOP_MOTOR 30.0f
#define R_s_0 0.2f//1.5-0.25-0.2
