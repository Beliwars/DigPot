#ifndef _I2C_LCD_H
#define _I2C_LCD_H

#include "global.h"
#include <stdio.h>  


#ifdef __cplusplus  // Provide C++ Compatibility
    extern "C" {
#endif
// Global variables
uint8_t RS = 0;       // select lcd command register

#define POWER_UP_DELAY  50
#define NIBBLE          4
#define INIT_DELAY_5    5
#define ACQ_DELAY       5
#define INIT_DELAY_2    2
#define FOUR_BIT        0x20
#define FOUR_MORE       0x28
#define CURSOR          0x0C
#define INC_CURSOR      6
//#define NULL            0x00
#define EN_OFF          0b11111011
#define ACK             0
#define LCD_CMD         0
#define OFF             0
#define ON              1
#define LCD_DATA        1
#define EN_ON           0b00000100
#define NACK            1
#define START_BIT       1
#define ALL_HI          0xFF
#define RC34            0x18
#define MASTER          0x38
#define BUS100          0x31
#define ADC80           0x80
#define ADC81           0x81
#define MAX_BUF         10
#define SAMPLES         30
#define UPPER           0xF0
#define LCD_ADD_WRITE   0x4E
#define LCD_ADD_READ    0x4F
#define SEC_2           2000
#define SEC_3           3000
#define SSPCON2_MASK    0x1F
#define SET_8_BIT       0x30
#define LCD_DELAY       1.64
#define LCD_CLR         0b00000001
#define LCD_HOME        0b00000011

#define LCD_CLEAR_DISPLAY           0x01    // Mode : Clears display

#define LCD_RETURN_HOME             0x02    // Mode : Returns cursor to home posn.

// Entry Mode Set
#define LCD_ENTRY_MODE_SET          0x04    // Mode : Entry Mode Set, Sets the cursor move dir and specs whether or not to shift the display
#define LCD_INCREMENT               0x02        // Sub Mode of ENTRY_MODE_SET : Increment DDRAM (I/D), Entry Left
#define LCD_DECREMENT               0x00        // Sub Mode of ENTRY_MODE_SET : Decrement DDRAM (I/D), Entry Right
#define LCD_SHIFT_ON                0x01        // Sub Mode of ENTRY_MODE_SET : Shift On  (S), Shift Display when byte written. Display Shift
#define LCD_SHIFT_OFF               0x00        // Sub Mode of ENTRY_MODE_SET : Shift Off (S), Don't shift display when byte written. Cursor Move

// Display Function
#define LCD_DISPLAY_ON_OFF          0x08    // Mode : Display On/Off, Sets on/off of all display, Cursor on/off, Cursor Blink on/off
#define LCD_DISPLAY_ON              0x04        // Sub Mode of DISPLAY_ON_OFF : Puts display on  (D)
#define LCD_DISPLAY_OFF             0x00        // Sub Mode of DISPLAY_ON_OFF : Puts display off (D)
#define LCD_CURSOR_ON               0x02        // Sub Mode of DISPLAY_ON_OFF : Puts cursor on   (C)
#define LCD_CURSOR_OFF              0x00        // Sub Mode of DISPLAY_ON_OFF : Puts cursor off  (C)
#define LCD_BLINKING_ON             0x01        // Sub Mode of DISPLAY_ON_OFF : Blinking cursor  (B)
#define LCD_BLINKING_OFF            0x00        // Sub Mode of DISPLAY_ON_OFF : Solid cursor     (B)

// Display Control
#define LCD_MV_CUR_SHIFT_DISPLAY    0x10    // Mode : Move the cursor and shifts the display
#define LCD_DISPLAY_SHIFT           0x08        // Sub Mode of CURSOR_SHFT_DIS : Display shifts after char print   (SC)
#define LCD_CURSOR_SHIFT            0x00        // Sub Mode of CURSOR_SHFT_DIS : Cursor shifts after char print    (SC)
#define LCD_SHIFT_RIGHT             0x04        // Sub Mode of CURSOR_SHFT_DIS : Cursor or Display shifts to right (RL)
#define LCD_SHIFT_LEFT              0x00        // Sub Mode of CURSOR_SHFT_DIS : Cursor or Display shifts to left  (RL)

// Function Set
#define LCD_FUNCTION_SET            0x20    // Mode : Set the type of interface that the display will use
#define LCD_INTF8BITS               0x10        // Sub Mode of FUNCTION_SET : Select 8 bit interface         (DL)
#define LCD_INTF4BITS               0x00        // Sub Mode of FUNCTION_SET : Select 4 bit interface         (DL)
#define LCD_TWO_LINES               0x08        // Sub Mode of FUNCTION_SET : Selects two char line display  (N)
#define LCD_ONE_LINE                0x00        // Sub Mode of FUNCTION_SET : Selects one char line display  (N)
#define LCD_FONT_5_10               0x04        // Sub Mode of FUNCTION_SET : Selects 5 x 10 Dot Matrix Font (F)
#define LCD_FONT_5_7                0x00        // Sub Mode of FUNCTION_SET : Selects 5 x 7 Dot Matrix Font  (F)

#define LCD_CG_RAM_ADDRESS          0x40        // Mode : Enables the setting of the Char Gen (CG) Ram Address, to be or'ed with require address
#define LCD_CG_RAM_ADDRESS_MASK     0b00111111  // Used to mask off the lower 6 bits of valid CG Ram Addresses

#define LCD_DD_RAM_ADDRESS          0x80        // Mode : Enables the setting of the Display Data (DD) Ram Address, to be or'ed with require address
#define LCD_DD_RAM_ADDRESS_MASK     0b01111111    // Used to mask off the lower 6 bits of valid DD Ram Addresses

//#define USE_BUSY_FLAG               // Define this if you wish to use busy flag polling on slow LCD activities

// Change here for your I2C to 16 pin parallel interface // TODO Adapt
#define Bl 0b00001000  // Backlight enable bit (On = 1, Off =0)
#define En 0b00000100  // Enable bit (Enable on low edge)
#define Rw 0b00000010  // Read/Write bit (Read = 1, Write = 0)
#define Rs 0b00000001  // Register select bit (Data = 1, Control = 0)

// Change here for your I2C to 16 pin parallel interface // TODO Adapt
#define LCD_INIT      ((0b00000000 | En) & ~Rs) & (~Rw) // Used to set all the O/Ps on the PCF8574 to initialise the LCD
#define LCD_8BIT_INIT 0b00110000 // Used to initialise the interface at the LCD
#define LCD_4BIT_INIT 0b00100000 // Used to initialise the interface at the LCD

#define LCD_RETRY_MAX  100  // define the retry count
#define LCD_I2C_ADDR         0x27  // Modify this if the default address is altered 
#define LCD_PCF8574_WEAK_PU      0b11110000 // Used to turn on PCF8574 Bits 7-4 on. To allow for read of LCD.

#define LCD_BUSY_FLAG_MASK       0b10000000 // Used to mask off the status of the busy flag
#define LCD_ADDRESS_COUNTER_MASK 0b01111111 // Used to mask off the value of the Address Counter
#define LCD_MAX_COLS             16
#define LCD_MAX_ROWS             2

#define RS_TRAP         '\v'        // used to signal that RS needs to be set 
                                    // to command mode for following command
#define CLS             "\v\x01"    // Clear Screen
#define LINE2           "\v\xC0"    // Line 2 Col 1
#define LINE2_13        "\v\xC9"    // Line 2 Col 13
#define LCD_LINE1       0x00		// Constant used to point to start of LCD Line 1
#define LCD_LINE2       0x40		// Constant used to point to start of LCD Line 2
#define PRINT_USART     "\b"        // Print redirected to USART

uint8_t uart = 0;

// Prototypes
void Write_I2C(uint8_t data);
//void init();
void i2c_start();
void i2c_stop();
void lcd_write_byte(uint8_t data);
void LCD_Init();
void I2C_Start(void);
void test_adc(void);
void strobe(uint8_t cmd);
void is_I2C_idle(void);
//void putch(uint8_t data);
void banners(void);
void scrollDisplayLeft(void);
void scrollDisplayRight(void);
void LCD_Write_Char(char message);
void LCD_Write_Str(const char *message);
void lcd_write_data(uint8_t data);
void lcd_write_cmd(uint8_t cmd);
void LCD_go2pos(unsigned char col, unsigned char row);

#ifdef __cplusplus  // Provide C++ Compatibility
}
#endif

#endif //_I2C_H
    
/**
 End of File
*/