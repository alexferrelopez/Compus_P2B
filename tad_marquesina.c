#include <xc.h>

#include "tad_marquesina.h"

static unsigned char xPos, stringIndex, menuIndex, flagChange, marquesinaTimer;
static char PortNameStr[] = "PORT NAME:";
static char MenuOp1[] = "1. Start Recording";
static char MenuOp2[] = "2. Play Recordings";
static char MenuOp3[] = "3. Modify Current Time";
static char MenuOp4[] = "4. Show Current Time";
static char MenuOp5[] = "5. End Communication";

void marquesinaInit (void) {
    xPos = 0;
    stringIndex = 0;
    menuIndex = 0;
    TI_NewTimer(&marquesinaTimer);
    flagChange = 1;
}

void printPortString (unsigned char *menuStrHandle, unsigned char size, unsigned char row) {
    if (!flagChange) return;
    LcGotoXY(xPos, row);
    LcPutChar(menuStrHandle[stringIndex]);
    xPos++;
    stringIndex ++;
    if (xPos == 17) xPos = 0;
    if (stringIndex == size - 1) flagChange = 0;
}

void clearScreen (void) {
    LcClear();
    xPos = 0;
    stringIndex = 0;
    flagChange = 1;
}

void startMenu(void) {
    flagChange = 1;
    menuIndex = 1;
}

void nextOp () {
    if (menuIndex < 4) {
        menuIndex++;
        flagChange = 1;
    }
}

void previousOp () {
    if (menuIndex > 0) {
        menuIndex--;
        flagChange = 1;
    }
}

void marquesinaMotor(void){
    if (flagChange) {
        switch (menuIndex) {
            case 0:// PORT NAME
            
                break;
            case 1://OP 1,2
                
                break;
            case 2://OP 2,3
                
                break;
            case 3://OP 3,4
                
                break;
            case 4://OP 4,5
                
                break;
        }
    }
}
