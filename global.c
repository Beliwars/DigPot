
#include "global.h"


void SYSTEM_Initialize(void)
{    
    PIN_Initialize();
    EUSART_Initialize();
    //I2C_Initialize();
    //I2C_Start();
    //LCD_Init();
}

void PIN_Initialize(void)
{
    TRISD = 0x00;                   // set the PORTD to output 
    //TRISB = 0b00110000;
    TRISCbits.TRISC0 = OUTPUT;
    TRISCbits.TRISC6 = INPUT;      //Make RC6/TX an output
    TRISCbits.TRISC7 = INPUT;       //Make RC7/RX an Input
    TRISCbits.TRISC3 = INPUT;
    TRISCbits.TRISC4 = INPUT;
    TRISBbits.TRISB4 = INPUT;		// set rotary encoder pins to input
	TRISBbits.TRISB5 = INPUT;
    OPTION_REGbits.nRBPU = LOW;
    ADCON0 = 0x01;                  // FOSC/2, Channel 0 (RA0/AN0), 
    ADCON1 = 0x02;                  // Left Justified, PORT A all Analog Inputs, PORTE all digital, Vref+ = Vdd Vref- = Vss
    TRISA = 0x1F;                   //Port A all input
    SSPCON2 = 0x00;
    SSPCON = 0x38;                  //SSPEN ON, CKP ON(Enable Clock I2C), Master mode, clock = FOSC / (4 * (SSPADD+1)) 
    SSPADD = 0x31;                  // 100 KHz clock rate
    SSPSTAT = 0x80;                 // SMP off, Slew rate disabled
    SSPIF = OFF;                    // clear SSPIF interrupt flag
    BCLIF = OFF;
}

/**
 End of File
*/
