/*
 * File:   LCD.c
 * Author: SAGASTUME
 *
 * Created on 8 de febrero de 2021, 01:03 AM
 */

#include<stdint.h>
#include <xc.h>
#define _XTAL_FREQ 8000000
#define RS PORTEbits.RE0
#define EN PORTEbits.RE1
//LCD Functions Developed by electroSome

void Lcd_Port(char a) {
    PORTD = a;
}

void Lcd_Cmd(char a) {
    RS = 0; // => RS = 0
    Lcd_Port(a);
    EN = 1; // => E = 1
    __delay_ms(4);
    EN = 0; // => E = 0
}

void Lcd_Clear(void) {
    Lcd_Cmd(0);
    Lcd_Cmd(1);
}

void Lcd_Set_Cursor(char a, char b) {
    char temp;
    if (a == 1) {
        temp = 0x80 + b - 1;
        //		z = temp>>4;
        //		y = temp & 0x0F;
        //		Lcd_Cmd(z);
        Lcd_Cmd(temp);
    } else if (a == 2) {
        temp = 0xC0 + b - 1;
        //		z = temp>>4;
        //		y = temp & 0x0F;
        //		Lcd_Cmd(z);
        Lcd_Cmd(temp);
    }
}

void Lcd_Init() {
    Lcd_Port(0x00);
    __delay_ms(20);
    Lcd_Cmd(0x3F);
    __delay_ms(10);
    Lcd_Cmd(0x3F);
    __delay_us(200);
    Lcd_Cmd(0x3F);
    /////////////////////////////////////////////////////
    Lcd_Cmd(0x32);
    Lcd_Cmd(0x38);
    Lcd_Cmd(0x0C);
    Lcd_Cmd(0x06);
}

void Lcd_Write_Char(char a) {
    char temp, y;
    temp = a & 0x0F;
    y = a & 0xF0;
    RS = 1; // => RS = 1
    Lcd_Port(a); //Data transfer
    EN = 1;
    __delay_us(40);
    EN = 0;
//    Lcd_Port(temp);
//    EN = 1;
    __delay_us(40);
//    EN = 0;
}

void Lcd_Write_String(char *a) {
    int i;
    for (i = 0; a[i] != '\0'; i++)
        Lcd_Write_Char(a[i]);
}

void Lcd_Shift_Right() {
    Lcd_Cmd(0x01);
    Lcd_Cmd(0x0C);
}

void Lcd_Shift_Left() {
    Lcd_Cmd(0x01);
    Lcd_Cmd(0x08);
}