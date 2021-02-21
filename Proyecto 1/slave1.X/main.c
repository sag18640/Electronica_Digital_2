/*
 * File:   esclavo1.c
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
#include "ADC.h"
#include "SPI.h"
//******************************************************************************
//  Variables
//******************************************************************************
uint8_t count;
uint8_t flag = 1;
uint8_t valor_MSB;


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
        ADC_con(flag);

    }

}


//******************************************************************************
//  Configuración
//******************************************************************************

void setup(void) {
    TRISAbits.TRISA5 = 1;
    TRISD = 0b00000000; //puerto D como salida contador leds
    TRISC = 0b00011000;
    TRISB = 0b11111110;
    PORTB = 0; //limpiamos puertos
    PORTC = 0;
    PORTD = 0;
    PORTE = 0;
    SSPIF = 0;
    SSPIE = 1;
    INTCON = 0b11101000; //se configuran las interrupciones GIE, PIE, T0IE y RBIE
}


//******************************************************************************
//  Funciones
//******************************************************************************

void __interrupt() ISR(void) {
    if (PIR1bits.ADIF == 1) {//verificamos si fue interrupt ADC
        flag = 1;
        valor_MSB = ADRESH;
        PORTD = valor_MSB;
        PIR1bits.ADIF = 0; //apagamos la bandera de ADC
    }
    if (PIR1bits.SSPIF == 1 && SSPSTATbits.BF == 1) {
        count = spiRead();
        spiWrite(v alor_MSB);
        PIR1bits.SSPIF = 0;




    }
}
