# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <math.h>
# include "avl.h"

/* avl.c
 * Implements all AVL tree operations.
 * 
 * Sections:
 *  1. Internal utilities (height, balance factor, rotations)
 *  2. Core AVL operations (insert, remove, search)
 *  3. Update operations (status, severity)
 *  4. Tree metrics (height, node count, active count)
 *  5. Memory management (free tree)*/

/* Global rotation counter.
 * Incremented inside rotateRight and rotateLeft.*/
int totalRotations = 0;


/* --- INTERNAL UTILITIES --- */

/* getHeight
 * Returns the height stores in the node.
 * Returns 0 for NULL (empty subtree).*/
int getHeight(EventNode *node) {
    if (node == NULL) return 0;
    return node->height;
}

/* getBalanceFactor
 * balance = height(left) - height(right)*/
int getBalanceFactor(EventNode *node) {
    if (node == NULL) return 0;
    return getHeight(node->left) - getHeight(node->right);
}

/* updateHeight
 * Recalculates the height of a node as:
 *  height = 1 + max(height(left), height(right))
 * 
 * Must be called after any structural change to keep height fields consistent
 * throughout the tree.*/
void updateHeight(EventNode *node) {
    if (node == NULL) return;

    int leftHeight = getHeight(node->left);
    int rightHeight = getHeight(node->right);

    node->height = 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
}

/* rotateRight
 * Applied when the tree is left-heavy (balance > 1).*/
EventNode *rotateRight(EventNode *a) {
    EventNode *b = a->left;
    EventNode *orphan = b->right;

    // Perform rotation
    b->right = a;
    a->left = orphan;

    // Update heights - a first (it went down), then b (it went down)
    updateHeight(a);
    updateHeight(b);

    totalRotations++;

    return b;
}

/* rotateLeft
 * Applied when the tree is right-heavy (balance < -1)*/
EventNode *rotateLeft(EventNode *a) {
    EventNode *b = a->right;
    EventNode *orphan = b->left;

    // Perform rotation
    b->left = a;
    a->right = orphan;

    // Update heights - a first (it went down), then b (it went up)
    updateHeight(a);
    updateHeight(b);

    totalRotations++;

    return b;
}

/* balance
 * Checks the balance factor of a node and applies the correct rotation if needed.*/
EventNode *balance(EventNode *node) {
    updateHeight(node);

    int bf = getBalanceFactor(node);
    
    // Left-Left
    if (bf > 1 && getBalanceFactor(node->left) >= 0)
        return rotateRight(node);

    // Left-Right
    if (bf > 1 && getBalanceFactor(node->left) < 0) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    // Right-Right
    if (bf < -1 && getBalanceFactor(node->right) <= 0)
        return rotateLeft(node);

    // Right-Left
    if (bf < -1 && getBalanceFactor(node->right) > 0) {
        node->right = rotateLeft(node->right);
        return rotateLeft(node);
    }

    return node;
}


/* --- CORE AVL OPERATIONS --- */

/* insertEvent
 * Inserts a new event into the tree recursively.*/
EventNode *insertEvent(EventNode *root, int id, EventType type, int severity,
DateTime timestamp, const char *region) {
    // 1. Base case - found the insertion spot
    if (root == NULL) {
        EventNode *newNode = (EventNode *)malloc(sizeof(EventNode));
        if (newNode == NULL) {
            printf(" [Error] Memory allocation failed.\n");
            return NULL;
        }

        newNode->id = id;
        newNode->type = type;
        newNode->severity = severity;
        newNode->timestamp = timestamp;
        newNode->status = ACTIVE;
        newNode->height = 1;
        newNode->left = NULL;
        newNode->right = NULL;
        strncpy(newNode->region, region, sizeof(newNode->region) - 1);
        newNode->region[sizeof(newNode->region) - 1] = '\0';

        return newNode;
    }

    // 2. Navigate left or right by ID
    if (id < root->id)
        root->left = insertEvent(root->left, id, type, severity, timestamp, region);
    else if (id > root->id)
        root->right = insertEvent(root->right, id, type, severity, timestamp, region);
    else {
        // Duplicate ID - now allowed
        printf(" [Warning] Event with ID %d already exists.\n", id);
        return root;
    }

    // 3. Rebalance on the way back up
    return balance(root);
}

/* getMinNode
 * Returns the node with the smallest ID in the subtree.
 * Used to find the in-order successor during removal.*/
EventNode *getMinNode(EventNode *node) {
    if (node == NULL) return NULL;
    if (node->left == NULL) return node;
    return getMinNode(node->left);
}

/* removeEvent
 * Removes an event by ID from the AVL tree.
 * Only RESOLVED events can be removed.*/
