#include "tad_eeprom.h"

static recording recordings[8];
static unsigned char firstInsertedValue;

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

unsigned char EEPROMWriteIsFinished () {
    if (PIR2bits.EEIF){
        PIR2bits.EEIF = 0;
        EECON1bits.WREN = 1;
        return 1;
    } else return 0; 
}


