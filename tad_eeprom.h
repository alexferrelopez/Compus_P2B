#ifndef _EEPROM_H
#define	_EEPROM_H

#include <xc.h> 

typedef struct recording {
    char index[10];
    char timestamp[4];
} recording;

unsigned char loadFromEEPROM(unsigned char index);
void saveOnEEPROM (unsigned char index, char c);
unsigned char EEPROMWriteIsFinished ();

#endif