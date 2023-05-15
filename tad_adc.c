#include <xc.h>

#include "tad_adc.h"

#define ADCON1_CONFIG 0b00001101
#define ADCON2_CONFIG 0b00000000

unsigned char ADCON0_CONFIG = 0b00000001, up, down, center, valorMicro, flagMicro, status;

void adc_init(void) {
    TRISAbits.TRISA0 = 1; // Configuramos el pin de entrada del ADC para el joystick
    TRISAbits.TRISA1 = 1; // Configuramos el pin de entrada del ADC para el micro
    ADCON0 = ADCON0_CONFIG // He creado una variable en vez de un define dado que su valor puede cambiar en tiempo de ejecución.
    ADCON1 = ADCON1_CONFIG
    ADCON2 = ADCON2_CONFIG
    status = 0;
    up = 0;
    down = 0;
    center = 0;
    flagMicro = 0;
}

void adcMotor(void) {
    switch(status) {
        case 0:
            // seleccionamos el puerto AN0 (eje Y del joystick)
            ADCON0bits.CHS0 = 0;
            ADCON0bits.CHS1 = 0;
            ADCON0bits.CHS2 = 0;
            ADCON0bits.CHS3 = 0;

            ADCON0bits.GODONE = 1;
            status++;
            break;
        case 1:
            if (ADCON0bits.GODONE == 0) {
                //Obtenemos el valor del ADC y lo guardamos en una variable
                if (ADRESH > 0x80) {
                    //Si el valor es mayor que 0x80, el joystick está en la posición de arriba
                    
                    up = 1; // Hay que desplazar la LCD hacia arriba
                } else if (ADRESH < 0x40) {
                    //Si el valor es menor que 0x40, el joystick está en la posición de abajo
                    
                    down = 1; // Hay que desplazar la LCD hacia abajo
                } else {
                    //Si el valor está entre 0x40 y 0x80, el joystick está en la posición central
                    
                    center = 1; // No hay que hacer nada
                }
            }
            status++;
            break;    
        case 2:
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
            break;     
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


