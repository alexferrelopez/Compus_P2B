#ifndef _HORA_H_
#define _HORA_H_

#include "tad_marquesina.h"
#include "tad_timer.h"

void hora_init(void);
void startHora(void);
void horaMotor(void);
unsigned char* getHora(void);
void setHora(unsigned char* c);

#endif