# ifndef AVL_H
# define ALV_H

/* avl.h
 * Declares all functions for the AVL tree.
 * 
 * The tree is ordered by event ID (primary key).
 * 
 * Sections:
 *  1. Internal utilities (height, balance factor, rotations)
 *  2. Core AVL operations (insert, remove, search)
 *  3. Update operations (status, severity)
 *  4. Tree metrics (height, node count, active count)
 *  5. Memory management (free tree)*/

# include "event.h"

/* Global rotation counter.
 * Tracks the total number of rotations performed since the program started. Defined in avl.c.*/
extern int totalRotations;


/* --- INTERNAL UTILITIES --- */

/* Returns the height of a node.
 * Return 0 if node is NULL (safe for leaf children).*/
int getHeight(EventNode *node);

/* Returns the balance factor of a node:
 *  balance = height(left) - height(right)*/
int getBalanceFactor(EventNode *node);

/* Recalculates and updates the height field of a node based on its current children's heights.
 * Must be called after any structural change.*/
void updateHeight(EventNode *node);

/* Performs a simple right rotation on the given node.
 * Returns the new root of the subtree.*/
EventNode *rotateRight(EventNode *node);

/* Performs a simple left rotation on the given node.
 * Return the new root of the subtree.*/
EventNode *rotateLeft(EventNode *node);

/* Detects the imbalance case and applis the correct rotation (simple or double).
 * Returns the new root of the subtree.*/
EventNode *balance(EventNode *node);


/* --- CORE AVL OPERATIONS --- */

/* Inserts a new event into the AVL tree.
 * Only events with status RESOLVED can be removed.
 * Rebalances the tree after removal.
 * Returns the new root of the (sub)tree.
 * Sets *success to 1 if removed, 0 otherwise.*/
EventNode *insertEvent(EventNode *root, int id, EventType type, int severity, 
DateTime timestamp, const char *region);

/* Removes an event from the AVL tree by ID.
 * Only events with status RESOLVED can be removed.
 * Rebalances the tree after removal.
 * Return the new root of the (sub)tree.
 * Sets *success to 1 if removed, 0 otherwise.*/
EventNode *removeEvent(EventNode *root, int id, int *success);

/* Searches for an event by ID.
 * Returns a pointer to the node if found, NULL otherwise.*/
EventNode *searchEvent(EventNode *root, int id);

/* Returns the node with the smallest ID in the subtree.
 * Used internally during removal (in-order successor).*/
EventNode *getMinNode(EventNode *node);


/* --- UPDATE OPERATIONS --- */

/* Changes the status of an event from ACTIVE to RESOLVED.
 * Returns 1 if successful, 0 if event not found.*/
int updateStatus(EventNode *root, int id);

/* Updates the severity of an ACTIVE event.
 * Returns 1 if successful, 0 if event not found or inactive.*/
int updateSeverity(EventNode *root, int id, int newSeverity);


/* --- TREE METRICS --- */

// Returns the total height of the tree.
int getTreeHeight(EventNode *root);

// Returns the total number of nodes in the tree.
int countNodes(EventNode *root);

// Returns the number of nodes with status ACTIVE.
int countActiveEvents(EventNode *root);

/* Calculates the average absolute balance factor across all nodes in the tree.
 * Returns 0.0 if the tree is empty.*/
float getAverageBalanceFactor(EventNode *root);


/* --- MEMORY MANAGEMENT --- */

/* Recursively frees all nodes in the tree.
 * Must be called before the program exists.*/
void freeTree(EventNode *root);

# endif /* AVL_H */
