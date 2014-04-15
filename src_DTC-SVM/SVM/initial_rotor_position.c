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
void initial_rotor_position_pulses(float *psisD,float*psisQ,float *VsD,float *VsQ,float *Vs, float *cita_Vs,
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

#define INITIAL_DELAY                       17

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






void do_nothing(float* Vs,float* cita_Vs,float* VsD, float* VsQ, float *psisD, float* psisQ)
{
       *Vs=*Vs;
       *cita_Vs=*cita_Vs;
       *VsD=*VsD;
       *VsQ=*VsQ;
   
       *psisD=*psisD;
       *psisQ=*psisQ;
}

void A_pulse_voltage_vector(float* Vs,float* cita_Vs,float* VsD, float* VsQ, float *psisD, float* psisQ,float initial_stator_voltage)
{
       *Vs    =initial_stator_voltage;
       *cita_Vs=0.0f;
       *VsD=initial_stator_voltage;
       *VsQ=0.0f;

       *psisD=*psisD;
       *psisQ=*psisQ;      
}

void negative_A_pulse_voltage_vector(float* Vs,float* cita_Vs,float* VsD, float* VsQ, float *psisD, float* psisQ,float initial_stator_voltage)
{
       *Vs    =initial_stator_voltage;
       *cita_Vs=180.0f;
       *VsD=-initial_stator_voltage;
       *VsQ=0.0f;
       *psisD=*psisD;
       *psisQ=*psisQ;    
}

void B_pulse_voltage_vector(float* Vs,float* cita_Vs,float* VsD, float* VsQ, float *psisD, float* psisQ,float initial_stator_voltage)
{
       *Vs    =initial_stator_voltage;
       *cita_Vs=120.0f;
       *VsD=initial_stator_voltage*fast_cos(120.0f);
       *VsQ=initial_stator_voltage*fast_sine(120.0f);

       *psisD=*psisD;
       *psisQ=*psisQ;      
}

void negative_B_pulse_voltage_vector(float* Vs,float* cita_Vs,float* VsD, float* VsQ, float *psisD, float* psisQ,float initial_stator_voltage)
{
       *Vs    =initial_stator_voltage;
       *cita_Vs=300.0f;
       *VsD=initial_stator_voltage*fast_cos(300.0f);
       *VsQ=initial_stator_voltage*fast_sine(300.0f);

       *psisD=*psisD;
       *psisQ=*psisQ;       
}

void C_pulse_voltage_vector(float* Vs,float* cita_Vs,float* VsD, float* VsQ, float *psisD, float* psisQ,float initial_stator_voltage)
{
       *Vs    =initial_stator_voltage;
       *cita_Vs=240.0f;
       *VsD=initial_stator_voltage*fast_cos(240.0f);
       *VsQ=initial_stator_voltage*fast_sine(240.0f);

       *psisD=*psisD;
       *psisQ=*psisQ;      
}

void negative_C_pulse_voltage_vector(float* Vs,float* cita_Vs,float* VsD, float* VsQ, float *psisD, float* psisQ,float initial_stator_voltage)
{
       *Vs    =initial_stator_voltage;
       *cita_Vs=60.0f;
       *VsD=initial_stator_voltage*fast_cos(60.0f);
       *VsQ=initial_stator_voltage*fast_sine(60.0f);

       *psisD=*psisD;
       *psisQ=*psisQ;       
}




void zero_voltage_vector(float* Vs,float* cita_Vs,float* VsD, float* VsQ, float *psisD, float* psisQ)
{
       *Vs    =0.0f;
       *cita_Vs=0.0f;
       *VsD=0.0f;
       *VsQ=0.0f;
       *psisD=*psisD;
       *psisQ=*psisQ;    
}






void initial_rotor_position_ABC_pulses(float *psisD,float*psisQ,float *VsD,float *VsQ,float *Vs, float *cita_Vs,
                                    float initial_stator_voltage, 
                                    bool *initial_rotor_position_ignition, int short_maximum_pulse_ticks,int off_delay,bool shutdown_motor)
{

    static int initial_rotor_position_state = ROTOR_POSITION_UNKNOWN;
    static int pulse_tick_counter=0;

    if (initial_rotor_position_state== ROTOR_POSITION_UNKNOWN &&
        *initial_rotor_position_ignition==false)                {
                                                                   do_nothing(Vs,cita_Vs,VsD,VsQ,psisD,psisQ);
                                                                   pulse_tick_counter=0;
                                                                   initial_rotor_position_state=ROTOR_POSITION_UNKNOWN;
                                                                   *initial_rotor_position_ignition=false;
                                                                }

    else if (initial_rotor_position_state== ROTOR_POSITION_UNKNOWN &&
            *initial_rotor_position_ignition==true && shutdown_motor ==false)
                                                                {
                                                                   do_nothing(Vs,cita_Vs,VsD,VsQ,psisD,psisQ);
                                                                   pulse_tick_counter=0;
                                                                   initial_rotor_position_state=ROTOR_POSITION_UNKNOWN;
                                                                   *initial_rotor_position_ignition=false;
                                                                }

    else if (initial_rotor_position_state== ROTOR_POSITION_UNKNOWN &&
            *initial_rotor_position_ignition==true && shutdown_motor ==true)
                                                                {
                                                                   zero_voltage_vector(Vs,cita_Vs,VsD,VsQ,psisD,psisQ);
                                                                   pulse_tick_counter=0;
                                                                   initial_rotor_position_state=INITIAL_DELAY;//WAITING_FOR_A_PULSE_TO_END;//INITIAL_DELAY;
                                                                   *initial_rotor_position_ignition=true;
                                                                }

    else if (initial_rotor_position_state== INITIAL_DELAY &&
             pulse_tick_counter<off_delay                   )
                                                            {
                                                               zero_voltage_vector(Vs,cita_Vs,VsD,VsQ,psisD,psisQ);  
                                                               pulse_tick_counter+=1;
                                                               initial_rotor_position_state=INITIAL_DELAY;
                                                               *initial_rotor_position_ignition=true;
                                                            }


    else if (initial_rotor_position_state== INITIAL_DELAY &&
                pulse_tick_counter>=off_delay )
                                                            {
                                                               A_pulse_voltage_vector(Vs,cita_Vs,VsD,VsQ,psisD,psisQ,initial_stator_voltage);    
                                                               pulse_tick_counter=0;
                                                               initial_rotor_position_state=WAITING_FOR_A_PULSE_TO_END;
                                                               *initial_rotor_position_ignition=true;
                                                            }


    else if (initial_rotor_position_state== WAITING_FOR_A_PULSE_TO_END &&
             pulse_tick_counter<short_maximum_pulse_ticks )
                                                                {
                                                                   A_pulse_voltage_vector(Vs,cita_Vs,VsD,VsQ,psisD,psisQ,initial_stator_voltage);
                                                                   pulse_tick_counter+=1;
                                                                   initial_rotor_position_state=WAITING_FOR_A_PULSE_TO_END;
                                                                   *initial_rotor_position_ignition=true;
                                                                }


    else if (initial_rotor_position_state== WAITING_FOR_A_PULSE_TO_END &&
                pulse_tick_counter>=short_maximum_pulse_ticks )
                                                                {
                                                                   negative_A_pulse_voltage_vector(Vs,cita_Vs,VsD,VsQ,psisD,psisQ,initial_stator_voltage);
                                                                   pulse_tick_counter=0;
                                                                   initial_rotor_position_state=WAITING_FOR_NEGATIVE_A_PULSE_TO_END;
                                                                   *initial_rotor_position_ignition=true;
                                                                }
    
    else if (initial_rotor_position_state== WAITING_FOR_NEGATIVE_A_PULSE_TO_END &&
             pulse_tick_counter<short_maximum_pulse_ticks                   )
                                                                {
                                                                   negative_A_pulse_voltage_vector(Vs,cita_Vs,VsD,VsQ,psisD,psisQ,initial_stator_voltage);   
                                                                   pulse_tick_counter+=1;
                                                                   initial_rotor_position_state=WAITING_FOR_NEGATIVE_A_PULSE_TO_END;
                                                                   *initial_rotor_position_ignition=true;
                                                                }

    else if (initial_rotor_position_state== WAITING_FOR_NEGATIVE_A_PULSE_TO_END &&
                pulse_tick_counter>=short_maximum_pulse_ticks )
                                                                {
                                                                   zero_voltage_vector(Vs,cita_Vs,VsD,VsQ,psisD,psisQ);  
                                                                   pulse_tick_counter=0;
                                                                   initial_rotor_position_state=A_DELAY;
                                                                   *initial_rotor_position_ignition=true;
                                                                }

    else if (initial_rotor_position_state== A_DELAY &&
             pulse_tick_counter<off_delay                   )
                                                            {
                                                               zero_voltage_vector(Vs,cita_Vs,VsD,VsQ,psisD,psisQ);  
                                                               pulse_tick_counter+=1;
                                                               initial_rotor_position_state=A_DELAY;
                                                               *initial_rotor_position_ignition=true;
                                                            }


    else if (initial_rotor_position_state== A_DELAY &&
                pulse_tick_counter>=off_delay )
                                                            {
                                                               B_pulse_voltage_vector(Vs,cita_Vs,VsD,VsQ,psisD,psisQ,initial_stator_voltage);    
                                                               pulse_tick_counter=0;
                                                               initial_rotor_position_state=WAITING_FOR_B_PULSE_TO_END;
                                                               *initial_rotor_position_ignition=true;
                                                            }
                                                           

    else if (initial_rotor_position_state== WAITING_FOR_B_PULSE_TO_END &&
             pulse_tick_counter<short_maximum_pulse_ticks                   )
                                                            {
                                                               B_pulse_voltage_vector(Vs,cita_Vs,VsD,VsQ,psisD,psisQ,initial_stator_voltage);     
                                                               pulse_tick_counter+=1;
                                                               initial_rotor_position_state=WAITING_FOR_B_PULSE_TO_END;
                                                               *initial_rotor_position_ignition=true;
                                                            }

    else if (initial_rotor_position_state== WAITING_FOR_B_PULSE_TO_END &&
                pulse_tick_counter>=short_maximum_pulse_ticks )
                                                            {
                                                               negative_B_pulse_voltage_vector(Vs,cita_Vs,VsD,VsQ,psisD,psisQ,initial_stator_voltage);   
                                                               pulse_tick_counter=0;
                                                               initial_rotor_position_state=WAITING_FOR_NEGATIVE_B_PULSE_TO_END;
                                                               *initial_rotor_position_ignition=true;
                                                            }
 
    else if (initial_rotor_position_state== WAITING_FOR_NEGATIVE_B_PULSE_TO_END &&
             pulse_tick_counter<short_maximum_pulse_ticks                   )
                                                            {
                                                               negative_B_pulse_voltage_vector(Vs,cita_Vs,VsD,VsQ,psisD,psisQ,initial_stator_voltage);     
                                                               pulse_tick_counter+=1;
                                                               initial_rotor_position_state=WAITING_FOR_NEGATIVE_B_PULSE_TO_END;
                                                               *initial_rotor_position_ignition=true;
                                                            }

    else if (initial_rotor_position_state== WAITING_FOR_NEGATIVE_B_PULSE_TO_END &&
                pulse_tick_counter>=short_maximum_pulse_ticks )
                                                            {
                                                               zero_voltage_vector(Vs,cita_Vs,VsD,VsQ,psisD,psisQ);  
                                                               initial_rotor_position_state=B_DELAY;
                                                               *initial_rotor_position_ignition=true;
                                                            }

    else if (initial_rotor_position_state== B_DELAY &&
             pulse_tick_counter<off_delay                   )
                                                            {
                                                               zero_voltage_vector(Vs,cita_Vs,VsD,VsQ,psisD,psisQ);  
                                                               pulse_tick_counter+=1;
                                                               initial_rotor_position_state=B_DELAY;
                                                               *initial_rotor_position_ignition=true;
                                                            }


    else if (initial_rotor_position_state== B_DELAY &&
                pulse_tick_counter>=off_delay )
                                                            {
                                                               C_pulse_voltage_vector(Vs,cita_Vs,VsD,VsQ,psisD,psisQ,initial_stator_voltage);    
                                                               pulse_tick_counter=0;
                                                               initial_rotor_position_state=WAITING_FOR_C_PULSE_TO_END;
                                                               *initial_rotor_position_ignition=true;
                                                            }
                                                           

    else if (initial_rotor_position_state== WAITING_FOR_C_PULSE_TO_END &&
             pulse_tick_counter<short_maximum_pulse_ticks                   )
                                                            {
                                                               C_pulse_voltage_vector(Vs,cita_Vs,VsD,VsQ,psisD,psisQ,initial_stator_voltage);     
                                                               pulse_tick_counter+=1;
                                                               initial_rotor_position_state=WAITING_FOR_C_PULSE_TO_END;
                                                               *initial_rotor_position_ignition=true;
                                                            }

    else if (initial_rotor_position_state== WAITING_FOR_C_PULSE_TO_END &&
                pulse_tick_counter>=short_maximum_pulse_ticks )
                                                            {
                                                               negative_C_pulse_voltage_vector(Vs,cita_Vs,VsD,VsQ,psisD,psisQ,initial_stator_voltage);   
                                                               pulse_tick_counter=0;
                                                               initial_rotor_position_state=WAITING_FOR_NEGATIVE_C_PULSE_TO_END;
                                                               *initial_rotor_position_ignition=true;
                                                            }
 
    else if (initial_rotor_position_state== WAITING_FOR_NEGATIVE_C_PULSE_TO_END &&
             pulse_tick_counter<short_maximum_pulse_ticks                   )
                                                            {
                                                               negative_C_pulse_voltage_vector(Vs,cita_Vs,VsD,VsQ,psisD,psisQ,initial_stator_voltage);     
                                                               pulse_tick_counter+=1;
                                                               initial_rotor_position_state=WAITING_FOR_NEGATIVE_C_PULSE_TO_END;
                                                               *initial_rotor_position_ignition=true;
                                                            }

    else if (initial_rotor_position_state== WAITING_FOR_NEGATIVE_C_PULSE_TO_END &&
                pulse_tick_counter>=short_maximum_pulse_ticks )
                                                            {
                                                               zero_voltage_vector(Vs,cita_Vs,VsD,VsQ,psisD,psisQ);  
                                                               initial_rotor_position_state=C_DELAY;
                                                               *initial_rotor_position_ignition=true;
                                                            }

    else if (initial_rotor_position_state== C_DELAY &&
             pulse_tick_counter<off_delay                   )
                                                            {
                                                               zero_voltage_vector(Vs,cita_Vs,VsD,VsQ,psisD,psisQ);  
                                                               pulse_tick_counter+=1;
                                                               initial_rotor_position_state=C_DELAY;
                                                               *initial_rotor_position_ignition=true;
                                                            }


    else if (initial_rotor_position_state== C_DELAY &&
                pulse_tick_counter>=off_delay )
                                                            {
                                                               zero_voltage_vector(Vs,cita_Vs,VsD,VsQ,psisD,psisQ);    
                                                               pulse_tick_counter=0;
                                                               initial_rotor_position_state=ROTOR_POSITION_UNKNOWN;
                                                               *initial_rotor_position_ignition=false;
                                                            }


    else
                                                            {
                                                               do_nothing(Vs,cita_Vs,VsD,VsQ,psisD,psisQ);
                                                               pulse_tick_counter=0;
                                                               initial_rotor_position_state=ROTOR_POSITION_UNKNOWN;
                                                               *initial_rotor_position_ignition=false;
                                                            }

}

