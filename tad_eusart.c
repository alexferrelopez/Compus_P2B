#include <pic18f4321.h>
#include <xc.h>

#include "tad_eusart.h"

// Declaración de constantes
#define BAUD_RATE 9600

static unsigned char state;

// Función para inicializar el EUSART
void eusartInit(void) {
    // Configurar los pines de transmisión y recepción
    TRISCbits.TRISC6 = 0; // Configurar el pin TX como salida para recibir datos
    TRISCbits.TRISC7 = 1; // Configurar el pin RX como entrada para enviar datos

    // Configuración del baudrate y del modo asíncrono
    TXSTAbits.BRGH = 1;  // High Baudrate Select bit
    BAUDCONbits.BRG16 = 0; // Modo de 8 bits
    //SPBRG = ((_XTAL_FREQ/BAUD_RATE)/64)-1;  // Calcular el valor del registro SPBRG

    // Configurar el EUSART
    TXSTAbits.SYNC = 0; // Modo asíncrono
    RCSTAbits.SPEN = 1; // Habilitar el módulo EUSART
    TXSTAbits.TXEN = 1; // Habilitar la transmisión de datos
    RCSTAbits.CREN = 1; // Habilitar la recepción de datos
}

// Función para enviar un caracter a través del EUSART
void eusartSendChar(unsigned char c) {
    while(!TXSTAbits.TRMT);   // Esperar hasta que se haya completado la transmisión anterior
    TXREG = c;                // Enviar el caracter
}

// Función para recibir un caracter a través del EUSART
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