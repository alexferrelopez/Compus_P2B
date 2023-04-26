#include <xc.h>

#include "tad_timer.h"

// Definicions, per interrupcio cada 1ms.
#define T0CON_CONFIG 0x82  	   // T0CON = b'10000010'
#define RECARREGA_TMR0 55536       

#define TI_NUMTIMERS 4              // Nombre de timers virtuals gestionats per aquest TAD. Si cal, s'incrementa o es disminueix...

struct Timer {
	unsigned long TicsInicials;
	unsigned char Busy;
} static Timers[TI_NUMTIMERS];

static volatile unsigned long Tics = 0;

void RSI_Timer0 () {
    // Pre: IMPORTANT! Funci� que ha der ser cridada des de la RSI, en en cas que TMR0IF==1.
	//Recargar el timer0
	TMR0L = RECARREGA_TMR0 & 0xFF; // Asignar los 8 bits de menor peso a TMR0L
    TMR0H = (RECARREGA_TMR0 >> 8) & 0xFF; // Asignar los 8 bits de mayor peso a TMR0H

    INTCONbits.TMR0IF = 0;
    Tics++;    
}

void TI_Init () {
	for (unsigned char counter = 0; counter < TI_NUMTIMERS; counter++) {
		Timers[counter].Busy = TI_FALS;
	}
	T0CON = T0CON_CONFIG; 
    TMR0 = RECARREGA_TMR0;
	INTCONbits.TMR0IF = 0;
	INTCONbits.TMR0IE = 1;
    // Caldr� que des del main o des d'on sigui s'activin les interrupcions globals!
}

unsigned char TI_NewTimer(unsigned char *TimerHandle) {
	unsigned char Comptador = 0;
	while (Timers[Comptador].Busy == TI_CERT) {
		if (++Comptador == TI_NUMTIMERS) return (TI_FALS);
	}
	Timers[Comptador].Busy = TI_CERT;
	*TimerHandle = Comptador;
    return (TI_CERT);
}

void TI_ResetTics (unsigned char TimerHandle) {
	di(); Timers[TimerHandle].TicsInicials = Tics; ei();
}


unsigned long TI_GetTics (unsigned char TimerHandle) {
    di(); unsigned long CopiaTicsActual = Tics; ei();
	return (CopiaTicsActual - (Timers[TimerHandle].TicsInicials));
}

void TI_CloseTimer (unsigned char TimerHandle) {
	Timers[TimerHandle].Busy = TI_FALS;
}

void TI_End () {
}