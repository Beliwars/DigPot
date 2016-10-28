
#include "global.h"
//#include <stdio.h>


/****************************************************************************/
//                  VERY IMPORTANT for proper I2C function
void is_I2C_idle(void)      
{
  while (( SSPCON2 & SSPCON2_MASK ) | R_W ); // wait for idle
}

/****************************************************************************/

void I2C_Start(void)
{
  is_I2C_idle();
  SEN = START_BIT;
  is_I2C_idle();
  SSPBUF = LCD_ADD_WRITE;        // address PCF8574A I2C port expander
}

/****************************************************************************/
// * This function actually initiates I2C write sequence. Also added NACK error
// * checking which will repeat the nacked transmission until an ACK is rec'd.
// * TO DO: Add a time out counter to abort in case I2C bus becomes locked.

void Write_I2C(uint8_t data)
{
  uint8_t ackflag = NACK;
  while(ackflag)
  {
    is_I2C_idle();
    SSPIF = OFF;        // clear MSSP Interrupt Flag
    SSPBUF = data | 0b00001000;      // send a char out on bus
    while(!SSPIF);      // goes high after 9th bit time, after ACK bit
    ackflag = ACK;
    if(ACKSTAT) ackflag = NACK; //* if ACTSTAT == 1 (nak) repeat loop
  }
}

/****************************************************************************/

void lcd_write_byte(uint8_t data)
{
  uint8_t shadow;                   // the world famous "Shadow" register
    
  shadow = (data & UPPER);        // EN, RS, RW = 0 0 0
  if(RS) shadow |= 0b00000001;    // skip if cmd, select lcd data register
  Write_I2C(shadow);
  strobe(shadow);
// - - - - - - - - - - - - - 
  shadow = (data << NIBBLE);      // EN, RS, RW = 0 0 0 by left shift fill
  if(RS) shadow |= 0b00000001;    // skip if cmd, select lcd data register
  Write_I2C(shadow);
  strobe(shadow);
}

void lcd_write_data(uint8_t data)
{
  uint8_t shadow;                   // the world famous "Shadow" register
    
  shadow = (data & UPPER);        // EN, RS, RW = 0 0 0
  shadow |= 0b00000001;    // skip if cmd, select lcd data register
  Write_I2C(shadow);
  strobe(shadow);
// - - - - - - - - - - - - - 
  shadow = (data << NIBBLE);      // EN, RS, RW = 0 0 0 by left shift fill
  shadow |= 0b00000001;    // skip if cmd, select lcd data register
  Write_I2C(shadow);
  strobe(shadow);
}

void lcd_write_cmd(uint8_t cmd)
{
  uint8_t shadow;                   // the world famous "Shadow" register
    
  shadow = (cmd & UPPER);        // EN, RS, RW = 0 0 0
  Write_I2C(shadow);
  strobe(shadow);
// - - - - - - - - - - - - - 
  shadow = (cmd << NIBBLE);      // EN, RS, RW = 0 0 0 by left shift fill
  Write_I2C(shadow);
  strobe(shadow);
}

/****************************************************************************/
//      used by printf() function to print to LCD
// Uses ASCII control code VT (0x0B) #defined as 'v' to indicate that an LCD
// control code follows and RS must be set to CMD mode (0) for the next byte

void putch(uint8_t data)
{
  if(data == RS_TRAP)
  {
    RS = LCD_CMD;
    return;               // ends the \v sequence
  }else if(data == '\b'){
      uart = 1;
      return;
  }
  if(uart == 1){
      EUSART_Write(data);
      uart = 0;
      return;
  }
  lcd_write_byte(data);
  if(!RS)
  {
    if ((data >= LCD_CLR) && (data <= LCD_HOME))
    __delay_ms(LCD_DELAY);
  }
  RS = LCD_DATA;
}

/****************************************************************************/
//                    Wiggle the E line
void strobe(uint8_t cmd)      // shadow --> cmd
{
  cmd |= En;             // EN 0000_0001
  Write_I2C(cmd);           // send modified shadow to LCD
  __delay_us(1);
  cmd &= ~En;            // EN 0b11111110
  Write_I2C(cmd);           // send restored shadow to LCD
  __delay_us(50);
  PORTD = cmd;              // show activity ( for glitz))
  //__delay_ms(5000);
}

