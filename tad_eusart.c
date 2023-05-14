#include <xc.h>

#include "tad_eusart.h"

// Declaraci�n de constantes
#define BAUD_RATE 32000

static unsigned char state;

// Funci�n para inicializar el EUSART
void eusartInit(void) {
    // Configurar los pines de transmisi�n y recepci�n
    TRISCbits.TRISC6 = 0; // Configurar el pin TX como salida para recibir datos
    TRISCbits.TRISC7 = 1; // Configurar el pin RX como entrada para enviar datos

    // Configuraci�n del baudrate y del modo as�ncrono
    TXSTAbits.BRGH = 1;  // High Baudrate Select bit
    BAUDCONbits.BRG16 = 0; // Modo de 8 bits
    //SPBRG = ((_XTAL_FREQ/BAUD_RATE)/64)-1;  // Calcular el valor del registro SPBRG

    // Configurar el EUSART
    TXSTAbits.SYNC = 0; // Modo as�ncrono
    RCSTAbits.SPEN = 1; // Habilitar el m�dulo EUSART
    TXSTAbits.TXEN = 1; // Habilitar la transmisi�n de datos
    RCSTAbits.CREN = 1; // Habilitar la recepci�n de datos
}

// Funci�n para enviar un caracter a trav�s del EUSART
void eusartSendChar(unsigned char c) {
    while(!TXSTAbits.TRMT);   // Esperar hasta que se haya completado la transmisi�n anterior
    TXREG = c;                // Enviar el caracter
}

// Funci�n para recibir un caracter a trav�s del EUSART
char eusartReceiveChar(void) {
    if(PIR1bits.RCIF) {       // Si se ha recibido un caracter
        return RCREG;        // Retornar el caracter recibido
    }
}

//esperar a que el canal este disponible, si envio no paso.
void motorEusart(void) {
    switch (state) {
        case 1:
            /* code */
            break;
        case 2:
            /* code */
            break;
        case 3:
            /* code */
            break;        
    }        
}