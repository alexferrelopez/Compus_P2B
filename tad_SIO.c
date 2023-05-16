#include <xc.h>
#include "tad_SIO.h"

void initSIO(void) {
//Pre: -
//Post: Inicialitza la SIO a 9600bps
//Assumim que anem a 10MHZ
    TXSTA = 0x20;   //00100000
    RCSTA = 0x90;   //10010000
    BAUDCON = 0;
    TXSTAbits.BRGH = 1;
    BAUDCONbits.BRG16 = 0;
    SPBRG = 64;
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