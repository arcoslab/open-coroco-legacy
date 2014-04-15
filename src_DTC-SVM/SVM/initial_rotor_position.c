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

#define SQRT_3 1.73205080756887729352f


bool checking_initial_rotor_currents_sign(float Ia, float Ib, float Ic)
{
    return Ia>=0.0f && Ib>=0.0f && Ic>=0.0f;
}

float initial_rotor_position_I_VI_quadrants (float Ia, float Ib, float Ic)
{
    return 0.5f*extended_fast_atan( SQRT_3*(Ib-Ia)/(2.0f*Ia-Ib-Ic) );
}

#define ZONE_A 1
#define ZONE_B 2
#define ZONE_C 3
#define NO_ZONE 0
int initial_rotor_sector_120_degrees_ABC (float voltage_vector_angle, float Ia, float Ib, float Ic)
{
    if      (   voltage_vector_angle==  0.0f&& Ia>Ib && Ia>Ic)  {   return ZONE_A;   }   //zone A=>-60°<cita<=60°
    else if (   voltage_vector_angle==120.0f&& Ib>Ia && Ib>Ic)  {   return ZONE_B;   }   //zone B=> 60°<cita<=180°
    else if (   voltage_vector_angle==240.0f&& Ic>Ib && Ic>Ia)  {   return ZONE_C;   }   //zone B=> 60°<cita<=180°
    else                                                        {   return NO_ZONE;   }   //no zone at all (error)
}



float initial_rotor_position_angle_discrimination(float undetermined_angle, int ABC_sector)
{
    float angle;

    if      (undetermined_angle>=  0.0f && undetermined_angle< 45.0f && ABC_sector==ZONE_A)    {  angle=undetermined_angle       ;} //first sector
    else if (undetermined_angle>=  0.0f && undetermined_angle< 45.0f && ABC_sector==ZONE_B)    {  angle=0.0f                     ;} //error
    else if (undetermined_angle>=  0.0f && undetermined_angle< 45.0f && ABC_sector==ZONE_C)    {  angle=undetermined_angle+180.0f;} //fith  sector

    else if (undetermined_angle>= 45.0f && undetermined_angle< 90.0f && ABC_sector==ZONE_A)    {  angle=undetermined_angle       ;} //second sector
    else if (undetermined_angle>= 45.0f && undetermined_angle< 90.0f && ABC_sector==ZONE_B)    {  angle=undetermined_angle       ;} //second sector
    else if (undetermined_angle>= 45.0f && undetermined_angle< 90.0f && ABC_sector==ZONE_C)    {  angle=undetermined_angle+180.0f;} //sixth  sector
    
    else if (undetermined_angle>=315.0f && undetermined_angle<  0.0f && ABC_sector==ZONE_A)    {  angle=undetermined_angle       ;} //eigth  sector
    else if (undetermined_angle>=315.0f && undetermined_angle<  0.0f && ABC_sector==ZONE_B)    {  angle=undetermined_angle+180.0f;} //fourth sector
    else if (undetermined_angle>=315.0f && undetermined_angle<  0.0f && ABC_sector==ZONE_C)    {  angle=0.0f                     ;} //error

    else if (undetermined_angle>=270.0f && undetermined_angle<315.0f && ABC_sector==ZONE_A)    {  angle=undetermined_angle       ;} //seventh sector
    else if (undetermined_angle>=270.0f && undetermined_angle<315.0f && ABC_sector==ZONE_B)    {  angle=undetermined_angle+180.0f;} //third   sector
    else if (undetermined_angle>=270.0f && undetermined_angle<315.0f && ABC_sector==ZONE_C)    {  angle=undetermined_angle       ;} //seventh sector

    else                                                                    {  angle=0.0f                     ;} //error

 
    if (angle>=360.0f)      angle=angle-360.0f;
    else if (angle <0.0f)   angle=angle+360.0f;  

    return angle;
}

