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
float x;
float valor;
#define _XTAL_FREQ 8000000


//******************************************************************************
//  Prototipo de funciones
//******************************************************************************
void setup(void);
void __interrupt() ISR(void);
double conversor(uint8_t val);

//******************************************************************************
//  Ciclo Principal
//******************************************************************************

void main(void) {
    //configuramos puertos y comunicación SPI
    setup();
    count = 0;
    flag = 1;
    spiInit(SPI_SLAVE_SS_EN, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    while (1) {
        //llamamos a la configuración del ADC
        ADC_con(flag);
        //convertimos el valor del ADC a flotante
        valor = conversor(valor_MSB);

        //verificamso si el valor obtenido del ADC es menor a 25 para encender
        //led verde
        if (valor < 25) {
            PORTDbits.RD2 = 1;
            PORTDbits.RD1 = 0;
            PORTDbits.RD0 = 0;
        //verificamso si el valor obtenido del ADC es mayor a 25 y menor a 36
        // para encender led amarillo
        } else if (valor > 25 && valor < 36) {
            PORTDbits.RD2 = 0;
            PORTDbits.RD1 = 1;
            PORTDbits.RD0 = 0;
        //verificamso si el valor obtenido del ADC es mayor a 36 para encender
        //led rojo
        } else if (valor > 36) {
            PORTDbits.RD2 = 0;
            PORTDbits.RD1 = 0;
            PORTDbits.RD0 = 1;
        }

    }

}


//******************************************************************************
//  Configuración
//******************************************************************************

void setup(void) {
    TRISA = 0b11111111;
    TRISD = 0b00000000; //puerto D como salida contador leds
    TRISC = 0b00011000;//configuramos la comunicación SPI
    //limpiamos los puertos
    PORTC = 0;
    PORTAbits.RA0 = 0;
    PORTD = 0;
    SSPIF = 0;
    SSPIE = 1;

    INTCON = 0b11101000;//se configuran las interrupciones GIE, PIE, T0IE y RBIE
}


//******************************************************************************
//  Funciones
//******************************************************************************
//función de conversión del ADC a flotante
double conversor(uint8_t val) {
    x = 1.95 * val;
    return (x);
}

void __interrupt() ISR(void) {
    if (PIR1bits.ADIF == 1) {//verificamos si fue interrupt ADC
        flag = 1;
        valor_MSB = ADRESH;
        PIR1bits.ADIF = 0; //apagamos la bandera de ADC
    }//verificamos si fue interrupción por recepción
    if (PIR1bits.SSPIF == 1 && SSPSTATbits.BF == 1) {
        count = spiRead();
        spiWrite(valor_MSB);
        PIR1bits.SSPIF = 0;//pagamos bandera de recepción
    }
}
