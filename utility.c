#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "utility.h"
#include "game.h"


int getNumberOfEdges(Edge *paths)
{
	if (paths == NULL)
	{
		return 0;
	}

	int numberOfEdges = 0;

	Edge *currentEdge = paths;

	while (currentEdge != NULL)
	{
		numberOfEdges++;
		currentEdge = currentEdge->nextEdge;
	}

	return numberOfEdges;
}

void getDirections(char **directions, Edge *paths)
{
	if (paths == NULL)
	{
		return;
	}

	int index = 0;
	Edge *currentEdge = paths;

	while (currentEdge != NULL)
	{
		directions[index] = (char *)malloc(strlen(currentEdge->direction) + 1);
		strcpy(directions[index], currentEdge->direction);

		index++;
		currentEdge = currentEdge->nextEdge;
	}
}

void addEdgeToLocation(Location *from, Location *to, char *direction, char *pathType)
{
	// Input validation
	if (from == NULL || to == NULL)
		return;
	
	// Input validation complete
	
	// Create new Edge
	Edge *edge = (Edge *)malloc(sizeof(Edge));

	edge->from = from;
	edge->to = to;
	
	edge->direction = (char *)malloc(strlen(direction) + 1);
	strcpy(edge->direction, direction);
	
	edge->pathType = (char *)malloc(strlen(pathType) + 1);
	strcpy(edge->pathType, pathType);
	
	edge->nextEdge = NULL;

	// Edge created, now add to linked list in from location
	if (from->paths == NULL)
	{
		// location has no paths
		from->paths = edge;
	} else
	{
		// Traverse paths until get to an empty node
		Edge *current_edge = from->paths;

		while (current_edge->nextEdge != NULL)
		{
			current_edge = current_edge->nextEdge;
		}

		current_edge->nextEdge = edge;
	}
}

void printEdge(Edge *edge)
{
	// Input validation
	if (edge == NULL)
	{
		return;
	}
	// Input validation complete

	printf("Edge From: %s | To: %s | Direction: %s | Type: %s\n", edge->from->name, edge->to->name, edge->direction, edge->pathType);
}

void printEdges(Location *loc)
{
	// Input validation
	if (loc == NULL || loc->paths == NULL)
		return;
	
	// Input validation complete

	Edge *current_edge = loc->paths;

	while (current_edge != NULL)
	{
		printEdge(current_edge);

		current_edge = current_edge->nextEdge;
	}
}

void toLowerCase(char *stringPtr)
{
	char *charPtr = stringPtr;
	while (*charPtr != '\0')
	{
		if (*charPtr >= 65 && *charPtr <= 90)
		{
			*charPtr = *charPtr + 32;
		}
		charPtr++;
	}
}

void printLocation(Location *location)
{
	printf("Location: %s | Description: %s\n", location->name, location->description);
}

void printMap(Location *map, int map_size)
{
	for (int i = 0; i < map_size; i++)
	{
		printLocation(&map[i]);
		printf("Edges from %s:\n", map[i].name);
		printEdges(&map[i]);
		printf("\n");
	}
}

void printDirections(Location *currentLocation)
{
	if (currentLocation == NULL)
	{
		return;
	} else if (currentLocation->paths == NULL)
	{
		printf("You can't go anywhere.\n");
		return;
	}

	Edge *currentEdge = currentLocation->paths;
	
	int numPaths = getNumberOfEdges(currentLocation->paths);

	printf("You can go ");

	if (numPaths == 1)
	{
		printf("%s.\n", currentEdge->direction);
	} else if (numPaths == 2)
	{
		printf("%s and %s.\n", currentEdge->direction, currentEdge->nextEdge->direction);
	} else
	{
		int count = numPaths;
		while (currentEdge != NULL)
		{
			if (count == 1)
			{
				printf("and %s.", currentEdge->direction);
			}
			else {
				printf("%s, ", currentEdge->direction);
			}

			count--;
			currentEdge = currentEdge->nextEdge;
		}
	}
}

char *trimWhiteSpace(char *string, int length)
{
	// Return original string without whitespace at the ends

	// Create two pointers - one that points at first non-whitespace character, and other that
	// points at last non-whitespace character
	char *start = string;
	char *end = string + length - 1;

	//printf("Start: %c | end: %c\n", *start, *end);

	// Define starting and ending points
	while (*start == ' ' || *start == '\n' || *end == ' ' || *end == '\n')
	{
		if (start > end)
		{
			return "";	// Occurs if entire string is whitespace
		}
		if (*start == ' ' || *start == '\n')
		{
			start++;
		}
		if (*end == ' ' || *end == '\n')
		{
			end--;
		}
	}

	//printf("End char: %c", *end);

	// Create new trimmed string
	int newLength = end - start + 2;
	//printf("New length: %d\n", newLength);
	char *newString = (char *)malloc(newLength);	// +2 because +1 for actual number of characters and + 1 for \0 sentinel
	int i;
	for (i = 0; i < newLength - 1; i++)
	{
		//printf("char: %c\n", *start);
		newString[i] = *start;
		//printf("newString[i] = %c\n", newString[i]);
		start++;
	}
	newString[i] = '\0';
	return newString;
}

