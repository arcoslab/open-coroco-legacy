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
#include "sine_table.h"
//#include "cos_table.h"
#include "atan_table.h"

float fast_sine(float angle);
float fast_cos(float angle); 
float fast_atan(float tan_value);
float sine_linear_interpolation(float angle, int rounded_angle);
float atan_linear_interpolation(float transformed_tan,int rounded_tan);
int corrected_rounded_angle(int rounded_angle);
int corrected_atan_rounded_angle(int rounded_angle);


int corrected_rounded_angle(int rounded_angle)
{
  if (rounded_angle>SIN_TABLE_SIZE_F)   {rounded_angle=rounded_angle-SIN_TABLE_SIZE_F;}
  if (rounded_angle<0               )   {rounded_angle=rounded_angle+SIN_TABLE_SIZE_F;}

  return rounded_angle;
}

int corrected_atan_rounded_angle(int rounded_angle)
{
  if (rounded_angle>ATAN_TABLE_SIZE_F)   {rounded_angle=rounded_angle-ATAN_TABLE_SIZE_F;}
  if (rounded_angle<0               )   {rounded_angle=rounded_angle+ATAN_TABLE_SIZE_F;}

  return rounded_angle;
}

/*
float sine_linear_interpolation(float angle, int rounded_angle)
{
  float fixed_angle;
  




  if (angle>=rounded_angle) 
  {

    fixed_angle =  sine_table[rounded_angle]+
                  (sine_table[rounded_angle+1]-sine_table[rounded_angle])*
                  (angle-(float)(rounded_angle))/1.0f;//(rounded_angle+1-rounded_angle);
  }
  else
  {
    fixed_angle =  sine_table[rounded_angle-1]+
                  (sine_table[rounded_angle]-sine_table[rounded_angle-1])*
                  (angle-(float)(rounded_angle-1))/1.0f;//(rounded_angle-(rounded_angle-1));
  }


  return fixed_angle;
}
*/

float sine_linear_interpolation(float angle, int rounded_angle)
{
  float fixed_angle;
  /*
  float rounded_minus=0;
  float rounded_plus =0;
  rounded_plus=rounded_angle+1;
  rounded_minus=rounded_angle-1;


  if (rounded_angle<0) rounded_angle=0;
  if (rounded_angle>ATAN_TABLE_SIZE_F) rounded_angle=ATAN_TABLE_SIZE_F;

  if (rounded_minus<0) rounded_minus=0;
  if (rounded_minus>ATAN_TABLE_SIZE_F) rounded_minus=ATAN_TABLE_SIZE_F;

    if (rounded_plus<0) rounded_plus=0;
  if (rounded_plus>ATAN_TABLE_SIZE_F) rounded_plus=ATAN_TABLE_SIZE_F;
  
  if (angle>=rounded_angle) 
  {

    fixed_angle =  sine_table[corrected_rounded_angle(rounded_angle)  ]+
                  (sine_table[corrected_rounded_angle(rounded_plus)]-sine_table[corrected_rounded_angle(rounded_angle)])*
                  (angle-(float)(rounded_angle))/1.0f;//(rounded_angle+1-rounded_angle);
  }
  else
  {
    fixed_angle =  sine_table[corrected_rounded_angle(rounded_minus)]+
                  (sine_table[corrected_rounded_angle(rounded_angle  )]-sine_table[corrected_rounded_angle(rounded_angle-1)])*
                  (angle-(float)(rounded_minus))/1.0f;//(rounded_angle-(rounded_angle-1));
  }
*/

  if (angle>=rounded_angle) 
  {

    fixed_angle =  sine_table[corrected_rounded_angle(rounded_angle)  ]+
                  (sine_table[corrected_rounded_angle(rounded_angle+1)]-sine_table[corrected_rounded_angle(rounded_angle)])*
                  (angle-(float)(rounded_angle))/1.0f;//(rounded_angle+1-rounded_angle);
  }
  else
  {
    fixed_angle =  sine_table[corrected_rounded_angle(rounded_angle-1)]+
                  (sine_table[corrected_rounded_angle(rounded_angle  )]-sine_table[corrected_rounded_angle(rounded_angle-1)])*
                  (angle-(float)(rounded_angle-1))/1.0f;//(rounded_angle-(rounded_angle-1));
  }


  return fixed_angle;
}

