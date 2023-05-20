#include <xc.h>
#include "tad_hora.h"
#include "tad_adc.h"
#include "tad_eeprom.h"
#include "tad_SIO.h"

static unsigned char state, incomingCharCount, recordIndex[10], timestamp[5], *indexToSend;

void initSIO(void) {
//Pre: -
//Post: Inicialitza la SIO a 57600 bps (40Mhz)
    TXSTA = 0x20;   //00100000
    RCSTA = 0x90;   //10010000
    BAUDCON = 0;
    TXSTAbits.BRGH = 1;
    BAUDCONbits.BRG16 = 1;
    SPBRG = 174;
    SPBRGH = 0;
    state = 0;
    incomingCharCount = 0;
}

char SiCharAvail(void) {
//Post: returns the number of available characters that are in the reception queue.
//Retorna -1 if there are no available characters.
    return(PIR1bits.RCIF == 1 ? 1 : 0);
}

char SiGetChar(void){
//Pre: SiCharAvail() returns a number greater than zero.
//Post: returns and removes the first element from the reception queue.
    return RCREG;
}
char SiIsAvailable(void) {
//Post: returns 1 if the SIO module is available for sending.
    return TXSTAbits.TRMT;
}

void SiSendChar(char c) {
//Pre: SiIsAvailable().
//Post: starts sending the specified character.
    TXREG = c;
}

void startRecording(void) {
    state = 1;
}

void startReproducing(void) {
    state = 8;
}

unsigned char actionOngoing(void) {
    return state > 0;
}

void setIndexToSend (unsigned char *newIndexToSend) {
    indexToSend = newIndexToSend;
}

void SIOmotor (void) {
    switch (state) {
        case 0:
            break;
        case 1:
            if (SiIsAvailable()) {
                SiSendChar('D');
                state++;
            }
            break;
        case 2:
            if (SiCharAvail() > 0) {
                incomingCharCount = SiGetChar();
                state++;
            }
            break;
        case 3:
            if (SiCharAvail() > 0) {
                static char index = 0;
                recordIndex[index] = SiGetChar();
                if (index == incomingCharCount-1) {
                    index = 0;
                    state++;
                } else {
                    index++;
                }
            }
            break;
        case 4:
            if (SiIsAvailable()) {
                static char timestampCount = 0;
                timestamp[timestampCount] = getHora()[timestampCount];
                SiSendChar(timestamp[timestampCount]);
                if (timestampCount == 4) {
                    timestampCount = 0;
                    state++;
                } else {
                    timestampCount++;
                }
            }
            break;
        case 5:
            if (SiCharAvail() > 0) {
                if (SiGetChar() == 'K') {
                    state++;
                }
            }
            break;
        case 6:
            if (SiCharAvail()) state++;
            if (SiIsAvailable() && micValueIsAvail()) {
                SiSendChar(getValorMicro());
            }
            break;
        case 7:
            if (SiGetChar() == 'K') {
                setIndexLen(incomingCharCount);
                setIndex(recordIndex);
                setTimestamp(timestamp);
                saveRecording();
                state = 0;
            }
            break;
        case 8:
            if (SiIsAvailable()) {
                SiSendChar('P');
                state++;
            }
            break;
        case 9:
            if (SiCharAvail() > 0) {
                if (SiGetChar() == 'K') {
                    state++;
                }
            }
            break;
        case 10:
            if (SiIsAvailable()) {
                static char indexCount = 0;
                if (indexCount == 10) SiSendChar('\0');
                else SiSendChar(indexToSend[indexCount]);
                if (indexToSend[indexCount] == '\0' || indexCount == 10) {
                    indexCount = 0;
                    state++;
                } else {
                    indexCount++;
                }
            }
            break;
        case 11:
            if (SiCharAvail() > 0) {
                if (SiGetChar() == 'F') {
                    state = 0;
                }
            }
            break;
    }
}