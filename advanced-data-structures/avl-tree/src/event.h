# ifndef EVENT_H
# define EVENT_H

/* event.h
 * Defines the core data types used throughout the system:
 * - EventType: enum for the type of critical event
 * - EventStatus: enum for the current status of an event
 * - DateTime: struct for timestamp (date and time)
 * - EventNode: AVL tree node representing one event
*/


/* --- ENUMS --- */

/* EventType
 * Represents the category of a critical urban event.
 * Using an enum instead of raw strings ensures type safety and faster comparisons.*/
typedef enum {
    TRAFFIC_ACCIDENT,
    TRAFFIC_LIGHT_FAILURE,
    POWER_OUTAGE,
    FLOODING,
    FIRE
} EventType;

/* EventStatus
 * An event is ether active (being handled) or resolved (closed). Only resolved events can 
 * be removed from the tree.*/
typedef enum {
    ACTIVE,
    RESOLVED
} EventStatus;


/* --- STRUCTS --- */

/* DateTime
 * Stores the registration timestamp of an event.
 * Kept as a separate struct to group related fields and allow future extension (if necessary).*/
typedef struct {
    int day;
    int month;
    int year;
    int hour;
    int minute;
} DateTime;

/* EventNode
 * A single node in the AVL tree.
 * The primary sort key is 'id' (ascending).
 * 
 * AVL-specific fields:
 *  height: height of this node's subtree (leaf = 1)
 *  left: pointer to the left child (id < this->id)
 *  right: pointer to the right child (id > this->id)*/
typedef struct EventNode {
    int id; // primary key
    EventType type; // category of the event
    int severity; // 1 (low) to 5 (critical)
    DateTime timestamp; // registration date/time
    char region[64]; // City region
    EventStatus status; // ACTIVE or RESOLVED

    // AVL control fields
    int height;
    struct EventNode *left;
    struct EventNode *right;
} EventNode;


/* --- FUNCTIONS --- */

/* Helper: returns a readable string for an EventType.
 * Declared here, defined in event.c.*/
const char *eventTypeToString(EventType type);

/* Helper: returns a readable string for an EventStatus.
 * Declared here, defined in event.c.*/
const char *eventStatusToString(EventStatus status);

/* Helper: prints all fields of an event to stdout.
 * Declared here, defined in event.c.*/
void printEvent(const EventNode *node);

# endif /* EVENT_H */
