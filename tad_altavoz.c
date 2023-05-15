#include <xc.h>

#include "tad_altavoz.h"
#include "tad_timer.h"

#define PUERTO_ALTAVOZ TRISBbits.TRISB7
#define SALIDA_ALTAVOZ LATBbits.LATB7

static unsigned char state,timeOn,timerPWM, contadorDutyCycle, valor, timerSonido;

static unsigned char tecla;

static unsigned short periodos_tecla[12] = {1,2,3,4,5,6,7,8,9,10,11,12};
//Los periodos más comunes para sonidos audibles oscilan entre los 10 y los 1000 milisegundos

void altavoz_init(void) {
    PUERTO_ALTAVOZ = 0;   // Configuramos el pin de salida del PWM
    TI_NewTimer(&timerPWM); //Creamos un timer para el PWM
    TI_NewTimer(&timerSonido); //Creamos un timer para el PWM
    state = 0;
    return;
}

void altavozMotor(void) {
    switch (state) {
        case 0:
            SALIDA_ALTAVOZ = 0; // Apagamos el altavoz
            TI_ResetTics(timerPWM);
            TI_ResetTics(timerSonido);
            break;
        case 1:
            /*
            if(TI_GetTics(timerPWM) >= periodos_tecla[timeOn]) {   
                //BTG 
                SALIDA_ALTAVOZ = !SALIDA_ALTAVOZ;
                TI_ResetTics(timerPWM);
            }*/
            break;
        case 2:
            if (TI_GetTics(timerSonido) >= 200) {
                state = 0;
            } else if (TI_GetTics(timerPWM) >= periodos_tecla[tecla]) {
                SALIDA_ALTAVOZ = !SALIDA_ALTAVOZ;
                TI_ResetTics(timerPWM);
            }
            break;
    }
}

void setMelodia(char c) {
    state = c;
    timeOn = 0;
    contadorDutyCycle = 0;
    valor = 1;
    SALIDA_ALTAVOZ = valor; // Encendemos el altavoz
    TI_ResetTics(timerPWM);
}

void setSonidoTecla(char indexTecla) {
    state = 2;
    tecla = indexTecla;
    TI_ResetTics(timerPWM);
    TI_ResetTics(timerSonido);
}