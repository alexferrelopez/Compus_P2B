#include <xc.h>

#include "tad_marquesina.h"

static unsigned char state, xPos, lastIndex, stringIndex, state, menuIndex, flagChange, marquesinaTimer;
static char PortNameStr[] = "PORT NAME:";
static char MenuOp1[] = "1. Start Recording";
static char MenuOp2[] = "2. Play Recordings";
static char MenuOp3[] = "3. Modify Current Time";
static char MenuOp4[] = "4. Show Current Time";
static char MenuOp5[] = "5. End Communication";

void marquesinaInit (void) {
    xPos = 0;
    lastIndex = 0;
    stringIndex = 0;
    menuIndex = 0;
    TI_NewTimer(&marquesinaTimer);
}

unsigned char printPortString (unsigned char index) {
    if (lastIndex != index) {
        xPos = 0;
        stringIndex = 0;
    }
    if (index == 1) {
        LcGotoXY(xPos, 0);
        LcPutChar(PortNameStr[stringIndex]);
        xPos++;
        stringIndex ++;
        if (xPos == 17) xPos = 0;
    }
    lastIndex = index;
    
    return (stringIndex == sizeof(PortNameStr)-1) ? 1:0;
}

void clearScreen (void) {
    LcClear();
    xPos = 0;
    stringIndex = 0;
}

void startMenu(void) {
    flagChange = 1;
}

void nextOp () {
    if (menuIndex < 3) {
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
        
        flagChange = 0;
    }
}
