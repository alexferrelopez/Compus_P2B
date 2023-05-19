#ifndef _EEPROM_H
#define	_EEPROM_H

#include <xc.h> 

#define BASE_OFFSET 2

typedef struct Recording {
    unsigned char index[10];
    unsigned char timestamp[5];
} Recording;

void EEPROMinit (void);
void motorEEPROM (void);
void saveRecording (void);
void loadRecordings (void);
void setTimestamp (unsigned char* timestampToSave);
void setIndex (unsigned char* indexToSave);

#endif