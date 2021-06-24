#include "types.h"
typedef struct Entry {		/* Hash-table entry */
	struct Entry *next;		/* Next entry in same index */
	char *name;				/* Entry name */
	int (*fun)(char **, Player *, Location *);	/* Function pointer */
} Entry;