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
