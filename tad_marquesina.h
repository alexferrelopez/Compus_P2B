#ifndef _MARQUESINA_H
#define _MARQUESINA_H

#include "tad_eeprom.h"

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
void enterOption (signed char index);
void setNameCharCount(unsigned char charCount);
void setGoobyeName (unsigned char index, char character);
void printPortString (unsigned char *menuStrHandle, unsigned char size, unsigned char row);
void setCharClock(unsigned char character, unsigned char index);
void setRecordingOptions (Recording *newRecordingRow0, Recording *newRecordingRow1);
unsigned char* getNewHora(void);
void resetModifyClock(void);
void changeWelcomeView(void);
void resetStringSelector(void);
void changeNoRecView(void);
void setSingleRecordFlag (unsigned char c);

#endif