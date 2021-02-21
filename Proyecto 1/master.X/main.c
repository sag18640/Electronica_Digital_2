/*
 * File:   Master.c
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
#include <stdio.h>
#include <xc.h>
#include<stdint.h>
#include "LCD.h"
#include "USART.h"
#include "SPI.h"
#define _XTAL_FREQ 8000000
//******************************************************************************
//  Variables
//******************************************************************************
uint8_t flag = 1;
uint8_t turno = 1;
uint8_t retorno;
int8_t valorT = 0;
uint8_t valorC = 0;
uint8_t valorA = 0;
float v;
float vv;
float x;
float p;
char s[20];
char l[20];
char k[20];
uint8_t count = 0;
//******************************************************************************
//  Prototipo de funciones
//******************************************************************************
void setup(void);
//void __interrupt() ISR(void);
float conversor(uint8_t x);
float conversor2(uint8_t val);
//******************************************************************************
//  Ciclo Principal
//******************************************************************************

void main(void) {
    setup();
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    Config_USARTT();
    recibir();
    Lcd_Init();
    Lcd_Clear();


    while (1) {
        Lcd_Set_Cursor(1, 1); //colocamos el cursor en posicón 1,1
        Lcd_Write_String("S1:   S2:    S3:"); //escribimos los encabezados
        enviar("S1:");
        enviar(l);
        enviar("S2:");
        enviar(k);
        enviar("S3:");
        enviar(s);

        //lm35
        __delay_ms(1);
        PORTA = 0b00000110;
        __delay_ms(100);
        SSPBUF = 0;
        valorT = spiRead();
        //        enviar(250);
        __delay_ms(1);
        PORTA = 0b00000111;
        //        __delay_ms(200);
        Lcd_Set_Cursor(2, 13);
        p = conversor2(valorT);
        sprintf(s, "%3.0fC", p);
        Lcd_Write_String(s);

        //ADC
        __delay_ms(1);
        PORTA = 0b00000101;
        __delay_ms(100);
        SSPBUF = 1;
        valorA = spiRead();
        __delay_ms(1);
        PORTA = 0b00000111;
        Lcd_Set_Cursor(2, 1); //colocamos el cursor en posición 2,1
        v = conversor(valorA);
        sprintf(l, "%3.2fV", v);
        //        enviar(s);
        Lcd_Write_String(l);
        //        
        //
        if (valorC >= 0 && valorC < 10) {//limpiamos los espacios de decenas y 
            //centenas si en caso no se necesitaran
            Lcd_Set_Cursor(2, 8);
            Lcd_Write_String("   ");
        }
        // contador
        __delay_ms(1);
        PORTA = 0b00000011;
        __delay_ms(100);
        SSPBUF = 2;
        valorC = spiRead();
        __delay_ms(1);
        PORTA = 0b00000111;
        Lcd_Set_Cursor(2, 7);
        sprintf(k, "%d", valorC);
        Lcd_Write_String(k);
//        enviar(k);
                __delay_ms(100);

    }
}
//******************************************************************************
//  Configuración
//******************************************************************************

void setup(void) {
    TRISD = 0b00000000; // puerto D como salida
    TRISC = 0b10010000; //activamos el RX como entrada
    TRISE = 0b00000000;
    TRISB = 0b00000000;
    TRISA = 0b00000000;
    ANSEL = 0b00000011;
    PORTCbits.RC0 = 1; //limpiamos puertos
    PORTCbits.RC1 = 1;
    PORTCbits.RC2 = 1;
    PORTCbits.RC6 = 0;
    PORTCbits.RC7 = 0;
    PORTA = 0b11111111;
    PORTD = 0;
    PORTE = 0;
    PORTB = 0;
}


//******************************************************************************
//  Funciones
//******************************************************************************
//

float conversor(uint8_t val) {
    x = 0.0195 * val; //5V/256bits=0.0195 convertir de bits a voltaje
    return (x);
}

float conversor2(uint8_t val2) {
    vv = 1.95 * val2;
    return (vv);
}

//******************************************************************************
//  Interrupciones
//******************************************************************************

