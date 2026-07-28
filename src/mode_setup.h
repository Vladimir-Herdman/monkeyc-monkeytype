#ifndef MCMT_MODE_SETUP_H
#define MCMT_MODE_SETUP_H

//These methods exist to setup result->text so that mcmt_playmode can
//then start a typing session no matter the mode (generally).

#include "result.h"


typedef void (*SetupModeFuncPtr)(mcmt_Result*, char* option);

void mcmt_time_setup(mcmt_Result* result, char* option);
void mcmt_word_setup(mcmt_Result* result, char* option);
void mcmt_quote_setup(mcmt_Result* result, char* option);
void mcmt_zen_setup(mcmt_Result* result, char* option);
void mcmt_custom_setup(mcmt_Result* result, char* option);


#endif //MCMT_MODE_SETUP_H
