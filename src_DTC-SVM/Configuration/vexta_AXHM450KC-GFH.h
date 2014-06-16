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

//vexta_AXHM450KC-GFH4G200_parameters


//motor parameters
#define R_s_0          0.78f    //Ohms
#define L_s_d_0        0.0001f  //0.000038671875f  //H   0.000089f H
#define L_s_q_0	       0.0001f  //0.00003515625f  //(54uH)H   0.000089f H
#define psi_F_0        0.0075f  //0.0016f//0.005f    //0.0016f  //Wb-t (weber-turn) (kg m2 s-2 A-1)
#define pole_pairs_0   5.0f	    //five pole pairs (five d-axis)
#define gear_ratio     200.0f   //200:1




//40-> 0.005
//60->0.01
//70->0.03
//80->0.04

//PID parameters

//voltage-frequency-based speed controller
#define I_MAX_SENSORLESS_SVM            90.0f//  0.0005f//(90.0f*frequency/interrupt_frequency) 
#define P_MAX_SENSORLESS_SVM            90.0f//  0.0005f//(90.0f*frequency/interrupt_frequency) 
#define PI_MAX_SENSORLESS_SVM           90.0f//  0.0005f//(90.0f*frequency/interrupt_frequency) 
#define PI_MIN_SENSORLESS_SVM          -90.0f// -0.0005f//-(90.0f*frequency/interrupt_frequency) 

/*
#define I_MAX_SENSORLESS_SVM            0.0005f//  0.0005f//(90.0f*frequency/interrupt_frequency) 
#define P_MAX_SENSORLESS_SVM            0.0005f//  0.0005f//(90.0f*frequency/interrupt_frequency) 
#define PI_MAX_SENSORLESS_SVM           0.0005f//  0.0005f//(90.0f*frequency/interrupt_frequency) 
#define PI_MIN_SENSORLESS_SVM          -0.0005f// -0.0005f//-(90.0f*frequency/interrupt_frequency) 
*/
/*
float P_SENSORLESS_SVM      = 0.0001f; 
float P_DOWN_SENSORLESS_SVM = 0.0001f; 
float fake_P_SENSORLESS_SVM = 0.0001f;
float I_SENSORLESS_SVM      = 0.000000002f;
float I_DOWN_SENSORLESS_SVM = 0.000000002f;
float fake_I_SENSORLESS_SVM = 0.000000002f;
*/

/*
//extra_load_angle=frequency*360.0f*(2.0f*TICK_PERIOD)+extra_load_angle_increase;
float P_SENSORLESS_SVM      = 0.000001f; 
float P_DOWN_SENSORLESS_SVM = 0.000001f; 
float fake_P_SENSORLESS_SVM = 0.000001f;
float I_SENSORLESS_SVM      = 0.0000000005f;
float I_DOWN_SENSORLESS_SVM = 0.0000000005f;
float fake_I_SENSORLESS_SVM = 0.0000000005f;
*/

//fast
//extra_voltage_angle=extra_voltage_angle+extra_load_angle;
float P_SENSORLESS_SVM      = 0.00001f; 
float P_DOWN_SENSORLESS_SVM = 0.00001f; 
float fake_P_SENSORLESS_SVM = 0.00001f;
float I_SENSORLESS_SVM      = 0.0f;
float I_DOWN_SENSORLESS_SVM = 0.0f;
float fake_I_SENSORLESS_SVM = 0.0f;

//slow
/*
float P_SENSORLESS_SVM      = 0.000001f;//0.00001f; 
float P_DOWN_SENSORLESS_SVM = 0.000001f;//0.00001f; 
float fake_P_SENSORLESS_SVM = 0.000001f;//0.00001f;
float I_SENSORLESS_SVM      = 0.0f;
float I_DOWN_SENSORLESS_SVM = 0.0f;
float fake_I_SENSORLESS_SVM = 0.0f;
*/
//DTC-SVM speed controller
#define I_MAX_SENSORLESS            60.0f//  0.0005f//(90.0f*frequency/interrupt_frequency) 
#define P_MAX_SENSORLESS            60.0f//  0.0005f//(90.0f*frequency/interrupt_frequency) 
#define PI_MAX_SENSORLESS           60.0f//  0.0005f//(90.0f*frequency/interrupt_frequency) 
#define PI_MIN_SENSORLESS          -60.0f// -0.0005f//-(90.0f*frequency/interrupt_frequency) 

float P_SENSORLESS      = 0.8f; 
float P_DOWN_SENSORLESS = 0.8f; 
float fake_P_SENSORLESS = 0.8f;
float I_SENSORLESS      = 0.0f;
float I_DOWN_SENSORLESS = 0.0f;
float fake_I_SENSORLESS = 0.0f;


//DTC-SVM torque controller
#define I_MAX_SENSORLESS_TORQUE            90.0f//  0.005f//(90.0f*frequency/interrupt_frequency) 
#define P_MAX_SENSORLESS_TORQUE            90.0f//  0.005f//(90.0f*frequency/interrupt_frequency) 
#define PI_MAX_SENSORLESS_TORQUE           90.0f//  0.005f//(90.0f*frequency/interrupt_frequency) 
#define PI_MIN_SENSORLESS_TORQUE          -90.0f//  -0.005f//-(90.0f*frequency/interrupt_frequency) 

float P_SENSORLESS_TORQUE      = 0.0f;
float P_DOWN_SENSORLESS_TORQUE = 0.0f;
float fake_P_SENSORLESS_TORQUE = 0.0f;
float I_SENSORLESS_TORQUE      = 0.0f;
float I_DOWN_SENSORLESS_TORQUE = 0.0f;
float fake_I_SENSORLESS_TORQUE = 0.0f;


#define FIXED_LOAD_ANGLE 80.0f


//Admittance controller
#define STIFFNESS 1.0f
#define DAMPING   1.0f
#define MAX_SPEED 120.0f

//Battey
#define UD_PERCENTAGE   0.7f    //0.8f
#define MAXIMUM_OPEN_LOOP_SPEED 100.0f
#define MAXIMUM_OPEN_LOOP_ANGLE_INCREASE 0.0005f//0.0005f

//Hall sensor parameters
#define HALL_FACTOR 1.0f

//strain gauge
# define STRAIN_GAUGE_REFERENCE_VOLTAGE   1.57f
float strain_gauge_reference=0.0f;//STRAIN_GAUGE_REFERENCE_VOLTAGE;
bool reset_strain_gauge_reference   =true;

