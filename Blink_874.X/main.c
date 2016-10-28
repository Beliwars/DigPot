/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

//#include <xc.h>

#pragma config FOSC = HS        // Oscillator Selection bits (XT oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config CP = OFF         // FLASH Program Memory Code Protection bits (Code protection off)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF         // Low Voltage In-Circuit Serial Programming Enable bit (RB3/PGM pin has PGM function; low-voltage programming enabled)
#pragma config CPD = OFF        // Data EE Memory Code Protection (Code Protection off)
#pragma config WRT = OFF         // FLASH Program Memory Write Enable (Unprotected program memory may be written to by EECON control)


//#include <stdint.h>        /* For uint8_t definition */
//#include <stdbool.h>       /* For true/false definition */
//#include <stdio.h>
#include "global.h"


/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/

/* i.e. uint8_t <variable_name>; */
#define HIGH 1
#define LOW 0

#define PhaseA PORTBbits.RB4        // Encoder A Pin
#define PhaseB PORTBbits.RB5        // Encoder B Pin

#define FALSE 0
#define TRUE ~FALSE

//void exerciseDisplay(void);
//void displayLeftRightScroll(void);
//void autoIncrement(void);
//void displayOnOff(void);
//void backLightControl(void);
void delay(uint8_t count);
    

volatile unsigned char counter=1;    // Used to control brightness of LED, values 0 to 25 (we dont need 250 increments)
unsigned char RBP1;
unsigned char RBP2;
unsigned char RBP1_prev=0;
unsigned char RBP2_prev=0;
volatile unsigned char changed=0;    // Used to control brightness of LED, values 0 to 25 (we dont need 250 increments)
/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/

void interrupt InterruptManager (void)
{
    if(INTCONbits.RBIF==1)
    {
        //__delay_ms(10);
        RBP1 = PhaseA;
        RBP2 = PhaseB;
        /*unsigned char state = RBP2 << 1 | RBP1;
        unsigned char prev_state = RBP2_prev << 1 | RBP1_prev;
        
        if(state == 0x11){
            if(prev_state == 0x01){
                if(counter>0){
                    counter--;
                }
            }else if(prev_state == 0x10){
                if(counter<32){
                    counter++;
                }
            }
        }*/
        
        if((RBP1 != RBP1_prev) && RBP1 == OFF){
            // A has gone from high to low
            changed = 1;
            if(!(RBP2^RBP1))
            {
                    // B is low so counter-clockwise            
                if(counter>1){
                    counter--;
                }
            }else{
                // B is high so clockwise
                if(counter<63){
                    counter++;
                }
            } 
        }/*else if(RBP2 == RBP2_prev 10){
            if(RBP1^RBP2)
            {
                // B is high so clockwise
                if(counter<25){
                    counter++;
                }
            }else{
                // B is low so counter-clockwise            
                if(counter>0){
                    counter--;
                }
            }
            RBP2_prev = RBP2;
        } */   
        RBP1_prev = RBP1;     // Store value for next time
        RBP2_prev = RBP2;     // Store value for next time
        int temp = PORTB;
        INTCONbits.RBIF = LOW; 
        //PORTD = counter << 3;        
    }else if(PIE1bits.TXIE == ON && PIR1bits.TXIF == ON)
    {
        EUSART_Transmit_ISR();
    }
    else if(PIE1bits.RCIE == ON && PIR1bits.RCIF == ON)
    {
        EUSART_Receive_ISR();
    }
    /*else if(INTCONbits.PEIE == ON && PIE2bits.BCLIE == ON && PIR2bits.BCLIF == ON)
    {
        I2C_BusCollisionISR();
    }
    else if(INTCONbits.PEIE == ON && PIE1bits.SSPIE == ON && PIR1bits.SSPIF == ON)
    {
        I2C_ISR();
    }*/
}
void main(void)
{   
    // Initialisation
    //EUSART_Initialize();
    //I2C_Initialize();
    //LCD_Init();
    SYSTEM_Initialize();
    I2C_Start();
    LCD_Init();
    /*TRISD = 0;            // configure GP2 (only) as an output
    TRISB = 0b00110000;
    PORTD = 0x01;
    TRISCbits.TRISC0 = LOW;
    OPTION_REGbits.nRBPU = LOW;*/
    RBIE_ON();   //INTCONbits.RBIE = HIGH;            
    PEIE_ON();
    GIE_ON();   //INTCONbits.GIE = HIGH;
    
    printf(PRINT_USART "UART Inicializado...\n");
    //printf("Probando envio en interrupcion\n");
    printf( CLS "Digital Pot Initializing ...");
    lcd_write_cmd(LCD_CLEAR_DISPLAY);
    __delay_ms(5);
    LCD_Write_Str("DigPot Initializing ...");
    __delay_ms(50);
    printf( LINE2 ".");
    __delay_ms(500);
    printf(".");
    __delay_ms(500);
    printf(".");
    __delay_ms(500);
    printf(".");
    __delay_ms(500);
    printf(".");
    __delay_ms(500);
    //exerciseDisplay();
    for(int i=0; i <32;i++){
      scrollDisplayLeft();
      __delay_ms(400);
    }
    printf( CLS "DigPot Ready 2go");
    while(1)
    { 
        //PORTCbits.RC0 = HIGH;        // 
        //uint8_t delay = counter * 50;
        PORTD = 0x04;
        delay(counter);        // stay on for 200 ms
        PORTD = 0x08;
        delay(counter);        // stay on for 200 ms
        //PORTCbits.RC0 = LOW;        // 
        PORTD = 0x10;
        delay(counter);        // stay on for 200 ms
        PORTD = 0x20;
        delay(counter);        // stay on for 200 ms
        //PORTCbits.RC0 = LOW;        // 
        PORTD = 0x40;
        delay(counter);        // stay on for 200 ms
        PORTD = 0x80;
        delay(counter);        // stay on for 200 ms
        //PORTCbits.RC0 = LOW;        // 
        PORTD = 0x40;
        delay(counter);        // stay on for 200 ms
        PORTD = 0x20;
        delay(counter);        // stay on for 200 ms
        //PORTCbits.RC0 = LOW;        // 
        PORTD = 0x10;
        delay(counter);        // stay on for 200 ms
        PORTD = 0x08;
        delay(counter);        // stay on for 200 ms
        if(changed){
            printf(LINE2 "Counter: %d dB",counter);
            changed = 0;
        }
    } 
    // repeat forever
}

