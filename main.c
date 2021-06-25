#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "types.h"
//#include "utility.h"
//#include "dictionary.h"
#include "game.h"
#define	DELIMETER			'|'
#define SPACE				' '
#define NUMBER_DIRECTIONS	10

static const char *directions[] = {"north", "northeast", "east", "southeast", 
		"south", "southwest", "west", "northwest", "up", "down"};

static const char *shortDirections[] = {"n", "ne", "e", "se", "s", "sw", "w", "nw", "u", "d"};

Location *initializeLocations(int *map_size)
{
	// Read file with location info
	// File must be of the following format:
	// Number of locations\n
	// Location Name | Location Description

	// Define map (Location array)
	Location *map = NULL;

	FILE *file;
	char *line = NULL;
	size_t len = 0;
	size_t read;
	int line_count = 0;

	file = fopen("./locations", "r");

	if (file == NULL)
	{
		printf("locations file is missing. Exiting game.\n");
		exit(EXIT_FAILURE);
	}

	char *location_name = NULL;
	char *location_description = NULL;
	char *current_char = NULL;
	while ((read = getline(&line, &len, file)) != -1)
	{
		if (line_count == 0)
		{
			// Reading first line, allocate space for map array

			// Get size of map (number of locations)
			*map_size = atoi(line);

			//printf("map_size: %d", map_size);
			map = (Location *)malloc(*map_size * sizeof(Location));
		} else 
		{
			// Create Location struct from given line -> parse line

			// Replace delimeter with \0 
			// Get location name
			location_name = line;
			current_char = line;

			while (*current_char != '\0')
			{
				if (*current_char == DELIMETER)
				{
					*current_char = '\0';
					// Set description to point to character after delimeter
					location_description = current_char + 1;
					break;
				}
				current_char++;
			}

			// Create Location struct

			// Set location name
			map[line_count - 1].name = (char *)malloc(strlen(location_name) + 1);
			strcpy(map[line_count - 1].name, location_name);

			// Set location description
			map[line_count - 1].description = (char *)malloc(strlen(location_description) + 1);
			strcpy(map[line_count - 1].description, location_description);

			// Set location paths
			map[line_count - 1].paths = NULL;	
		}

		line_count++;
	}

	free(line);

	fclose(file);

	return map;
}

void initializePaths(Location *map, int map_size)
{
	// Read next file with path info
	// Number of locations\n
	// index of 1st location connected to | 1st location direction to | 1st location how connected to | etc.
	// until
	// first \n then repeat for next location (each line represents the connections the corresponding
	// location in the previous file had in the same line)

	FILE *file;
	char *line = NULL;
	size_t len = 0;
	size_t read;
	int line_count = 0;

	file = fopen("./paths", "r");

	if (file == NULL)
	{
		printf("paths file is missing. Exiting game.\n");
		exit(EXIT_FAILURE);
	}

	int read_map_size = -1;
	int index_of_to_location = -1;
	int num_edge_records = 0;	// if 0, looking at index; 1, direction; 2, path type (mods back)
	char *to_Location = NULL;
	char *path_direction = NULL;
	char *path_type = NULL; 
	char *current_char = NULL;

	Location *from = NULL;
	Location *to = NULL;

	//int encountered_delim = 0; 	// Boolean (1 if previous char is a delimeter (useful for double delim separation))
	while ((read = getline(&line, &len, file)) != -1)
	{
		if (line_count == 0)
		{
			// Reading first line, should match map_size. If not, exit (incorrect number of locations)
			read_map_size = atoi(line);

			if (read_map_size != map_size)
			{
				printf("Number of locations in paths file does not match number in locations file. Exiting game.\n");
				exit(EXIT_FAILURE);
			}
		} else
		{
			// Read paths that location has
			current_char = line;

			if (*current_char == '\0')
			{
				// Location has no edges, skip it
				continue;
			}

			to_Location = line;
			num_edge_records = 1;

			while (*current_char != '\0')
			{
				//printf("%c", *current_char);
				if (*current_char == DELIMETER)
				{
					*current_char = '\0';
					if (*(current_char + 1) == DELIMETER)
					{
						// Finished reading edge data. Add new edge to location.
						// The three pointers have been set to a particular path

						// Add edge to location at index line_count - 1
						
						index_of_to_location = atoi(to_Location);

						from = &map[line_count - 1];
						to = &map[index_of_to_location];

						addEdgeToLocation(from, to, path_direction, path_type);
					} else
					{
						// Still reading edge data. Assign data to correct variable.
						switch (num_edge_records)
						{
							case 0:
								to_Location = current_char + 1;
								break;
							case 1:
								path_direction = current_char + 1;
								break;
							case 2:
								path_type = current_char + 1;
								break;
							default:
								break;
						}

						num_edge_records = (num_edge_records + 1) % 3;
					}
				}

				current_char++;
			}
		}

		line_count++;
	}

	free(line);

	fclose(file);
}