char **splitString(char *string, int *size, char delim)
{
	//printf("Entered splitString\n");
	if (string == NULL)
	{
		return NULL;
	}

	// Split the string by the delimeter into an array of words
	int length = strlen(string);	// Length of the string
	int index = 0;					// index of current character
	//int delimPositions[length];		// Positions (indexes) of delimeters in string after words
	int wordPositions[length];		// Positions (indexes) of first chars of each word in string
	int numWords = 0;				// Number of words in string
	int isWord = 0;					// Boolean (1 if current substring is a word)
	char *currentChar = string;		// Current character pointer (iterates over string)

	while (*currentChar != '\0')
	{
		//printf("char: %c\n", *currentChar);
		if (*currentChar == delim && isWord)
		{
			// If current character is a delimeter and the previous few characters are a word
			//delimPositions[numWords] = index;
			//numWords++;
			isWord = 0;
			*currentChar = '\0';
		}
		else if (*currentChar >= 33 && *currentChar <= 126 && *currentChar != delim)
		{
			// If current character is not a delimeter but is some other type of character
			if (isWord == 0)
			{
				// currentChar is first character of a new word
				wordPositions[numWords] = index;
				numWords++;
			}
			isWord = 1;
		}
		currentChar++;
		index++;
	}

	// Now, delimPositions should contain all indeces of delimeters after words. All that is left is to split
	// them into an array.
	//printf("loop\n");
	char **wordsArray = (char **)malloc(numWords * sizeof(char *));

	for (int i = 0; i < numWords; i++)
	{
		//printf("Loop: %d\n", i);
		//printf("wordPositions[%d] = %d\n", i, wordPositions[i]);

		char *currentWord = string + wordPositions[i];

		//printf("%dth Word: %s|\n", i, currentWord);

		wordsArray[i] = (char *)malloc(strlen(currentWord) + 1);
		strcpy(wordsArray[i], currentWord);
	}

	for (int i = 0; i < numWords; i++)
	{
		//printf("Array word: %s|\n", wordsArray[i]);
	}

	// Set size to numWords
	*size = numWords;

	return wordsArray;
}

int isStringInArray(char *string, char **stringArray, int arrayLength)
{
	for (int i = 0; i < arrayLength; i++)
	{
		if (strcmp(string, stringArray[i]) == 0)
		{
			return i;
		}
	}

	return -1;
}

void freePaths(Edge *paths)
{
	//printf("freePaths()\n");
	if (paths == NULL)
	{
		return;
	}
	Edge *currentEdge = paths;
	Edge *nextEdge = NULL;
	
	while (currentEdge != NULL)
	{
		nextEdge = currentEdge->nextEdge;

		//printEdge(currentEdge);

		// Free currentEdge
		//printf("direction\n");
		free(currentEdge->direction);
		//printf("pathType\n");
		free(currentEdge->pathType);
		//printf("edge\n");
		free(currentEdge);

		currentEdge = nextEdge;
	}
}

void freeLocation(Location * location)
{
	//printf("freeLocation()\n");
	if (location == NULL)
	{
		return;
	}
	//printLocation(location);
	// Free location->name
	free(location->name);
	free(location->description);

	freePaths(location->paths);

	//free(location);
}

void freeMap(Location *map, int mapSize)
{
	
	/*for (int i = 0; i < mapSize; i++)
	{
		printf("Freeing paths of location %d\n", i);
		freePaths(map[i].paths);
	}*/

	for (int i = 0; i < mapSize; i++)
	{
		//printf("Freeing location %d\n", i);
		freeLocation(&map[i]);
	}

	free(map);
}

void freePlayer(Player *player)
{
	free(player);
}

void freeEntries(Entry *entry)
{
	Entry *currentEntry = entry;
	Entry *nextEntry = NULL;

	while (currentEntry != NULL)
	{
		nextEntry = currentEntry->next;

		free(currentEntry->name);
		free(currentEntry);

		currentEntry = nextEntry;
	}
}

void freeHashTable(Entry **hashTable)
{
	for (int i = 0; i < TABLE_SIZE; i++)
	{
		freeEntries(hashTable[i]);
	}

	free(hashTable);
}

void freeStringArray(char **stringArray, int length)
{
	for (int i = 0; i < length; i++)
	{
		free(stringArray[i]);
	}
	free(stringArray);
}