void delay(uint8_t count){
    for(int i=0; i < count; i++)
        __delay_ms(25);
}
/*
void exerciseDisplay(void)
{
    autoIncrement();
    displayLeftRightScroll();
    displayOnOff();
    backLightControl();
}

void autoIncrement(void)
{
  char Autoscroll[] = "Autoscroll";
  char NoAutoscroll[] = "No Autoscroll";
  unsigned char * p;

  LCDclear();
  LCDcursorOff();
  LCDblinkOff();
  LCDautoscroll();
  p = Autoscroll;
  LCDsetCursor(15,1);
  while (*p)
  {
      LCD_Write_Char((char) *p++);
      __delay_ms(400);
  }
  __delay_ms(5000);

  LCDclear();
  LCDnoAutoscroll();
  p = NoAutoscroll;
  LCDsetCursor(3,1);
  while (*p)
  {
      LCD_Write_Char((char) *p++);
      __delay_ms(400);
  }
  __delay_ms(5000);
}


void displayLeftRightScroll(void)
{
  LCDclear();
  LCDcursorOff();
  LCDblinkOff();
  LCDsetCursor(0,0);
  LCD_Write_Str("Scroll Right");
  LCDsetCursor(0,1);
  LCD_Write_Str("---->");
  for(unsigned char x = 0; x< 8; x++)
  {
    LCDscrollDisplayRight();
    __delay_ms(500);
  }

  LCDclear();
  LCDsetCursor(8,0);
  LCD_Write_Str("Scroll Left ");
  LCDsetCursor(8,1);
  LCD_Write_Str("<----");
  for(unsigned char x = 0; x< 8; x++)
  {
    LCDscrollDisplayLeft();
    __delay_ms(500);
  }

  __delay_ms(5000);
}



void displayOnOff(void)
{
  LCDclear();
  LCDcursorOff();
  LCDblinkOff();
  for (unsigned char x = 0; x < 6; x++)
  {
      if (x%2) {
        LCDhome();
        LCD_Write_Str("           ");
        LCDhome();
        LCD_Write_Str("Display On ");
        LCDdisplayOn();
      } else {
        LCDsetCursor(0,0);
        LCD_Write_Str("           ");
        LCDsetCursor(0,0);
        LCD_Write_Str("Display Off");
        LCDdisplayOff();
      }
      __delay_ms(750);
  }
}

void backLightControl(void)
{
  LCDclear();
  LCDcursorOff();
  LCDbacklight();
  LCD_Write_Str("Backlight On");
  __delay_ms(1000);
  LCDclear();
  //LCDsetCursor(0,0);
  LCDnoBacklight();
  LCD_Write_Str("Backlight Off");
  __delay_ms(1000);
  LCDclear();
  //LCDsetCursor(0,0);
  LCDbacklight();
  LCD_Write_Str("Backlight On");
  __delay_ms(1000);
  for (unsigned char x = 0; x < 6; x++)
  {
      if (x%2) {
        LCDsetCursor(0,0);
        LCD_Write_Str("             ");
        LCDsetCursor(0,0);
        LCD_Write_Str("Backlight On ");
        LCDbacklight();
      } else {
        LCDclear();
        LCD_Write_Str("Backlight Off");
        LCDnoBacklight();
      }
      __delay_ms(750);
  }
}


*/