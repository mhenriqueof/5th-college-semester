# include <stdio.h>
# include "event.h"

/* event.c
 * Implements the helper functions declared in event.h:
 *  - eventTypeToString
 *  - eventStatusToString
 *  - printEvent*/


/* eventTypeToString
 * Maps an EventType enum value to a readable string.
 * Returns "Unknown" as a safe fallback for invalid values.*/
const char *eventTypeToString(EventType type) {
    switch (type) {
        case TRAFFIC_ACCIDENT:
            return "Traffic Accident";
        case TRAFFIC_LIGHT_FAILURE:
            return "Traffic Light Failure";
        case POWER_OUTAGE:
            return "Power Outage";
        case FLOODING:
            return "Flooding";
        case FIRE:
            return "Fire";
        default:
            return "Unknown";
    }
}

/* eventStatusToString
 * Maps an EventStatus enum value to a readable string.*/
const char *eventStatusToString(EventStatus status) {
    switch (status)
    {
    case ACTIVE:
        return "Active";
    case RESOLVED:
        return "Resolved";
    default:
        return "Unknown";
    }
}

/* printEvent
 * Prints all fields of an event node to stdout in a formatted, readable layout.
 * Receives a const pointer.*/
void printEvent(const EventNode *node) {
    if (node == NULL) {
        printf(" [No event to display]\n");
        return;
    }

    printf("  +----------------------------------+\n");
    printf("  | ID       : %-4d                  \n", node->id);
    printf("  | Type     : %-24s \n", eventTypeToString(node->type));
    printf("  | Severity : %-4d                  \n", node->severity);
    printf("  | Region   : %-24s \n", node->region);
    printf("  | Status   : %-24s \n", eventStatusToString(node->status));
    printf("  | Date     : %02d/%02d/%04d             \n",
           node->timestamp.day,
           node->timestamp.month,
           node->timestamp.year);
    printf("  | Time     : %02d:%02d             \n",
           node->timestamp.hour,
           node->timestamp.minute);
    printf("  +----------------------------------+\n");
}
