#include <stdio.h>
#include <string.h>

#include "menu.h"
#include "playmode.h"

#define option_is(str) strncmp(option, (str), sizeof((str))) == 0
#define return_error(funcstr, errstr)                                                                   \
    do {                                                                                    \
        result->gameloop = false;                                                           \
        snprintf(result->error_msg, sizeof(result->error_msg), "\n\tin %s > %s\n\t%s", __FILE__, (funcstr), (errstr)); \
        return;                                                                             \
    } while (0)

//TODO: Currently only works when calling binary from the monkeyc-monkeytype directory.
static const char* quote_filepath = "./data/quotes.txt";

static void time(mcmt_Result* result, char* option) {
    return_error("static void time()", "TODO: not implemented");
}

static void word(mcmt_Result* result, char* option) {
    return_error("static void word()", "TODO: not implemented");
}

static void quote(mcmt_Result* result, char* option) {
    //TODO: generate off first four lines in quotes.txt lengths to use for all these values, so how to get the line number randomly generated and properly gotten.
    FILE* f = fopen(quote_filepath, "r");
    if (!f)
        return_error("static void quote()", "data/quote.txt not readable.");

    fclose(f);

    if (option_is("all")) {
        return_error("static void quote()", "option 'all' quotes not implemented");
    }
    else if (option_is("short")) {
        return_error("static void quote()", "option 'short' quotes not implemented");
    }
    else if (option_is("medium")) {
        return_error("static void quote()", "option 'medium' quotes not implemented");
    }
    else if (option_is("long")) {
        return_error("static void quote()", "option 'long' quotes not implemented");
    }
    else if (option_is("thicc")) {
        return_error("static void quote()", "option 'thicc' quotes not implemented");
    }
    else {
        return_error("static void quote()", "Improper option passed to quote.");
    }

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
