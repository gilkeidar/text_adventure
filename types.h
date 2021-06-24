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