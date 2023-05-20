#ifndef _EEPROM_H
#define	_EEPROM_H

#define BASE_OFFSET 2

typedef struct Recording {
    unsigned char index_len;
    unsigned char index[10];
    unsigned char timestamp[5];
} Recording;

unsigned char getNumRecordings (void);
Recording * getRecording(unsigned char index);
void EEPROMinit (void);
void motorEEPROM (void);
void saveRecording (void);
void loadRecordings (void);
void setTimestamp (unsigned char* timestampToSave);
void setIndex (unsigned char* indexToSave);
void setIndexLen (unsigned char indexLenToSave);

#endif