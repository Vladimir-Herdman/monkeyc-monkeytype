#ifndef MCMT_MENU_H
#define MCMT_MENU_H

typedef enum {TIME=0, WORD, QUOTE, ZEN, CUSTOM, SETTINGS, QUIT} mcmt_Modes;
extern const char* modemap[];

typedef struct {
    mcmt_Modes mode;
    char* option;
} Choice;

Choice mcmt_menu();

#endif //MCMT_MENU_H
