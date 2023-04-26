#include <xc.h>

#include "tad_altavoz.h"

char key_freq[12][2] = {
    {'1', 261},   
    {'2', 277},   
    {'3', 294},   
    {'4', 311},   
    {'5', 330},   
    {'6', 349},   
    {'7', 370},   
    {'8', 392},  
    {'9', 415},   
    {'*', 440},  
    {'0', 466},   
    {'#', 494}    
};

void altavoz_init() {
    TRISCbits.TRISC2 = 0;   // Configuramos el pin de salida del PWM
    PORTCbits.RC2 = 0;     // Apagamos el altavoz al iniciar
}