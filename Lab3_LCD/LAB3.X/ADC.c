/*
 * File:   ADC.c
 * Author: SAGASTUME
 *
 * Created on 2 de febrero de 2021, 11:46 PM
 */

#include<stdint.h>
#include <xc.h>
#define _XTAL_FREQ 8000000

uint8_t ADC_con(uint8_t flag, uint8_t turno) {
    uint8_t var;
    ADCON1bits.ADFM = 0; //se justifica la resolución del ADC a la izquierda  en ADRESH
    ANSEL = 0b00000011; //se configura el RA0 como entrada analógica
    INTCON = 0b11101000; //se configuran las interrupciones GIE, PIE, T0IE y RBIE
    //ADCON0 = 0b01000001;//frecuencia de oscilacion 1/8 canal analógico AN0 y 
    //encender ADC
    PIE1bits.ADIE = 1; //se configura la interrupcion del ADC
    PIR1bits.ADIF = 0; //se apaga la bandera de interrupcion ADC
    if (flag == 1 && turno == 1) {
        ADCON0 = 0b01000001;//frecuencia de oscilacion 1/8 canal analógico AN0 y 
    //encender ADC
        __delay_us(20);
        ADCON0bits.GO = 1; //se indica que empiece a convertir al ADC
        flag = 0;
        var=0;
        
    } else if (flag == 1 && turno == 0) {
        ADCON0 = 0b01000101;//frecuencia de oscilacion 1/8 canal analógico AN1 y 
    //encender ADC
        __delay_us(20);
        ADCON0bits.GO = 1; //se indica que empiece a convertir al ADC
        flag = 0;
        var=1;
    }
    return (var);
}