/*
 *This file is part of the open-coroco project.
 *
 *  Copyright (C) 2013  Sebastian Chinchilla Gutierrez <tumacher@gmail.com>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#define INITIAL_STATOR_ANGLE_OPEN_LOOP	0
#define NO_HALL_UPDATE	 		1
#define FIRST_HALL_UPDATE		2
#define SECOND_HALL_UPDATE		3
#define OPEN_LOOP			4
#define CLOSE_LOOP 			5


#define MAX_AMPLITUDE 1.0f
#define AMPLITUDE_REDUCTION 90.0f 
//#define MAX_PHASE_ADVANCE 90.0f*PI/180.0f
//#define KP (10.0f/(max_ticks*1.0f))
//#define KI 100.0f
//#define KC 100.0f

#define OPEN_LOOP_MIN_ATTENUATION 0.8f


//--------rotor initial angle and rotation---------------
//60-120-180° clock wise
//240-300-0°	counter clock

//stator=rotor => 120 -240 intercambiados
	//240 clock wise
	//120 counter clock








//offset
//----180
//0 	clock wise
//-45
//-90   counter clock 	

//----120
//+120
//+90	clock wise
//+45	clock wise
//0	clock wise
//-45	clock wise
//-90	clock wise
void initial_stator_angle_open_loop(int* rotor_speed_loop_state)
{
	attenuation = OPEN_LOOP_MIN_ATTENUATION;
	sine_freq=sine_freq_fixed;
	*rotor_speed_loop_state=NO_HALL_UPDATE;
	
	//V_s_angle=rotor_angle+180.0f;//rotor_angle+180.0f;
	//phase_A_stator_angle=stator_angle_to_phase_A(V_s_angle);
	//phase_A_stator_angle=rotor_angle+240.0f;//200.0f;
/*
	if (rotor_angle==0.0f)
		phase_A_stator_angle=rotor_angle-180.0f;//200.0f;
	else
		phase_A_stator_angle=rotor_angle-120.0f;//200.0f;
*/	
}
//--0°
//0 y 180° counter clock, el resto clock wise como se esperaría
//-10°
//
//-45
//
//-90


//+90
	//0	 0 -
	//60+	
	//120 -
	//180 0-
	//240 +
	//300	+
//-90
	//0	 0 -
	//60-	
	//120 +
	//180 0-
	//240 -
	//300	+

void no_hall_update (int* rotor_speed_loop_state)
{	//gpio_set(GPIOD, GPIO12);
	if (hall1_data.hall_update)
	{
		attenuation = OPEN_LOOP_MIN_ATTENUATION;
		sine_freq=sine_freq_fixed;
		*rotor_speed_loop_state=FIRST_HALL_UPDATE;
	}

	else
	{
		attenuation = OPEN_LOOP_MIN_ATTENUATION;
		sine_freq=sine_freq_fixed;
		*rotor_speed_loop_state=NO_HALL_UPDATE;
	}

	//max_ticks=pwmfreq_f/sine_freq;

	

}

void first_hall_update (int* rotor_speed_loop_state)
{	//gpio_set(GPIOD, GPIO13);
	if (hall1_data.hall_update)
	{
		attenuation = OPEN_LOOP_MIN_ATTENUATION;
		sine_freq=sine_freq_fixed;
		*rotor_speed_loop_state=OPEN_LOOP;
	}

	else
	{
		attenuation = OPEN_LOOP_MIN_ATTENUATION;
		sine_freq=sine_freq_fixed;
		*rotor_speed_loop_state=FIRST_HALL_UPDATE;
	}
	//max_ticks=pwmfreq_f/sine_freq;
}
/*
void second_hall_update (int* rotor_speed_loop_state)
{
	
}
*/


//*****open loop
//attenuation=0.5 freq=10Hz => counter clock, most of the time does not rotate
//attenuation=0.5 freq=15Hz => counter clock, it does not rotate
//attenuation=0.6 freq=10Hz => counter clock, counter clock but once clockwise and once did not started
//attenuation=0.7 freq=10Hz => counter clock, sometimes clock wise, sometimes counter, but it always starts.
//attenuation=0.7 freq=15Hz => counter clock, sometimes clock wise, sometimes counter, but it always starts.
//attenuation=0.7 freq=20Hz => counter clock, sometimes clock wise, it cannot start, it swings from side to side (frecuency to fast)
//attenuation=0.7 freq=5Hz => counter clock, sometimes clock wise, it cannot start, it swings from side to side (frecuency to fast)

