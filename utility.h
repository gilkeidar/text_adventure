#include "types.h"

int getNumberOfEdges(Edge *paths);

void getDirections(char **directions, Edge *paths);

void addEdgeToLocation(Location *from, Location *to, char *direction, char *pathType);

void printEdge(Edge *edge);

void printEdges(Location *loc);

void toLowerCase(char *stringPtr);

void printMap(Location *map, int map_size);

void printDirections(Location *currentLocation);

char *trimWhiteSpace(char *string, int length);

char **splitString(char *string, char delim);