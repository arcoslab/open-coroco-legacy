#include <stdio.h>
#include <math.h>
#include <number_representation_convertion.c>
//#include 'number_representation_convertion.c'

//This function is for converting a floating point
//number representated to fixed representation

unsigned short int float2fix(float n)

//This function is for converting a fixed point
//number representated to floating representation

float fix2float(unsigned short int n)


//In the DTC.c file, all of the additions can be done by using the CLIB, either if you pass the float number or the fixed number and then turns back to the float number.
//But in multiplications and division, the float2fix representations does not work yet