//open loop frecuencia fija inicial y sin aceleraar
//attenuation=0.7 freq=5Hz	=> se invierte sentido de giro una y otra vez
//attenuation=0.5 freq=5Hz	=> no tiene la fuerza para arrancar
//attenuation=0.6 freq=5Hz	=> se invierte
//attenuation=0.8 freq=5Hz	=> no tiene la fuerza para arrancar

//attenuation=0.5 freq=10Hz	=> gira suave pero a veces arranca en sentidos opuestos
//attenuation=0.6 freq=10Hz	=> gira medio tac tac en sentido del reloj, suave en opuesto, y a veces se pega. es poco lo que le falta
//attenuation=0.7 freq=10Hz	=> se invierte inclusive luego de haber arrancado


//attenuation=0.5 freq=15Hz	=> solo vibra
//attenuation=0.6 freq=15Hz	=> arranca en diferente dir
//attenuation00.9 freq=15Hz	=> arranca en diferente dir









void open_loop (int* rotor_speed_loop_state)

{	
	//gpio_set(GPIOD, GPIO13);
	//gpio_set(GPIOD, GPIO15);
	//static int
		//frequency_change_counter=0;
	//actual_sine_frequency=pwmfreq_f/(2.0f*previous_hall_ticks);
	actual_sine_frequency=1.0f/(2.0f*previous_hall_time);


	if (sine_freq>20.0f)
	{
		//*rotor_speed_loop_state=CLOSE_LOOP;
	}
	else if (frequency_change_counter>max_sinusoidal_periods)
	{
		sine_freq=sine_freq+0.75f;
		
		if (sine_freq<50.0f)
			attenuation=OPEN_LOOP_MIN_ATTENUATION;
		else 
			attenuation=1.0f;

		*rotor_speed_loop_state=OPEN_LOOP;
		frequency_change_counter=0;
	}
	else 
	{
		
		*rotor_speed_loop_state=OPEN_LOOP;
	}

/*

	if (hall1_data.hall_update)
	{
		attenuation = 0.5f;
		#define OPEN_LOOP_P 0.2f
		
		actual_sine_frequency=pwmfreq_f/(2.0f*previous_hall_ticks);


		if (actual_sine_frequency>40.0f)//open_loop_desired_sine_frequency*1.0f)
		{
			*rotor_speed_loop_state=CLOSE_LOOP;
			phase_sum=90.0f;
		}


		else
		{
			open_loop_error=open_loop_desired_sine_frequency-actual_sine_frequency;

			if (open_loop_error>8.0f)
				open_loop_error=8.0f;
			else if (open_loop_error<-8.0f)
				open_loop_error=-8.0f;

			sine_freq=actual_sine_frequency;//+OPEN_LOOP_P*open_loop_error;		
			*rotor_speed_loop_state=OPEN_LOOP;
		}
	}
	

	
	else 
	{			
		*rotor_speed_loop_state=OPEN_LOOP;		
	}
*/
	//max_ticks=pwmfreq_f/sine_freq;
	offset=0.0f;

}

