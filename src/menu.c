#include <ncurses.h>

#include "menu.h"

static const int WIDTH = 30;
static const int HEIGHT = 10;

static char* options[] = {"Time", "Word", "Infinite"};
static int options_len = sizeof(options)/sizeof(options[0]);
static int choice = 0;

static void print_menu(WINDOW *menu_win, int highlight) {
    int x, y, i;	

    x = 2;
    y = 2;
    box(menu_win, 0, 0);
    for(i = 0; i < options_len; ++i)
    {	if(highlight == i + 1) /* High light the present choice */
        {	wattron(menu_win, A_REVERSE); 
            mvwprintw(menu_win, y, x, "%s", options[i]);
            wattroff(menu_win, A_REVERSE);
        } else {
            mvwprintw(menu_win, y, x, "%s", options[i]);
        }
        ++y;
    }
    wrefresh(menu_win);
}

char* mcmt_menu() {
    int row, col, cury, curx;
    int highlight = 1;
    int ch = 0;

    getmaxyx(stdscr, row, col);
    int startx = (col - WIDTH) / 2;
    int starty = (row - HEIGHT) / 2;

	WINDOW* menu_win = newwin(HEIGHT, WIDTH, starty, startx);
    keypad(menu_win, true);
	mvprintw(0, 0, "Use arrow keys to go up and down, Press enter to select a choice");
    refresh();
    print_menu(menu_win, highlight);
    while(true) {
        ch = wgetch(menu_win);

        switch (ch) {
            case 'k':
            case KEY_UP:
                if(highlight == 1)
                    highlight = options_len;
                else
                    --highlight;
                break;
            case 'j':
            case KEY_DOWN:
                if(highlight == options_len)
                    highlight = 1;
                else 
                    ++highlight;
                break;
            case 10:
                choice = highlight;
                break;
            default:
                mvprintw(1, 0, "Character pressed is = %3d Hopefully it can be printed as '%c'", ch, ch);
                refresh();
                break;
        }
        print_menu(menu_win, highlight);
		if(choice != 0)
			break;
    }

    return options[choice - 1];
}
