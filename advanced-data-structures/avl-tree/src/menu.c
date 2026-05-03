# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "menu.h"
# include "avl.h"
# include "queries.h"

/* menu.c
 * Implements the interactive text menu system.
 *
 * Static (internal) submenus:
 *  - eventsMenu: insert, remove, search
 *  - updatesMenu: status, severity
 *  - queriesMenu: advanced queries
 *  - metricsMenu: tree metrics*/

 
/* clearInputBuffer
 * Clears leftover characters from stdin after a scanf.
 * Without this, leftover '\n' characters cause menus to skip waiting for user input.*/
static void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/* readInt
 * Safely reads an integer from stdin.
 * Keeps asking until the user provides a valid integer.*/
static int readInt(const char *prompt) {
    int value;
    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &value) == 1) {
            clearInputBuffer();
            return value;
        }
        clearInputBuffer();
        printf("  [Error] Invalid input. Please enter a number.\n");
    }
}

/* readString
 * Safely reads a string from stdin into a buffer.*/
static void readString(const char *prompt, char *buffer, int size) {
    printf("%s", prompt);
    fgets(buffer, size, stdin);
    // Remove trailing newline left by fgets
    buffer[strcspn(buffer, "\n")] = '\0';
}

/* printMainMenu
 * Displays the main menu options.*/
void printMainMenu(void) {
    printf("\n  ╔══════════════════════════════════════╗\n");
    printf("  ║   Smart City Critical Event System   ║\n");
    printf("  ╠══════════════════════════════════════╣\n");
    printf("  ║  1. Events                           ║\n");
    printf("  ║  2. Updates                          ║\n");
    printf("  ║  3. Queries                          ║\n");
    printf("  ║  4. Metrics                          ║\n");
    printf("  ║  0. Exit                             ║\n");
    printf("  ╚══════════════════════════════════════╝\n");
}

/* eventsMenu
 * Handles event insertion, removal and search by ID.*/
static void eventsMenu(EventNode **root) {
    int option;

    do {
        printf("\n  ╔══════════════════════════════╗\n");
        printf("  ║         Events Menu          ║\n");
        printf("  ╠══════════════════════════════╣\n");
        printf("  ║  1. Register new event       ║\n");
        printf("  ║  2. Remove resolved event    ║\n");
        printf("  ║  3. Search event by ID       ║\n");
        printf("  ║  0. Back                     ║\n");
        printf("  ╚══════════════════════════════╝\n");

        option = readInt("  Option: ");

        switch (option) {
            case 1: {
                int id       = readInt("  Event ID: ");
                int severity = readInt("  Severity (1-5): ");

                if (severity < 1 || severity > 5) {
                    printf("  [Error] Severity must be between 1 and 5.\n");
                    break;
                }

                if (searchEvent(*root, id) != NULL) {
                    printf("  [Warning] Event with ID %d already exists.\n", id);
                    break;
                }

                printf("  Event type:\n");
                printf("    1. Traffic Accident\n");
                printf("    2. Traffic Light Failure\n");
                printf("    3. Power Outage\n");
                printf("    4. Flooding\n");
                printf("    5. Fire\n");
                int typeChoice = readInt("  Type: ");

                if (typeChoice < 1 || typeChoice > 5) {
                    printf("  [Error] Invalid event type.\n");
                    break;
                }

                EventType type = (EventType)(typeChoice - 1);

                char region[64];
                readString("  Region: ", region, sizeof(region));

                DateTime dt;
                dt.day = readInt("  Day: ");
                dt.month = readInt("  Month: ");
                dt.year = readInt("  Year: ");
                dt.hour = readInt("  Hour: ");
                dt.minute = readInt("  Minute: ");

                *root = insertEvent(*root, id, type, severity, dt, region);
                printf("  [OK] Event %d registered successfully.\n", id);
                break;
            }
            case 2: {
                int id = readInt("  Event ID to remove: ");
                int success = 0;
                *root = removeEvent(*root, id, &success);
                if (success)
                    printf("  [OK] Event %d removed successfully.\n", id);
                break;
            }
            case 3: {
                int id = readInt("  Event ID to search: ");
                EventNode *node = searchEvent(*root, id);
                if (node == NULL)
                    printf("  [Warning] Event with ID %d not found.\n", id);
                else
                    printEvent(node);
                break;
            }
            case 0:
                printf("  Returning to main menu...\n");
                break;
            default:
                printf("  [Error] Invalid option.\n");
        }
    } while (option != 0);
}

/* updatesMenu
 * Handles status and severity updates.*/