void close_loop(int* rotor_speed_loop_state)
{
	#define Pkp 1.0f/12.0f
	
	#define MAX_PHASE_ADVANCE 90.0f

	float Pki=1000000.0f;
	float Pkc=1.0f/1000.0f;

if (hall1_data.hall_update)
{

	gpio_set(GPIOD, GPIO14);

	//actual_sine_frequency=pwmfreq_f/(2.0f*previous_hall_ticks);
	actual_sine_frequency=1.0f/(2.0f*previous_hall_time);

	close_loop_error=close_loop_desired_frequency-actual_sine_frequency;

	//desired_previous_hall_ticks=pwmfreq_f/(2.0f*close_loop_desired_frequency);
	//close_loop_tick_error= -0.25f*(desired_previous_hall_ticks-previous_hall_ticks);

	//phaseU=close_loop_tick_error*Pkp;//+phase_sum;
	phaseU=close_loop_error*1.0f/1.0f; 

//revisar en los casos que funciona el control cuanto es el offset en ángulos
//revisar cuando no funciona para ver el offset y comparar


//deseada 100Hz
//KP=0 KP=1/10
	//1/10 130-148 // más de eso 
	//1/7  124-140
	//1/5	117-132
	//1/3	107-122	
	//1/2.9 18-43
	//1/2.8 23-45
	//1/2.5 17-44
	//1/2	35-40

	//2 36-62
	//3 54-70
	//10 



//KP=1/1: 
	//deseado 100Hz, final 15-41
	//deseado 90Hz, final 12-39
	//deseado 80Hz, final 11-35
	//deseado 70 Hz, final 21-35
	//deseado 60 Hz, final 66-75Hz
	//deseado 50Hz, final 52-59
	//deseado 40Hz, final 43-48
	//deseado 30Hz, final 33-39
	//deseado 20 HZ, final 25-27
	//deseado 10 HZ, final 16-19
	//deseado 5Hz, final 14-22


	

//deseada 100HZ
	//KP=10 , final 41-68
	//KP=5	, final 41-68


//velocidad deseada lazo cerrado: 60Hz
	//KP=1/10: no hace nada
	//KP=1/5: no hace nada
	//KP=1/2.0: velocidad final 66-75
	//KP=1/1.0: 61-70******************best configuration***********
	
	//KP=1/0.9: 60-68
	//KP=1/0.8: 11-41

	//KP=1/0.75: bounces between 16-36 (riiing) 
	//KP=1/0.5: got stuck (riiiiing)
	


		//regular phase advance
		if (phaseU>MAX_PHASE_ADVANCE)
		{
			phase_advance=MAX_PHASE_ADVANCE;
			//attenuation=1.0f;//1.0f;
			//attenuation=1.0f;//phase_advance*1.0f/75.0f;
			Pki=1.0f/1000.0f;//8.0f;
		}
		
		else if (phaseU<-MAX_PHASE_ADVANCE)
		{
			phase_advance=-MAX_PHASE_ADVANCE;
			//attenuation=1.0f;
			//attenuation=1.0f;//-phase_advance*1.0f/75.0f;
			Pkc=8.0f;
		}

		else
		{
			phase_advance=phaseU;
			
			if (phase_advance<0.0f)
			{
				//attenuation=phase_advance*1.0f/100.0f;//-phase_advance/72.0f;
				//attenuation=0.75f;
			}
			else
			{
				//attenuation=phase_advance*1.0f/100.0f;//phase_advance/72.0f;
				//attenuation=0.75f;
			}
		}







/*
	if (attenuation<1)
	{
		//regular phase advance
		if (phaseU>MAX_PHASE_ADVANCE)
		{
			phase_advance=MAX_PHASE_ADVANCE;
			//attenuation=1.0f;//1.0f;
			attenuation=phase_advance*1.0f/75.0f;
			Pki=1.0f/1000.0f;//8.0f;
		}
		
		else if (phaseU<-MAX_PHASE_ADVANCE)
		{
			phase_advance=-MAX_PHASE_ADVANCE;
			//attenuation=1.0f;
			attenuation=-phase_advance*1.0f/75.0f;
			Pkc=8.0f;
		}

		else
		{
			phase_advance=phaseU;
			
			if (phase_advance<0.0f)
			{
				attenuation=phase_advance*1.0f/100.0f;//-phase_advance/72.0f;
			}
			else
			{
				attenuation=phase_advance*1.0f/100.0f;//phase_advance/72.0f;
			}
		}

		

	}

	else 
	{
		//double phase advance
		attenuation=1.0f;

		if (phaseU>MAX_PHASE_ADVANCE*2.0f)
		{
			phase_advance=MAX_PHASE_ADVANCE*2.0f;
		} 
		else if (phaseU<-MAX_PHASE_ADVANCE*2.0f)
		{
			phase_advance=-MAX_PHASE_ADVANCE*2.0f;
		}
		else
		{
			phase_advance=phaseU;
		}
	}
*/
	//phase_sum+phase_excess
	phase_excess=phaseU-phase_advance;

	//BEMF adjustment
	//phase_sum+=close_loop_tick_error/Pki-Pkc*phase_excess;

	phase_stator=phase_rotor+phase_advance;


	*rotor_speed_loop_state=CLOSE_LOOP;


	if (attenuation>1.0f)
		attenuation=1.00f;

	//max_ticks=pwmfreq_f/sine_freq;
	//ticks=ticks+phase_advance*max_ticks/360.0f;
	//attenuation=0.75f;
	//max_ticks=2.0f*previous_hall_ticks;
	sine_freq=actual_sine_frequency;
/*
	static bool evaluation_close_loop=false;
	

	if (actual_sine_frequency>130)
	{
		evaluation_close_loop=true;
		
	}

	if (evaluation_close_loop==true)
	{
		ticks=ticks+7.5f*max_ticks/360.0f;
	}*/
	

	//ticks=ticks+phase_advance*max_ticks/360.0f;
	//ticks=ticks+0.0f*max_ticks/360.0f;
	offset=0.0f; 

//Open loop initial angle
//H1	starts counter clock
//H1H2  starts clockwise
//H2	starts clockwise
//H2H3	starts clockwise
//H3	starts counter clock
//H3H1	starts counter clock



//-70	17-32
//-35   102-109
//-10	122-131
//-5	78-89
// 0	15-32
//+5	acelera desacelera, se invierte, final 12-24 counter clock
//+10	acelera desacelera, se invierte, final se pega

//offset fijo 

//-5.0f 		80Hz
//-10.0f		70-80Hz

//-15.0f se invierta y da vueltaas por todo lado
//-25.0f ~40Hz
//-50.0f ~50Hz
//-70.0f ~25-48
// 0	17-32


//---------------------------------------

//attenuation=0.75f;
	//ticks=ticks-150.0f*max_ticks/360.0f;

//hasta 130 libre, +90°
	//f: 31-53 (se desacelera y queda oscilando)
//hasta 130 libre, -90°
	//f: 6-12... se invierte y tac tac tack

//hasta 130 libre, +45°
	//f: 17-41 (se desacelera y queda con tack suave)
//hasta 130 libre, -45°
	//f: 6-12... se invierte y tac tac tack

//hasta 130 libre, +20°
	//f: 23-36 (se desacelera y queda con tack suave)
//hasta 130 libre, -20°
	//f: 6-12... se invierte y queda invertido

//hasta 130 libre, +10°
	//f: 140-10 (se queda un rato en 140, se desacelera, cae hasta 10, vuelve a 140 y se repite)
//hasta 130 libre, -10°
	//f: 10-45... se frena hasta 10 y se acelera hasta 45, se repite

//hasta 130 libre, +7.5°
	//f: 136-147: no afecta en nada la velocidad
//hasta 130 libre, -7.5°
	//f: 98-110... se frena hasta 10 y se acelera hasta 45, se repite

//hasta 130 libre, +5°
	//f: 130-140: no afecta en nada la velocidad
//hasta 130 libre, -5°
	//f: 96-104... no afecta en nada la velocidad

	
//basicamente ahora que no hay delays por adc ni usart se está dando el comportamiento esperado al igualar la frecuencia eléctrica
//con la mecánica: el motor se acelera rápidamente hasta 140Hz, pero no se desboca ni se sale de sincronía.


//angulo constante de offset

//offset: 90°
	//freq final open loop:20Hz
	//freq deseada closed loop: 100Hz
		//attenuation: 1
			//frecuencia final: 29-50
			//giro forzado, tack tack tack
		//attenuation: 0.5
			//frecuencia final: 13-23
			//giro suave
		//attenuation: 0.25
			//frecuancia final: 0
			//no gira
		//attenuation: 0.75
			//frecuencia final: 32-54
			//tack tack tack suave
//offset: 45°
	//freq final open loop:20Hz
	//freq deseada closed loop: 100Hz
		//attenuation: 0.75
			//frecuencia final: 16-32
			//tack tack tack suave			

//offset: 20°
	//freq final open loop:20Hz
	//freq deseada closed loop: 100Hz
		//attenuation: 0.75
			//frecuencia final: 29-45
			//giro suave	

//offset: 10°
	//freq final open loop:20Hz
	//freq deseada closed loop: 100Hz
		//attenuation: 0.75
			//frecuencia final: 10-142
			//gira rapidamente de forma robusta

//offset: 5°
	//freq final open loop:20Hz
	//freq deseada closed loop: 100Hz
		//attenuation: 0.75
			//frecuencia final: 137-140
			//gira rapidamente de forma robusta. No se detiene.

//offset: 1°
	//freq final open loop:20Hz
	//freq deseada closed loop: 100Hz
		//attenuation: 0.75
			//frecuencia final: 137-146
			//gira rapidamente de forma robusta y cte

//offset: 0.5°
	//freq final open loop:20Hz
	//freq deseada closed loop: 100Hz
		//attenuation: 0.75
			//frecuencia final: 129-145
			//gira rapidamente de forma robusta y cte

//offset: 0.25°
	//freq final open loop:20Hz
	//freq deseada closed loop: 100Hz
		//attenuation: 0.75
			//frecuencia final: 129-145
			//gira rapidamente de forma robusta y cte

//offset: 0.0°
	//freq final open loop:20Hz
	//freq deseada closed loop: 100Hz
		//attenuation: 0.75
			//frecuencia final: 129-145
			//gira rapidamente de forma robusta y cte

//pruebas con la lectura digital de los hall-sensors
	//freq final open loop:60Hz
	//freq deseada closed loop: 100Hz
		//se queda entre 16 y25Hz
	//freq final open loop:30Hz
	//freq deseada closed loop: 100Hz
		//se queda entre 16 y25Hz
	//freq final open loop:30Hz
	//freq deseada closed loop: 100Hz
	//attenuacion=phaseU/100
		//se queda entre 26 y 40Hz

//pruebas viejas con ADC y usar revueltas en el main
	//ticks=ticks+45.0f*max_ticks/360.0f;
	//180° pierde sincronía completamente	
	//175° se corre y se invierte
	//150°  mantiene sincronia y frequencia de 50Hz -60Hz (pocas veces a 25Hz)
	//125°  mantiene sincronia y frequencia de 50Hz -60Hz (pocas veces a 25Hz)
	//+90°  mantiene velocidad constante (cerca de 50Hz, cambia exactamente entre 50 y 25)
	//45°   salta entre 29 y 18Hz
	//20°	entre 18 y 25 pero pierde sincronía y se detiene
	//open loop sin actualizar velocidad: 12-16Hz

	//-45° se invierte constantemente
	//-20 se invierte cte mente
	//-90 se invierte y se queda pegado
	//-150 se sale de sincronia completamente
	//-180
}


else
{

}



}


