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


float isA_moving_average_filter(float isA)
{

    static float isA_older=0.0f;
    static float isA_old=0.0f;
    static float isA_new=0.0f;
    static int   k=0;
    float filtered_isA=0.0f;

    if (k==0)
    {
      isA_older = isA;
      isA_old   = isA;
      isA_new   = isA;
      k         = 1;
    }

    isA_older = isA_old;    
    isA_old   = isA_new;
    isA_new   = isA;

    filtered_isA = (isA_new+isA_old+isA_older)/3.0f;
    
    return filtered_isA;
}

float isB_moving_average_filter(float isB)
{

    static float isB_older = 0.0f;
    static float isB_old   = 0.0f;
    static float isB_new   = 0.0f;
    static int   k = 0;
    float filtered_isB = 0.0f;

    if (k==0)
    {
      isB_older = isB;
      isB_old   = isB;
      isB_new   = isB;
      k         =   1;
    }


    isB_older = isB_old;    
    isB_old   = isB_new;
    isB_new   = isB;

    filtered_isB = (isB_new+isB_old+isB_older)/3.0f;
    
    return filtered_isB;
}
