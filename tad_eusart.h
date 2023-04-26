#ifndef _EUSART_H
#define _EUSART_H

void eusartInit(void);
void eusartSendChar(unsigned char caracter);
char eusartReceiveChar(void);
void motorEusart(void);

#endif