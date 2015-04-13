/******************************************************************************
*   MSP430FG439 Horner's method to perform efficient multiplication
*
*   Description: This code performs a integer-integert multplication of numbers 
*                41 and 441 to give the result 18081. The main C file calls
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
*   K. Venkat
*   Texas Instruments Inc.
*   July 2006
*   Built with IAR Embedded Workbench Version: 3.41A
********************************************************************************/
#include  <stdio.h>
#include  <msp430xG43x.h>
int mul_c_int(int,int);
int mul_hamacher(register int, register int); 
int mul_horner_int(register int);
int mul_csd_int(register int);
int input=41,mult=441;
int result1, result2,result3,result4;
main()
{
    
    WDTCTL = WDTPW + WDTHOLD;         //    Stop WDT.
    FLL_CTL0 |= XCAP18PF;             //    Configure load caps.
    
    result1=mul_c_int(input,mult);    //    C routine called for multiplication.
        
    result2=mul_hamacher(input,mult); //    MSP430 Assembly routine for algorithm. 
                                      //    explained for multiplication in Hamacher etal. 
    result3=mul_horner_int(input);    //    MSP430 Assembly routine with Horner's
                                      //    method for multiplication.    
    result4=mul_csd_int(input);       //    MSP430 Assembly routine with Horner's
                                      //    method using CSD format for multiplication.

    printf("%d\n%d\n%d\n%d\n",result1,result2,result3,result4);
}

int mul_c_int(int x, int y)           //    C routine for multiplication
{
      return(x*y);
}


   