#include "tad_altavoz.h"

#define PUERTO_ALTAVOZ TRISBbits.TRISB7
#define SALIDA_ALTAVOZ LATBbits.LATB7

static unsigned char state, timerPWM, timerSonido, pasoMelodia;

static unsigned char tecla;

static unsigned short periodos_tecla[12] = {1,2,3,4,5,6,7,8,9,10,11,12};
//Los periodos más comunes para sonidos audibles oscilan entre los 10 y los 1000 milisegundos

void altavoz_init(void) {
    PUERTO_ALTAVOZ = 0;   // Configuramos el pin de salida del PWM
    TI_NewTimer(&timerPWM); //Creamos un timer para el PWM
    TI_NewTimer(&timerSonido); //Creamos un timer para el PWM
    state = 0;
    pasoMelodia = 0;
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
            if(TI_GetTics(timerSonido) >= 500) {
                pasoMelodia++;
                if (pasoMelodia == 10) state = 0;
                TI_ResetTics(timerSonido);
            }
            else if (TI_GetTics(timerPWM) >= periodos_tecla[10-pasoMelodia]) {
                SALIDA_ALTAVOZ = !SALIDA_ALTAVOZ;
                TI_ResetTics(timerPWM);
            }
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

void setMelodia(void) {
    state = 1;
    pasoMelodia = 0;
    TI_ResetTics(timerPWM);
    TI_ResetTics(timerSonido);
}

void setSonidoTecla(char indexTecla) {
    state = 2;
    tecla = indexTecla;
    TI_ResetTics(timerPWM);
    TI_ResetTics(timerSonido);
}