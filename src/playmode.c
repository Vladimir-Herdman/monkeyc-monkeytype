#include <string.h>

#include "menu.h"
#include "playmode.h"

#define option_is(str) (strncmp(option, str, sizeof(str)) == 0)

static void quote(mcmt_Result* result, char* option) {
    //TODO: generate off first four lines in quotes.txt lengths to use for all these values, so how to get the line number randomly generated and properly gotten.

    if (option_is("all")) {

    } else if (option_is("short")) {

    } else if (option_is("medium")) {

    } else if (option_is("medium")) {

    } else if (option_is("medium")) {

    } else {
        result->gameloop = false;
        char* error_msg = "in mcmt_Result quote(mcmt_Result, char*) - Improper option passed to quote.";
        memcpy(result->error_msg, error_msg, sizeof(result->error_msg));
    }
}

void mcmt_playmode(mcmt_Result* result, Choice choice) {
    switch (choice.mode) {
        case TIME:
            break;
        case WORD:
            break;
        case QUOTE:
            quote(result, choice.option);
        case ZEN:
            break;
        case CUSTOM:
            break;
        case SETTINGS:
            break;
    }
}

#undef strsame
