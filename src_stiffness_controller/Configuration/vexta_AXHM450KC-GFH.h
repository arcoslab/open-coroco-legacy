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


//PID parameters

//voltage-frequency-based speed controller
#define I_MAX_SENSORLESS_SVM            90.0f//  0.0005f//(90.0f*frequency/interrupt_frequency) 
#define P_MAX_SENSORLESS_SVM            90.0f//  0.0005f//(90.0f*frequency/interrupt_frequency) 
#define PI_MAX_SENSORLESS_SVM           90.0f//  0.0005f//(90.0f*frequency/interrupt_frequency) 
#define PI_MIN_SENSORLESS_SVM          -90.0f// -0.0005f//-(90.0f*frequency/interrupt_frequency) 

//fast
//extra_voltage_angle=extra_voltage_angle+extra_load_angle;
//[Original dirty speed controller]
float P_SENSORLESS_SVM      = 0.001f; 
float P_DOWN_SENSORLESS_SVM = 0.001f; 
//float fake_P_SENSORLESS_SVM = 0.000002f;
float I_SENSORLESS_SVM      = 0.0f;
float I_DOWN_SENSORLESS_SVM = 0.0f;
float fake_I_SENSORLESS_SVM = 0.0f;



/************************************************************
****Activate when using the function: "SVM_speed_close_loop_of_voltage_frequency_old"****
//fast
//extra_voltage_angle=extra_voltage_angle+extra_load_angle;
//[Original dirty speed controller]
float P_SENSORLESS_SVM      = 0.00001f; 
float P_DOWN_SENSORLESS_SVM = 0.00001f; 
float fake_P_SENSORLESS_SVM = 0.000002f;
float I_SENSORLESS_SVM      = 0.0f;
float I_DOWN_SENSORLESS_SVM = 0.0f;
float fake_I_SENSORLESS_SVM = 0.0f;
****************************************************************/



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









//Admittance controller
#define STIFFNESS 0.00001f
#define DAMPING   0.00001f
#define MAX_SPEED 120.0f

//Battery
#define UD_PERCENTAGE   0.8f    //0.8f is the max value. More than that causes the battery to deliver an oscilating voltage instead of a dc voltaje. Furthermore, if you use 100% the current rises too much and the fuse melts.
#define MAXIMUM_OPEN_LOOP_SPEED 100.0f
#define MAXIMUM_OPEN_LOOP_ANGLE_INCREASE 0.0005f//0.0005f

//Hall sensor parameters
#define HALL_FACTOR 1.0f

//strain gauge
#define STRAIN_GAUGE_REFERENCE_VOLTAGE   0.0f//1.002f//1.02f
#define STRAIN_GAUGE_CONVERSION_FACTOR   2.0f//1.303f
float strain_gauge_reference=STRAIN_GAUGE_REFERENCE_VOLTAGE;
bool reset_strain_gauge_reference   =true;

