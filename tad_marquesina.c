#include <xc.h>

#include "tad_marquesina.h"
#include "tad_timer.h"
#include "tad_lcd.h"
#include "tad_hora.h"

static unsigned char stringSelector, timer1s, nameCharCount, updateRecordingsFlag, singleRecordingFlag;
static RowState rowStates[2];
static const unsigned char PortNameStr[] = "PORT NAME:";
static const unsigned char MenuOp1[] = "1. START RECORDING ";
static const unsigned char MenuOp2[] = "2. PLAY RECORDINGS ";
static const unsigned char MenuOp3[] = "3. MODIFY CURRENT TIME ";
static const unsigned char MenuOp4[] = "4. SHOW CURRENT TIME ";
static const unsigned char MenuOp5[] = "5. END COMMUNICATION ";
static const unsigned char OP1[] = "RECORDING...";
static const unsigned char OP2_2[] = "PLAYING...";
static const unsigned char OP3[] = "MODIFY TIME:";
static const unsigned char OP4[] = "CURRENT TIME:";
static const unsigned char NoRecordings[] = "THERE ARE NO RECORDINGS! ";
static unsigned char OP5[13] = "bye bye <n>!";
static unsigned char clock[6] = "MM:SS";
static unsigned char recordingStrRow0[18];
static unsigned char recordingStrRow1[18];
static Recording *recordingRow0;
static Recording *recordingRow1;

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
    resetStringSelector();
    singleRecordingFlag = 0;
}

void resetStringSelector(void) {
    stringSelector = 0;
    clearScreen();
}

unsigned char* getNewHora(void) {
    return clock;
}

void resetModifyClock(void) {
    clock[0] = 'M';
    clock[1] = 'M';
    clock[3] = 'S';
    clock[4] = 'S';
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

void changeNoRecView(void) {
    stringSelector = 10;
}

void setRecordingOptions(Recording *newRecordingRow0, Recording *newRecordingRow1) {
    recordingRow0 = newRecordingRow0;
    recordingRow1 = newRecordingRow1;
    updateRecordingsFlag = 1;
    clearScreen();
}

void setSingleRecordFlag (unsigned char c) {
    singleRecordingFlag = c;
}

void updateRecordingsStr(void) {
    if (updateRecordingsFlag) {
        static unsigned char transferSteps = 0;
        static unsigned char counter = 0;
        switch (transferSteps) {
            case 0:
                if (recordingRow0->index[counter] == '\0') {
                    recordingStrRow0[counter++] = ' ';
                    recordingStrRow0[counter++] = '-';
                    recordingStrRow0[counter++] = ' ';
                    counter = 0;
                    transferSteps++;
                }else {
                    recordingStrRow0[counter] = recordingRow0->index[counter];
                    counter++;
                }
                break;
            case 1:
                if (counter == recordingRow0->index_len + 5 + 3) {
                    transferSteps++;
                    counter = 0;
                }else {
                    recordingStrRow0[recordingRow0->index_len + 3 + counter] = recordingRow0->timestamp[counter];
                    counter++;
                }
                break;
            case 2:
                if (recordingRow1->index[counter] == '\0') {
                    recordingStrRow1[counter++] = ' ';
                    recordingStrRow1[counter++] = '-';
                    recordingStrRow1[counter++] = ' ';
                    counter = 0;
                    transferSteps++;
                }else {
                    recordingStrRow1[counter] = recordingRow1->index[counter];
                    counter++;
                }
                break;
            case 3:
                if (counter == recordingRow1->index_len + 5 + 3) {
                    transferSteps=0;
                    counter = 0;
                    updateRecordingsFlag = 0;
                }else {
                    recordingStrRow1[recordingRow1->index_len + 3 + counter] = recordingRow1->timestamp[counter];
                    counter++;
                }
                break;       
        }
    }
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
            updateRecordingsStr();
            printPortString(recordingStrRow0, recordingRow0->index_len + 5 + 3, 0);
            if (!singleRecordingFlag) {
                printPortString(recordingStrRow1, recordingRow1->index_len + 5 + 3, 1);
            }
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
        case 10:
            printPortString((unsigned char *)NoRecordings, sizeof(NoRecordings) - 1, 0);
            break;
    }
    
}
