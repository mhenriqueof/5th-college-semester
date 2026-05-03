# include <stdio.h>
# include <string.h>
# include "queries.h"

/* queries.c
 * Implements the three advanced query functions.*/


/* queryBySeverityRange
 * Full in-order traversal.*/
int queryBySeverityRange(EventNode *root, int minSev, int maxSev) {
    if (root == NULL) return 0;

    int count = 0;

    count += queryBySeverityRange(root->left, minSev, maxSev);

    if (root->status == ACTIVE &&
        root->severity >= minSev &&
        root->severity <= maxSev) {
        printEvent(root);
        count++;
    }

    count += queryBySeverityRange(root->right, minSev, maxSev);

    return count;
}

/* queryByRegion
 * Full in-order traversal
 * Results are automatically ordered by ID because in-order traversal visits nodes in 
 * ascensing ID order.*/
int queryByRegion(EventNode *root, const char *region) {
    if (root == NULL) return 0;

    int count = 0;

    count += queryByRegion(root->left, region);

    if (root->status == ACTIVE && strcmp(root->region, region) == 0) {
        printEvent(root);
        count++;
    }

    count += queryByRegion(root->right, region);

    return count;
}

/* queryByIdRange
 * Pruned traversal.*/
int queryByIdRange(EventNode *root, int minId, int maxId) {
    if (root == NULL) return 0;

    int count = 0;

    // Only go left if there could be valid IDs there
    if (root->id > minId) {
        count += queryByIdRange(root->left, minId, maxId);
    }
    
    // Visit current node if it's within range
    if (root->id >= minId && root->id <= maxId) {
        printEvent(root);
        count++;
    }

    // Only go right if there could be valid IDs there
    if (root->id < maxId) {
        count += queryByIdRange(root->right, minId, maxId);
    }

    return count;
}