void next_stator_angle_and_hall_time(void)
{	
	//PWM angle
	phase_A_stator_angle=phase_A_stator_angle+360.0f*CYCLE_TIME*sine_freq+offset;

	if (phase_A_stator_angle>=360.0f)
	{
		phase_A_stator_angle=phase_A_stator_angle-360.0f;
		frequency_change_counter++;	
	}
	
	//speed time
	hall_time=hall_time+CYCLE_TIME;
	
	if (hall1_data.hall_update )//|| hall2_data.hall_update || hall3_data.hall_update
	{
		previous_hall_time=hall_time;
		hall_time=0.0f;	
	}
}

void PID_control_loop(void)
{

	static int 
		//frequency_change_counter=0,
		rotor_speed_loop_state=OPEN_LOOP;//INITIAL_STATOR_ANGLE_OPEN_LOOP;


	hall_hysteresis_window(V_hall_1_V1,HALL_1_UPPER_BAND,HALL_1_LOWER_BAND,&hall1_data);

	next_stator_angle_and_hall_time();
	

/*
	if (rotor_speed_loop_state==INITIAL_STATOR_ANGLE_OPEN_LOOP)
		initial_stator_angle_open_loop(&rotor_speed_loop_state);	

	//waiting for having two hall sensor measures in order to calculate the rotor speed
	else if (rotor_speed_loop_state==NO_HALL_UPDATE)
		no_hall_update(&rotor_speed_loop_state);
		//open_loop(&rotor_speed_loop_state);

	else if (rotor_speed_loop_state==FIRST_HALL_UPDATE)
		first_hall_update(&rotor_speed_loop_state);
		//open_loop(&rotor_speed_loop_state);		
		
	//else if (rotor_speed_loop_state==SECOND_HALL_UPDATE)
		//second_hall_update(&rotor_speed_loop_state);
*/
	if (rotor_speed_loop_state==OPEN_LOOP)
		open_loop(&rotor_speed_loop_state);

	else if (rotor_speed_loop_state==CLOSE_LOOP)
		close_loop(&rotor_speed_loop_state);
	else
		rotor_speed_loop_state=INITIAL_STATOR_ANGLE_OPEN_LOOP; //NO_HALL_UPDATE;

}







