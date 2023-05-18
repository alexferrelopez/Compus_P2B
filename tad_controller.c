#include "tad_controller.h"
#include "tad_SIO.h"
#include "tad_adc.h"
#include "tad_hora.h"

static unsigned char state, nameCharCount;
static char portName[3];
static signed char menuIndex;

void controllerInit (void) {
    state = 0;
    nameCharCount = 0;
    LcGotoXY(0,0);
    menuIndex = 0;
}

void changeMenuOption (signed char move) {
    menuIndex += move;
    if (menuIndex > 4) {
        menuIndex = 4;
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
        case 3: //SEND THE NAME WE HAVE SAVED
            if (SiIsAvailable()) {
                static unsigned char charIndex = 0;
                if (charIndex == nameCharCount) {
                    SiSendChar('\0');
                    setGoobyeName(charIndex, '!');
                    setGoobyeName(charIndex+1, '\0');
                    setNameCharCount(nameCharCount);
                    state++;
                    startMenu();
                    //START THE TIME COUNTER?
                    startHora();
                } else {
                    SiSendChar(portName[charIndex]);
                    setGoobyeName(charIndex, portName[charIndex]);
                    charIndex++;
                }
            }
        case 4: //SHOW MENU OPTIONS WAITING FOR '#'
            if (joystickIsDiffPos()) {
                changeMenuOption(getJoystickMove());
            }
            
            if (hiHaTecla()) {
                unsigned char tecla = getTecla();
                if (tecla == '#') {
                    enterOption(menuIndex);
                    if (menuIndex == 2) {
                        state = 7;
                    }
                }
                setSonidoTecla(getIndexTecla());
                teclaProcesada();
            }
        case 7:
            if (hiHaTecla()) {
                static unsigned char indiceModifHora = 0;
                unsigned char tecla = getNumber();
                if (tecla == '*') {
                    startMenu();
                    state = 4;
                } else if (tecla == '#') {
                    // Guardar como hora actual
                    setHora(getNewHora());
                    indiceModifHora = 0;
                    startMenu();
                } else if (indiceModifHora < 5) {
                    if (indiceModifHora == 2) indiceModifHora++;
                    
                    setCharClock(tecla, indiceModifHora);
                    indiceModifHora++;
                }
                teclaProcesada();
            }
            break; 
    }    
}