#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "menu.h"
#include "playmode.h"

void mcmt_init() {
    srand(time(NULL));
    initscr();
    noecho();
    cbreak();
    start_color();
    use_default_colors();
}

void mcmt_cleanup(mcmt_Result* result) {
    refresh();
	endwin();
    mcmt_result_free(result);
}

int main(int argc, char *argv[])
{
    mcmt_init();

    mcmt_Result result = {0};
    mcmt_Choice choice = {0};

    choice = mcmt_menu();
    mcmt_playmode(&result, choice);

    mcmt_cleanup(&result);

	printf("You chose gamemode '%s' with option '%s'.\n\r", modemap[choice.mode], choice.option);
    if (result.error_msg[0] != '\0') {
        printf("You also had an error message: %s\n", result.error_msg);
    }

	return EXIT_SUCCESS;
}

// for ncurses example: ~/code/c/tests/all-libraries/etc/ncurses/pager.c
