#include "tad_controller.h"

static unsigned char state, nameCharCount;
static char portName[3];
static unsigned char PortNameStr[] = "PORT NAME:";

void controllerInit (void) {
    state = 0;
    nameCharCount = 0;
    LcGotoXY(0,0);
}

void controllerMotor(void) {
     switch (state) { //TOWER SELECTION
        case 0:
            printPortString(PortNameStr, sizeof(PortNameStr), 0);
            unsigned char pos = getPosTecla();
            if (hiHaTecla()) {
            unsigned char tecla = getTecla();
            if (tecla == '*')
            {
                clearScreen();
                resetPosTecla();
            } else if (tecla == '#'){
                state++;
            }
            else{
                if (pos > 2) pos = 2;
                LcGotoXY(pos,1);
                portName[pos] = tecla;
                LcPutChar(tecla);
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