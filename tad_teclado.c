#include <xc.h>

#include "tad_teclado.h"
#include "tad_timer.h"

#define ROW0 PORTBbits.RB0
#define ROW1 PORTBbits.RB1
#define ROW2 PORTBbits.RB2
#define ROW3 PORTBbits.RB3
#define COL0 PORTBbits.RB4
#define COL1 PORTBbits.RB5
#define COL2 PORTBbits.RB6

static unsigned char caracteres[] = {'1','A','B','C','2','D','E','F','3','G','H','I','4','J','K','L','5','M','N','O','6','P','Q','R','S','7','T','U','V','8','W','X','Y','Z','9','*','0',' ','#'};

static unsigned char state, caracterActual, ultimoCaracter, timerTeclado, numPulsaciones, flagTecla, tecla, flagAnterior;

void teclado_init(void) {
    TRISBbits.TRISB0 = 1; // Fila 1 como entrada
    TRISBbits.TRISB1 = 1; // Fila 2 como entrada
    TRISBbits.TRISB2 = 1; // Fila 3 como entrada
    TRISBbits.TRISB3 = 1; // Fila 4 como entrada
    TRISBbits.TRISB4 = 0; // Columna 1 como salida
    TRISBbits.TRISB5 = 0; // Columna 2 como salida
    TRISBbits.TRISB6 = 0; // Columna 3 como salida

    OPTION_REGbits.nRBPU = 0; // Habilitamos las resistencias de pull-up
    WPUBbits.WPUB0 = 1; // Pull-up en la fila 1
    WPUBbits.WPUB1 = 1; // Pull-up en la fila 2
    WPUBbits.WPUB2 = 1; // Pull-up en la fila 3
    WPUBbits.WPUB3 = 1; // Pull-up en la fila 4

    timerTeclado = TI_NewTimer(&timerTeclado); //Creamos un timer para el teclado
    
    state = 0;
    numPulsaciones = 0;
    flagTecla = 0;
	flagAnterior = 0;
    caracterActual = 0;
    ultimoCaracter = 0;

    TI_ResetTics(timerTeclado);
}

char getTecla(void) {
    flagTecla = 0;
    return tecla;
}

char getflagTecla(void){
    return flagTecla;
}

void tecladoMotor(void) {
    switch (state) {
        case 0: //Barremos la primera columna
            LATBbits.LATB4 = 1;
            LATBbits.LATB5 = 0;
            LATBbits.LATB6 = 0;
            if (ROW0 == 1) {
                caracterActual = 0;
                state = 3;
            } else if (ROW1 == 1) {
                caracterActual = 9;
                state = 3;
            } else if (ROW2 == 1) {
                caracterActual = 21;
                state = 3;
            } else if (ROW3 == 1) {
                caracterActual = 35; 
                state = 3;
            } else {
                //Pasamos a barrer la siguiente columna
                state++;
            }
            break;
        case 1:  //Barremos la segunda columna
            LATBbits.LATB4 = 0;
            LATBbits.LATB5 = 1;
            LATBbits.LATB6 = 0;
            if (ROW0 == 1) {
                caracterActual = 1;
                state = 3;
            } else if (ROW1 == 1) {
                caracterActual = 13;
                state = 3;
            } else if (ROW2 == 1) {
                caracterActual = 26;
                state = 3;
            } else if (ROW3 == 1) {
                caracterActual = 36;
                state = 3;
            } else {
                //Pasamos a barrer la siguiente columna
                state++;
            }
            break;
        case 2: //Barremos la tercera columna
            LATBbits.LATB4 = 0;
            LATBbits.LATB5 = 0;
            LATBbits.LATB6 = 1;
            if (ROW0 == 1) {
                caracterActual = 5; 
                state = 3;
            } else if (ROW1 == 1) {
                caracterActual = 17;
                state = 3;
            } else if (ROW2 == 1) {
                caracterActual = 30;
                state = 3;
            } else if (ROW3 == 1) {
                caracterActual = 38;
                state = 3;
            } else {
                //Pasamos a barrer la primera columna
                state = 0;
            }
            break;
        case 3:
            TI_ResetTics(timerTeclado); //Reiniciamos el timer para empezar a contar los 16ms de rebotes en el estado 4
            state++;
            break;    
        case 4: 
            if (ROW0 == 0 && ROW1 == 0 && ROW2 == 0 && ROW3 == 0) {
				state = 0;
			}
            // Procesar los 16ms de rebotes del teclado
            if (TI_GetTics(timerTeclado) > 8) {
                state++;
            }
            break;
        case 5:
            if ((caracterActual != ultimoCaracter) && (flagAnterior != 0)) {
                tecla = caracteres[caracterActual + numPulsaciones];
                numPulsaciones = 0;
                flagTecla = 1;
            }
            ultimoCaracter = caracterActual;
            flagAnterior = 1;

            //Si la tecla pulsada es: (1 o * o 0 o #) ignoramos la tecla
            if (caracterActual == 0 || caracterActual == 35 || caracterActual == 36 || caracterActual == 38) {
                numPulsaciones = 0;
            } else {
                if ((caracterActual == 1 || caracterActual == 5 || caracterActual == 9 || caracterActual == 13 || caracterActual == 17 || caracterActual == 26) && (numPulsaciones < 3)) {
                    numPulsaciones++;
                } else if (caracterActual == 1 || caracterActual == 5 || caracterActual == 9 || caracterActual == 13 || caracterActual == 17 || caracterActual == 26) {
                    numPulsaciones = 0;
                }
        
                if ((caracterActual == 21 || caracterActual == 30) && (numPulsaciones < 4)) {
                    numPulsaciones++;
                } else if (caracterActual == 21 || caracterActual == 30) {
                    numPulsaciones = 0;
                }
            }
            state++;
            break;
        case 6:
            //Esperamos a que se suelte la tecla
            if (ROW0 == 0 && ROW1 == 0 && ROW2 == 0 && ROW3 == 0) {
                TI_ResetTics(timerTeclado);
                state++;
            }
            break;  
        case 7:
            // Comprobamos si se ha pulsado una tecla durante 1 segundo
            if (TI_GetTics(timerTeclado) >= 1000) {
				tecla = caracteres[caracterActual + numPulsaciones];
				flagAnterior = 0;
				numPulsaciones = 0;
				flagTecla = 1;
                state = 0;
            }else if (TI_GetTics(timerTeclado) < 1000 && (ROW0 == 1 || ROW1 == 1 || ROW2 == 1 || ROW3 == 1)) {
                state = 0;
            }
            break;         
    }
}