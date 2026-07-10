#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "data/data.h"
#include "menu.h"
#include "playmode.h"

void mcmt_init() {
    initscr();
    noecho();
    cbreak();
    start_color();
    use_default_colors();
}

void mcmt_cleanup() {
    refresh();
	endwin();
}

int main(int argc, char *argv[])
{
    mcmt_init();

    mcmt_Result result = {0};
    result.gameloop = true;
    Choice choice = {0};
    while (result.gameloop) {
        choice = mcmt_menu();
        mcmt_playmode(&result, choice);
    }

    mcmt_cleanup();

	printf("You chose gamemode '%s' with option '%s'.\n\r", modemap[choice.mode], choice.option);
    if (result.error_msg[0] != '\0') {
        printf("You also had an error message: %s\n", result.error_msg);
    }
    printf("Your line is:\n\t%s\n", quotes_data[5]);

	return EXIT_SUCCESS;
}

// for ncurses example: ~/code/c/tests/all-libraries/etc/ncurses/pager.c
