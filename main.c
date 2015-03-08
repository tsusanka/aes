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
 * @var block  the whole block we're working on
 * @var blockColumn  which column in `block` is beeing multiplied
 *
 * @const mixColumns  constant located in tables.h
 */
void matrixVectorMultiply(uint8_t block[BLOCK_SIZE_ROW_LENGTH][BLOCK_SIZE_ROW_LENGTH], int blockColumn)
{
	uint8_t result[BLOCK_SIZE_ROW_LENGTH];
	int i, y = 0;
	uint8_t high_nibble;
	uint8_t low_nibble;
	uint8_t afterMulti;

	for(i = 0; i < BLOCK_SIZE_ROW_LENGTH; i++)
	{
		result[i] = 0;
		for(y = 0; y < BLOCK_SIZE_ROW_LENGTH; y++)
		{
		    afterMulti = block[y][blockColumn];
		    high_nibble = afterMulti >>4 & 0x0F;
		    low_nibble = afterMulti & 0x0F;

			if (mixColumns[i][y] == 0x02)
			{
                afterMulti = multiply2[high_nibble * SBOX_TABLE_ROWS + low_nibble];
			}
			else if (mixColumns[i][y] == 0x03)
			{
				afterMulti = multiply3[high_nibble * SBOX_TABLE_ROWS + low_nibble];
			}
			result[i] ^= afterMulti;
		}
	}

	// todo: is this needed?
	for (i = 0; i < BLOCK_SIZE_ROW_LENGTH; i++)
	{
		block[i][blockColumn] = result[i];
	}
}

void mixColumnSublayer(uint8_t block[][BLOCK_SIZE_ROW_LENGTH])
{
	matrixVectorMultiply(block, 0);
	matrixVectorMultiply(block, 1);
	matrixVectorMultiply(block, 2);
	matrixVectorMultiply(block, 4);
}

int main(int argc, char** argv)
{
	uint8_t block[BLOCK_SIZE_ROW_LENGTH][BLOCK_SIZE_ROW_LENGTH] = {
		{0x01, 0x44, 0x88, 0xCC},
		{0x02, 0x55, 0x99, 0xDD},
		{0x03, 0x66, 0xAA, 0xEE},
		{0x04, 0x77, 0xBB, 0xFF},
	};

	printf("Default matrix:\n");
	printBlock(block);

	substitutionLayer(block);
	printf("After Substitution Layer:\n");
	printBlock(block);

	shiftRowsSublayer(block);
	printf("After ShiftRows Sublayer:\n");
	printBlock(block);

	mixColumnSublayer(block);
	printf("After MixColumn Sublayer:\n");
	printBlock(block);

	return 0;
}
