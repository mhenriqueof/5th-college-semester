# ifndef MENU_H
# define MENU_H

/* menu.h
 * Declares the public interface for the interactive text menu system.
 * The submenus are static functions defined in menu.c.*/

# include "event.h"


/* runSystem
 * Initializes the system and runs the main loop.
 * Keeps running until the user chooses to exit.
 * Receives a double pointer to the tree root so it can modify it (insertions, removals).*/
void runSystem(EventNode **root);

/* printMainMenu
 * Displays the main menu options to stdout.*/
void printMainMenu(void);

# endif /* MENU_H */