float fast_sine(float angle)
{
  int rounded_angle; 
  float transformed_angle;
 
  while (angle>=360.0f) { angle=angle-360.0f; }
  while (angle<   0.0f) { angle=angle+360.0f; }
  
  if      (angle>=  0.0f && angle< 90.0f) { 
                                             transformed_angle=angle*SIN_TABLE_SIZE_F/90.0f;
                                             rounded_angle=(( int )(transformed_angle));
                                             return sine_linear_interpolation(transformed_angle,rounded_angle);//sine_table[rounded_angle]; 
                                          }

  else if (angle>= 90.0f && angle<180.0f) { 
                                              angle=180.0f-angle;
                                              transformed_angle=angle*SIN_TABLE_SIZE_F/90.0f;
                                              rounded_angle=(( int )(transformed_angle));
                                              return sine_linear_interpolation(transformed_angle,rounded_angle);
                                              //rounded_angle=(( int )(angle*SIN_TABLE_SIZE_F/90.0f));
                                              //return sine_table[rounded_angle]; 
                                          }

  else if (angle>=180.0f && angle<270.0f) { 
                                              angle=angle-180.0f;
                                              transformed_angle=angle*SIN_TABLE_SIZE_F/90.0f;
                                              rounded_angle=(( int )(transformed_angle));
                                              return -sine_linear_interpolation(transformed_angle,rounded_angle);
                                              //rounded_angle=(( int )(angle*SIN_TABLE_SIZE_F/90.0f));
                                              //return -sine_table[rounded_angle]; 
                                          }

  else if (angle>=270.0f && angle<360.0f) { 
                                              angle=360.0f-angle;//angle-270.0f;
                                              transformed_angle=angle*SIN_TABLE_SIZE_F/90.0f;
                                              rounded_angle=(( int )(transformed_angle));
                                              return -sine_linear_interpolation(transformed_angle,rounded_angle);
                                              //rounded_angle=(( int )(angle*SIN_TABLE_SIZE_F/90.0f));
                                              //return -sine_table[rounded_angle]; 
                                          }
  else 
                                          {
                                              return 3.0f;
                                          }

  //rounded_angle=(( int )(angle*SIN_TABLE_SIZE_F/360.0f));
  
  //return sine_table[rounded_angle];

}


/*
float fast_sine(float angle)
{
  int rounded_angle; 
 
  while (angle>=360.0f) { angle=angle-360.0f; }
  while (angle<   0.0f) { angle=angle+360.0f; }
  
  if      (angle>=  0.0f && angle< 90.0f) { 
                                             rounded_angle=(( int )(angle*SIN_TABLE_SIZE_F/90.0f));
                                             return sine_table[rounded_angle]; 
                                          }

  else if (angle>= 90.0f && angle<180.0f) { 
                                              angle=180.0f-angle;
                                              rounded_angle=(( int )(angle*SIN_TABLE_SIZE_F/90.0f));
                                              return sine_table[rounded_angle]; 
                                          }

  else if (angle>=180.0f && angle<270.0f) { 
                                              angle=angle-180.0f;
                                              rounded_angle=(( int )(angle*SIN_TABLE_SIZE_F/90.0f));
                                              return -sine_table[rounded_angle]; 
                                          }

  else if (angle>=270.0f && angle<360.0f) { 
                                              angle=360.0f-angle;//angle-270.0f;
                                              rounded_angle=(( int )(angle*SIN_TABLE_SIZE_F/90.0f));
                                              return -sine_table[rounded_angle]; 
                                          }
  else 
                                          {
                                              return 3.0f;
                                          }

  //rounded_angle=(( int )(angle*SIN_TABLE_SIZE_F/360.0f));
  
  //return sine_table[rounded_angle];

}
*/




