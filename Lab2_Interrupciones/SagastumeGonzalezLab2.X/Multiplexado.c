/*
 * File:   Multiplexado.c
 * Author: SAGASTUME
 *
 * Created on 3 de febrero de 2021, 12:55 PM
 */


#include <xc.h>
//uint8_t seg1;
//uint8_t seg2;
void Multiplex(void) {
    OPTION_REG= 0b10000111;
    TMR0=10;
    //INTCON = 0b11101000;
    
}
