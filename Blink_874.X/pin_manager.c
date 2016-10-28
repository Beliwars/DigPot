
#include <xc.h>
#include "pin_manager.h"

void PIN_MANAGER_Initialize(void)
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
 * 
 * TRISD = 0;            // configure GP2 (only) as an output
    TRISB = 0b00110000;
    PORTD = 0x01;
    TRISCbits.TRISC0 = LOW;
    OPTION_REGbits.nRBPU = LOW;
 * void init(void)
{
  // Make all unused ports output and set to 0. Makes things more stable.
    
  PORTA = TRISB = ADCON0 = PORTC = PORTE = SSPCON2 = PORTD = TRISD = NULL;
  //TRISA = ANSEL = 1;  // Only RA0 is Analog Input
  ADCON1 = 0x02;                  // Left Justified, PORT A all Analog Inputs, PORTE all digital, Vref+ = Vdd Vref- = Vss
  TRISA = 0x1F;
  PORTB =   ALL_HI;
  TRISC =   RC34;     // RC3 & RC4 assigned to i2c
  SSPCON =  MASTER;   // set I2C master mode
  SSPADD =  BUS100;   // 100kHz i2c bus with 20MHz xtal
  SSPSTAT = 0x80;
  //CKE = ON;           // use I2C levels
  //SMP = ON;           // disable slew rate control
  SSPIF = OFF;        // clear SSPIF interrupt flag
  BCLIF = OFF;        // clear bus collision flag
    
  // Set up ADC for reading RP1 on 44 Pin Demo board
  ADCON1 = ADC80;     // right justify & internal Vref
  ADCON0 = ADC81;     // FOSC/32, chan 0, ADC on
}
*/
