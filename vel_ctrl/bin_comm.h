#include <stdio.h>

typedef struct Out{
  int16_t cur_angle;
  uint8_t ad2s1210_fault;
  int16_t est_freq;
  uint16_t current[4];
  uint16_t voltage[4];
  uint16_t voltage_neutral;
  uint16_t strain_gauge;
  uint16_t temp;
} Out;

typedef struct In{
  short int *ref_freq;
} In;
