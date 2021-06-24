#include <stdio.h>
#include <stdlib.h>
#include "dictionary.h"
#define TABLE_SIZE	50

void initializeHashTable(Entry ***hashTable)
{
	// Allocate space for hashtable
	*hashTable = (Entry **)malloc(TABLE_SIZE * sizeof(Entry *));

	// Set every Entry table in the array to NULL
	for (int i = 0; i < TABLE_SIZE; i++)
	{
		*hashTable[i] = NULL;
	}
}