int move(char *direction, Player *player, Location *map)
{
	if (direction == NULL || strlen(direction) == 0)
	{
		//printf("direction: %s\n", direction);
		return -1;
	}

	//int numPaths = getNumberOfEdges(player->currentLocation->paths);

	//printf("numPaths: %d\n", numPaths);

	//char **directions = (char **)malloc(numPaths * sizeof(char *));

	//getDirections(directions, player->currentLocation->paths);

	Edge *currentEdge = player->currentLocation->paths;
	while (currentEdge != NULL)
	{
		//printf("Direction: %s\n", currentEdge->direction);

		if (strcmp(direction, currentEdge->direction) == 0)
		{
			printf("Going %s...\n", currentEdge->direction);
			player->currentLocation = currentEdge->to;
			return 0;
		}

		currentEdge = currentEdge->nextEdge;
	}

	return -1; // Didn't find direction
}

void inputCommand(char *input, int length, Player *player, Location *map)
{
	if (length <= 0)
	{
		printf("I'm afraid you must say something if I am to help you.");
	}

	// Split input line into an array, convert all lines to lowercase, then
	// funnel input by first word command.

	int spacePositions[length];

	// Iterate over input string and specify locations of spaces
	char *currentChar = input;
	int charPos = 0;
	int numWords = 0;
	int isWord = 0;

	while (*currentChar != '\0')
	{
		//printf("Current char: %c %d\n", *currentChar, *currentChar);
		if (*currentChar == SPACE && isWord)
		{
			// store space position (right after previous word, and right before next word)
			spacePositions[numWords] = charPos;
			numWords++;
			isWord = 0;
			*currentChar = '\0';
		} else if (*currentChar >= 33 && *currentChar <= 126)
		{
			isWord = 1;

			if (numWords == 0) // If first char is not a space, it must be a word
			{
				spacePositions[numWords] = charPos - 1; // store -1 to start at index 0 (imaginary space before first word in index 0)
				numWords++;
			}
		}
		else if (*currentChar == '\n') // Replace new line with \0 char
		{
			*currentChar ='\0';
		}
		currentChar++;
		charPos++;
	}

	//printf("Modified line: %s\n", input);

	// Now the number of words and the positions of the spaces separating them is known,
	// get first word

	char *firstWord = input + spacePositions[0] + 1;

	//printf("space before first word position: %d\n", spacePositions[0]);

	//printf("First word: %s |\n", firstWord);
	//printf("numWords = %d\n", numWords);

	if (strcmp(firstWord, "quit") == 0)
	{
		printf("Bye!\n");
		exit(EXIT_SUCCESS);
	} else if (strcmp(firstWord, "go") == 0)
	{
		//printf("Going!\n");
		if (numWords < 2)
		{
			printf("Where do you want to go?\n");
		} else {
			//printf("Going somewhere\n");
			char *direction = input + spacePositions[1] + 1;
			if (move(direction, player, map) != 0)
			{
				printf("You can't go %s.\n", direction);
			}
		}
	} else if (strcmp(firstWord, "look") == 0)
	{
		if (numWords < 2)
		{
			printf("%s\n", player->currentLocation->description);
		} else 
		{
			// Look at an object
			printf("Looking at an object...\n");
		}
	} else 
	{
		printf("For an aspring adventurer, you sure know how to mumble.\n");
	}

	//printf("exiting inputCommands\n");
}

