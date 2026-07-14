#ifndef MCMT_PLAYMODE_H
#define MCMT_PLAYMODE_H

#include <stdbool.h>

#include "menu.h"

typedef struct {
    char* text;
    char* text_source;
    float wpm;

    bool  malloced;
    char  error_msg[256];
} mcmt_Result;

void mcmt_result_free(mcmt_Result* result);

void mcmt_playmode(mcmt_Result* result, mcmt_Choice choice);

#endif //MCMT_PLAYMODE_H
