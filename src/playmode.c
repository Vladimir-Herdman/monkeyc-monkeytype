#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data/data.h"
#include "menu.h"
#include "playmode.h"

#define option_is(str) strncmp(option, (str), sizeof((str))) == 0
#define return_error(funcstr, errstr)                                                                                  \
    do {                                                                                                               \
        result->gameloop = false;                                                                                      \
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
    //TODO: generate off first four lines in quotes.txt lengths to use for all these values, so how to get the line number randomly generated and properly gotten.
    const int nall    = strtol(strchr(quotes_data[0], ':')+1, NULL, 10);
    const int nshort  = strtol(strchr(quotes_data[1], ':')+1, NULL, 10);
    const int nmedium = strtol(strchr(quotes_data[2], ':')+1, NULL, 10);
    const int nlong   = strtol(strchr(quotes_data[3], ':')+1, NULL, 10);
    const int nthicc  = strtol(strchr(quotes_data[4], ':')+1, NULL, 10);
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

    const int qindex = (rand() % end_range) + start_range;
    const char* quote = quotes_data[qindex];
    return_error("hre", quote);

    result->gameloop = false;
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

void mcmt_playmode(mcmt_Result* result, Choice choice) {
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
            result->gameloop = false;
            break;
    }
}

#undef option_is
#undef return_error
