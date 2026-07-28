#ifndef MCMT_DS_RESULT_H
#define MCMT_DS_RESULT_H

#include <stdbool.h>


typedef struct {
    char* text;
    char* text_source;
    float wpm;

    bool  play;
    bool  malloced;

    char  error_msg[256];
} mcmt_Result;

void mcmt_result_init(mcmt_Result* result);
void mcmt_result_free(mcmt_Result* result);


#endif //MCMT_DS_RESULT_H
