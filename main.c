#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define	DELIMETER	'|'

struct Edge;
struct Location;

typedef struct Location {
	char *name;
	char *description;
	struct Edge *paths;
} Location;

typedef struct Edge {
	struct Location *from;
	struct Location *to;
	char *direction;
	char *pathType;
	struct Edge *nextEdge;	// Next connection of particular location (acts as linked list node)
} Edge;

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


void printMap(Location *map, int map_size)
{
	for (int i = 0; i < map_size; i++)
	{
		printf("Location: %s | Description: %s\n", map[i].name, map[i].description);
		printf("Edges from %s:\n", map[i].name);
		printEdges(&map[i]);
		printf("\n");
	}
}

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

int main(void) 
{
	/*	Variables	*/
	Location *map = NULL;
	int map_size = -1;

	// Initialize locations
	map = initializeLocations(&map_size);

	// Initialize paths
	initializePaths(map, map_size);

	printMap(map, map_size);

	


	return 0;
}