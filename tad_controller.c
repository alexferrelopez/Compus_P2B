#include "tad_controller.h"
#include "tad_SIO.h"
#include "tad_adc.h"

static unsigned char state, nameCharCount;
static char portName[3];
static signed char menuIndex;
static unsigned char PortNameStr[] = "PORT NAME:";

void controllerInit (void) {
    state = 0;
    nameCharCount = 0;
    LcGotoXY(0,0);
    menuIndex = 0;
}

void changeMenuOption (signed char move) {
    menuIndex += move;
    if (menuIndex > 3) {
        menuIndex = 3;
    } else if (menuIndex < 0) {
        menuIndex = 0;
    }
    setMenuOption((unsigned char) menuIndex);
}

void controllerMotor(void) {
     switch (state) { //TOWER SELECTION
        case 0: 
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
                    nameCharCount = pos+1;
                    LcGotoXY(4,1);
                    LcPutChar(nameCharCount + '0');
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
            if (SiCharAvail() > 0) {
                if (SiGetChar() == 'K') {
                    state++;
                };
            }
            break;
        case 3:
            if (SiIsAvailable()) {
                static unsigned char charIndex = 0;
                if (charIndex == nameCharCount) {
                    SiSendChar('\0');
                    state++;
                    startMenu();
                    //START THE TIME COUNTER?¿
                } else {
                    SiSendChar(portName[charIndex]);
                    charIndex++;
                }
            }
         case 4:
            if (joystickIsDiffPos()) {
                changeMenuOption(getJoystickMove());
            }
            
            
        /* code */
        break; 
    }    
}