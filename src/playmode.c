#include <ctype.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data/data.h"
#include "menu.h"
#include "playmode.h"

#define option_is(str) strncmp(option, (str), sizeof((str))) == 0
#define return_error(funcstr, errstr)                                                                                  \
    do {                                                                                                               \
        snprintf(result->error_msg, sizeof(result->error_msg), "\n\tin %s > %s\n\t%s", __FILE__, (funcstr), (errstr)); \
        return;                                                                                                        \
    } while (0)
#define pass do { } while (0)

static void time(mcmt_Result* result, char* option) {
    return_error("static void time()", "TODO: not implemented");
}

static void word(mcmt_Result* result, char* option) {
    return_error("static void word()", "TODO: not implemented");
}

static void quote(mcmt_Result* result, char* option) {
    const int nall    = strtol(strchr(quotes_data[0].quote, ':')+1, NULL, 10);
    const int nshort  = strtol(strchr(quotes_data[1].quote, ':')+1, NULL, 10);
    const int nmedium = strtol(strchr(quotes_data[2].quote, ':')+1, NULL, 10);
    const int nlong   = strtol(strchr(quotes_data[3].quote, ':')+1, NULL, 10);
    const int nthicc  = strtol(strchr(quotes_data[4].quote, ':')+1, NULL, 10);
    int start_range=5, end_range=nall;

    if (option_is("all"))
        pass;
    else if (option_is("short"))
        end_range = nshort+start_range-1;
    else if (option_is("medium")) {
        start_range = nshort+start_range;
        end_range = nmedium+start_range-1;
    }
    else if (option_is("long")) {
        start_range = nshort+nmedium+start_range;
        end_range = nlong+start_range-1;
    }
    else if (option_is("thicc")) {
        start_range = nshort+nmedium+nlong+start_range;
        end_range = nthicc+start_range-1;
    }
    else
        return_error("static void quote()", "Improper option passed to quote.");

    const int qindex = (rand() % (end_range-start_range)) + start_range;
    result->text = quotes_data[qindex].quote;
    result->text_source = quotes_data[qindex].source;
}

static void zen(mcmt_Result* result) {
    return_error("static void zen()", "TODO: not implemented");
}

static void custom(mcmt_Result* result) {
    return_error("static void custom()", "TODO: not implemented");
}

static void settings(mcmt_Result* result, char* option) {
    return_error("static void settings()", "TODO: not implemented");
}

static bool istext(const char c) {
    if (c >= 33 && c <= 126)
        return true;
    return false;
}

//fill dst from src, and set src to point to new position.
static void fill_line_nowrap(char* restrict dst, const char** restrict srcp, const int maxlen) {
    int i=0, rstrip=0;
    const char* src = *srcp;
    for (; i<maxlen-1; i++) {
        const char sc = src[i];
        dst[i] = sc;
        if (sc == '\0') goto endfunc;
    }

    while (istext(src[i-rstrip]))
        ++rstrip;

    dst[i-rstrip] = ' ';
    dst[i-rstrip+1] = '\0';
    --rstrip;

    endfunc:
    *srcp = (*srcp) + (i - rstrip);
}

static enum {
    DEFAULT = 1,
    SUCCESS,
    ERROR,
};

static bool is_end_of_line(WINDOW* w) {
    int y, x, i=0, wch;
    getyx(w, y, x);
    while (wmove(w, y, x+(i++)) != ERR) {
        wch = winch(w) & A_CHARTEXT;
        if (wch != ' ') {
            wmove(w, y, x);
            return false;
        }
    }
    return true;
}

static void goto_next_line(mcmt_Result* res, WINDOW* w) {
    int y, x;
    getyx(w, y, x);
    wmove(w, y+1, 0);
    if (is_end_of_line(w))
        res->play = false;
}

static void goto_prev_line(WINDOW* w) {
    int y, x, i=0;
    getyx(w, y, x);
    wmove(w, y-1, 0);
    while (!is_end_of_line(w))
        wmove(w, y-1, ++i);
    wmove(w, y-1, i-1);
    const int wch = winch(w) & A_CHARTEXT;
    wattron(w, COLOR_PAIR(DEFAULT));
    waddch(w, wch);
    wattroff(w, COLOR_PAIR(DEFAULT));
    wmove(w, y-1, i-1);
}

static void correct_letter(WINDOW* w, const int wch) {
    wattron(w, COLOR_PAIR(SUCCESS));
    waddch(w, wch);
    wattroff(w, COLOR_PAIR(SUCCESS));
}

static void back_space(WINDOW* w) {
    int y, x, wch;
    getyx(w, y, x);
    if (x == 0) {
        if (y != 0) goto_prev_line(w);
        return;
    };
    wmove(w, y, x-1);
    wch = winch(w) & A_CHARTEXT;
    wattron(w, COLOR_PAIR(DEFAULT));
    waddch(w, wch);
    wattroff(w, COLOR_PAIR(DEFAULT));
    wmove(w, y, x-1);
}

static void error_letter(WINDOW* w, const int wch) {
    wattron(w, COLOR_PAIR(ERROR));
    waddch(w, wch);
    wattroff(w, COLOR_PAIR(ERROR));
}

//TODO: take whatever value of 'result' we have, the text, and display
    //it to the screen with the ability to type. Make it look like monkeytype.
    //Afterwards, display info about your typeing results.
static void play(mcmt_Result* result) {
    if (result->text == NULL) return;
    result->play = true;

    int row, col;
    int ch, wch;

    init_pair(DEFAULT, -1, -1);
    init_pair(SUCCESS, COLOR_GREEN, -1);
    init_pair(ERROR, COLOR_RED, -1);

    getmaxyx(stdscr, row, col);
    int startx = col / 2;
    int starty = row / 2;
    const int ntext = strlen(result->text);
    const int nline = 80;
    const int nlines = ntext/nline + 2;
    char line[150];
	WINDOW* play_pad = newpad(nlines, col);

    const char* linestart = result->text;
    for (int i=0; *linestart != '\0'; i++) {
        fill_line_nowrap(line, &linestart, nline);
        mvwprintw(play_pad, i, 0, "%s", line);
    }
    wmove(play_pad, 0, 0);
    refresh();
    prefresh(play_pad, 0, 0, 0, 0, row-1, col-1);
    while (result->play) {
        ch = wgetch(play_pad);
        wch = winch(play_pad) & A_CHARTEXT;

        if (ch == wch) {
            correct_letter(play_pad, wch);
        }
        else if (ch == '1') //REMOVE
            scroll(play_pad);
        else if (ch == 127)
            back_space(play_pad);
        else {
            error_letter(play_pad, wch);
        }

        if (is_end_of_line(play_pad))
            goto_next_line(result, play_pad);

        //refresh();
        prefresh(play_pad, 0, 0, 0, 0, row-1, col-1);
    }

    wclear(play_pad);
    delwin(play_pad);
}

void mcmt_result_free(mcmt_Result* result) {
    if (! result->malloced) return;

    if (result->text != NULL) free(result->text);
    if (result->text_source != NULL) free(result->text_source);
}

void mcmt_playmode(mcmt_Result* result, mcmt_Choice choice) {
    switch (choice.mode) {
        case TIME:
            time(result, choice.option);
            break;

        case WORD:
            word(result, choice.option);
            break;

        case QUOTE:
            quote(result, choice.option);
            break;

        case ZEN:
            zen(result);
            break;

        case CUSTOM:
            custom(result);
            break;

        case SETTINGS:
            settings(result, choice.option);
            break;

        case QUIT:
            return;
    }

    play(result);
}

#undef option_is
#undef return_error
