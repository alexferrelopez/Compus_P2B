#include <xc.h>

#include "tad_teclado.h"
#include "tad_altavoz.h"
#include "tad_timer.h"
//#include "tad_eusart.h"
#include "tad_lcd.h"
//#include "tad_controller.h"

#pragma config OSC = HS //Convertimos el cristal externo de 10MHz en 40MHz
#pragma config PBADEN = DIG
#pragma config MCLRE = ON
#pragma config DEBUG = OFF
#pragma config PWRT = OFF
#pragma config BOR = OFF
#pragma config WDT = OFF
#pragma config LVP = OFF

//Definicion de la interrupcion de alta prioridad
void __interrupt(high_priority) RSI_High(void) {
	if(INTCONbits.TMR0IF == 1) RSI_Timer0(); // Si se ha producido una interrupción por overflow del Timer0
}

void config_interrupts(void) {
	RCONbits.IPEN = 0; // Deshabilitamos las interrupciones por prioridad
	INTCONbits.GIE = 1; // Habilitamos las interrupciones globales
	INTCONbits.PEIE = 1; // Habilitamos las interrupciones periféricas
}

void config_ports(void) {
    TI_Init();
	//teclado_init();
    
	altavoz_init();
    
	//eusartInit();
	LcInit(2, 16);
    
    LcClear();
}

void main(void) {
	config_ports();
	config_interrupts();
    
    /*LcCursorOn();
    LcGotoXY(0,0);
	LcPutString("KEVIN");
    LcGotoXY(0,1);
    LcPutString("LEVANA");*/
    LcGotoXY(0,0);
    
    setSonidoTecla('9');
    
    while(1) {
		tecladoMotor();
        altavozMotor();
	}		
}