#include <xc.h>

#include "tad_marquesina.h"

static unsigned char stringSelector;
static RowState rowStates[2];
static unsigned char PortNameStr[] = "PORT NAME:";
static unsigned char MenuOp1[] = "1. Start Recording ";
static unsigned char MenuOp2[] = "2. Play Recordings ";
static unsigned char MenuOp3[] = "3. Modify Current Time ";
static unsigned char MenuOp4[] = "4. Show Current Time ";
static unsigned char MenuOp5[] = "5. End Communication ";

void resetRowStates (void) {
    rowStates[0].basePos = rowStates[0].count = rowStates[0].screenXPos = rowStates[0].stringIndex = 0;
    rowStates[1].basePos = rowStates[1].count = rowStates[1].screenXPos = rowStates[1].stringIndex = 0;
    TI_ResetTics(rowStates[0].marquesinaTimer);
    TI_ResetTics(rowStates[1].marquesinaTimer);
}

void marquesinaInit (void) {
    TI_NewTimer(&rowStates[0].marquesinaTimer);
    TI_NewTimer(&rowStates[1].marquesinaTimer);
    resetRowStates();
}

void printPortString (unsigned char *menuStrHandle, unsigned char strLen, unsigned char row) {
    if (TI_GetTics(rowStates[row].marquesinaTimer) >= 5) {
        rowStates[row].count++;
        if (strLen > 16 && rowStates[row].count == 200) {
            rowStates[row].basePos++;
            if (rowStates[row].basePos == strLen) {
                rowStates[row].basePos = 0;
            }
            rowStates[row].count = 0;
        }
        TI_ResetTics(rowStates[row].marquesinaTimer);
        LcGotoXY(rowStates[row].screenXPos, row);
        unsigned char pos = (rowStates[row].stringIndex + rowStates[row].basePos);
        pos >= strLen ? (pos -= strLen) : (pos);
        LcPutChar(menuStrHandle[pos]);
        rowStates[row].screenXPos++;
        rowStates[row].stringIndex++;
        if (rowStates[row].screenXPos > 16) rowStates[row].screenXPos = 0;
        if (rowStates[row].stringIndex == strLen) {
            rowStates[row].stringIndex = 0; 
            rowStates[row].screenXPos = 0;
        }
    } 
}

void clearScreen (void) {
    LcClear();
    resetRowStates();
}

void startMenu(void) {
    stringSelector = 1;
}

void setMenuOption (unsigned char index) {
    stringSelector = index;
}

void marquesinaMotor(void){
    switch (stringSelector) {
        case 0:// PORT NAME
            printPortString(PortNameStr, sizeof(PortNameStr) - 1, 0);
            break;
        case 1://OP 1,2
            printPortString(MenuOp1, sizeof(MenuOp1) - 1, 0);
            printPortString(MenuOp2, sizeof(MenuOp2) - 1, 1);
            break;
        case 2://OP 2,3
            printPortString(MenuOp2, sizeof(MenuOp2) - 1, 0);
            printPortString(MenuOp3, sizeof(MenuOp3) - 1, 1);
            break;
        case 3://OP 3,4
            printPortString(MenuOp3, sizeof(MenuOp3) - 1, 0);
            printPortString(MenuOp4, sizeof(MenuOp4) - 1, 1);
            break;
        case 4://OP 4,5
            printPortString(MenuOp4, sizeof(MenuOp4) - 1, 0);
            printPortString(MenuOp5, sizeof(MenuOp5) - 1, 1);
            break;
    }
    
}
