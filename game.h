// Includes everything

// types.h
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

typedef struct Player {
	struct Location *currentLocation;	// Player's currnt location
	int HP;
	int maxHP;
} Player;

// dictionary.h

typedef struct Entry {		/* Hash-table entry */
	struct Entry *next;		/* Next entry in same index */
	char *name;				/* Entry name */
	int (*fun)(char **, Player *, Location *);	/* Function pointer */
} Entry;

void initializeHashTable(Entry ***hashTable);

void addToTable(char *funcName, int (*fun)(char **, Player *, Location *), Entry **hashTable);

Entry *get(char *funcName, Entry** hashTable);

// utility.h

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