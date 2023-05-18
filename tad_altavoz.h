#ifndef _ALTAVOZ_H_
#define _ALTAVOZ_H_

#include <xc.h>

#include "tad_timer.h"

void altavoz_init(void);
void altavozMotor(void);
void setMelodia(void);
void setSonidoTecla(char tecla);

#endif
