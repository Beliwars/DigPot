
#ifndef GLOBAL_H
#define	GLOBAL_H

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include "eusart.h"
#include "i2c_lcd.h"            // for printf

#define GIE_ON() (INTCONbits.GIE = ON)

#define GIE_OFF() (INTCONbits.GIE = OFF)

#define PEIE_ON() (INTCONbits.PEIE = ON)

#define PEIE_OFF() (INTCONbits.PEIE = OFF)

#define RBIE_ON() (INTCONbits.RBIE = ON)

#define RBIE_OFF() (INTCONbits.RBIE = OFF)

#define _XTAL_FREQ  20000000

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ON    1
#define OFF     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      0
#define PULL_UP_DISABLED     1

#define REA PORTBbits.RB4	// Rotary encoder pin definition
#define REB PORTBbits.RB5


void PIN_Initialize(void);

void PIN_IOC(void);

void SYSTEM_Initialize(void);



#endif	/* MCC_H */
/**
 End of File
*/