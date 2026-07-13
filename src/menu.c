#include <ncurses.h>

#include "menu.h"

static const int WIDTH = 30;
static const int HEIGHT = 10;

typedef struct {
    char* name;
    char* settings[6];
} MenuOptions;

const char* modemap[] = {"time", "word", "quote", "zen", "custom", "settings", "quit"};
static MenuOptions options[] = {
    {"time",     {"15", "30", "60", "120", "Custom", NULL}},
    {"word",     {"10", "25", "50", "100", "Custom", NULL}},
    {"quote",    {"all", "short", "medium", "long", "thicc", NULL}},
    {"zen",      {NULL}},
    {"custom",   {NULL}},
    {"settings", {"punctuation", "numbers", NULL}}, //TODO: Figure out settings and what should go in here, or if it should just open a separate application windows.
    {"quit", {NULL}},
};
static const int full_options_len = (sizeof(options)/sizeof(options[0])) - 1;
static int choice = -1;

static void clear_menu(WINDOW* menu_win) {
    int x, y;	

    x = 2;
    y = 2;
    for (int i=0; i<full_options_len; i++) {
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
        if(highlight == i) /* Highlight the present choice */
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
    int highlight = 0;
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

    int options_len = full_options_len;
    char* curoptions[full_options_len+1];
    for (int i=0; i<full_options_len; i++)
        curoptions[i] = options[i].name;
    print_menu(menu_win, highlight, curoptions, options_len);
    bool final_choice = false;
    int mode = 0;
    while(true) {
        ch = wgetch(menu_win);

        switch (ch) {
            case 'k':
            case KEY_UP:
                if(highlight == 0)
                    highlight = options_len-1;
                else
                    --highlight;
                break;

            case 'j':
            case KEY_DOWN:
                if(highlight == options_len-1)
                    highlight = 0;
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
                        setting = options[highlight].settings[i];
                        curoptions[i] = setting;
                        ++i;
                    } while (setting != NULL);
                    options_len = i-1;
                    highlight = 0;
                }
                break;

            case 'q':
            case 27: //escape
                if (final_choice) {
                    final_choice = false;
                    highlight = mode;
                    options_len = full_options_len;
                    for (int i=0; i<full_options_len; i++)
                        curoptions[i] = options[i].name;
                } else {
                    final_choice = true;
                    mode = QUIT;
                    choice = highlight = 0;
                }
                break;

            default:
                //mvprintw(1, 0, "Character pressed is = %3d Hopefully it can be printed as '%c'", ch, ch);
                refresh();
                break;
        }
        print_menu(menu_win, highlight, curoptions, options_len);
		if(choice != -1 && final_choice == true)
			break;
    }

    curs_set(1);
    return (Choice){(mcmt_Modes)(mode), options[mode].settings[choice]};
}
