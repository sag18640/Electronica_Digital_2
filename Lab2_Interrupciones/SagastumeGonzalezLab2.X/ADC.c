/*
 * File:   ADC.c
 * Author: SAGASTUME
 *
 * Created on 2 de febrero de 2021, 11:46 PM
 */

#include<stdint.h>
#include <xc.h>
#define _XTAL_FREQ 8000000
uint8_t flag;


void ADC_con(uint8_t flag) {
   
    ADCON1bits.ADFM = 0;
    ANSEL = 0b00000001;
    INTCON = 0b11101000;
    ADCON0 = 0b01000001;
    //INTCON = 0b11101000;
    PIE1bits.ADIE = 1;
    PIR1bits.ADIF = 0;
    if (flag == 1) {
        __delay_us(20);
        ADCON0bits.GO = 1;
        flag = 0;
        

    }
    

}
