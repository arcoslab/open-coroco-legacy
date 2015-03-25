/******************************************************************************
*   MSP43FG439 Horner's method to perform efficient multiplication
*
*   Description: This code performs a integer-float multplication of numbers 
*                41 and 441.8375 to give the result 18115. The main C file calls
*                four multiplication functions. The first is a IAR CLIB function
*                followed by a conventional multiply algorithm, Horner's method 
*                and Horner's method with CSD format. 
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
*
*   K. Venkat
*   Texas Instruments Inc.
*   July 2006
*   Built with IAR Embedded Workbench Version: 3.41A
********************************************************************************/
#include  <stdio.h>
#include  <msp430xG43x.h>
float mul_c(int,float);
int mul_hamacher(register int, register int); 
int mul_horner_float(register int);
int mul_csd_float(register int);
int input=41,mult;
float mult_float=441.8375,result1;
int result2,result3,result4;
main()
{
    
    WDTCTL = WDTPW + WDTHOLD;         //    Stop WDT.
    FLL_CTL0 |= XCAP18PF;             //    Configure load caps.
    result1=mul_c(input,mult_float);  //    C routine called for multiplication.
    mult=(int)(mult_float);           //    Type conversion done to accomodate. 
                                      //    Algorithm explained in Hamacher etal. 
    
    result2=mul_hamacher(input,mult); //    MSP430 Assembly routine for algorithm. 
                                      //    explained for multiplication in Hamacher etal. 
    result3=mul_horner_float(input);  //    MSP430 Assembly routine with Horner's
                                      //    method for multiplication.    
    result4=mul_csd_float(input);     //    MSP430 Assembly routine with Horner's
                                      //    method using CSD format for multiplication.

    printf("%f\n%d\n%d\n%d\n",result1,result2,result3,result4);
}

float mul_c(int x, float y)           //    C routine for multiplication
{
      return(x*y);
}


   