/**
 * MI-BHW ukol 1 - AES
 *
 * Tomas Susanka & Peter Poljak
 *
 **/

#include <stdio.h>
#include <stdint.h>
#include "tables.h"

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

void printBlock(uint8_t block[][BLOCK_SIZE_ROW_LENGTH])
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
	uint8_t block[BLOCK_SIZE_ROW_LENGTH][BLOCK_SIZE_ROW_LENGTH] = {
		{0x00, 0x44, 0x88, 0xCC},
		{0x11, 0x55, 0x99, 0xDD},
		{0x22, 0x66, 0xAA, 0xEE},
		{0x33, 0x77, 0xBB, 0xFF},
	};

	printBlock(block);

	return 0;
}
