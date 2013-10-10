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


void sensorless_pi_controller(
     float reference_frequency, float frequency,float interrupt_frequency,float* sensorless_attenuation, float* rotating_angle);
 
void sensorless_open_loop(
     float *reference_frequency, float* sensorless_attenuation,float interrupt_frequency,float max_frequency,float frequency_increment);

float psi_advance_calculator(float reference_frequency, float interrupt_frequency);

void psi_finitite_state_machine (float reference_frequency, float real_frequency, float* rotating_angle);
