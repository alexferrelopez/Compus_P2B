#ifndef _CONTROLLER_H
#define _CONTROLLER_H

#include <xc.h>

#include "tad_timer.h"
#include "tad_teclado.h"
#include "tad_lcd.h"
#include "tad_altavoz.h"
#include "tad_marquesina.h"
#include "tad_SIO.h"
#include "tad_adc.h"
#include "tad_hora.h"

void controllerInit (void);
void controllerMotor(void);

#endif