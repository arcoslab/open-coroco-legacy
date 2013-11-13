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

//pwm-related timer configuration
#define SYSFREQ                              168000000 //168MHz
#define PWMFREQ                                  50000//58000 //32000
#define PWMFREQ_F                  ((float )(PWMFREQ)) //32000.0f
#define PWMFREQ_CENTER_ALIGNED_F   (PWMFREQ_F/2.0f)            


#define PRESCALE                                                   1 //freq_CK_CNT=freq_CK_PSC/(PSC[15:0]+1)
#define PWM_PERIOD_ARR              SYSFREQ/( PWMFREQ*(PRESCALE+1) )
#define INIT_DUTY                                               0.5f
#define PI                                                   3.1416f
#define TICK_PERIOD                                   1.0f/PWMFREQ_F
#define MYUINT_MAX                                         536870912
#define t ticks/TICK_PERIOD


#define TIME_CITA ticks*TICK_PERIOD
#define CUR_FREQ 1.0f/(period*TICK_PERIOD)//1.0f/(period/TICK_PERIOD)   //**********:S*******//
#define CRAZY_FREQUENCY period/TICK_PERIOD
#define HALL_A() gpio_get(GPIOE, GPIO15);
