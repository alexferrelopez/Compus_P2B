#ifndef _TECLADO_H
#define _TECLADO_H

#define TECLA_READY 1
#define REPLACE_LAST 3

void teclado_init(void);
void tecladoMotor(void);
char getTecla(void);
char getIndexTecla(void);
char hiHaTecla(void);
void teclaProcesada(void);
unsigned char getPosTecla(void);
void resetPosTecla (void);
unsigned char getNumber(void);
void resetVariablesTeclado(void);

#endif