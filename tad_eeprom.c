#include <xc.h> 

#include "tad_eeprom.h"

static Recording recordings[8];
static unsigned char oldestInsertedRec, numRecordings, state, counter,
        loadRecordingIndex;

void EEPROMinit (void) {
    state = 0;
    counter = 0;
    loadRecordingIndex = 0;
}

/*
 MOVLW DATA_EE_ADDR ;
MOVWF EEADR ; Data Memory Address to read
BCF EECON1, EEPGD ; Point to DATA memory
BCF EECON1, CFGS ; Access EEPROM
BSF EECON1, RD ; EEPROM Read
MOVF EEDATA, W ; W = EEDATA
 */
unsigned char loadFromEEPROM(unsigned char index) {
    EEADR = index;
    EECON1bits.EEPGD = 0;
    EECON1bits.CFGS = 0;
    EECON1bits.RD = 1;
    return EEDATA;
}

/**
    MOVLW DATA_EE_ADDR ;
    MOVWF EEADR ; Data Memory Address to write
    MOVLW DATA_EE_DATA ;    
    MOVWF EEDATA ; Data Memory Value to write
    BCF EECON1, EPGD ; Point to DATA memory
    BCF EECON1, CFGS ; Access EEPROM
    BSF EECON1, WREN ; Enable writes
    BCF INTCON, GIE ; Disable Interrupts
    MOVLW 55h ;
    MOVWF EECON2 ; Write 55h
    MOVLW 0AAh ;
    MOVWF EECON2 ; Write 0AAh
    BSF EECON1, WR ; Set WR bit to begin write
    BSF INTCON, GIE ; Enable Interrupts
 
 */
void saveOnEEPROM (unsigned char index, char c) {
    EEADR = index;
    EEDATA = c;
    EECON1bits.EEPGD = 0;
    EECON1bits.CFGS = 0;
    EECON1bits.WREN = 1;
    di();
    EECON2 = 0x55;
    EECON2 = 0x0AA;
    EECON1bits.WR = 1;
    ei();
}

unsigned char EEPROMWriteIsFinished (void) {
    if (PIR2bits.EEIF){
        PIR2bits.EEIF = 0;
        EECON1bits.WREN = 0;
        return 1;
    } else return 0; 
}

Recording * getRecording(unsigned char index) {
    return &recordings[index];
}

unsigned char getNumRecordings (void) {
    return numRecordings;
}

void motorEEPROM (void) {
    switch (state) {
        case 0:
            counter = 0;
            loadRecordingIndex = 0;
            break;
        case 1: //NEW RECORGING SAVE
            if (counter == 11) {
                state ++;
            }
            else if (counter == 0) {
                saveOnEEPROM(BASE_OFFSET + oldestInsertedRec * sizeof(Recording) + counter, recordings[oldestInsertedRec].index_len);
                counter++;
            }
            else if (EEPROMWriteIsFinished() && EECON1bits.WR == 0){
                saveOnEEPROM(BASE_OFFSET + oldestInsertedRec * sizeof(Recording) + counter, recordings[oldestInsertedRec].index[counter-1]);
                counter++;
            }
            break;
        case 2:
            if (counter == 16) state++;
            else if (EEPROMWriteIsFinished() && EECON1bits.WR == 0){
                saveOnEEPROM(BASE_OFFSET + oldestInsertedRec * sizeof(Recording) + counter, recordings[oldestInsertedRec].timestamp[counter-11]);
                counter++;
            }
            break;
        case 3:
            state++;
            oldestInsertedRec = oldestInsertedRec == 7 ? 0 : oldestInsertedRec + 1;
            if (numRecordings < 8) numRecordings ++;
            break;
        case 4:
            if (EEPROMWriteIsFinished() && EECON1bits.WR == 0){
                saveOnEEPROM(0, numRecordings);
                state++;
            }
            break;
        case 5: 
            if (EEPROMWriteIsFinished() && EECON1bits.WR == 0){
                saveOnEEPROM(1, oldestInsertedRec);
                state = 0;
                counter = 0;
            }
            break;
        case 6: //INITIAL LOAD
            numRecordings =  loadFromEEPROM(0);
            oldestInsertedRec = loadFromEEPROM(1);
            if (numRecordings > 8) {
                numRecordings = 0;
                oldestInsertedRec = 0;
            }
            state++;
            break;
        case 7:
            if (loadRecordingIndex == numRecordings) {
                state = 0; 
            }
            else if (counter == 11) { 
                state++;
            }
            else if (counter == 0) {
                recordings[loadRecordingIndex].index_len = loadFromEEPROM(BASE_OFFSET + loadRecordingIndex * sizeof(Recording) + counter);
                counter++;
            }
            else {
                recordings[loadRecordingIndex].index[counter-1] = loadFromEEPROM(BASE_OFFSET + loadRecordingIndex * sizeof(Recording) + counter);
                counter++;
            }
            break;
        case 8:
            if (counter == 16) {
                state--;
                counter = 0;
                loadRecordingIndex++;
            }
            else {
                recordings[loadRecordingIndex].timestamp[counter-11] = loadFromEEPROM(BASE_OFFSET + loadRecordingIndex * sizeof(Recording) + counter);
                counter++;
            }
            break;
    }
}

void saveRecording (void) {
    state = 1;
}

void loadRecordings (void) {
    state = 6;
}

void setTimestamp (unsigned char* timestampToSave) {
    recordings[oldestInsertedRec].timestamp[0] = timestampToSave[0];
    recordings[oldestInsertedRec].timestamp[1] = timestampToSave[1];
    recordings[oldestInsertedRec].timestamp[2] = timestampToSave[2];
    recordings[oldestInsertedRec].timestamp[3] = timestampToSave[3];
    recordings[oldestInsertedRec].timestamp[4] = timestampToSave[4];
}

void setIndex (unsigned char* indexToSave) {
    recordings[oldestInsertedRec].index[0] = indexToSave[0];
    recordings[oldestInsertedRec].index[1] = indexToSave[1];
    recordings[oldestInsertedRec].index[2] = indexToSave[2];
    recordings[oldestInsertedRec].index[3] = indexToSave[3];
    recordings[oldestInsertedRec].index[4] = indexToSave[4];
    recordings[oldestInsertedRec].index[5] = indexToSave[5];
    recordings[oldestInsertedRec].index[6] = indexToSave[6];
    recordings[oldestInsertedRec].index[7] = indexToSave[7];
    recordings[oldestInsertedRec].index[8] = indexToSave[8];
    recordings[oldestInsertedRec].index[9] = indexToSave[9];
}

void setIndexLen (unsigned char indexLenToSave) {
    recordings[oldestInsertedRec].index_len = indexLenToSave;
}
