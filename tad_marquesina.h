#ifndef _MARQUESINA_H
#define _MARQUESINA_H

#include "tad_timer.h"
#include "tad_lcd.h"

typedef struct RowState {
    unsigned char basePos;
    unsigned char stringIndex;
    unsigned char screenXPos;
    unsigned char marquesinaTimer;
} RowState;

void marquesinaInit (void);
void marquesinaMotor(void);
void clearScreen (void);
void startMenu(void);
void setMenuOption (unsigned char index);
void printPortString (unsigned char *menuStrHandle, unsigned char size, unsigned char row);
#endif