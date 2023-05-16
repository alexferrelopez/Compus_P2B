#include "tad_controller.h"
#include "tad_SIO.h"
#include "tad_adc.h"

static unsigned char state, nameCharCount, menuIndex;
static char portName[3];
static unsigned char PortNameStr[] = "PORT NAME:";

void controllerInit (void) {
    state = 0;
    nameCharCount = 0;
    LcGotoXY(0,0);
}

void nextMenuOption () {
    if (menuIndex < 4) {
        menuIndex++;
        clearScreen();
    }
}

void previousMenuOption () {
    if (menuIndex > 0) {
        menuIndex--;
        clearScreen();
    }
}

void controllerMotor(void) {
     switch (state) { //TOWER SELECTION
        case 0:
            //printPortString(PortNameStr, sizeof(PortNameStr), 0);
            
            if (hiHaTecla()) {
                unsigned char pos = getPosTecla();
                unsigned char tecla = getTecla();
                if (tecla == '*')
                {
                    clearScreen();
                    resetPosTecla();
                } else if (tecla == '#'){
                    state++;
                }
                else {
                    if (pos > 2) pos = 2;
                    LcGotoXY(pos,1);
                    portName[pos] = tecla;
                    LcPutChar(tecla);
                }
                    setSonidoTecla(getIndexTecla());
                    teclaProcesada();
            }
            break;
        case 1: //START COMMS
            if (SiIsAvailable()) {
                SiSendChar('T');
                state++;
            }
            break;
        case 2: //WAIT FOR ANSWER
            if (SiCharAvail()) {
                if (SiGetChar() == 'K') state++;
            }
            break;
        case 3:
            if (SiIsAvailable()) {
                static unsigned char charIndex = 0;
                if (charIndex == 3) {
                    SiSendChar('\0');
                    state ++;
                    startMenu();
                    //START THE TIME COUNTER?�
                } else {
                    SiSendChar(portName[charIndex]);
                    charIndex++;
                }
            }
         case 4:
            getJoystickMove();
        /* code */
        break; 
    }    
}