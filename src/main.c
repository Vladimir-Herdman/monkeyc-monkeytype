#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "menu.h"

void mcmt_init() {
    initscr();
    noecho();
    cbreak();
    start_color();
    use_default_colors();
}

void mcmt_cleanup() {
    refresh();
	clrtoeol();
	endwin();
}

int main(int argc, char *argv[])
{
    mcmt_init();
    char* choice = mcmt_menu();
    mcmt_cleanup();

	printf("You chose %s\n\r", choice);

	return 0;
}

// for ncurses example: ~/code/c/tests/all-libraries/etc/ncurses/pager.c
