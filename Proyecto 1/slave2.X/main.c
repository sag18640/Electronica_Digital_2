/*
 * File:   contador.c
 * Author: Cristopher Sagastume 18640
 *
 * Created on 29 de enero de 2021, 08:46 AM
 */
//******************************************************************************
// PALABRA DE CONFIGURACIÓN
//******************************************************************************

// CONFIG1
#pragma config FOSC = INTRC_CLKOUT
#pragma config WDTE = OFF       
#pragma config PWRTE = OFF      
#pragma config MCLRE = OFF      
#pragma config CP = OFF         
#pragma config CPD = OFF        
#pragma config BOREN = OFF      
#pragma config IESO = OFF       
#pragma config FCMEN = OFF      
#pragma config LVP = OFF        

// CONFIG2
#pragma config BOR4V = BOR40V   
#pragma config WRT = OFF        

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.
#include<stdint.h>
#include <xc.h>
#include "SPI.h"
//******************************************************************************
//  Variables
//******************************************************************************
uint8_t count;
uint8_t flag = 1;
uint8_t flag2;
uint8_t a;
#define _XTAL_FREQ 8000000


//******************************************************************************
//  Prototipo de funciones
//******************************************************************************
void setup(void);
void __interrupt() ISR(void);


//******************************************************************************
//  Ciclo Principal
//******************************************************************************

void main(void) {
    setup();
    count = 0;
    flag = 1;
    spiInit(SPI_SLAVE_SS_EN, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    while (1) {

        //se le da el valor constantemente a puerto D
        PORTD = count;

        //comparación para alarma visual si la resolución del ADC es mayor
        //al contador de 8 bits
    }

}


//******************************************************************************
//  Configuración
//******************************************************************************

void setup(void) {
    TRISAbits.TRISA5 = 1;
    TRISD = 0b00000000; //puerto D como salida contador leds
    TRISC = 0b00011000; //puerto C como salida display
    TRISB = 0b11111110; //puerto B como entradas a excepción del pin 0 como-
    IOCBbits.IOCB6 = 1;
    IOCBbits.IOCB7 = 1;
    PORTB = 0; //limpiamos puertos
    PORTC = 0;
    PORTD = 0;
    flag2 = 0;
    INTCON = 0b11101000; //se configuran las interrupciones GIE, PIE, T0IE y RBIE
}


//******************************************************************************
//  Funciones
//******************************************************************************

void __interrupt() ISR(void) {
    if (PIR1bits.SSPIF == 1 && SSPSTATbits.BF == 1) {

        a = spiRead();
        spiWrite(0x3F);
        PIR1bits.SSPIF = 0;


    }
    if (INTCONbits.RBIF == 1) {//verificamos si fue interrupt on change
        if (PORTBbits.RB6 == 0) {//antirebote aumentar
            while (flag2 == 0) {
                if (PORTBbits.RB6 == 1) {
                    flag2 = 1;
                }
            }
            flag2 = 0;
            count++;

        } else if (PORTBbits.RB7 == 0) { //antirebote disminuir
            while (flag2 == 0) {
                if (PORTBbits.RB7 == 1) {
                    flag2 = 1;
                }
            }
            flag2 = 0;
            count--;

        }
    }
    INTCONbits.RBIF = 0; //apagamos la bandera de interrupt on change
}

