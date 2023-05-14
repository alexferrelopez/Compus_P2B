#include <xc.h>

#include "tad_controller.h"

static unsigned char state, nameCharCount;

void controllerInit (void) {
    state = 0;
    nameCharCount = 0;
}

void controllerMotor(void) {
     switch (state) { //TOWER SELECTION
        case 1:
            
            /* code */
            break;
        case 2: //MENU 
            /* code */
            break;
        case 3:
            /* code */
            break;        
    }    
}