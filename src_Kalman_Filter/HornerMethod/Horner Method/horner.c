
/* Illustration of Horner's method */

#include <stdio.h>
#include <math.h>

#define NN 40        /* Check for a polynomial this size */
#define XVAL 0.50    /* Use this value */

double coeff[NN+7];  /* Polynomial coefficients */

/* Calculate                     */
/*   f(x) = cof[0] + x*cof[1]  +  x^2 * cof[2] */
/*       + ... + x^{n-1}*cof[n-1]  */
/*                               */
/* (The polynomial has n coefficients and */
/*    is of degree n-1)          */
/*                               */
/* Horner's method can be written:  */
/*   nn=n;                       (The number of coefficients)  */
/*   fval=cof[--nn];             (Fetch the last coefficient)  */
/*   while (--nn>=0)             (For each succeeding coefficient,  */
/*     fval=xx*fval + cof[nn];     multiply by xx and then add the coeff.) */
/*                               */
/* (See below for a worked-out example.)   */
/*                               */


void main(void)
 { int i,nn;  double fval, term;
   printf ("Compare Horner's method for evaluating a polynomial\n"
     "  with an alternative method.\n");
   printf ("Horner's method has N multiplications instead of 2N,\n"
     "  and has slightly less roundoff error.\n");
   printf ("\nUsing the %dth Taylor polynomial for -log(1-x)  (x=%g)\n",
     NN, XVAL);
   /* Fill in the coefficient vector with NN entries */
   /*   corresponding to a Taylor polynomial for -log(1-x) */
   coeff[0]=0.0;
   for (i=1; i<NN; i++)  coeff[i]=1.0/i;
   /* Evaluate the polynomial using Horner's method */
   nn=NN;  fval=coeff[--nn];   /* Fetch the highest-order coefficient */
   /* For all other coefficients, first multiply by xx */
   /*   and then add the coefficient: */
   while (--nn>=0)
     fval=XVAL*fval + coeff[nn];
   /* The TRUE VALUE of -log(1-XVAL) = log(2)  is stored in <math.h> */
   /*   as the constant M_LN2  */
   printf ("Polynomial value (Horner's method, n=%d): %12.10f\n", NN, fval);
   printf ("True value (n=infinity): %12.10f   Difference: %g\n",
     M_LN2, M_LN2-fval);
   /* Compare with a plausible method that requires twice as many */
   /*   multiplications: */
   fval=0.0;  term=1.0;
   for (i=0; i<NN; i++)
    { fval+=coeff[i]*term;  term*=XVAL; }
   printf ("\nAlternative polynomial value (n=%d): %12.10f\n", NN, fval);
   printf ("True value (n=infinity): %12.10f   Difference: %g\n",
     M_LN2, M_LN2-fval); }





/* Evaluation of a polynomial using Horner's method */

/* If   f(x) = 1 + 3x + 5x^2 + 7x^3:  */
/*                               */
/* Then  double cof[] = { 1, 3, 5, 7 };   nn=4;  */
/* At the first step:  fval=cof[--n] = cof[3] = 7   */
/* At steps in the loop:  */
/*  (n==2)        fval=xx*fval + cof[2] =  7x+5
/*  (n==1)        fval=xx*fval + cof[1] =  3+x(7x+5) = 3 + 5x + 7x^2
/*  (n==0)        fval=xx*fval + cof[0] =  1+x(7x^2+5x+3) */
/*                                         = 1 + 3x + 5x^2 + 7x^3
/*  (n==-1)       Exit loop, return fval */

double fpoly(double xx, double *cof, int nn)
 { double fval=cof[--nn];    /* Fetch the highest-order coefficient */
   while (--nn>=0)  fval=xx*fval + cof[nn];
   return fval; }


