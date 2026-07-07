#include <ncurses.h>

#include "menu.h"

static const int WIDTH = 30;
static const int HEIGHT = 10;

typedef struct {
    char* name;
    char* settings[6];
} MenuOptions;

static MenuOptions options[] = {
    {"time",     {"15", "30", "60", "120", "Custom", NULL}},
    {"word",     {"10", "25", "50", "100", "Custom", NULL}},
    {"quote",    {"all", "short", "medium", "long", "thicc", NULL}},
    {"zen",      {NULL}},
    {"custom",   {NULL}},
    {"settings", {"punctuation", "numbers", NULL}}, //TODO: Figure out settings and what should go in here, or if it should just open a separate application windows.
};
static int choice = 0;

static void clear_menu(WINDOW* menu_win) {
    int x, y, i;	

    x = 2;
    y = 2;
    const int base_menu_len = sizeof(options)/sizeof(options[0]);
    for (int i=0; i<base_menu_len; i++) {
        wmove(menu_win, y, x);
        wclrtoeol(menu_win);
        ++y;
    }
}

static void print_menu(WINDOW *menu_win, int highlight, char** curoptions, int options_len) {
    clear_menu(menu_win);
    int x, y, i;	

    x = 2;
    y = 2;
    box(menu_win, 0, 0);
    for(i = 0; i < options_len; ++i)
    {
        if(highlight == i + 1) /* Highlight the present choice */
        {
            wattron(menu_win, A_REVERSE); 
            mvwprintw(menu_win, y, x, "%s", curoptions[i]);
            wattroff(menu_win, A_REVERSE);
        } else {
            mvwprintw(menu_win, y, x, "%s", curoptions[i]);
        }
        ++y;
    }
    wrefresh(menu_win);
}

Choice mcmt_menu() {
    int row, col;
    int highlight = 1;
    int ch = 0;

    getmaxyx(stdscr, row, col);
    int startx = (col - WIDTH) / 2;
    int starty = (row - HEIGHT) / 2;

	WINDOW* menu_win = newwin(HEIGHT, WIDTH, starty, startx);
    keypad(menu_win, true);
    curs_set(0);

    char menu_text[] = "Use arrow keys to go up and down, Press enter to select a choice";
	mvprintw(starty-1, ((col-WIDTH)/2)-(sizeof(menu_text)/4), "%s", menu_text);
    refresh();

    int base_options_len = sizeof(options)/sizeof(options[0]);
    int* options_len = &base_options_len;
    char* curoptions[base_options_len+1];
    for (int i=0; i<base_options_len; i++)
        curoptions[i] = options[i].name;
    print_menu(menu_win, highlight, curoptions, *options_len);
    bool final_choice = false;
    int mode = 0;
    while(true) {
        ch = wgetch(menu_win);

        switch (ch) {
            case 'k':
            case KEY_UP:
                if(highlight == 1)
                    highlight = *options_len;
                else
                    --highlight;
                break;
            case 'j':
            case KEY_DOWN:
                if(highlight == *options_len)
                    highlight = 1;
                else 
                    ++highlight;
                break;
            case 10: //enter
                if (final_choice)
                    choice = highlight;
                else {
                    mode = highlight;
                    final_choice = true;
                    char* setting;
                    int i = 0;
                    do {
                        setting = options[highlight-1].settings[i];
                        curoptions[i] = setting;
                        ++i;
                    } while (setting != NULL);
                    *options_len = i-1;
                    highlight = 1;
                }
                break;
            default:
                //mvprintw(1, 0, "Character pressed is = %3d Hopefully it can be printed as '%c'", ch, ch);
                for (int i=0; i<*options_len; i++) {
                    mvprintw(i, 0, "curoptions[%d]: %s", i, curoptions[i]);
                }
                refresh();
                break;
        }
        print_menu(menu_win, highlight, curoptions, *options_len);
		if((choice != 0 && final_choice) || ch == 'q')
			break;
    }

    curs_set(1);
    return (Choice){options[mode - 1].name, options[mode - 1].settings[choice - 1]};
}
