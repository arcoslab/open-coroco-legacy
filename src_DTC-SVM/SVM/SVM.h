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

float SVM_V_s_ref_D(float psi_s_ref, float psi_s, float psi_s_angle, float phase_advance,float i_sD, float R_s,float T_s);
float SVM_V_s_ref_Q(float psi_s_ref, float psi_s, float psi_s_angle, float phase_advance,float i_sQ, float R_s,float T_s);
float SVM_T1(float T_s,float U_s, float U1, float V_s_ref_relative_angle);
float SVM_T2(float T_s,float U_s, float U2, float V_s_ref_relative_angle);
float SVM_Taon(float T_s, float T1, float T2);
float SVM_Tbon(float Taon, float T1);
float SVM_Tcon(float Tbon, float T2);
void  SVM_phase_duty_cycles(float *duty_A, float *duty_B, float *duty_C,float V_s_ref_angle,float Taon, float Tbon,float Tcon);
void  DTC_SVM(void);