static void updatesMenu(EventNode **root) {
    int option;

    do {
        printf("\n  ╔══════════════════════════════╗\n");
        printf("  ║        Updates Menu          ║\n");
        printf("  ╠══════════════════════════════╣\n");
        printf("  ║  1. Mark event as resolved   ║\n");
        printf("  ║  2. Update event severity    ║\n");
        printf("  ║  0. Back                     ║\n");
        printf("  ╚══════════════════════════════╝\n");

        option = readInt("  Option: ");

        switch (option) {
            case 1: {
                int id = readInt("  Event ID: ");
                updateStatus(*root, id);
                break;
            }
            case 2: {
                int id = readInt("  Event ID: ");
                int severity = readInt("  New severity (1-5): ");
                updateSeverity(*root, id, severity);
                break;
            }
            case 0:
                printf("  Returning to main menu...\n");
                break;
            default:
                printf("  [Error] Invalid option.\n");
        }
    } while (option != 0);
}

/* queriesMenu
 * Handles the three advanced queries.*/
static void queriesMenu(EventNode **root) {
    int option;

    do {
        printf("\n  ╔══════════════════════════════╗\n");
        printf("  ║        Queries Menu          ║\n");
        printf("  ╠══════════════════════════════╣\n");
        printf("  ║  1. Events by severity range ║\n");
        printf("  ║  2. Events by region         ║\n");
        printf("  ║  3. Events by ID range       ║\n");
        printf("  ║  0. Back                     ║\n");
        printf("  ╚══════════════════════════════╝\n");

        option = readInt("  Option: ");

        switch (option) {
            case 1: {
                int minSev = readInt("  Minimum severity (1-5): ");
                int maxSev = readInt("  Maximum severity (1-5): ");

                if (minSev > maxSev) {
                    printf("  [Error] Minimum severity cannot be greater than maximum.\n");
                    break;
                }

                printf("\n  --- Events with severity between %d and %d ---\n",
                       minSev, maxSev);
                int count = queryBySeverityRange(*root, minSev, maxSev);
                if (count == 0)
                    printf("  No events found matching your criteria.\n");
                else
                    printf("  %d event(s) found.\n", count);
                break;
            }
            case 2: {
                char region[64];
                readString("  Region name: ", region, sizeof(region));

                printf("\n  --- Active events in region: %s ---\n", region);
                int count = queryByRegion(*root, region);
                if (count == 0)
                    printf("  No events found matching your criteria.\n");
                else
                    printf("  %d event(s) found.\n", count);
                break;
            }
            case 3: {
                int minId = readInt("  Minimum ID: ");
                int maxId = readInt("  Maximum ID: ");

                if (minId > maxId) {
                    printf("  [Error] Minimum ID cannot be greater than maximum.\n");
                    break;
                }

                printf("\n  --- Events with ID between %d and %d ---\n",
                       minId, maxId);
                int count = queryByIdRange(*root, minId, maxId);
                if (count == 0)
                    printf("  No events found matching your criteria.\n");
                else
                    printf("  %d event(s) found.\n", count);
                break;
            }
            case 0:
                printf("  Returning to main menu...\n");
                break;
            default:
                printf("  [Error] Invalid option.\n");
        }
    } while (option != 0);
}

/* metricsMenu
 * Displays AVL tree metrics.*/
static void metricsMenu(EventNode **root) {
    int option;

    do {
        printf("\n  ╔══════════════════════════════╗\n");
        printf("  ║        Metrics Menu          ║\n");
        printf("  ╠══════════════════════════════╣\n");
        printf("  ║  1. Show all metrics         ║\n");
        printf("  ║  0. Back                     ║\n");
        printf("  ╚══════════════════════════════╝\n");

        option = readInt("  Option: ");

        switch (option) {
            case 1: {
                if (*root == NULL) {
                    printf("  [Warning] The tree is empty.\n");
                    break;
                }

                printf("\n  --- AVL Tree Metrics ---\n");
                printf("  Total height          : %d\n", getTreeHeight(*root));
                printf("  Total nodes           : %d\n", countNodes(*root));
                printf("  Active events         : %d\n", countActiveEvents(*root));
                printf("  Avg. balance factor   : %.2f\n", getAverageBalanceFactor(*root));
                printf("  Total rotations       : %d\n", totalRotations);
                break;
            }
            case 0:
                printf("  Returning to main menu...\n");
                break;
            default:
                printf("  [Error] Invalid option.\n");
        }
    } while (option != 0);
}

/* runSystem
 * Main loop of the system.
 * Keeps running until the user chooses to exit.*/
void runSystem(EventNode **root) {
    int option;

    printf("\n  Welcome to the Smart City Critical Event System!\n");

    do {
        printMainMenu();
        option = readInt("  Option: ");

        switch (option) {
            case 1:
                eventsMenu(root);
                break;
            case 2:
                updatesMenu(root);
                break;
            case 3:
                queriesMenu(root);
                break;
            case 4:
                metricsMenu(root);
                break;
            case 0:
                printf("\n  Shutting down the system. Goodbye!\n");
                break;
            default:
                printf("  [Error] Invalid option.\n");
        }
    } while (option != 0);
}
