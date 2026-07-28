#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "datafiles/data.h"
#include "play/mode_setup.h"

#define option_is(str) strncmp(option, (str), sizeof((str))) == 0
#define return_error(funcstr, errstr)                                                                                  \
    do {                                                                                                               \
        snprintf(result->error_msg, sizeof(result->error_msg), "\n\tin %s > %s\n\t%s", __FILE__, (funcstr), (errstr)); \
        return;                                                                                                        \
    } while (0)
#define pass do { } while (0)


void mcmt_time_setup(mcmt_Result* result, char* option) {
    if (option != NULL) option[0] = '\0'; //REMOVE. For now until implemented, to get rid of option warning.
    return_error("static void time()", "TODO: not implemented");
}

void mcmt_word_setup(mcmt_Result* result, char* option) {
    //TODO: Get array from options of which english type to use.
    const char** wordarr = english_data;
    const int nwordarr = nenglish_data;
    const int nwords = strtol(option, NULL, 10);
    int str_size=100, cur_size=0;
    result->text = malloc(sizeof(char)*str_size);
    result->text[0] = '\0';
    int windex;
    const char* word;
    for (int i=0; i<nwords; i++) {
        windex = rand() % nwordarr;
        word = wordarr[windex];
        cur_size += strlen(word) + 1;
        if (cur_size >= str_size) {
            str_size *= 1.5;
            char* temp = realloc(result->text, str_size);
            if (temp == NULL)
                return_error("static void word", "realloc failure for result->text");
            result->text = temp;
        }
        strncat(result->text, word, strlen(word));
        strncat(result->text, (i == (nwords-1) ? "." : " "), 1);
    }
}

void mcmt_quote_setup(mcmt_Result* result, char* option) {
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

void mcmt_zen_setup(mcmt_Result* result, char* option) {
    if (option != NULL) option[0] = '\0'; //get rid of warning so it just fits in map.
    return_error("static void zen()", "TODO: not implemented");
}

void mcmt_custom_setup(mcmt_Result* result, char* option) {
    if (option != NULL) option[0] = '\0'; //get rid of warning so it just fits in map.
    return_error("static void custom()", "TODO: not implemented");
}


#undef option_is
#undef return_error