void inputCommand2(char *input, int length, Player *player, Location *map, Entry **hashTable)
{
	if (length <= 0)
	{
		printf("I'm afraid you must say something if I am to help you.");
	}

	// Trim input string (remove white space from both ends)
	char *trimmedString = trimWhiteSpace(input, length);
	//printf("Trimmed input: %s|\n", trimmedString);

	// Convert string to lowercase

	toLowerCase(trimmedString);

	int sizeOfInput = 0;

	// Split trimmed input
	char **inputArray = splitString(trimmedString, &sizeOfInput, SPACE);

	//printf("inputSize: %d\n", sizeOfInput);

	// Call function based on first word and send rest of input to it

	char *functionName = inputArray[0];

	// Convert any alias (e.g. "north" or "go" to "move") to fit hash table key

	// Check whether input is a direction and convert it to "go [direction]"
	int directionIndex = isStringInArray(functionName, directions, NUMBER_DIRECTIONS);

	if (directionIndex == -1)
	{
		directionIndex = isStringInArray(functionName, shortDirections, NUMBER_DIRECTIONS);
	}

	if (directionIndex != -1)
	{
		functionName = inputArray[0] = "go";

		sizeOfInput = 2;

		realloc(inputArray, sizeOfInput * sizeof(char *));
		
		inputArray[1] = (char *)malloc(strlen(directions[directionIndex]) + 1);
		strcpy(inputArray[1], directions[directionIndex]);
	}

	Entry *result = get(functionName, hashTable);

	if (result == NULL)
	{
		printf("I'm afraid I don't understand.\n");
	} else {
		//printf("result->name: %s\n", result->name);

		// Call function
		result->fun(inputArray, sizeOfInput, player, map);
	}
}

int look(char **inputArray, int inputSize, Player *player, Location *map)
{
	printf("%s\n", player->currentLocation->description);
}

int quit(char **inputArray, int inputSize, Player *player, Location *map)
{
	printf("Bye!\n");
	exit(EXIT_SUCCESS);
}

int go(char **inputArray, int inputSize, Player *player, Location *map)
{
	char *direction = NULL;
	if (inputSize == 1 && strcmp(inputArray[0], "go") == 0) 
	{
		printf("Where do you want to go?\n");
		return 1;
	}
	else
	{
		direction = inputArray[1];
	}

	Edge *currentEdge = player->currentLocation->paths;
	while (currentEdge != NULL)
	{
		//printf("Direction: %s\n", currentEdge->direction);

		if (strcmp(direction, currentEdge->direction) == 0)
		{
			printf("Going %s...\n", currentEdge->direction);
			player->currentLocation = currentEdge->to;
			return 0;
		}

		currentEdge = currentEdge->nextEdge;
	}

	printf("You can't go %s.\n\n", direction);
	return -1; // Didn't find direction
}

void fillHashTable(Entry **hashTable)
{
	// Add "move" to the hash table
	//addToTable("move", move, hashTable);

	// Add "look" to the hash table
	addToTable("look", look, hashTable);

	// Add "quit" to the hash table
	addToTable("quit", quit, hashTable);

	// Add "go" to the hash table
	addToTable("go", go, hashTable);
}

int main(void) 
{
	/*	Variables	*/
	Location *map = NULL;
	int map_size = -1;

	Player *player;

	Entry **funHashTable = NULL;	// Function hash table

	const char *directions[] = {"north", "northeast", "east", "southeast", 
		"south", "southwest", "west", "northwest", "up", "down"};

	const char *shortDirections[] = {"n", "ne", "e", "se", "s", "sw", "w", "nw", "u", "d"};

	// Initialize locations
	map = initializeLocations(&map_size);

	// Initialize paths
	initializePaths(map, map_size);

	// Initialize function hash table
	initializeHashTable(&funHashTable);

	fillHashTable(funHashTable);

	//printMap(map, map_size);

	// Create player
	player = (Player *)malloc(sizeof(Player));

	// Set player's HP
	player->maxHP = player->HP = 250;

	// Initialize player's location to Evelore Woods
	player->currentLocation = &map[0];

	// Clear screen - thanks, Jamesits! https://stackoverflow.com/questions/2347770/how-do-you-clear-the-console-screen-in-c
	system("@cls||clear");

	// Variables to read user input
	char *line = NULL;
	size_t len = 0;
	size_t read;

	while (player->HP > 0)
	{
		printf("%s\n", player->currentLocation->name);
		printf("%s\n", player->currentLocation->description);
		printDirections(player->currentLocation);
		//printf("\n");
		printf("> ");

		read = getline(&line, &len, stdin);
		//printf("\n");

		//printf("Line: %s\n", line);
		//inputCommand(line, read, player, map);
		inputCommand2(line, read, player, map, funHashTable);
		//printf("\n");
	}
	
	return 0;
}