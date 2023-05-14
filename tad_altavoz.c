#include <xc.h>

#include "tad_altavoz.h"
#include "tad_timer.h"

static unsigned char state,timeOn,timerPWM, contadorDutyCycle, valor;
static unsigned char dutys[5] = {6,4,5,3,2};
static unsigned short periodos[5] = {167,250,200,333,500};

static unsigned char tecla;

static unsigned short periodos_tecla[12] = {100,150,200,250,300,350,400,450,500,550,600,650};
//Los periodos más comunes para sonidos audibles oscilan entre los 10 y los 1000 milisegundos

void altavoz_init(void) {
    TRISBbits.TRISB7 = 0;   // Configuramos el pin de salida del PWM
    timerPWM = TI_NewTimer(&timerPWM); //Creamos un timer para el PWM
    state = 0;
    return;
}

void altavozMotor(void) {
    switch (state) {
        case 0:
            LATBbits.LATB7 = 0; // Apagamos el altavoz
            TI_ResetTics(timerPWM);   
            break;
        case 1:
            if(TI_GetTics(timerPWM) >= periodos[timeOn]) {   
                //BTG 
                if (valor == 0) {
                    valor = 1;
                    LATBbits.LATB7 = valor;
                } else if (valor == 1) {
                    valor = 0;
                    LATBbits.LATB7 = valor;
                }
                contadorDutyCycle++;
                
                if(contadorDutyCycle == dutys[timeOn]) {
                    contadorDutyCycle = 0; //Resetea contador de duty cycle
                    timeOn++; // Pasamos a la siguiente nota
                    if(timeOn == 5) {
                        // Ha terminado la reproduccion de la melodia (han pasado 5 segundos)
                        state = 0; 
                        LATBbits.LATB7 = 0;
                    }
                } 
                TI_ResetTics(timerPWM);
            }            
            case 2:
                if (TI_GetTics(timerPWM) >= periodos_tecla[tecla]) {
                    //BTG
                    if (valor == 0) {
                        valor = 1;
                        LATBbits.LATB7 = valor;
                    } else if (valor == 1) {
                        valor = 0;
                        LATBbits.LATB7 = valor;
                    }
                    contadorDutyCycle++;

                    if (contadorDutyCycle == 2) { // 50% duty cycle
                        contadorDutyCycle = 0;
                        state = 0;
                        LATBbits.LATB7 = 0;
                    }
                    TI_ResetTics(timerPWM);
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
    LATBbits.LATB7 = valor; // Encendemos el altavoz
    TI_ResetTics(timerPWM);
}

void setSonidoTecla(char tecla) {
    state = 2;
    valor = 1;
    LATBbits.LATB7 = valor; // Enceendemos el altavoz
    contadorDutyCycle = 0;

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