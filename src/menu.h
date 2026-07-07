#ifndef MCMT_MENU_H
#define MCMT_MENU_H

typedef struct {
    char* mode;
    char* option;
} Choice;

Choice mcmt_menu();

#endif //MCMT_MENU_H
