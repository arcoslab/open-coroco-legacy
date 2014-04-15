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

bool checking_initial_rotor_currents_sign(float Ia, float Ib, float Ic);
float initial_rotor_position_I_VI_quadrants (float Ia, float Ib, float Ic);
int initial_rotor_sector_120_degrees_ABC (float voltage_vector_angle, float Ia, float Ib, float Ic);
float initial_rotor_position_angle_discrimination(float undetermined_angle, int ABC_sector);


void initial_rotor_position_pulses(float *psisD,float*psisQ,
                                    float *VsD,float *VsQ,float *Vs, float *cita_Vs,
                                    float initial_stator_voltage, float initial_rotor_angle, 
                                    bool *initial_rotor_position_ignition, int maximum_pulse_ticks,bool shutdown_motor);

void do_nothing                     (float* Vs,float* cita_Vs,float* VsD, float* VsQ, float *psisD, float* psisQ);
void zero_voltage_vector            (float* Vs,float* cita_Vs,float* VsD, float* VsQ, float *psisD, float* psisQ);
void A_pulse_voltage_vector         (float* Vs,float* cita_Vs,float* VsD, float* VsQ, float *psisD, float* psisQ,float initial_stator_voltage);
void negative_A_pulse_voltage_vector(float* Vs,float* cita_Vs,float* VsD, float* VsQ, float *psisD, float* psisQ,float initial_stator_voltage);
void B_pulse_voltage_vector         (float* Vs,float* cita_Vs,float* VsD, float* VsQ, float *psisD, float* psisQ,float initial_stator_voltage);
void negative_B_pulse_voltage_vector(float* Vs,float* cita_Vs,float* VsD, float* VsQ, float *psisD, float* psisQ,float initial_stator_voltage);
void C_pulse_voltage_vector         (float* Vs,float* cita_Vs,float* VsD, float* VsQ, float *psisD, float* psisQ,float initial_stator_voltage);
void negative_C_pulse_voltage_vector(float* Vs,float* cita_Vs,float* VsD, float* VsQ, float *psisD, float* psisQ,float initial_stator_voltage);

void initial_rotor_position_ABC_pulses  (float*Ia_peak,float*Ib_peak,float* Ic_peak,float IsA,float IsB,
                                         float *psisD,float*psisQ,float *VsD,float *VsQ,float *Vs, float *cita_Vs,
                                         float initial_stator_voltage, 
                                         bool *initial_rotor_position_ignition, int short_maximum_pulse_ticks,int off_delay,bool shutdown_motor);

