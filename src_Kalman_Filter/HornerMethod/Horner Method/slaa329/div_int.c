/******************************************************************************
*   MSP430FG439 Horner's method to perform efficient division
*
*   Description: This code performs a integer-integer division of number 9280 by 
*                41. The main C file calls four division functions. 
*                The first is a IAR CLIB function followed by a conventional 
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
********************************************************************************/
#include  <stdio.h>
#include  <msp430xG43x.h>
int div_c_int(int,int);
int div_horner_int(register int);
int div_csd_int(register int);
int div_hamacher(register int, register int);
int input=9280;
int divisor=41;
int result1,result2,result3,result4;

main()
{
    WDTCTL = WDTPW + WDTHOLD;            //  Stop WDT.
    FLL_CTL0 |= XCAP18PF;                //  Configure load caps.
    result1=div_c(input,divisor);        //  C routine called for division
                                        
    result2=div_hamacher(input,divisor); //  Division by the algorithm in Hamacher
    result3=div_horner_int(input);       //  MSP430 Assembly routine with Horner's
                                         //  method for division    
    result4=div_csd_int(input);          //  MSP430 Assembly routine with Horner's
                                         //  method using CSD format for division        
    printf("%d\n%d\n%d\n%d\n",result1,result2,result3,result4);
}

int div_c(int x, int y)                  //  C routine for division
{
      return(x/y);
}


   