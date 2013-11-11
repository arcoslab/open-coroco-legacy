#define MIN_ATTENUATION 0.2f
#define MAX_ATTENUATION 0.3f
#define MAX_MOTOR_FREQ 1000.0f
#define P 0.01
#define P_DOWN 0.01 //To control deacceleration speed and therefore braking current
#define I 0.000001
#define I_DOWN 0.000001 //To control deacceleration speed and therefore braking current
#define I_MAX 80*PI/180.0f
#define P_MAX 80*PI/180.0f
#define PI_MAX 70*PI/180.0f //acceleration torque
#define PI_MIN -80*PI/180.0f //braking torque
#define START_ATTENUATION_FREQ 20.0f
#define START_UP_REF_FREQ 3.0f
#define MIN_CLOSE_LOOP_FREQ 3.0f
#define FIRST_CLOSE_LOOP_REF_FREQ 60.0f
#define HALL_CAL_OFFSET -40.0f //degrees
#define FREQ_TO_STOP_MOTOR 30.0f
