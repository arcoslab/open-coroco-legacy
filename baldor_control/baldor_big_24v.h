#define MIN_ATTENUATION 0.7f
#define MAX_ATTENUATION 1.0f
//#define MAX_MOTOR_FREQ 40.0f
#define MAX_MOTOR_FREQ 20.0f
#define P 0.01f
#define P_BREAK 0.01f //To control deacceleration speed and therefore braking current
#define I 0.0001f //NMB motor
#define I_BREAK 0.0004f //NMB motor
#define D 0.1f
#define I_MAX (87.0f*PI/180.0f)
#define P_MAX (87.0f*PI/180.0f)
#define D_MAX (87.0f*PI/180.0f)
#define PI_MAX (87.0f*PI/180.0f) //acceleration torque
#define PI_MAX_BREAK (-60.0f*PI/180.0f) //braking torque
/* #define START_ATTENUATION_FREQ 20.0f */
/* #define START_UP_REF_FREQ 3.0f */
/* #define MIN_CLOSE_LOOP_FREQ 3.0f */
/* #define FIRST_CLOSE_LOOP_REF_FREQ 10.0f */
#define START_ATTENUATION_FREQ 5.0f
#define START_UP_REF_FREQ 1.0f
#define MIN_CLOSE_LOOP_FREQ 5.0f
#define FIRST_CLOSE_LOOP_REF_FREQ 12.0f
#define HALL_CAL_OFFSET -0.0f //degrees (H1)
//#define FREQ_TO_STOP_MOTOR 30.0f
#define FREQ_TO_STOP_MOTOR 7.0f
