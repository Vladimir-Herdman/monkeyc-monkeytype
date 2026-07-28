#include <ncurses.h>
#include <stdio.h>
#include <string.h>

#include "play/mode_setup.h"
#include "play/playmode.h"

#define return_error(funcstr, errstr)                                                                                  \
    do {                                                                                                               \
        snprintf(result->error_msg, sizeof(result->error_msg), "\n\tin %s > %s\n\t%s", __FILE__, (funcstr), (errstr)); \
        return;                                                                                                        \
    } while (0)
#define pass do { } while (0)


typedef enum {
    DEFAULT = 1,
    SUCCESS,
    ERROR,
} mcmt_COLOR ;


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
    wmove(w, y, x);
    return true;
}

//will return the style of the current character cell.
static mcmt_COLOR back_space(WINDOW* w) {
    int y, x, wch;
    getyx(w, y, x);

    if (x == 0 || (mvwinch(w, y, x-1) & A_CHARTEXT) == ' ') {
        wmove(w, y, x);
        return DEFAULT;
    }

    wmove(w, y, x-1);
    wch = winch(w) & A_CHARTEXT;
    mcmt_COLOR cell_colorpair = PAIR_NUMBER(winch(w));
    wattron(w, COLOR_PAIR(DEFAULT));
    waddch(w, wch);
    wattroff(w, COLOR_PAIR(DEFAULT));
    wmove(w, y, x-1);

    return cell_colorpair;
}

static void goto_next_line(mcmt_Result* res, WINDOW* w) {
    int y, maxy, x, maxx, ch=0;
    getyx(w, y, x);
    getmaxyx(w, maxy, maxx);
    if (y == (maxy - 1)) {
        res->play = false;
        return;
    }
    while ((ch = wgetch(w)) != ' ')
        if (ch == 127) {
            back_space(w);
            return;
        }

    getyx(w, y, x);
    wmove(w, y+1, 0);
    if (is_end_of_line(w))
        res->play = false;
}

static void correct_letter(WINDOW* w, const int wch) {
    wattron(w, COLOR_PAIR(SUCCESS));
    waddch(w, wch);
    wattroff(w, COLOR_PAIR(SUCCESS));
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
    const int ntext = strlen(result->text);
    const int nline = 80;
    const int nlines = ntext/nline + 1;
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
    int nerror = 0;
    while (result->play) {
        ch = wgetch(play_pad);
        wch = winch(play_pad) & A_CHARTEXT;

        if (ch == 127) {
            if (back_space(play_pad) == ERROR)
                --nerror;
        }
        else if (wch == ' ' && nerror > 0)
            continue;
        else if (ch == wch)
            correct_letter(play_pad, wch);
        else if (ch == '1') //REMOVE
            scroll(play_pad);
        else {
            if (wch == ' ')
                continue;
            error_letter(play_pad, wch);
            ++nerror;
        }

        int y, x;
        getyx(play_pad, y, x);
        if (is_end_of_line(play_pad) && nerror == 0) {
            prefresh(play_pad, 0, 0, 0, 0, row-1, col-1);
            goto_next_line(result, play_pad);
        }
        else
            wmove(play_pad, y, x);

        prefresh(play_pad, 0, 0, 0, 0, row-1, col-1);
    }

    wclear(play_pad);
    delwin(play_pad);
}

//TODO: Add a settings screen. Once done, game loop should restart back at the main menu.
static void settings(mcmt_Result* result, char* option) {
    if (option != NULL) option[0] = '\0'; //get rid of warning so it just fits in map.
    return_error("static void settings()", "TODO: not implemented");
}

static void quit(mcmt_Result* result, char* option) {
    if (option != NULL) option[0] = '\0'; //get rid of warning so it just fits in map.
    result->play = false;
}

static const SetupModeFuncPtr mode_map[] = {
    [TIME] = &mcmt_time_setup,
    [WORD] = &mcmt_word_setup,
    [QUOTE] = &mcmt_quote_setup,
    [ZEN] = &mcmt_zen_setup,
    [CUSTOM] = &mcmt_custom_setup,
    [SETTINGS] = &settings,
    [QUIT] = &quit,
};


void mcmt_playmode(mcmt_Result* result, mcmt_Choice choice) {
    const SetupModeFuncPtr modesetup = mode_map[choice.mode];
    modesetup(result, choice.option);

    if (result->error_msg[0] == '\0' && result->play)
        play(result);
}


#undef return_error
