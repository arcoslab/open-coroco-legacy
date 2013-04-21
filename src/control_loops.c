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

#define NO_HALL_UPDATE	 	0
#define FIRST_HALL_UPDATE	1
#define SECOND_HALL_UPDATE	2
#define OPEN_LOOP		3
#define CLOSE_LOOP 		4


#define MAX_AMPLITUDE 1.0f
#define AMPLITUDE_REDUCTION 90.0f 
//#define MAX_PHASE_ADVANCE 90.0f*PI/180.0f
//#define KP (10.0f/(max_ticks*1.0f))
//#define KI 100.0f
//#define KC 100.0f

#define OPEN_LOOP_MIN_ATTENUATION 0.75f

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

	max_ticks=pwmfreq_f/sine_freq;
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
	max_ticks=pwmfreq_f/sine_freq;
}
/*
void second_hall_update (int* rotor_speed_loop_state)
{
	
}
*/
void open_loop (int* rotor_speed_loop_state)

{	
	//gpio_set(GPIOD, GPIO13);
	//gpio_set(GPIOD, GPIO15);
	//static int
		//frequency_change_counter=0;
	actual_sine_frequency=pwmfreq_f/(2.0f*previous_hall_ticks);

	if (sine_freq>20.0f)
	{
		*rotor_speed_loop_state=CLOSE_LOOP;
	}
	else if (frequency_change_counter>max_sinusoidal_periods)
	{
		sine_freq=sine_freq+0.25f;
		
		if (sine_freq<50.0f)
			attenuation=0.50f;
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
	max_ticks=pwmfreq_f/sine_freq;
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

	actual_sine_frequency=pwmfreq_f/(2.0f*previous_hall_ticks);
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
			attenuation=1.0f;//phase_advance*1.0f/75.0f;
			Pki=1.0f/1000.0f;//8.0f;
		}
		
		else if (phaseU<-MAX_PHASE_ADVANCE)
		{
			phase_advance=-MAX_PHASE_ADVANCE;
			//attenuation=1.0f;
			attenuation=1.0f;//-phase_advance*1.0f/75.0f;
			Pkc=8.0f;
		}

		else
		{
			phase_advance=phaseU;
			
			if (phase_advance<0.0f)
			{
				//attenuation=phase_advance*1.0f/100.0f;//-phase_advance/72.0f;
				attenuation=0.75f;
			}
			else
			{
				//attenuation=phase_advance*1.0f/100.0f;//phase_advance/72.0f;
				attenuation=0.75f;
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
	phase_sum+=close_loop_tick_error/Pki-Pkc*phase_excess;

	phase_stator=phase_rotor+phase_advance;


	*rotor_speed_loop_state=CLOSE_LOOP;


	if (attenuation>1.0f)
		attenuation=1.00f;

	//max_ticks=pwmfreq_f/sine_freq;
	//ticks=ticks+phase_advance*max_ticks/360.0f;
	//attenuation=0.75f;
	max_ticks=2.0f*previous_hall_ticks;
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
	ticks=ticks+0.0f*max_ticks/360.0f;
	offset=-70.0f; 
//offset fijo 

//-5.0f 		80Hz
//-10.0f		70-80Hz

//-15.0f se invierta y da vueltaas por todo lado
//-25.0f ~40Hz
//-50.0f ~50Hz
//-70.0f ~25-48
//


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

void PID_control_loop(void)
{

	static int 
		//frequency_change_counter=0,
		rotor_speed_loop_state=NO_HALL_UPDATE;

	//hall sensor readings
	V_hall_1_V1=gpio_get(GPIOE, GPIO15);	//32768	10000000 00000000
	V_hall_2_V1=gpio_get(GPIOB, GPIO11);	//2048	00001000 00000000
	V_hall_3_V1=gpio_get(GPIOB, GPIO13);	//8192	00100000 00000000


	hall_hysteresis_window(V_hall_1_V1,HALL_1_UPPER_BAND,HALL_1_LOWER_BAND,&hall1_data);
	
	
  	if (ticks<max_ticks)
	{
		ticks+=1.0f;
	}
	else
	{ 
		ticks=0.0f;
		frequency_change_counter++;
	}

	hall_ticks+=1.0f;
	//max_ticks=pwmfreq_f/sine_freq;



	if (hall1_data.hall_update )//|| hall2_data.hall_update || hall3_data.hall_update)
	{	
		//hall-sensor time	
		previous_hall_ticks=hall_ticks;
		hall_ticks=0.0f;
		
		
	}



	//waiting for having two hall sensor measures in order to calculate the rotor speed
	if (rotor_speed_loop_state==NO_HALL_UPDATE)
		no_hall_update(&rotor_speed_loop_state);

	else if (rotor_speed_loop_state==FIRST_HALL_UPDATE)
		first_hall_update(&rotor_speed_loop_state);
	
	//else if (rotor_speed_loop_state==SECOND_HALL_UPDATE)
		//second_hall_update(&rotor_speed_loop_state);

	else if (rotor_speed_loop_state==OPEN_LOOP)
		open_loop(&rotor_speed_loop_state);

	else if (rotor_speed_loop_state==CLOSE_LOOP)
		close_loop(&rotor_speed_loop_state);
	else
		rotor_speed_loop_state=NO_HALL_UPDATE;

}










