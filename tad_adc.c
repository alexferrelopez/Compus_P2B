#include <xc.h>

#include "tad_adc.h"

#define ADCON1_CONFIG 0x0D
#define ADCON2_CONFIG 0

unsigned char ADCON0_CONFIG = 1, up, down, center, valorMicro, flagMicro, status, check, move;

void adc_init(void) {
    ADCON1 = ADCON1_CONFIG;
    ADCON2 = ADCON2_CONFIG;
    TRISAbits.TRISA0 = 1; // Configuramos el pin de entrada del ADC para el joystick
    TRISAbits.TRISA1 = 1; // Configuramos el pin de entrada del ADC para el micro
    status = 0;
    up = 0;
    down = 0;
    center = 0;
    move = 0;
    flagMicro = 0;
}

void adcMotor(void) {
    switch(status) {
        case 0:
            // seleccionamos el puerto AN0 (eje Y del joystick)
            ADCON0 = 0;
            ADCON0bits.ADON = 1;
            ADCON0bits.GODONE = 1;
            status++;
            break;
        case 1:
            TRISAbits.TRISA3 = 0;
            //LATAbits.LATA3 = 1;
            if (ADCON0bits.GODONE == 0) {
            //LATAbits.LATA3 = 1;
                check = (ADRESH); //>> 4) & 0x7;
                // Obtenemos el valor del ADC solo si se detecta una alteraci�n en el joystick
                move = check;
                status--;
            }
        break;
        /*case 2:
            up = 0;
            down = 0;
            center = 0;
            status = 0;
            break;*/
        /*case 2:
            // Seleccionamos el puerto AN0 (microfono)
            ADCON0bits.CHS0 = 1;
            ADCON0bits.CHS1 = 0;
            ADCON0bits.CHS2 = 0;
            ADCON0bits.CHS3 = 0;

            ADCON0bits.GODONE = 1;
            status++;
            break;   
        case 3:
            if (ADCON0bits.GODONE == 0) {
                valorMicro = ADRESH;
                flagMicro = 1;
            }
            flagMicro = 0;
            status = 0;
            break;*/     
    }
}    

unsigned char getUp(void) {
    return up;
}

unsigned char getDown(void) {
    return down;
}

unsigned char getCenter(void) {
    return center;
}

unsigned char getValorMicro(void) {
    return valorMicro;
}

unsigned char getFlagMicro(void) {
    return flagMicro;
}

void resetUp() {
    up = 0;
}