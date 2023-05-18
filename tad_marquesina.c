#include <xc.h>

#include "tad_marquesina.h"

static unsigned char stringSelector, timer1s, nameCharCount;
static RowState rowStates[2];
static const unsigned char PortNameStr[] = "PORT NAME:";
static const unsigned char MenuOp1[] = "1. Start Recording ";
static const unsigned char MenuOp2[] = "2. Play Recordings ";
static const unsigned char MenuOp3[] = "3. Modify Current Time ";
static const unsigned char MenuOp4[] = "4. Show Current Time ";
static const unsigned char MenuOp5[] = "5. End Communication ";
static const unsigned char OP1[] = "RECORDING...";
static const unsigned char OP2_2[] = "PLAYING...";
static const unsigned char OP3[] = "MODIFY TIME:";
static const unsigned char OP4[] = "CURRENT TIME:";
unsigned char OP5[13] = "bye bye <n>!";
static unsigned char clock[6] = "MM:SS";

void resetRowStates (void) {
    rowStates[0].basePos = rowStates[0].screenXPos = rowStates[0].stringIndex = 0;
    rowStates[1].basePos = rowStates[1].screenXPos = rowStates[1].stringIndex = 0;
    TI_ResetTics(rowStates[0].marquesinaTimer);
    TI_ResetTics(rowStates[1].marquesinaTimer);
}

void marquesinaInit (void) {
    TI_NewTimer(&rowStates[0].marquesinaTimer);
    TI_NewTimer(&rowStates[1].marquesinaTimer);
    TI_NewTimer(&timer1s);
    resetRowStates();
}

unsigned char* getNewHora(void) {
    return clock;
}

void printPortString (unsigned char *menuStrHandle, unsigned char strLen, unsigned char row) {
    if (TI_GetTics(rowStates[row].marquesinaTimer) >= 2) {
        
        if (rowStates[row].basePos >= strLen) {
            rowStates[row].basePos = 0;
        }     
        
        TI_ResetTics(rowStates[row].marquesinaTimer);
        
        LcGotoXY(rowStates[row].screenXPos, row);
        
        unsigned char pos = strLen > 16 ? 
            (rowStates[row].stringIndex + rowStates[row].basePos) : rowStates[row].stringIndex;

        pos >= strLen ? (pos -= strLen) : (pos);
        LcPutChar(menuStrHandle[pos]);
        rowStates[row].screenXPos++;
        rowStates[row].stringIndex++;
        if (rowStates[row].screenXPos > 16 || 
                (rowStates[row].stringIndex == strLen && strLen <= 16)) {
            rowStates[row].screenXPos = 0;
            rowStates[row].stringIndex = 0;
        }
    } 
}

void clearScreen (void) {
    LcClear();
    TI_ResetTics(timer1s);
    resetRowStates();
}

void startMenu(void) {
    stringSelector = 1;
    clearScreen();
}

void setMenuOption (unsigned char index) {
    stringSelector = index > 3 ? index : index+1;
    clearScreen();
}

void incrementaColumna(void) {
    rowStates[0].basePos++;
    rowStates[1].basePos++;
    TI_ResetTics(timer1s);
}

void enterOption (signed char index) {
    stringSelector = (unsigned char) index + 5;
    clearScreen();
}

void setNameCharCount(unsigned char charCount) {
    nameCharCount = charCount;
}

void setGoobyeName (unsigned char index, char character) {
    OP5[8+index] = character;
}

void setCharClock (unsigned char newChar, unsigned char index) {
    clock[index] = newChar;
}

void marquesinaMotor(void){
    if (TI_GetTics(timer1s) >= 1000) incrementaColumna();
    switch (stringSelector) {
        case 0:// PORT NAME
            printPortString((unsigned char *) PortNameStr, sizeof(PortNameStr) - 1, 0);
            break;
        case 1://OP 1
            printPortString((unsigned char *) MenuOp1, sizeof(MenuOp1) - 1, 0);
            printPortString((unsigned char *) MenuOp2, sizeof(MenuOp2) - 1, 1);
            break;
        case 2://OP 2
            printPortString((unsigned char *) MenuOp2, sizeof(MenuOp2) - 1, 0);
            printPortString((unsigned char *) MenuOp3, sizeof(MenuOp3) - 1, 1);
            break;
        case 3://OP 3
            printPortString((unsigned char *) MenuOp3, sizeof(MenuOp3) - 1, 0);
            printPortString((unsigned char *) MenuOp4, sizeof(MenuOp4) - 1, 1);
            break;
        case 4://OP 4,5
            printPortString((unsigned char *) MenuOp4, sizeof(MenuOp4) - 1, 0);
            printPortString((unsigned char *) MenuOp5, sizeof(MenuOp5) - 1, 1);
            break;
        case 5://ENTER OP1
            printPortString((unsigned char *) OP1, sizeof(OP1) - 1, 0);
            break;
        case 6://ENTER OP2
            printPortString((unsigned char *) MenuOp4, sizeof(MenuOp4) - 1, 0);
            break;
        case 7://ENTER OP3
            printPortString((unsigned char *) OP3, sizeof(OP3) - 1, 0);
            printPortString(clock, 5, 1);
            break;
        case 8://ENTER OP4
            printPortString((unsigned char *) OP4, sizeof(OP4) - 1, 0);
            printPortString(getHora(), 5, 1);
            break;
        case 9://ENTER OP5
            printPortString(OP5, 9 + nameCharCount, 0);
            break;
    }
    
}
