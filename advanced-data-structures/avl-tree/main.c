# include <stdlib.h>
# include <windows.h>

# include "src/event.h"
# include "src/avl.h"
# include "src/menu.h"

/* main.c
 * Entry point of the Smart City Critical Event System.
 *
 * Responsibilities:
 *  - Set console output to UTF-8 to support special characters
 *  - Initialize the AVL tree root as NULL
 *  - Hand control to runSystem()
 *  - Free all allocated memory before exiting*/

int main() {
    SetConsoleOutputCP(65001);

    EventNode *root = NULL;
    runSystem(&root);
    freeTree(root);

    return 0;
}
