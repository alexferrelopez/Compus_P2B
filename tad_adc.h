#ifndef _ADC_H
#define _ADC_H

void adc_init(void);
void adcMotor(void);
unsigned char getUp(void);
unsigned char getDown(void);
unsigned char getCenter(void);
unsigned char getValorMicro(void);
unsigned char getFlagMicro(void);
void resetUp(void);
void resetDown(void);
void resetCenter(void);

#endif