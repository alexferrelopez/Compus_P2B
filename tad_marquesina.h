#ifndef _MARQUESINA_H
#define _MARQUESINA_H

#include "tad_timer.h"
#include "tad_lcd.h"

void marquesinaInit (void);
void marquesinaMotor(void);
void clearScreen (void);
unsigned char printPortString (unsigned char index);
#endif