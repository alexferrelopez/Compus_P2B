#include <xc.h>

#include "tad_adc.h"

#define ADCON1_CONFIG 0x0D
#define ADCON2_CONFIG_R 0x82
#define ADCON2_CONFIG_L 0x02

static unsigned char valorMicro, flagMicro, status;
static signed char lastPosition, position;

void adc_init(void) {
    ADCON1 = ADCON1_CONFIG;
    TRISAbits.TRISA0 = 1; // Configuramos el pin de entrada del ADC para el joystick
    TRISAbits.TRISA1 = 1; // Configuramos el pin de entrada del ADC para el micro
    //TRISAbits.TRISA3 = 0; // Configuramos el pin de entrada del ADC para el joystick
    //TRISAbits.TRISA4 = 0; // Configuramos el pin de entrada del ADC para el micro
    status = 0;
    position = 0;
    lastPosition = 0;
    flagMicro = 0;
}

void adcMotor(void) {
    switch(status) {
        case 0:
            // seleccionamos el puerto AN0 (eje Y del joystick)
            ADCON2 = ADCON2_CONFIG_R;
            ADCON0 = 0x03; //XX000011 CHANNEL 0, ADC ON, GO/DONE ON
            status++;
            break;
        case 1:
            if (ADCON0bits.GODONE == 0) {
                position = (signed char) ADRESH;
                if (position < 2) position --;
                else position -= 2;
                status++;
            }
        break;
        case 2:
            // Seleccionamos el puerto AN0 (microfono)
            ADCON2 = ADCON2_CONFIG_L;
            ADCON0 = 0x07; //XX000111 CHANNEL 1, ADC ON, GO/DONE ON
            status++;
            break;   
        case 3:
            if (ADCON0bits.GODONE == 0) {
                valorMicro = ADRESH;
                flagMicro = 1;
            }
            else {
                flagMicro = 0;
            }
            status = 0;
            break;     
    }
}    

unsigned char joystickIsDiffPos(void) {
    unsigned char result = ((position != lastPosition && position != CENTER) ? 1 : 0);
    lastPosition = position;
    return result;
}

signed char getJoystickMove(void) {
    return position;
}

unsigned char getValorMicro(void) {
    return valorMicro;
}

unsigned char getFlagMicro(void) {
    return flagMicro;
}