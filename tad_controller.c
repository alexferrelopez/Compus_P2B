#include <xc.h>

#include "tad_controller.h"
#include "tad_timer.h"
#include "tad_teclado.h"
#include "tad_lcd.h"
#include "tad_altavoz.h"
#include "tad_marquesina.h"
#include "tad_SIO.h"
#include "tad_adc.h"
#include "tad_hora.h"

static unsigned char state, nameCharCount, timerController;
static char portName[3];
static signed char menuIndex, recordingIndex;

void controllerInit (void) {
    TI_NewTimer(&timerController);
    state = 0;
    nameCharCount = 0;
    LcGotoXY(0,0);
    menuIndex = 0;
    recordingIndex = 0;
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

void changeRecordingOption (signed char move) {
    recordingIndex += move;
    signed char numRecordings = (signed char) getNumRecordings();
    if (recordingIndex >= numRecordings) {
        if (numRecordings > 0) {
            recordingIndex = (numRecordings - 1);
        } else {
            recordingIndex = 0;
        }
    } else if (recordingIndex < 0) {
        recordingIndex = 0;
    }
}

void returnToMenu (void) {
    startMenu();
    state = 4;
    menuIndex = 0;
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
                }
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
                    returnToMenu();
                    //START THE TIME COUNTER
                    startHora();
                    charIndex = 0;
                } else {
                    SiSendChar(portName[charIndex]);
                    setGoobyeName(charIndex, portName[charIndex]);
                    charIndex++;
                }
            }
            break;
        case 4: //SHOW MENU OPTIONS WAITING FOR '#'
            if (joystickIsDiffPos()) {
                changeMenuOption(getJoystickMove());
            }
            if (hiHaTecla()) {
                unsigned char tecla = getTecla();
                if (tecla == '#') {
                    enterOption(menuIndex);
                    state = 5 + (unsigned char) menuIndex;
                    if (menuIndex == 0 || menuIndex == 4) {
                        TI_ResetTics(timerController);
                    } else if (menuIndex == 1) {
                        setRecordingOptions(getRecording(0), getRecording(1));
                        if (getNumRecordings() == 1) setSingleRecordFlag(1);
                        else setSingleRecordFlag(0);
                    }
                }
                setSonidoTecla(getIndexTecla());
                teclaProcesada();
            }
            break;
        case 5: // RECORDING
            startRecording();
            state = 10;
            break;
        case 6: //REPRODUCING
            if (getNumRecordings() == 0) {
                state = 8;
                changeNoRecView();
                recordingIndex = 0;
            }
            else if (joystickIsDiffPos()) {
                changeRecordingOption(getJoystickMove());
                if (recordingIndex == getNumRecordings()-1 && getNumRecordings() > 1) {
                    setRecordingOptions(getRecording((unsigned char)recordingIndex-1), 
                            getRecording((unsigned char)recordingIndex));                
                } else {
                    setRecordingOptions(getRecording((unsigned char)recordingIndex), 
                            getRecording((unsigned char)recordingIndex+1));
                }
            }
            if (hiHaTecla()) {
                unsigned char tecla = getTecla();
                if (tecla == '#') {
                    startReproducing();
                    setIndexToSend(getRecording((unsigned char)recordingIndex)->index);
                    state = 10;
                    recordingIndex = 0;
                } else if (tecla == '*') {
                    returnToMenu();
                    recordingIndex = 0;
                }
                setSonidoTecla(getIndexTecla());
                teclaProcesada();
            }
            break;
        case 7:
            if (hiHaTecla()) {
                static unsigned char indiceModifHora = 0;
                unsigned char tecla = getNumber();
                if (tecla == '*') {
                    // Reiniciamos a MM:SS
                    resetModifyClock();
                    returnToMenu();
                    indiceModifHora = 0;
                } else if (tecla == '#') {
                    if (indiceModifHora == 5) {
                        setHora(getNewHora());
                        indiceModifHora = 0;
                        // Reiniciamos a MM:SS para la proxima vez que quiero modificar la hora.
                        resetModifyClock();
                        returnToMenu();
                    } else {
                        teclaProcesada();
                        break;
                    }
                } else if (indiceModifHora < 5) {
                    if (indiceModifHora == 2) indiceModifHora++;
                    setCharClock(tecla, indiceModifHora);
                    indiceModifHora++;
                }
                teclaProcesada();
            }
            break;
        case 8:
            if (hiHaTecla()) {
                unsigned char tecla = getTecla();
                if (tecla == '*') {
                    returnToMenu();
                }
                teclaProcesada();
            }
            break;    
        case 9:
            // Reiniciamos el tiempo de comunicacion con la torre de control.
            if (TI_GetTics(timerController) > 2000) {
                state = 0;
                menuIndex = 0;
                resetVariablesTeclado();
                resetStringSelector();
            }
            break;
        case 10:
            if (!actionOngoing()) {
                setMelodia();
                returnToMenu();
            }
            break;
    }    
}