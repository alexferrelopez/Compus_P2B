#ifndef _TECLADO_H
#define _TECLADO_H

#include "tad_timer.h"

#define POSSIBLE_TECLA 2
#define TECLA_READY 1
#define REPLACE_LAST 5

void teclado_init(void);
void tecladoMotor(void);
char getTecla(void);
char getFlagTecla(void);
char getIndexTecla(void);
char hiHaTecla(void);
char haDeSubstituir(void);
void teclaProcesada(void);

#endif