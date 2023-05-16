#include <xc.h>

#include "tad_adc.h"

#define ADCON1_CONFIG 0x0D
#define ADCON2_CONFIG 0x82

unsigned char valorMicro, flagMicro, status, position, lastPosition;

void adc_init(void) {
    ADCON1 = ADCON1_CONFIG;
    ADCON2 = ADCON2_CONFIG;
    TRISAbits.TRISA0 = 1; // Configuramos el pin de entrada del ADC para el joystick
    TRISAbits.TRISA1 = 1; // Configuramos el pin de entrada del ADC para el micro
    TRISAbits.TRISA3 = 0; // Configuramos el pin de entrada del ADC para el joystick
    TRISAbits.TRISA4 = 0; // Configuramos el pin de entrada del ADC para el micro
    status = 0;
    position = 0;
    lastPosition = 0;
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
            if (ADCON0bits.GODONE == 0) {
                position = ADRESH;
                if (position == 2) position--;
                status--;
            }
        break;
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

int getJoystickMove(void) {
    unsigned char result;
    
    position != lastPosition ? (result = position) : (result = DONT_MOVE);
    
    lastPosition = position;
    
    return result;
}

unsigned char getValorMicro(void) {
    return valorMicro;
}

unsigned char getFlagMicro(void) {
    return flagMicro;
}