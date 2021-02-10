/*
 * File:   Carrera.c
 * Author: Cristopher Sagastume 18640
 *
 * Created on 22 de enero de 2021, 08:46 AM
 */
//******************************************************************************
// PALABRA DE CONFIGURACIÓN
//******************************************************************************

// CONFIG1
#pragma config FOSC = INTRC_CLKOUT// Oscillator Selection bits (INTOSC oscillator: CLKOUT function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF        // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF       // Brown Out Reset Selection bits (BOR enabled)
#pragma config IESO = OFF        // Internal External Switchover bit (Internal/External Switchover mode is enabled)
#pragma config FCMEN = OFF       // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)
#pragma config LVP = OFF         // Low Voltage Programming Enable bit (RB3/PGM pin has PGM function, low voltage programming enabled)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include<stdint.h>
#include "ADC.h"
#include "LCD.h"
//******************************************************************************
//  Variables
//******************************************************************************
uint8_t flag;
uint8_t turno=1;
uint8_t retorno;
uint8_t valor_AN0;
uint8_t valor_AN1;

//******************************************************************************
//  Prototipo de funciones
//******************************************************************************




//******************************************************************************
//  Ciclo Principal
//******************************************************************************

void main(void) {
    setup();
    Lcd_Init();
    while (1) {
        retorno= ADC_con(flag,turno);
        Lcd_Clear();
        Lcd_Set_Cursor(1, 1);
        Lcd_Write_String("LCD Library for");
        Lcd_Set_Cursor(2, 1);
        Lcd_Write_String("MPLAB XC8");
        __delay_ms(2000);
    }
}


//******************************************************************************
//  Configuración
//******************************************************************************

void setup(void) {
    TRISD = 0b00000000;
    TRISC = 0b00000000;
    TRISE = 0b00000000;
    PORTC = 0;
    PORTD = 0;
    PORTE = 0;
}


//******************************************************************************
//  Funciones
//******************************************************************************




//******************************************************************************
//  Interrupciones
//******************************************************************************

void __interrupt() ISR(void) {
    if (PIR1bits.ADIF == 1) {//verificamos si fue interrupt ADC
        if (retorno == 0) {
            valor_AN0 = ADRESH;
            turno = 1;
        } else if (retorno == 1) {
            valor_AN1 = ADRESH;
            turno = 0;
        }
        flag = 1;

        PIR1bits.ADIF = 0; //apagamos la bandera de ADC
    }
}