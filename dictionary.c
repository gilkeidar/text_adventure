#include <stdio.h>
#include <stdlib.h>
//#include "dictionary.h"
#include "game.h"
#define TABLE_SIZE	50

void initializeHashTable(Entry ***hashTable)
{
	//printf("initializeHashTable()\n");
	// Allocate space for hashtable
	*hashTable = (Entry **)malloc(TABLE_SIZE * sizeof(Entry *));

	// Set every Entry table in the array to NULL
	for (int i = 0; i < TABLE_SIZE; i++)
	{
		//printf("Setting *hashTable[%d] to NULL\n", i);
		(*hashTable)[i] = NULL;
	}
}