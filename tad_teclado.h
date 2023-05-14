#ifndef _TECLADO_H
#define _TECLADO_H

#include "tad_timer.h"

void teclado_init(void);
void tecladoMotor(void);
char getTecla(void);
char getIndexTecla(void);
char getflagTecla(void);

#endif