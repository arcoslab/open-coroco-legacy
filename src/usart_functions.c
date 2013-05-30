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

#define MAX_SIZE_FLOATING_NUMBER_CARACTERS 10
#define MAX_SIZE_INTEGER_NUMBER_CARACTERS 10


void usart_transmit_a_floating_number(float value)
{
	char array [MAX_SIZE_FLOATING_NUMBER_CARACTERS];
	int i=0;
	int j=0;
	int rounded_value;
	
	rounded_value=value;

	sprintf(array, " %d k", rounded_value);
	i=0;
	while (i<MAX_SIZE_FLOATING_NUMBER_CARACTERS && array[i]!='k') 
	{	
		usart_send_blocking(USART2, array[i]); 
		i++;
	}
	//usart_send_blocking(USART2,' ');

	while( j<(MAX_SIZE_FLOATING_NUMBER_CARACTERS-i ))
	{
		usart_send_blocking(USART2,' ');
		j++;
	}
	//usart_send_blocking(USART2, '\r');
	//usart_send_blocking(USART2, '\n');
}

void usart_transmit_an_integer_number(int value)
{
	char array [MAX_SIZE_INTEGER_NUMBER_CARACTERS];
	int i=0;
	int j=0;
	sprintf(array, " %d k",value);
	i=0;
	while (i<MAX_SIZE_INTEGER_NUMBER_CARACTERS && array[i]!='k') 
	{	
		usart_send_blocking(USART2, array[i]); 
		i++;
	}

	//usart_send_blocking(USART2,' ');

	while( j<(MAX_SIZE_INTEGER_NUMBER_CARACTERS-i ))
	{
		usart_send_blocking(USART2,' ');
		j++;
	}
	//usart_send_blocking(USART2, '\r');
	//usart_send_blocking(USART2, '\n');*/
}

void usart_transmit_a_string(char* phrase)
{
	int string_length;
	int j=0;
			
	string_length=strlen(phrase);

	for (j=0;j<string_length;j++)
		usart_send_blocking(USART2,phrase[j]);	
}

void usart_transmit_a_tagged_floating_number(char* tag, float number)
{
	usart_transmit_a_string(tag);
	usart_transmit_a_floating_number(number);
}

void usart_transmit_a_tagged_integer_number(char* tag, int number)
{
	usart_transmit_a_string(tag);
	usart_transmit_an_integer_number(number);
}

float usart_receive_a_floating_number(void)
{

	static float number=0.0f;
	char string_number[MAX_SIZE_FLOATING_NUMBER_CARACTERS];
	char character;	
	int j=0;

	character=usart_recv(USART2);

	if (character=='a')
	{
		for (j=0; (j<MAX_SIZE_FLOATING_NUMBER_CARACTERS) && (string_number[j-1]!='k');j++)
		{
			string_number[j]=usart_recv_blocking(USART2);
		}

		number = atoi(string_number);
	}	

	return number;
}

float usart_receive_controller_parameters(float P,float I,float desired_speed)
{

	static float number=0.0f;
	char string_number[MAX_SIZE_FLOATING_NUMBER_CARACTERS];
	char character;	
	int j=0;

	character=usart_recv(USART2);

	if (character=='a')
	{
		for (j=0; (j<MAX_SIZE_FLOATING_NUMBER_CARACTERS) && (string_number[j-1]!='k');j++)
		{
			string_number[j]=usart_recv_blocking(USART2);
		}

		number = atoi(string_number);
	}	

	return number;
}


void usart_transmit_new_line(void)
{
	usart_send_blocking(USART2, '\r');
	usart_send_blocking(USART2, '\n');
}

