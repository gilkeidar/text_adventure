#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "dictionary.h"
#include "game.h"
#define TABLE_SIZE	50

void initializeHashTable(Entry ***hashTable)
{
	// Allocate hash table
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

int hash(char *funcName)
{
	long hashVal = 0;
	int length = strlen(funcName);

	// Using Java's hashing scheme
	for (int i = 0; i < length; i++)
	{
		hashVal = 31 * hashVal + (*(funcName + i));
	}

	// Mod by hash table sizeof
	return hashVal % TABLE_SIZE;
}

void addToTable(char *funcName, int (*fun)(char **, Player *, Location *), Entry **hashTable)
{
	// Add a function to the hash table

	// Hash function funcName
	int index = hash(funcName);

	//printf("index added: %d\n", index);

	// Check that index
	Entry **entryPtr = &hashTable[index];

	// Create a new entry
	Entry *newEntry = (Entry *)malloc(sizeof(Entry));
	
	// Initialize new entry
	newEntry->name = (char *)malloc(strlen(funcName) + 1);
	strcpy(newEntry->name, funcName);

	newEntry->fun = fun;

	newEntry->next = NULL;

	if (*entryPtr == NULL)
	{
		*entryPtr = newEntry;
	} else
	{
		while ((*entryPtr)->next != NULL)
		{
			entryPtr = &((*entryPtr)->next);
		}

		(*entryPtr)->next = newEntry;
	}

	printf("entryPtr->name: %s\n", (*entryPtr)->name);
}

Entry *get(char *funcName, Entry** hashTable)
{
	// Hash function name
	int index = hash(funcName);

	//printf("index: %d\n", index);

	// Check to see whether it exists in hashTable
	Entry *entryPtr = hashTable[index];

	//printf("ptr: %p\n", entryPtr);

	while (entryPtr != NULL)
	{
		//printf("entryPtr name: %s\n", entryPtr->name);
		if (strcmp(entryPtr->name, funcName) == 0)
		{
			//printf("entryPtr->name: %s\n", entryPtr->name);
			return entryPtr;
		}
	}

	return NULL;
}