/**
 * MI-BHW ukol 1 - AES
 *
 * Tomas Susanka & Peter Poljak
 *
 **/

#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include "tables.h"

#define BLOCK_SIZE_ROW_LENGTH 4
#define KEY_LENGTH 16
#define W_SIZE 4
#define SUB_W_LENGTH 4
#define NUM_OF_ITERATIONS 10

typedef struct w
{
	uint8_t subW[SUB_W_LENGTH];
} w_t;

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
			printf("%02X |", block[i][y]);
		}
		printf("\n");
	}
	printf("----------------\n");
}

void printW(w_t w[W_SIZE])
{
	int i;
	printf("W:\n");
	for (i = 0; i < W_SIZE; i++)
	{
		printf("W[%d]: %x, %x, %x, %x\n", i, w[i].subW[0], w[i].subW[1], w[i].subW[2], w[i].subW[3]);
	}
	printf("-----------\n");
}

/*****************************************************************************************************************************/
/*****************************************************************************************************************************/

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

/**
 *
 * The function g() rotates its four input bytes,
 * performs a byte-wise S-Box substitution, and adds a round coefficient RC to the first one.
 *
 */
void g(uint8_t subW[SUB_W_LENGTH], w_t * tmp, int iteration)
{
	assert(iteration >= 1 && iteration <= 10);

	tmp->subW[0] = sboxify(subW[1]);
	tmp->subW[1] = sboxify(subW[2]);
	tmp->subW[2] = sboxify(subW[3]);
	tmp->subW[3] = sboxify(subW[0]);

	tmp->subW[0] ^= RC[iteration-1];
}

void mixColumnSublayer(uint8_t block[][BLOCK_SIZE_ROW_LENGTH])
{
	matrixVectorMultiply(block, 0);
	matrixVectorMultiply(block, 1);
	matrixVectorMultiply(block, 2);
	matrixVectorMultiply(block, 3);
}

void keyAdditionLayer(uint8_t key[], uint8_t block[][BLOCK_SIZE_ROW_LENGTH])
{
    int i,y;
    for(i = 0; i < BLOCK_SIZE_ROW_LENGTH; i++)
    {
        for(y = 0; y < BLOCK_SIZE_ROW_LENGTH; y++)
        {
            block[y][i] ^= key[BLOCK_SIZE_ROW_LENGTH*i+y];
        }
    }
}

void keySchedule(w_t w[W_SIZE], uint8_t key[KEY_LENGTH], int iteration)
{
	int i;
	for(i = 0; i < KEY_LENGTH; i++)
	{
		w[i / 4].subW[i % 4] = key[i];
	}

    printf("kS: xx %d xx", iteration);

	w_t tmp;
	g(w[3].subW, &tmp, iteration);

	for (i = 0; i < SUB_W_LENGTH; ++i)
	{
		w[0].subW[i] ^= tmp.subW[i];
		w[1].subW[i] ^= w[0].subW[i];
		w[2].subW[i] ^= w[1].subW[i];
		w[3].subW[i] ^= w[2].subW[i];
	}

	for(i = 0; i < KEY_LENGTH; i++)
	{
		key[i] = w[i / 4].subW[i % 4];
	}
}

/*****************************************************************************************************************************/
/*****************************************************************************************************************************/

int main(int argc, char** argv)
{
	uint8_t block[BLOCK_SIZE_ROW_LENGTH][BLOCK_SIZE_ROW_LENGTH] = {
		{0x11, 0x55, 0x99, 0xDD},
		{0x22, 0x66, 0xAA, 0xEE},
		{0x33, 0x77, 0xBB, 0xFF},
		{0x44, 0x88, 0xCC, 0x00},
	};

	uint8_t key[KEY_LENGTH] = {
		0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x00
	};

	w_t w[W_SIZE];
	int iteration = 0;

	printf("Default matrix:\n");
	printBlock(block);

	printf("0. Key addition:\n");
	keyAdditionLayer(key, block);
	printBlock(block);

	for(iteration = 1; iteration < NUM_OF_ITERATIONS; iteration++)
	{
		printf("-------------------------------\nRound %d:\n", iteration);

		substitutionLayer(block);
		printf("After Substitution Layer:\n");
		printBlock(block);

		shiftRowsSublayer(block);
		printf("After ShiftRows Sublayer:\n");
		printBlock(block);

		mixColumnSublayer(block);
		printf("After MixColumn Sublayer:\n");
		printBlock(block);

		keySchedule(w,key,iteration);
		keyAdditionLayer(key, block);
		printf("After Key Addition Layer:\n");
		printBlock(block);
	}

	printf("-------------------------------\nRound %d:\n", NUM_OF_ITERATIONS);
	substitutionLayer(block);
	printf("After Substitution Layer:\n");
	printBlock(block);

	shiftRowsSublayer(block);
	printf("After ShiftRows Sublayer:\n");
	printBlock(block);

	keySchedule(w,key,NUM_OF_ITERATIONS);
	keyAdditionLayer(key, block);
	printf("After Key Addition Layer:\n");
	printBlock(block);

	return 0;
}
