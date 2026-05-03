# ifndef QUERIES_H
# define QUERIES_H

/* queries.h
 * Declares advanced query functions for the ALV tree.
 * Functions:
 *  1. queryBySeverityRange: active events by severity.
 *  2. queryByRegion: active events by region.
 *  3. queryByIdRange: events within an ID range.*/

# include "event.h"


/* queryBySeverityRange
 * Prints all ACTIVE events whose severity is between minSev and maxSev (inclusive).
 * Returns the count of matching events found.*/
int queryBySeverityRange(EventNode *root, int minSev, int maxSev);

/* queryByRegion
 * Prints all ACTIVE events from a specific region.
 * Returns the count of matching events found.*/
int queryByRegion(EventNode *root, const char *region);

/* queryByIdRange
 * Prints all events whose ID is between minId and maxId (inclusive), regardless of status.
 * Returns the count fo matching events found.*/
int queryByIdRange(EventNode *root, int minId, int maxId);

# endif /* QUERIES_H */
