/*
 * Copyright (C) 2013 ARCOS-Lab Universidad de Costa Rica
 * Author: Federico Ruiz Ugalde <memeruiz@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

void system_init(void);
void leds_init(void);
void spi_init(void);
void tim_init(void);
void serial_conf(void);
void update_est_freq(void);
inline int avg_filter(int);
inline int avg_filter2(int);
void pid_controller(void);
void gen_pwm(void);

#include "motor.h"
#include <math.h>

//pwm-related timer configuration
#define SYSFREQ     168000000 //168MHz
#define PWMFREQ        16000  //32000
#define PWMFREQ_F       ((float )(PWMFREQ)) //32000.0f
#define PRESCALE        1                                       //freq_CK_CNT=freq_CK_PSC/(PSC[15:0]+1)
#define PWM_PERIOD_ARR  (SYSFREQ/( PWMFREQ*(PRESCALE+1) ))
#define INIT_DUTY 0
#define TICK_PERIOD (1.0f/PWMFREQ_F)
#define PI 3.1416f
#define RAW_TO_RAD(RAW) ((RAW)*2.0f*PI/(1<<16))


