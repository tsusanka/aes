/**
 * MI-BHW ukol 1 - AES
 *
 * Tomas Susanka & Peter Poljak
 *
 **/

#include <stdio.h>

#define BLOCK_SIZE_ROW_LENGTH 4


void printBin(unsigned char x)
{
	int i; 
	for (i = (sizeof(char) * 8) - 1; i >= 0; i--)
	{
		(x & (1<<i)) ? putchar('1') : putchar('0');
	}

	printf("\n");
}

void printBlock(unsigned char block[BLOCK_SIZE_ROW_LENGTH][BLOCK_SIZE_ROW_LENGTH])
{
	int i, y = 0;
	printf("----------------\n");

	for (i = 0; i < BLOCK_SIZE_ROW_LENGTH; ++i)
	{
		printf("| ");
		for (y = 0; y < BLOCK_SIZE_ROW_LENGTH; ++y)
		{
			printf("%x |", block[i][y]);
		}
		printf("\n");
	}
	printf("----------------\n");
}

int main(int argc, char** argv)
{
	unsigned char block[BLOCK_SIZE_ROW_LENGTH][BLOCK_SIZE_ROW_LENGTH];
	int i, y = 0;
	unsigned char input;
	unsigned char xxx;

	printf("Enter %d bytes in hex seperated by newline: \n", BLOCK_SIZE_ROW_LENGTH * BLOCK_SIZE_ROW_LENGTH);

	for (i = 0; i < BLOCK_SIZE_ROW_LENGTH; i++)
	{
		for (y = 0; y < BLOCK_SIZE_ROW_LENGTH; y++)
		{
			scanf("%x", &input);
			block[y][i] = input;
		}
	}

	printBlock(block);

	return 0;
}