EventNode *removeEvent(EventNode *root, int id, int *success) {
    // Base case: ID not found
    if (root == NULL) {
        *success = 0;
        return NULL;
    }

    // Navigate to the target node
    if (id < root->id)
        root->left = removeEvent(root->left, id, success);
    else if (id > root->id)
        root->right = removeEvent(root->right, id, success);
    else {
        // Found the node - check if it can be removed
        if (root->status != RESOLVED) {
            printf(" [Warning] Event %d is still ACTIVE and cannot be removed.\n", *success = 0);
            return root;
        }

        *success = 1;

        // Case 1 and 2: zero or one child
        if (root->left == NULL || root->right == NULL) {
            EventNode *child = (root->left != NULL) ? root->left : root->right;
            free(root);
            return child; // child may be NULL (case 1) or a valid node (case 2)
        }

        // Case 3: two children
        EventNode *successor = getMinNode(root->right);

        // Copy successor data into current node
        root->id = successor->id;
        root->type = successor->type;
        root->severity = successor->severity;
        root->timestamp = successor->timestamp;
        root->status = successor->status;
        strncpy(root->region, successor->region, sizeof(root->region) - 1);
        root->region[sizeof(root->region) - 1] = '\0';

        // Remove the successor from the right subtree
        int dummy = 1;
        root->right = removeEvent(root->right, successor->id, &dummy);
    }

    // Rebalance on the way back up
    return balance(root);
}

/* searchEvent
 * Searches for an event by ID using BST navigation.
 * Return a pointer to the node if found, NULL otherwise.*/
EventNode *searchEvent(EventNode *root, int id) {
    if (root == NULL) return NULL;
    if (id == root->id) return root;
    if (id < root->id) return searchEvent(root->left, id);
    return searchEvent(root->right, id);
}


/* --- UPDATE OPERATIONS --- */

/* updateStatus
 * Searches for an event by ID and changes its status from ACTIVE to RESOLVED.
 * Returns 1 if successful, 0 if not found.*/
int updateStatus(EventNode *root, int id) {
    EventNode *node = searchEvent(root, id);

    if (node == NULL) {
        printf(" [Warning] Event with ID %d not found.\n", id);
        return 0;
    }

    if (node->status == RESOLVED) {
        printf(" [Warning] Event %d is already RESOLVED.\n", id);
        return 0;
    }

    node->status = RESOLVED;
    printf(" [OK] Event %d marked as RESOLVED.\n", id);
    return 1;
}

/* updateSeverity
 * Searches for an ACTIVE event by ID and updates its severity level.
 * Returns 1 if successful, 0 otherwise.*/
int updateSeverity(EventNode *root, int id, int newSeverity) {
    if (newSeverity < 1 || newSeverity > 5) {
        printf(" [Error] Severity must be between 1 and 5.\n");
        return 0;
    }

    EventNode *node = searchEvent(root, id);

    if (node == NULL) {
        printf(" [Warning] Event with ID %d not found.\n", id);
        return 0;
    }

    if (node->status == RESOLVED) {
        printf(" [Warning] Cannot update severity of a RESOLVED event.\n");
        return 0;
    }

    node->severity = newSeverity;
    printf(" [OK] Event %d severity updated to %d.\n", id, newSeverity);
    return 1;
}


/* --- TREE METRICS --- */

/* getTreeHeight
 * Returns the total height of the tree.*/
int getTreeHeight(EventNode *root) {
    return getHeight(root);
}

/* countNodes
 * Counts all nodes recursively.
 *  count = 1 (current) + count(left) + count(right)*/
int countNodes(EventNode *root) {
    if (root == NULL) return 0;
    return 1 + countNodes(root->left) + countNodes(root->right);
}

/* countActiveEvents
 * Counts only nodes with status ACTIVE.*/
int countActiveEvents(EventNode *root) {
    if (root == NULL) return 0;

    int current = (root->status == ACTIVE) ? 1 : 0;
    return current + countActiveEvents(root->left) + countActiveEvents(root->right);
}

/* getAverageBalanceFactor
 * Calculates the everage absolute balance factor across all nodes.
 * We use two helper variables passed by pointer:
 *  totalBF: sum of absolute balance factors.
 *  totalNodes: count of nodes visited.*/
static void _collectBalanceFactor(EventNode *root, float *totalBF, int *totalNodes) {
    if (root == NULL) return;

    *totalBF += (float)abs(getBalanceFactor(root));
    *totalNodes += 1;

    _collectBalanceFactor(root->left, totalBF, totalNodes);
    _collectBalanceFactor(root->right, totalBF, totalNodes);
}

float getAverageBalanceFactor(EventNode *root) {
    if (root == NULL) return 0.0f;

    float totalBF = 0.0f;
    int totalNodes = 0;

    _collectBalanceFactor(root, &totalBF, &totalNodes);

    return totalBF / (float)totalNodes;
}


/* --- MEMORY MANAGEMENT --- */

/* freeTree
 * Recursively frees all nodes using post-order traversal.*/
void freeTree(EventNode *root) {
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}