/****************************************************************************/
//                          ADC module
void test_adc(void)
{
  uint16_t result = 0;
  float result_avg = 0;
  uint8_t i;//, j, buf[MAX_BUF];
    
  //printf( CLS "Turn the dial...");
  __delay_ms(250);
  //printf( LINE2 "Voltage = ");
  __delay_ms(250);
  while(1)
  {
    result_avg = 0;
    for(i = 0; i < SAMPLES; i++)        // get 30 samples
    {
      __delay_ms(ACQ_DELAY);            // Acquisition delay 5ms
      GO = ON;                          // Start conversion
      while(GO);                        // wait for conversion to complete
      result = (ADRESH << 8) + ADRESL;
      result_avg += result;
    }
      //printf( LINE2_13 "%4f    ", result_avg / SAMPLES);
      __delay_ms(250);
  }
}

/****************************************************************************/
//              Bring the LCD to life in 4-bit mode
void LCD_Init(void)
{
    __delay_ms(50);
  //uint8_t shadow = SET_8_BIT;   // 0b0011_0000
  Write_I2C(0x00);
  __delay_ms(20);
  //PORTD = LCD_INIT;
  __delay_ms(1000);
  Write_I2C(LCD_8BIT_INIT);          // data = 3, EN & RS lo  to PCF8574 --> LCD
  strobe(LCD_8BIT_INIT);
  __delay_us(5);

  strobe(LCD_8BIT_INIT);             // toggle EN line   latch PCF8574 output into LCD
  __delay_ms(5);

  strobe(LCD_8BIT_INIT);             // toggle EN line   latch PCF8574 output into LCD
  __delay_ms(1);

  Write_I2C(LCD_4BIT_INIT);
  strobe(LCD_4BIT_INIT);             // toggle EN line   latch PCF8574 output into LCD
  __delay_ms(1);
    
  //shadow = FOUR_BIT;          // 0b0010_0000  4 bit data bus 
  //Write_I2C(shadow);          // data = 2, EN & RS lo  to PCF8574 --> LCD 
  //strobe(shadow);             // toggle EN line   latch PCF8574 output into LCD
    
  lcd_write_byte(FOUR_MORE);  // 0b0010_1000  set 4 bit IF, move cursor
  lcd_write_byte(CURSOR);     // 0b0000_1100  display ON, Cursor ON, no blink
  lcd_write_byte(INC_CURSOR); // 0b0000_0110  Cursor increments, no shift
  //scrollDisplayLeft();
  lcd_write_byte(LCD_RETURN_HOME);
}

/****************************************************************************/

void banners(void)
{
  //printf( CLS "Beliwars: Hello, World!!");
  __delay_ms(500);
  /*for(int i=0; i <8;i++){
      scrollDisplayLeft();
      __delay_ms(500);
  }*/
  //printf( LINE2 "Finally or not, or not? or not?!");
  __delay_ms(500);
  for(int i=0; i <96;i++){
      scrollDisplayLeft();
      __delay_ms(250);
  }
  //scrollDisplayLeft();
  
  
 // printf( CLS "OMG, is because of a sucky flag?");
  __delay_ms(500);
 /// printf( LINE2 "Viva Lucho el man que no ayuda");
  __delay_ms(500);
  for(int i=0; i <96;i++){
      scrollDisplayLeft();
      __delay_ms(250);
  }
  //scrollDisplayRight();
  //__delay_ms(SEC_3);  
  //lcd_write_byte('A');
  
}

/****************************************************************************/
void scrollDisplayLeft(void){
    RS = LCD_CMD;
	lcd_write_byte(LCD_MV_CUR_SHIFT_DISPLAY | LCD_DISPLAY_SHIFT | LCD_SHIFT_LEFT );
}
void scrollDisplayRight(void) {
    RS = LCD_CMD;
	lcd_write_byte(LCD_MV_CUR_SHIFT_DISPLAY | LCD_DISPLAY_SHIFT | LCD_SHIFT_RIGHT);
}

/********** high level commands, for the user! */
void LCD_Write_Char(char message)
{
    lcd_write_byte((unsigned char) message);
}


void LCD_Write_Str(const char *message)
{
    unsigned char *message_ptr = (unsigned char *) message;

    while (*message_ptr)
        lcd_write_byte((unsigned char) (*message_ptr++));
}

void LCD_go2pos(unsigned char col, unsigned char row)
{
  int row_offsets[] = {LCD_LINE1, LCD_LINE2};
  if (row >= LCD_MAX_ROWS) {
    row = 1;    // we count rows starting w/0
  }

  lcd_write_cmd(LCD_DD_RAM_ADDRESS | col  | row_offsets[row]);
}