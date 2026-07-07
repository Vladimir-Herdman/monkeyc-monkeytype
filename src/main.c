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
    Choice choice = mcmt_menu();
    mcmt_cleanup();

	printf("You chose gamemode %s with option %s\n\r", choice.mode, choice.option);

	return EXIT_SUCCESS;
}

// for ncurses example: ~/code/c/tests/all-libraries/etc/ncurses/pager.c
