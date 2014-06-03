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

//torque=K(cita_desired-cita) - D*wr

float stiffness = STIFFNESS;
float damping   = DAMPING;

float reference_electric_angle      = 0.0f;
float reference_mechanical_angle    = 0.0f;
float reference_gear_angle          = 0.0f;

float electric_angle                = 0.0f;
float mechanical_angle              = 0.0f;
float gear_angle                    = 0.0f;

float mechanical_frequency          = 0.0f;
float gear_frequency                = 0.0f;



float admittance_controller(    float stiffness,
                                float damping,
                                float reference_angle,
                                float angle,
                                float frequency,
                                float torque );

float mechanic_motor_to_electric(float value);
float electric_to_mechanic_motor(float value);

float mechanic_gear_to_electric(float value);
float electric_to_mechanic_gear(float value);


