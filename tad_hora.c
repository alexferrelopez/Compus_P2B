#include <xc.h>

#include "tad_hora.h"
#include "tad_timer.h"

//#include "tad_marquesina.h"

static unsigned char status, clock[5]; 

void hora_init(void) {
    TI_NewTimer(&timerHora);
    status = 0;

    clock[0] = '0'; //Minutos
    clock[1] = '0'; //Minutos
    clock[2] = ':';
    clock[3] = '0'; //Segundos
    clock[4] = '0'; //Segundos
}

// Funcion que inicia el timming de la hora a partir de que se establezca la comunicacion con la torre.
void startHora(void) {
    status = 1;
}

unsigned char* getHora(void) {
    return (unsigned char*)clock;
}

void horaMotor() {
    switch (status) {
        case 0:
            TI_ResetTics(timerHora);
            break;
        case 1:
            if(TI_GetTics(timerHora) > 1000) {
                TI_ResetTics(timerHora);
                clock[4]++;
                if (clock[4] > '9') {
                    clock[4] = '0';
                    clock[3]++;
                    if (clock[3] > '5') {
                        clock[3] = '0';
                        clock[1]++;
                        if (clock[1] > '9') {
                            clock[1] = '0';
                            clock[0]++;
                            if (clock[0] > '5') {
                                // Si hemos contado 59:59(1 hora) reinciamos el contador.
                                clock[0] = '0';
                                clock[1] = '0';
                                clock[3] = '0';
                                clock[4] = '0';
                            }
                        }
                    }
                }
            }
            break;
    }
    
}