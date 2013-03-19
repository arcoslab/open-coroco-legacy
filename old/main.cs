#define F_CPU 10000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "UART.h"
#include "Hardware.h"
#include "Timer.h"
#include <avr/interrupt.h>
#include <math.h>

double freq=0;
double phase=0; //in rads
int dutyA;
double A=0.0;
#define maxValTimer 0x1ff
//#define Kv 900 //motor sky
#define Kv 62.5
//#define Kv 416
//#define maxFreq 570 //motor sky
#define maxFreq 39.68
//#define maxFreq 50
//#define P 0.1
#define Tc 0.000064*8
//#define maxFreq 264
//This speed is for 12.7Volts: Formula: Poles*Kv*V/60. Poles=3, Kv=900
int T=20000;
int closeLoop=0;

ISR(SIG_OVERFLOW1) {
  //We enter this routine with a frequency of 15625Hz
  //One count is 0.000064s
  //static double freq=400;
//  static double counter=0;
  static double time=0;
  static double phaseStatorOld=0;
  static double phaseRotorOld=0;
  int dutyB;
  int dutyC;
  static int t=-1;
  t+=1;
  double phaseRotor;
  double phaseStator;
  double vError=0;

  //Rotor and Stator phase calculations
  phaseStator=phaseStatorOld+2*M_PI/T;
  phaseRotor=phaseRotorOld+2*M_PI/T;

  //code for hall sensoring
  static int hall1Rise=0;
  static int hall1Old=-20000;
  static int hall1tOld=-30000; //To produce a small initial frequency
  int hall1;
  hall1=(int) ADC;
  if (hall1 > hall1Old) {
    hall1Rise=1;
  } 
  if (hall1 < hall1Old) {
    hall1Rise=0;
  }
  if ((hall1Old<0) && (hall1>0)) {
    //zero crossing rising
    //Fix phase to zero
    //Calculate new frequency
    T=(t-hall1tOld)/2; //Half a cycle
    hall1tOld=t;
    phaseRotor=0;
  }
  if ((hall1Old>0) && (hall1<0)) {
    //zero crossing falling
    //Fix phase to 180
    //Calculate new frequency
    T=(t-hall1tOld)/2; //Half a cycle
    hall1tOld=t;
    phaseRotor=M_PI;
  }

  if (closeLoop==1) {
    phaseStator=phaseRotor;
  } else {
    phaseRotor=phaseStator;
  }

  if (phaseStator>=2*M_PI) {
    phaseStator=phaseStator-2*M_PI;
    phaseRotor=phaseRotor-2*M_PI;
  }
  
  phaseRotorOld=phaseRotor;
  phaseStatorOld=phaseStator;
  
//  if (freq==0) {
//    time=0;
//  }
//  if (time*freq >=1) {
//    time=(1/freq)-time;
    //counter=0;
//  }

  dutyA=A*maxValTimer*sin(phaseStator+phase);
  dutyB=A*maxValTimer*sin(phaseStator-2.0*M_PI/3.0+phase);
  dutyC=A*maxValTimer*sin(phaseStator-4.0*M_PI/3.0+phase);
  
  //counter+=1;
//  time+=0.000064*8;
  if (dutyA<0) {
    OCR1A=0;
    OCR3A=-dutyA;
  } else {
    OCR1A = dutyA;
    OCR3A =0;    
  }
  if (dutyB<0) {
    OCR1B=0;
    OCR3B=-dutyB;
  } else {
    OCR1B = dutyB;
    OCR3B = 0;
  }
  if (dutyC<0) {
    OCR1C=0;
    OCR3C=-dutyC;
  } else {
    OCR1C = dutyC;
    OCR3C = 0;
  }
  
}

void delayms(uint16_t millis) {
  //uint16_t loop;
  while ( millis ) {
    _delay_ms(1);
    millis--;
  }
}

int main(void) {
  //int i;
  FILE *u0;
  double error;
  double perror;
  double dfreq=0;
  double tfreq;
  double dphase=0;
  int initStage=1;
  double P=0.1;
  
  InitHardware();
  
#if defined( __AVR_LIBC_VERSION__ )
  u0 = fdevopen( UART0_PutCharStdio, UART0_GetCharStdio );
#else  
  u0 = fdevopen( UART0_PutCharStdio, UART0_GetCharStdio, 0 );
#endif
  OCR1A = 0x00;
  OCR1B = 0;
  OCR1C = 0;
  OCR3A = 0;
  OCR3B= 0 ;
  OCR3C=0;
  timer13Start();
  ADCSR |= (1 << ADFR);
  ADMUX |= (1 << ADLAR);
  ADCSR |= (1 << ADSC);
  ADMUX |= (1 << MUX4);
  //ADCSR=0xC7;
  //ADCSR=0xE7;
  //ADMUX=0x00;
  
  while(1) {
    LED_ON( BLUE );
    LED_OFF( RED );
    //printf("Led Blue On \n");
    delayms(50);
    LED_OFF( BLUE );
    LED_ON( RED );
    //printf("Led Blue OFF \n");
    //TCNT1=0;
    printf("Timer value: %04x ", TCNT1);
    printf("Duty Cycle: %d \n", dutyA);
    printf("ADC0 input: %d\n", (int) ADC);
    //printf("ADCSR: %04x \n", ADCSR);
    //printf("Control Registers 1: %02x 2: %02x 3: %02x\n", TCCR1A, TCCR1B, TCCR1C);
    //printf("XVID crystal divider %02x\n",XDIV);
    
    
    
    /*for ( i = 0; i < 100; i++ ) 
    {
      WaitForTimer0Rollover();
    }*/
    
     
    if ( UART0_IsCharAvailable() )
    {
                char    ch = getchar();

                printf( "Read: '%c'\n", ch );

                if ( ch == ' ' )
                {
                    printf( "*** Set the new frequency\n" );
                    //ch = getchar();
                    int rfreq;
                    scanf("%d",&rfreq);
                    printf( "*** Received %d. Continuing...\n",rfreq );
                    if (rfreq==0) {
                      freq=0;
                      printf( "*** Set the phase\n");
                      int rphase;
                      scanf("%d",&rphase);
                      printf( "*** Received %d. Continuing...\n",rphase);
                      dphase=rphase*2*M_PI/360;
                    }
                    //dfreq=rfreq;
                    A=(double) rfreq;
                    //freq=dfreq;
                    //A=freq/maxFreq;
                    //printf("Setting Amplitud to: %d \n",(int) (100*A));
                }
    }
    if (initStage==1) {
      printf("Starting up!\n");
      closeLoop=0;
      dfreq=10;
      A=0.8;
      P=0.05;
      dphase=0;
      perror=dphase-phase;
      phase=phase+P*perror;
      error=dfreq-freq;
      if (error>8.0) {
        error=8.0;
      }
      if (error<-8.0) {
        error=-8.0;
      }
      tfreq=freq+P*error;
      freq=tfreq;
      T=1/freq;
      if ((error<0.4) && (error>-0.4)) {
        initStage=0;
      }
    } else {
      printf("Doing nothing\n");
      closeLoop=1;
    }
    //A=tfreq/maxFreq;
    //if (A<0.6) {
    //  A=0.6;
    //}
    //if (freq>maxFreq) {
    //  A=1.0;
    //}
    printf("Phase: %d \n", (int) (phase*360/(2*M_PI)));
    printf("Frequency: %d, Dest Freq: %d \n",(int) freq, (int) dfreq);
    delayms(50);
  }
  return 0;
}
