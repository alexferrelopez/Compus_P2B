#include <xc.h>

#include "tad_altavoz.h"

static unsigned char state,timeOn,timerPWM, contadorDutyCycle, valor;
static unsigned char dutys[5] = {1,4,5,3,2};
static unsigned short periodos[5] = {1000,250,200,333,500};

static unsigned char tecla;
static unsigned short periodos_tecla[12] = {261,277,294,311,330,349,370,392,415,440,466,494};

void altavoz_init() {
    TRISCbits.TRISC2 = 0;   // Configuramos el pin de salida del PWM
    timerPWM = TI_NewTimer(&timerPWM); //Creamos un timer para el PWM
    state = 0;
}

void altavozMotor() {
    switch (state) {
        case 0:
            LATCbits.LATC2 = 0; // Apagamos el altavoz
            TI_ResetTics(timerPWM);   
            break;
        case 1:
            if(TiGetTics(timerPWM) >= periodos[timeOn]) {   
                //BTG 
                if (valor == 0) {
                    valor = 1;
                    LATCbits.LATC2 = valor;
                } else if (valor == 1) {
                    valor = 0;
                    LATCbits.LATC2 = valor;
                }
                contadorDutyCycle++;
                
                if(contadorDutyCycle == dutys[timeOn]) {
                    contadorDutyCycle = 0; //resetea contador DC
                    timeOn++; // Pasamos a la siguiente nota
                    if(timeOn == 5) {
                        // Ha terminado la reproduccion de la melodia (han pasado 5 segundos)
                        state = 0; 
                        LATCbits.LATC2 = 0;
                    }
                } 
                TI_ResetTics(timerPWM);
            }            
            case 2:
                if (TI_GetTics(timerPWM) >= periodos_tecla[tecla]) {
                    /*if (valor == 0) {
                        valor = 1;
                        LATCbits.LATC2 = valor;
                    } else if (valor == 1) {
                        valor = 0;
                        LATCbits.LATC2 = valor;
                    }*/
                    status = 0;
                }
                break;
        break;  
    }
}

void setMelodia(char c) {
    state = c;
    timeOn = 0;
    contadorDutyCycle = 0;
    valor = 1;
    LATCbits.LATC2 = valor; // Encendemos el altavoz
    TI_ResetTics(timerPWM);
}

void setSonidoTecla(char tecla) {
    state = 2;
    valor = 1;
    LATCbits.LATC2 = valor; // Enceendemos el altavoz

    switch (tecla) {
        case '1':
            tecla = 0;
            break;
        case '2':
            tecla = 1;
            break;
        case '3':
            tecla = 2;
            break;
        case '4':
            tecla = 3;
            break;
        case '5':
            tecla = 4;
            break;
        case '6':
            tecla = 5;
            break;
        case '7':
            tecla = 6;
            break;
        case '8':
            tecla = 7;
            break;
        case '9':
            tecla = 8;
            break;
        case '*':
            tecla = 9;
            break;
        case '0':
            tecla = 10;
            break;
        case '#':
            tecla = 11;
            break;
        default:
            break;
    }
    TI_ResetTics(timerPWM);
}