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

float admittance_controller(    float K,
                                float D,
                                float reference_angle,
                                float angle,
                                float torque )
{

/*
    if (angle<1.05f*reference_angle && angle>0.95f*reference_angle)
        return 0.0f;

    else if (angle<reference_angle)
        return 100.0f;
    else if (angle>reference_angle)
        return -100.0f;
    else 
        return 0.0f;
*/

    float w;

    //complete law
    w=(1.0f/360.0f)*(  (K/D)*(reference_angle-angle) -torque/D  );

    //ignoring torque
    //w=(1.0f/360.0f)*(  (K/D)*(reference_angle-angle) );//-torque/D  );
 

    //ZERO stiffness
    //w=(1.0f/360.0f)*(  (0.0f)*(reference_angle-angle) -torque/D  );
    if (w> MAX_SPEED) w=MAX_SPEED;
    if (w<-MAX_SPEED) w=-MAX_SPEED;
    return  w;
}