#define ROTOR_POSITION_UNKNOWN      0
#define VOLTAGE_PULSE_REQUESTED     1
#define WAITING_FOR_PULSE_TO_END    2
#define END_OF_PULSE                3
void initial_rotor_position_pulses(float psi_s_alpha,float *psisD,float*psisQ,float *VsD,float *VsQ,float *Vs, float *cita_Vs,
                                    float initial_stator_voltage, float initial_rotor_angle, 
                                    bool *initial_rotor_position_ignition, int maximum_pulse_ticks,bool shutdown_motor)
{

    static int initial_rotor_position_state = ROTOR_POSITION_UNKNOWN;
    static int pulse_tick_counter=0;

    if (initial_rotor_position_state== ROTOR_POSITION_UNKNOWN &&
        *initial_rotor_position_ignition==false)
    {

       *Vs=*Vs;
       *cita_Vs=*cita_Vs;
       *VsD=*VsD;
       *VsQ=*VsQ;
   
       *psisD=*psisD;
       *psisQ=*psisQ;
/*
       *Vs=0.0f;
       *cita_Vs=0.0f;
       *VsD=0.0f;
       *VsQ=0.0f;
*/


       pulse_tick_counter=0;
       initial_rotor_position_state=ROTOR_POSITION_UNKNOWN;
       *initial_rotor_position_ignition=false;
    }

    else if (initial_rotor_position_state== ROTOR_POSITION_UNKNOWN &&
            *initial_rotor_position_ignition==true && shutdown_motor ==false)
    {

       *Vs=*Vs;
       *cita_Vs=*cita_Vs;
       *VsD=*VsD;
       *VsQ=*VsQ;
       *psisD=*psisD;
       *psisQ=*psisQ;

/*
       *Vs=0.0f;
       *cita_Vs=0.0f;
       *VsD=0.0f;
       *VsQ=0.0f;
*/

       pulse_tick_counter=0;
       initial_rotor_position_state=ROTOR_POSITION_UNKNOWN;
       *initial_rotor_position_ignition=false;
    }


    else if (initial_rotor_position_state== ROTOR_POSITION_UNKNOWN &&
            *initial_rotor_position_ignition==true && shutdown_motor ==true)
    {
       *Vs    =initial_stator_voltage;
       *cita_Vs=initial_rotor_angle;
       *VsD=initial_stator_voltage;
       *VsQ=0.0f;

       *psisD=0.0f;
       *psisQ=0.0f;      

       pulse_tick_counter+=1;
       initial_rotor_position_state=WAITING_FOR_PULSE_TO_END;
       *initial_rotor_position_ignition=true;
    }

    else if (initial_rotor_position_state== WAITING_FOR_PULSE_TO_END &&
             pulse_tick_counter<maximum_pulse_ticks                   )
    {
       *Vs    =initial_stator_voltage;
       *cita_Vs=initial_rotor_angle;
       *VsD=initial_stator_voltage;
       *VsQ=0.0f;
       pulse_tick_counter+=1;
       initial_rotor_position_state=WAITING_FOR_PULSE_TO_END;
       *initial_rotor_position_ignition=true;
    }

    else if (initial_rotor_position_state== WAITING_FOR_PULSE_TO_END &&
                pulse_tick_counter>=maximum_pulse_ticks )
    {
       *Vs    =initial_stator_voltage;
       *cita_Vs=initial_rotor_angle;
       *VsD=initial_stator_voltage;
       *VsQ=0.0f;
       //*psisD=0.0f;
       //*psisQ=0.0f;   
       pulse_tick_counter=0;
       initial_rotor_position_state=ROTOR_POSITION_UNKNOWN;
       *initial_rotor_position_ignition=false;
    }
    
    else
    {

       *Vs=*Vs;
       *cita_Vs=*cita_Vs;
       *VsD=*VsD;
       *VsQ=*VsQ;


/*
       *Vs=0.0f;
       *cita_Vs=0.0f;
       *VsD=0.0f;
       *VsQ=0.0f;
*/
       pulse_tick_counter=0;
       initial_rotor_position_state=ROTOR_POSITION_UNKNOWN;
       *initial_rotor_position_ignition=false;
    }


}









#define ROTOR_POSITION_UNKNOWN               0

#define A_PULSE_START                        1
#define WAITING_FOR_A_PULSE_TO_END           2
#define NEGATIVE_A_PULSE_START               3
#define WAITING_FOR_NEGATIVE_A_PULSE_TO_END  4
#define A_DELAY                              5

