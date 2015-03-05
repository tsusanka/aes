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

uint8_t sboxify(uint8_t num)
{
	uint8_t high_nibble = num>>4 & 0x0F;
	uint8_t low_nibble = num & 0x0F;

	return sboxTable[high_nibble * SBOX_TABLE_ROWS + low_nibble];
}

void substitutionLayer(uint8_t block[][BLOCK_SIZE_ROW_LENGTH])
{
	int i, y = 0;
	for (i = 0; i < BLOCK_SIZE_ROW_LENGTH; i++)
	{
		for (y = 0; y < BLOCK_SIZE_ROW_LENGTH; y++)
		{
			block[i][y] = sboxify(block[i][y]);
		}
	}
}

void shiftRowLeft(uint8_t block[][BLOCK_SIZE_ROW_LENGTH], int i)
{
	int y = 0;
	uint8_t temp;

	temp = block[i][0];
	for (y = 0; y < BLOCK_SIZE_ROW_LENGTH; y++)
	{
		block[i][y] = block[i][(y+1) % 4] ;
	} 

	block[i][y-1] = temp;
}

void shiftRowsSublayer(uint8_t block[][BLOCK_SIZE_ROW_LENGTH])
{
	shiftRowLeft(block,1);
	shiftRowLeft(block,2);
	shiftRowLeft(block,2);
	shiftRowLeft(block,3);
	shiftRowLeft(block,3);
	shiftRowLeft(block,3);
}

/**
 * @var input  the whole block we're working on
 * @var inputColumn  which column in `input` is beeing multiplied
 *
 * @const mixColumns  constant located in tables.h
 */
void matrixVectorMultiply(uint8_t input[BLOCK_SIZE_ROW_LENGTH][BLOCK_SIZE_ROW_LENGTH], int inputColumn)
{
	uint8_t result[BLOCK_SIZE_ROW_LENGTH];

	int i, y = 0;

	for(i = 0; i < BLOCK_SIZE_ROW_LENGTH; i++) 
	{
		result[i] = 0;
		for(y = 0; y < BLOCK_SIZE_ROW_LENGTH; y++) 
		{
			result[i] += mixColumns[i][y] * input[y][inputColumn];
		}
	}
}

void mixColumn(uint8_t block[][BLOCK_SIZE_ROW_LENGTH]) 
{
	matrixVectorMultiply(block, 0);
	matrixVectorMultiply(block, 1);
	matrixVectorMultiply(block, 2);
	matrixVectorMultiply(block, 4);
}

int main(int argc, char** argv)
{
	uint8_t block[BLOCK_SIZE_ROW_LENGTH][BLOCK_SIZE_ROW_LENGTH] = {
		{0x00, 0x44, 0x88, 0xCC},
		{0x11, 0x55, 0x99, 0xDD},
		{0x22, 0x66, 0xAA, 0xEE},
		{0x33, 0x77, 0xBB, 0xFF},
	};

	// substitutionLayer(block);
	// printBlock(block);

	// shiftRowsSublayer(block);
	// printBlock(block);

	matrixVectorMultiply(block, 0);
	// printBlock(block);

	return 0;
}
