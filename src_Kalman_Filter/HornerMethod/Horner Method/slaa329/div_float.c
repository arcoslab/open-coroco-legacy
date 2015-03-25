/******************************************************************************
*   MSP430FG439 Horner's method to perform efficient division
*
*   Description: This code performs a integer-float division of number 1500 by 
*                37.12345 . The main C file calls four division functions. 
*                The first is an IAR CLIB function followed by a conventional 
*                division algorithm, Horner's method and Horner's method with 
*                CSD format. 
*
*   Note:        This code assumes 32.768kHz XTAL on LFXT1 on the MSP430FG439 
*
*                 MSP430FG439
*               -----------------
*              |              XIN|-  
*              |                 | 32.768kHz
*              |             XOUT|-
*              |                 |
*
*
*   K. Venkat
*   Texas Instruments Inc.
*   July 2006
*   Built with IAR Embedded Workbench Version: 3.41A
*******************************************************************************/
#include  <stdio.h>
#include  <msp430xG43x.h>
float div_c_float(int,float);
int div_horner_float(register int);
int div_csd_float(register int);
int div_hamacher(register int, register int);
int input=1500;
float divisor_float=37.12345;
int divisor;
float result1;
int result2,result3,result4;

main()
{
    WDTCTL = WDTPW + WDTHOLD;               //  Stop WDT.
    FLL_CTL0 |= XCAP18PF;                   //  Configure load caps.
    divisor=(int)(divisor_float);           //  Type conversion done as per 
                                            //  requirement for Conventional algorithm 
    result1=div_c_float(input,divisor_float);// C routine called for division
                                            //  algorithm for division
    
    result2=div_hamacher(input, divisor);   //  Algortithm from Hamacher
    result3=div_horner_float(input);        //  MSP430 Assembly routine with Horner's
                                            //  method for division    
    result4=div_csd_float(input);           //  MSP430 Assembly routine with Horner's
                                            //  method using CSD format for division    
    printf("%f\n%d\n%d\n%d\n",result1,result2,result3,result4);    
}

float div_c_float(int x, float y)           //  C routine for division
{
      return(x/y);
}


   
