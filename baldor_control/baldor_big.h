#define MIN_ATTENUATION 0.6f
#define MAX_ATTENUATION 1.0f
#define MAX_MOTOR_FREQ 40.0f
#define P 0.0001
#define P_DOWN 0.0001 //To control deacceleration speed and therefore braking current
#define P_DOWN 0.0001 //To control deacceleration speed and therefore braking current
#define I 0.00001
#define I 0.00001 //NMB motor
#define I_DOWN 0.00000001 //To control deacceleration speed and therefore braking current
#define I_DOWN 0.000001 //To control deacceleration speed and therefore braking current /NMB motors
#define I 0.00001 //NMB motor
#define I_DOWN 0.00001 //NMB motor
#define I_MAX 87*PI/180.0f
#define P_MAX 87*PI/180.0f
#define PI_MAX 87*PI/180.0f //acceleration torque
#define PI_MIN -60*PI/180.0f //braking torque
#define START_ATTENUATION_FREQ 20.0f
#define START_UP_REF_FREQ 3.0f
#define MIN_CLOSE_LOOP_FREQ 3.0f
#define FIRST_CLOSE_LOOP_REF_FREQ 10.0f
#define HALL_CAL_OFFSET 136.0f //degrees (H1)
#define HALL_CAL_OFFSET 140.0f //degrees (H3)
#define HALL_CAL_OFFSET 0.0f //degrees (H2)
#define HALL_CAL_OFFSET 5.0f //degrees (H1)
#define FREQ_TO_STOP_MOTOR 30.0f