/*
float fast_sine(float angle)
{
  int rounded_angle; 
 
  while (angle>=360.0f) { angle=angle-360.0f; }
  while (angle<   0.0f) { angle=angle+360.0f; }
  
  rounded_angle=(( int )(angle*SIN_TABLE_SIZE_F/360.0f));
  
  return sine_table[rounded_angle];

}
*/
/*
float fast_cos(float angle)
{
  int rounded_angle; 


  while (angle>=360.0f) { angle=angle-360.0f; }
  while (angle<   0.0f) { angle=angle+360.0f; }

 
  rounded_angle=(( int )(angle*COS_TABLE_SIZE_F/360.0f));

  

  return cos_table[rounded_angle];
}
*/

float fast_cos(float angle)
{
  //int rounded_angle; 

 
  //rounded_angle=(( int )( (angle+90.0f)*SIN_TABLE_SIZE_F/360.0f));

  

  return fast_sine(angle+90.0f);//sine_table[rounded_angle];
}



/*
float fast_atan(float tan_value)
{
  int rounded_tan;
  rounded_tan=(( int )(tan_value*ATAN_TABLE_SIZE_F));

  
  if  (rounded_tan<0.0f || rounded_tan>ATAN_TABLE_SIZE) { return 0.0f; }
  else                                                  { return atan_table[rounded_tan]; }
}
*/


/*
float atan_linear_interpolation(float transformed_tan,int rounded_tan)
{
  float fixed_atan;
  
  if (transformed_tan>=rounded_tan) 
  {

    fixed_atan =  atan_table[rounded_tan]+
                  (atan_table[rounded_tan+1]-atan_table[rounded_tan])*
                  (transformed_tan-(float)(rounded_tan))/1.0f;//(rounded_angle+1-rounded_angle);
  }
  else
  {
    fixed_atan =  atan_table[rounded_tan-1]+
                  (atan_table[rounded_tan]-atan_table[rounded_tan-1])*
                  (transformed_tan-(float)(rounded_tan-1))/1.0f;//(rounded_angle-(rounded_angle-1));



  }
*/


float atan_linear_interpolation(float transformed_tan,int rounded_tan)
{
  float fixed_atan;
  
  if (transformed_tan>=rounded_tan) 
  {
/*    fixed_angle =  sine_table[rounded_angle]+
                  (sine_table[rounded_angle+1]-sine_table[rounded_angle])*
                  (angle-rounded_angle)/(rounded_angle+1-rounded_angle);
*/
    fixed_atan =   atan_table[corrected_atan_rounded_angle(rounded_tan  )]+
                  (atan_table[corrected_atan_rounded_angle(rounded_tan+1)]-atan_table[corrected_atan_rounded_angle(rounded_tan)])*
                  (transformed_tan-(float)(rounded_tan))/1.0f;//(rounded_angle+1-rounded_angle);
  }
  else
  {
    fixed_atan =   atan_table[corrected_atan_rounded_angle(rounded_tan-1)]+
                  (atan_table[corrected_atan_rounded_angle(rounded_tan  )]-atan_table[corrected_atan_rounded_angle(rounded_tan-1)])*
                  (transformed_tan-(float)(rounded_tan-1))/1.0f;//(rounded_angle-(rounded_angle-1));
  }

  return fixed_atan;
}







float fast_atan(float tan_value)
{
  int rounded_tan;
  float transformed_tan;

  transformed_tan=tan_value*ATAN_TABLE_SIZE_F;
  rounded_tan=(( int )(transformed_tan));

  
  if  (rounded_tan<0.0f || rounded_tan>ATAN_TABLE_SIZE) { return 0.0f; }
  else                                                  { return atan_linear_interpolation(transformed_tan,rounded_tan);}//atan_table[rounded_tan]; }
}

