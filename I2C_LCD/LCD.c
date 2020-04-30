#include "LCD.h"
#include "i2c.h"

unsigned char RS, BackLight_State = LCD_BACKLIGHT;


void delay_ms(int t){
    for (int i=0; i<(t*8000); i++);
}

void LCD_Init(uint8_t I2C_Add){
  i2c_start_addr(I2C1,I2C_Add,Write);
  IO_Expander_Write(0x00);
  delay_ms(50);  // wait for >40ms
  LCD_CMD(0x03);
  delay_ms(5);
  LCD_CMD(0x03);
  delay_ms(1);
  LCD_CMD(0x03);
  delay_ms(1);
  LCD_CMD(LCD_RETURN_HOME);
  delay_ms(1);
  LCD_CMD(0x20 | (LCD_TYPE << 2));
  delay_ms(1);
  LCD_CMD(LCD_TURN_ON);
  delay_ms(1);
  LCD_CMD(LCD_CLEAR);
  delay_ms(1);
  LCD_CMD(LCD_ENTRY_MODE_SET | LCD_RETURN_HOME);
  delay_ms(1);
}

void IO_Expander_Write(unsigned char Data)
{
  i2c_write(I2C1,Data);//i2c_write(&i2c,value&0x0FF); asi viene en el ejemplo   
}

void LCD_Write_4Bit(unsigned char Nibble)
{
  // Get The RS Value To LSB OF Data
  Nibble |= RS;
  IO_Expander_Write(Nibble | 0x04);
  IO_Expander_Write(Nibble & 0xFB);
  delay_ms(1);
}

void LCD_CMD(unsigned char CMD)
{
  RS = 0; // Command Register Select
  LCD_Write_4Bit(CMD & 0xF0);
  LCD_Write_4Bit((CMD << 4) & 0xF0);
}

void LCD_Write_Char(char Data)
{
  RS = 1; // Data Register Select
  LCD_Write_4Bit(Data & 0xF0);
  LCD_Write_4Bit((Data << 4) & 0xF0);
}

void LCD_Write_String(char* Str)
{
  for(int i=0; Str[i]!='\0'; i++)
    LCD_Write_Char(Str[i]);
}

void LCD_Set_Cursor(unsigned char ROW, unsigned char COL)
{
  switch(ROW) 
  {
    case 2:
      LCD_CMD(0xC0 + COL-1);
      break;
    case 3:
      LCD_CMD(0x94 + COL-1);
      break;
    case 4:
      LCD_CMD(0xD4 + COL-1);
      break;
    // Case 1
    default:
      LCD_CMD(0x80 + COL-1);
  }
}


void LCD_putc(char ch)  {
    LCD_Write_Char(ch);
}

