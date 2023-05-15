#include "tad_controller.h"

static unsigned char state, nameCharCount, pos;
static char portName[3];
static unsigned char visited;

void controllerInit (void) {
    state = 0;
    nameCharCount = 0;
    pos = 0;
    LcGotoXY(0,0);
    visited = 0; 
}

void controllerMotor(void) {
     switch (state) { //TOWER SELECTION
        case 0:
            if (!visited) visited = printPortString(1);
            LcGotoXY(pos,1);
            if (hiHaTecla()) {
            unsigned char tecla = getTecla();
            if (tecla == '*')
            {
                clearScreen();
                pos = 0;
                visited = 0;
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