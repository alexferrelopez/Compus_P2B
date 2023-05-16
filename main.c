#include <xc.h>

#include "tad_altavoz.h"
#include "tad_timer.h"
#include "tad_SIO.h"
#include "tad_lcd.h"
#include "tad_controller.h"
#include "tad_adc.h"

#pragma config OSC = HSPLL //Convertimos el cristal externo de 10MHz en 40MHz
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
	teclado_init();
    controllerInit();
	altavoz_init();
    marquesinaInit();
    adc_init();
	initSIO();
	LcInit(2, 16);
    LcClear();
}

void main(void) {
    //TRISAbits.TRISA3 = 0;
	config_interrupts();
    config_ports();
    
    while(1) {
		tecladoMotor();
        altavozMotor();
        controllerMotor();
        marquesinaMotor();
        adcMotor();
	}		
}