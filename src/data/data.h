#ifndef MCMT_DATA_H
#define MCMT_DATA_H

typedef struct {
    char* quote;
    char* source;
} mcmt_Quote;

extern const mcmt_Quote quotes_data[];

extern const int nenglish_data;
extern const char* english_data[];

extern const int nenglish_10k_data;
extern const char* english_10k_data[];

extern const int nenglish_1k_data;
extern const char* english_1k_data[];

extern const int nenglish_25k_data;
extern const char* english_25k_data[];

extern const int nenglish_450k_data;
extern const char* english_450k_data[];

extern const int nenglish_5k_data;
extern const char* english_5k_data[];

extern const int nenglish_commonly_misspelled_data;
extern const char* english_commonly_misspelled_data[];

extern const int nenglish_contractions_data;
extern const char* english_contractions_data[];

extern const int nenglish_doubleletter_data;
extern const char* english_doubleletter_data[];

extern const int nenglish_legal_data;
extern const char* english_legal_data[];

extern const int nenglish_medical_data;
extern const char* english_medical_data[];

extern const int nenglish_old_data;
extern const char* english_old_data[];

extern const int nenglish_shakespearean_data;
extern const char* english_shakespearean_data[];

#endif //MCMT_DATA_H