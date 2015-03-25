// ****************************************************************************
// This file the usage of the zip file slaa329.zip. included with the application 
// report "Efficient Multiplication and Division on the MSP430"
//*****************************************************************************

The package sllaa329.zip contains the following

1. 4 C-language files 
   multiply_int.c, multiply_float.c, div_int.c and div_float.c
     
2. 10 MSP430 Assembly files 
   mul.s43, horner_mul_int.s43, csd_mul_int.s43, horner_mul_float.s43, csd_mul_float.s43
   div.s43, horner_div_int.s43, csd_div_int.s43, horner_div_float.s43, csd_div_float.s43  
     
     Each of the four C-language files is used to generate the results obtained 
     in Table 1 of the Application Report. 
       
Procedure to run the tests: 

Integer-Integer Multiplication

1. Create a new project on the IAR Embedded Workbench Ver 3.41A 
   Include multiply_int.c,mul.s43, horner_mul_int.s43 and csd_mul_int.s43 in the
   current project. 
  
2. From the project options choose the MSP430FG439 device from the list.

3. Choose the FET Debugger or the Simulator from the project options.

4. Build the project.

5. Select the Terminal I/0 if results need to be seen or Select the variables 
   result1,result2,result3 and result4 and add to Watch Window.
     
6. Select profiler if the cycle time is required and enable profiling. 

Integer-Float Multiplication

1. Create a new project on the IAR Embedded Workbench Ver 3.41A 
   Include multiply_float.c,mul.s43, horner_mul_float.s43 and csd_mul_float.s43 in the
   current project.
     
 Follow Steps 2 through 6 as before. 
   
Integer-Integer Division

1. Create a new project on the IAR Embedded Workbench Ver 3.41A 
   Include div_int.c,div.s43, horner_div_int.s43 and csd_div_int.s43 in the
   current project.
     
 Follow Steps 2 through 6 as before.       
   
Integer-Float Division

1. Create a new project on the IAR Embedded Workbench Ver 3.41A 
   Include div_float.c,div.s43, horner_div_float.s43 and csd_div_float.s43 in the
   current project.
     
 Follow Steps 2 through 6 as before.    
   
   
   
