#ifndef _ADC_H
#define _ADC_H

void adc_init(void);
void adcMotor(void);
unsigned char getValorMicro(void);
unsigned char getFlagMicro(void);
int getPosicion(void);

#endif