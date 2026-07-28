#include <stdlib.h>
#include <stdbool.h>

#include "result.h"


void mcmt_result_init(mcmt_Result* result) {
    result->text = NULL;
    result->text_source = NULL;
    result->wpm = 0;

    result->play = true;
    result->malloced = false;

    result->error_msg[0] = '\0';
}

void mcmt_result_free(mcmt_Result* result) {
    if (! result->malloced) return;

    if (result->text != NULL) free(result->text);
    if (result->text_source != NULL) free(result->text_source);
}
