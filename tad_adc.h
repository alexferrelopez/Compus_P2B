#ifndef _ADC_H
#define _ADC_H

#define GO_UP 0
#define GO_DOWN 3
#define DONT_MOVE 1

void adc_init(void);
void adcMotor(void);
unsigned char getValorMicro(void);
unsigned char getFlagMicro(void);
int getJoystickMove(void);

#endif