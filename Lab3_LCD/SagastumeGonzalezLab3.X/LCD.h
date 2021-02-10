
#ifndef LCD
#define	LCD

#include <xc.h> 
void Lcd_Port(char a);
void Lcd_Cmd(char a);
void Lcd_Set_Cursor(char a, char b);
void Lcd_Init();
void Lcd_Write_Char(char a);
void Lcd_Write_String(char *a);
void Lcd_Shift_Right();
void Lcd_Shift_Left();
void Lcd_Clear(void);
#endif	/* XC_HEADER_TEMPLATE_H */

