#ifndef MCMT_PLAYMODE_H
#define MCMT_PLAYMODE_H

#include <stdbool.h>

#include "menu.h"

typedef struct {
    bool gameloop;
    char error_msg[128];
} mcmt_Result;

void mcmt_playmode(mcmt_Result* result, Choice choice);

#endif //MCMT_PLAYMODE_H
