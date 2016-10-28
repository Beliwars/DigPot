
#ifndef INTERRUPT_MANAGER_H
#define INTERRUPT_MANAGER_H

#define ON 1
#define OFF 0

#define GIE_ON() (INTCONbits.GIE = ON)

#define GIE_OFF() (INTCONbits.GIE = OFF)

#define PEIE_ON() (INTCONbits.PEIE = ON)

#define PEIE_OFF() (INTCONbits.PEIE = OFF)

#define RBIE_ON() (INTCONbits.RBIE = ON)

#define RBIE_OFF() (INTCONbits.RBIE = OFF)

#endif  // INTERRUPT_MANAGER_H
/**
 End of File
*/