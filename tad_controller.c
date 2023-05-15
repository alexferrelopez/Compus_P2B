#include <xc.h>

#include "tad_controller.h"

static unsigned char state, nameCharCount, pos;
static char menu1[] = "PORT NAME:";
static char portName[3];
static unsigned char xPos, lastIndex, stringIndex, visited, marquesinaTimer;

void controllerInit (void) {
    state = 0;
    nameCharCount = 0;
    xPos = 0;
    lastIndex = 0;
    pos = 0;
    LcGotoXY(0,0);
    visited = 0;
    TI_NewTimer(&marquesinaTimer);
}


#define macro(x) menu(x)

void printString (unsigned char index) {
    if (lastIndex != index) {
        xPos = 0;
        stringIndex = 0;
    }
    if (index == 1) {
        LcGotoXY(xPos, 0);
        LcPutChar(menu1[stringIndex]);
        xPos++;
        stringIndex ++;
        if (xPos == 17) xPos = 0;
        if (stringIndex == sizeof(menu1)-1) visited = 1;
    }
    lastIndex = index;
    LcGotoXY(pos,1);
}

void controllerMotor(void) {
     switch (state) { //TOWER SELECTION
        case 0:
            if (!visited) printString(1);
            if (hiHaTecla()) {
            unsigned char tecla = getTecla();
            if (tecla == '*')
            {
                LcClear();
                pos = 0;
                visited = 0;
                xPos = 0;
                stringIndex = 0;
            } else if (tecla == '#'){
            
                state++;
            }
            else{
                if (haDeSubstituir() && pos < 2) {
                    pos--;
                }
                
                LcGotoXY(pos,1);
                portName[pos] = tecla;
                LcPutChar(tecla);
                if (pos < 2) pos++;
            }
                setSonidoTecla(getIndexTecla());
                teclaProcesada();
            }
            break;
        case 2: //MENU 
            /* code */
            break;
        case 3:
            /* code */
            break;        
    }    
}