/**
 * MI-BHW ukol 1 - AES
 *
 * Tomas Susanka & Peter Poljak
 *
 **/

#include <stdio.h>
#include <stdint.h>

#define BLOCK_SIZE_ROW_LENGTH 4


void printBin(uint8_t x)
{
	int i; 
	for (i = (sizeof(char) * 8) - 1; i >= 0; i--)
	{
		(x & (1<<i)) ? putchar('1') : putchar('0');
	}

	printf("\n");
}

void printBlock(uint8_t block[BLOCK_SIZE_ROW_LENGTH][BLOCK_SIZE_ROW_LENGTH])
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
	uint8_t block[BLOCK_SIZE_ROW_LENGTH][BLOCK_SIZE_ROW_LENGTH];
	int i, y = 0;
	uint8_t input;
	uint8_t xxx;

	printf("Enter %d bytes in hex seperated by newline: \n", BLOCK_SIZE_ROW_LENGTH * BLOCK_SIZE_ROW_LENGTH);

	for (i = 0; i < BLOCK_SIZE_ROW_LENGTH; i++)
	{
		for (y = 0; y < BLOCK_SIZE_ROW_LENGTH; y++)
		{
			printf("%d ", scanf("%x", &input));
			block[y][i] = input;
		}
	}

	printBlock(block);

	return 0;
}
