#ifndef _ADC_H
#define _ADC_H

#define GO_UP -1
#define GO_DOWN 1
#define CENTER 0

void adc_init(void);
void adcMotor(void);
unsigned char getValorMicro(void);
unsigned char getFlagMicro(void);
signed char getJoystickMove(void);
unsigned char joystickIsDiffPos(void);
#endif