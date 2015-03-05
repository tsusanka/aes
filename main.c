/**
 * MI-BHW ukol 1 - AES
 *
 * Tomas Susanka & Peter Poljak
 *
 **/

#include <stdio.h>

 
void printBin(char x)
{
	int i; 
	for (i = (sizeof(char) * 8) - 1; i >= 0; i--)
	{
		(x & (1<<i)) ? putchar('1') : putchar('0');
	}

	printf("\n");
}

void printHex(char x)
{
	printf("0x%x", x);
	printf("\n");
}

int main(int argc, char** argv)
{

	return 0;
}
