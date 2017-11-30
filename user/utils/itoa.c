#include "itoa.h"

void uint8_to_str(char *buffer, uint8_t number)
{
	uint8_t data;
	if (number > 99)
	{
		data = number / 100;
		number %= 100;
		*buffer++ = data + 48;
		data = number / 10;
		number %= 10;
		*buffer++ = data + 48;
		*buffer++ = number + 48;
		*buffer = 0; //string termination
	}
	else if (number > 9)
	{
		data = number / 10;
		number %= 10;
		*buffer++ = data + 48;
		*buffer++ = number + 48;
		*buffer = 0; //string termination
	}
	else if (number >= 0)
	{
		*buffer++ = number + 48;
		*buffer = 0; //string termination
	}
}
void uint16_to_str(char *buffer, uint32_t number)
{
	//this is a realy big number?
	if (number > 255)
	{
		//how long is the number?
		uint32_t divider = 100;
		uint8_t number_len = 2;
		while (number / divider)
		{
			number_len++;
			divider *= 10;
		}
		//rolling back the divider
		divider /= 10;
		//tearing down
		while (number_len--)
		{
			*buffer++ = (number / divider) + 48;
			number %= divider;
			divider /= 10;
		}
		*buffer = 0; //terminate string
	}
	else
	{
		return uint8_to_str(buffer, number);
	}
}
void int8_to_str(char *buffer, int8_t number)
{
	if (number & 0x80)
	{						 //If the sign bit 1 this is a negative number
		number = number & 0x7F; //Get the abs. value
				 uint8_to_str(buffer, number);
		int i = 4;
		while (i--)
		{
			buffer[i] = buffer[i + 1];
		}
		buffer[i] = '-';
	}
	else
	{
		uint8_to_str(buffer, number);
	}
}