#define B_PULSE_START                        6
#define WAITING_FOR_B_PULSE_TO_END           7
#define NEGATIVE_B_PULSE_START               8
#define WAITING_FOR_NEGATIVE_B_PULSE_TO_END  9
#define B_DELAY                             10

#define C_PULSE_START                       11
#define WAITING_FOR_C_PULSE_TO_END          12
#define NEGATIVE_C_PULSE_START              13
#define WAITING_FOR_NEGATIVE_C_PULSE_TO_END 14
#define C_DELAY                             15

#define END_OF_C_PULSE                      16





void initial_rotor_position_ABC_pulses(float psi_s_alpha,float *psisD,float*psisQ,float *VsD,float *VsQ,float *Vs, float *cita_Vs,
                                    float initial_stator_voltage, float initial_rotor_angle, 
                                    bool *initial_rotor_position_ignition, int short_maximum_pulse_ticks,bool shutdown_motor)
{

    static int initial_rotor_position_state = ROTOR_POSITION_UNKNOWN;
    static int pulse_tick_counter=0;

    if (initial_rotor_position_state== ROTOR_POSITION_UNKNOWN &&
        *initial_rotor_position_ignition==false)
    {

       *Vs=*Vs;
       *cita_Vs=*cita_Vs;
       *VsD=*VsD;
       *VsQ=*VsQ;
   
       *psisD=*psisD;
       *psisQ=*psisQ;

       pulse_tick_counter=0;
       initial_rotor_position_state=ROTOR_POSITION_UNKNOWN;
       *initial_rotor_position_ignition=false;
    }

    else if (initial_rotor_position_state== ROTOR_POSITION_UNKNOWN &&
            *initial_rotor_position_ignition==true && shutdown_motor ==false)
    {
       *Vs=*Vs;
       *cita_Vs=*cita_Vs;
       *VsD=*VsD;
       *VsQ=*VsQ;
       *psisD=*psisD;
       *psisQ=*psisQ;

       pulse_tick_counter=0;
       initial_rotor_position_state=ROTOR_POSITION_UNKNOWN;
       *initial_rotor_position_ignition=false;
    }

    else if (initial_rotor_position_state== ROTOR_POSITION_UNKNOWN &&
            *initial_rotor_position_ignition==true && shutdown_motor ==true)
    {
       *Vs    =initial_stator_voltage;
       *cita_Vs=0.0f;
       *VsD=initial_stator_voltage;
       *VsQ=0.0f;

       *psisD=0.0f;
       *psisQ=0.0f;      

       pulse_tick_counter+=1;
       initial_rotor_position_state=WAITING_FOR_A_PULSE_TO_END;
       *initial_rotor_position_ignition=true;
    }

    else if (initial_rotor_position_state== WAITING_FOR_A_PULSE_TO_END &&
             pulse_tick_counter<short_maximum_pulse_ticks                   )
    {
       *Vs    =initial_stator_voltage;
       *cita_Vs=0.0f;
       *VsD=initial_stator_voltage;
       *VsQ=0.0f;
       pulse_tick_counter+=1;
       initial_rotor_position_state=WAITING_FOR_A_PULSE_TO_END;
       *initial_rotor_position_ignition=true;
    }


    else if (initial_rotor_position_state== WAITING_FOR_A_PULSE_TO_END &&
                pulse_tick_counter>=short_maximum_pulse_ticks )
    {
       *Vs    =initial_stator_voltage;
       *cita_Vs=180.0f;
       *VsD=-initial_stator_voltage;
       *VsQ=0.0f;
       //*psisD=0.0f;
       //*psisQ=0.0f;   
       pulse_tick_counter=0;
       initial_rotor_position_state=WAITING_FOR_NEGATIVE_A_PULSE_TO_END;
       *initial_rotor_position_ignition=true;
    }
    
    else if (initial_rotor_position_state== WAITING_FOR_NEGATIVE_A_PULSE_TO_END &&
             pulse_tick_counter<short_maximum_pulse_ticks                   )
    {
       *Vs    =initial_stator_voltage;
       *cita_Vs=180.0f;
       *VsD=-initial_stator_voltage;
       *VsQ=0.0f;
       //*psisD=0.0f;
       //*psisQ=0.0f;   
       pulse_tick_counter+=1;
       initial_rotor_position_state=WAITING_FOR_NEGATIVE_A_PULSE_TO_END;
       *initial_rotor_position_ignition=true;
    }

    else if (initial_rotor_position_state== WAITING_FOR_NEGATIVE_A_PULSE_TO_END &&
                pulse_tick_counter>=short_maximum_pulse_ticks )
    {
       *Vs    =initial_stator_voltage;
       *cita_Vs=180.0f;
       *VsD=-initial_stator_voltage;
       *VsQ=0.0f;
       //*psisD=0.0f;
       //*psisQ=0.0f;   
       pulse_tick_counter=0;
       initial_rotor_position_state=A_DELAY;
       *initial_rotor_position_ignition=false;
    }





    else if (initial_rotor_position_state== A_DELAY &&
             pulse_tick_counter<off_delay                   )
    {
       *Vs     =0.0f;
       *cita_Vs=0.0f;
       *VsD=0.0f;
       *VsQ=0.0f;
       pulse_tick_counter+=1;
       initial_rotor_position_state=A_DELAY;
       *initial_rotor_position_ignition=true;
    }


    else if (initial_rotor_position_state== A_DELAY &&
                pulse_tick_counter>=off_delay )
    {
       *Vs    =0.0f;
       *cita_Vs=0.0f;
       *VsD=0.0f;
       *VsQ=0.0f;
       //*psisD=0.0f;
       //*psisQ=0.0f;   
       pulse_tick_counter=0;
       initial_rotor_position_state=WAITING_FOR_B_PULSE_TO_END;
       *initial_rotor_position_ignition=true;
    }
   

    else if (initial_rotor_position_state== WAITING_FOR_B_PULSE_TO_END &&
             pulse_tick_counter<short_maximum_pulse_ticks                   )
    {
       *Vs    =initial_stator_voltage;
       *cita_Vs=180.0f;
       *VsD=-initial_stator_voltage;
       *VsQ=0.0f;
       //*psisD=0.0f;
       //*psisQ=0.0f;   
       pulse_tick_counter+=1;
       initial_rotor_position_state=WAITING_FOR_NEGATIVE_A_PULSE_TO_END;
       *initial_rotor_position_ignition=true;
    }

    else if (initial_rotor_position_state== WAITING_FOR_NEGATIVE_A_PULSE_TO_END &&
                pulse_tick_counter>=short_maximum_pulse_ticks )
    {
       *Vs    =initial_stator_voltage;
       *cita_Vs=180.0f;
       *VsD=-initial_stator_voltage;
       *VsQ=0.0f;
       //*psisD=0.0f;
       //*psisQ=0.0f;   
       pulse_tick_counter=0;
       initial_rotor_position_state=ROTOR_POSITION_UNKNOWN;
       *initial_rotor_position_ignition=false;
    }


 
    else if (initial_rotor_position_state== WAITING_FOR_NEGATIVE_A_PULSE_TO_END &&
             pulse_tick_counter<short_maximum_pulse_ticks                   )
    {
       *Vs    =initial_stator_voltage;
       *cita_Vs=180.0f;
       *VsD=-initial_stator_voltage;
       *VsQ=0.0f;
       //*psisD=0.0f;
       //*psisQ=0.0f;   
       pulse_tick_counter+=1;
       initial_rotor_position_state=WAITING_FOR_NEGATIVE_A_PULSE_TO_END;
       *initial_rotor_position_ignition=true;
    }

    else if (initial_rotor_position_state== WAITING_FOR_NEGATIVE_A_PULSE_TO_END &&
                pulse_tick_counter>=short_maximum_pulse_ticks )
    {
       *Vs    =initial_stator_voltage;
       *cita_Vs=180.0f;
       *VsD=-initial_stator_voltage;
       *VsQ=0.0f;
       //*psisD=0.0f;
       //*psisQ=0.0f;   
       pulse_tick_counter=0;
       initial_rotor_position_state=ROTOR_POSITION_UNKNOWN;
       *initial_rotor_position_ignition=false;
    }











    else
    {

       *Vs=*Vs;
       *cita_Vs=*cita_Vs;
       *VsD=*VsD;
       *VsQ=*VsQ;

       pulse_tick_counter=0;
       initial_rotor_position_state=ROTOR_POSITION_UNKNOWN;
       *initial_rotor_position_ignition=false;
    }

}

