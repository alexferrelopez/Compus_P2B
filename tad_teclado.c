#include <xc.h>

#include "tad_teclado.h"
#include "tad_timer.h"

#define ROW0 PORTBbits.RB0
#define ROW1 PORTBbits.RB1
#define ROW2 PORTBbits.RB2
#define ROW3 PORTBbits.RB3
#define COL0 LATBbits.LATB4
#define COL1 LATBbits.LATB5
#define COL2 LATBbits.LATB6

static unsigned char caracteres[12][5] = {
{'1'}, //1 indice 0
{'A','B','C','2'}, //2 indice 1
{'D','E','F','3'}, //2 indice 2
{'G','H','I','4'}, //2 indice 3
{'J','K','L','5'}, //2 indice 4
{'M','N','O','6'}, //2 indice 5
{'P','Q','R','S','7'}, //3 indice 6
{'T','U','V','8'}, //2 indice 7
{'W','X','Y','Z','9'}, //3 indice 8
{'*'}, //1 indice 9
{'0',' '}, //4 indice 10
{'#'}
}; //1 indice 11

static unsigned char state, timerTeclado, timer1s, numPulsaciones, flagTecla, lastFlag,
        flagtmp, tecla, indexTecla, ultimoIndiceTecla, posTecla, letterIsSet;

static unsigned long ab;

void teclado_init(void) {
    TRISBbits.TRISB0 = 1; // Fila 1 como entrada
    TRISBbits.TRISB1 = 1; // Fila 2 como entrada
    TRISBbits.TRISB2 = 1; // Fila 3 como entrada
    TRISBbits.TRISB3 = 1; // Fila 4 como entrada
    TRISBbits.TRISB4 = 0; // Columna 1 como salida
    TRISBbits.TRISB5 = 0; // Columna 2 como salida
    TRISBbits.TRISB6 = 0; // Columna 3 como salida
    

    INTCON2bits.RBPU = 0; // Habilitamos las resistencias de pull-up

    TI_NewTimer(&timerTeclado); //Creamos un timer para el teclado
    TI_NewTimer(&timer1s); //Creamos un timer para el teclado

    state = 0;
    numPulsaciones = 0;
    flagTecla = 0;
    indexTecla = 12;
    ultimoIndiceTecla = 12;
    posTecla = 0;
    letterIsSet = 1;

    TI_ResetTics(timerTeclado);
    TI_ResetTics(timer1s);
}

char getTecla(void) {
    return tecla;
}

char getIndexTecla(void){
    return indexTecla;
}

char hiHaTecla(void){
    return flagTecla & 1;
}

void teclaProcesada(void) {
    lastFlag = flagTecla;
    flagTecla = 0;
}

unsigned char getPosTecla (void) {
    return posTecla-1;
}

void resetPosTecla (void) {
    posTecla = 0;
}

void tecladoMotor(void) {
    switch (state) {
        case 0: //Barremos la primera columna
            if (TI_GetTics(timer1s) >= 1000 && !letterIsSet) {
                indexTecla = 12;
                ultimoIndiceTecla = 12;
                numPulsaciones = 0;
                letterIsSet = 1;
            }
            
            COL0 = 0;
            COL1 = 1;
            COL2 = 1;
            if (ROW0 == 0) {
                indexTecla = 0;
                state = 3;
            } else if (ROW1 == 0) {
                indexTecla = 3;
                state = 3;
            } else if (ROW2 == 0) {
                indexTecla = 6;
                state = 3;
            } else if (ROW3 == 0) {
                indexTecla = 9;
                state = 3;
            } else {
                //Pasamos a barrer la siguiente columna
                state++;
            }
            break;
        case 1:  //Barremos la segunda columna
            COL0 = 1;
            COL1 = 0;
            COL2 = 1;
            if (ROW0 == 0) {
                indexTecla = 1;
                state = 3;
            } else if (ROW1 == 0) {
                indexTecla = 4;
                state = 3;
            } else if (ROW2 == 0) {
                indexTecla = 7;
                state = 3;
            } else if (ROW3 == 0) {
                indexTecla = 10;
                state = 3;
            } else {
                //Pasamos a barrer la siguiente columna
                state++;
            }
            break;
        case 2: //Barremos la tercera columna
            COL0 = 1;
            COL1 = 1;
            COL2 = 0;
            if (ROW0 == 0) {
                indexTecla = 2;
                state = 3;
            } else if (ROW1 == 0) {
                indexTecla = 5;
                state = 3;
            } else if (ROW2 == 0) {
                indexTecla = 8;
                state = 3;
            } else if (ROW3 == 0) {
                indexTecla = 11;
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
            // Procesar los 16ms de rebotes del teclado
            ab = TI_GetTics(timerTeclado);
            if (ab >= 16) {
                if (ROW0 == 0 || ROW1 == 0 || ROW2 == 0 || ROW3 == 0) {
                    // Se ha detectado una tecla
                    state++;
                } else {
                    // Eran rebotes de subida
                    state = 0;
                }
            }
            break;
        case 5:
            if (indexTecla != ultimoIndiceTecla) {
                numPulsaciones = 0;
                tecla = caracteres[indexTecla][numPulsaciones];
                flagtmp = TECLA_READY;
                posTecla++;
            } else if (indexTecla == ultimoIndiceTecla) { 
                flagtmp = REPLACE_LAST;
                numPulsaciones++;
                if ((indexTecla == 0 || indexTecla == 9 || indexTecla == 11) && numPulsaciones == 1) {
                    numPulsaciones = 0;
                } else if ((indexTecla == 1 || indexTecla == 2 || indexTecla == 3 || indexTecla == 4 || indexTecla == 5 || indexTecla == 7) && numPulsaciones == 4) {
                    numPulsaciones = 0;
                } else if ((indexTecla == 6 || indexTecla == 8) && numPulsaciones == 5) {
                    numPulsaciones = 0;
                } else if ((indexTecla == 10) && numPulsaciones == 2) {
                    numPulsaciones = 0;
                }
                tecla = caracteres[indexTecla][numPulsaciones];
            }
            TI_ResetTics(timer1s);
            ultimoIndiceTecla = indexTecla;
            state++;
            break;
        case 6:
            if (ROW0 == 1 && ROW1 == 1 && ROW2 == 1 && ROW3 == 1) {
                TI_ResetTics(timerTeclado); 
                state++;
            }   
            break;    
        case 7: 
            // Procesar los 16ms de rebotes del teclado
            if (TI_GetTics(timerTeclado) > 16) {
                if (ROW0 == 1 && ROW1 == 1 && ROW2 == 1 && ROW3 == 1) {
                    state = 0;
                    flagTecla = flagtmp;
                    letterIsSet = 0;
                }  else {
                    state--;
                }
            }   
            break;        
    }
}