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
uint8_t valorT;
uint8_t valorC;
uint8_t valorA;
//double valor1;
//double valor2;
float v;
float vv;
float x;
float p;
char s[20];

uint8_t count = 0;
uint8_t recibido;
uint8_t enviado;
//******************************************************************************
//  Prototipo de funciones
//******************************************************************************
void setup(void);
void __interrupt() ISR(void);
double conversor(uint8_t x);
double conversor2(uint8_t val);
//******************************************************************************
//  Ciclo Principal
//******************************************************************************

void main(void) {
    setup();
    Config_USARTT();
    recibir();
    Lcd_Init();
    Lcd_Clear();
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);

    while (1) {

        Lcd_Set_Cursor(1, 1); //colocamos el cursor en posicón 1,1
        Lcd_Write_String("S1:   S2:    S3:"); //escribimos los encabezados
        Lcd_Set_Cursor(2, 1); //colocamos el cursor en posición 2,1
        v = conversor(valorA);
        sprintf(s, "%3.2fV", v);
        Lcd_Write_String(s);
        Lcd_Set_Cursor(2, 12);
        p=conversor2(valorT);
        sprintf(s, "%3.2fV", p);
        Lcd_Write_String(s);
        Lcd_Set_Cursor(2, 7);
        sprintf(s, "%d", valorC);
        Lcd_Write_String(s);

        __delay_ms(200);
        PORTCbits.RC0 = 0;
        __delay_ms(1);
        SSPBUF = 0;
        valorT = spiRead();
        __delay_ms(1);
        PORTCbits.RC0 = 1;

        __delay_ms(200);
        PORTCbits.RC1 = 0;
        __delay_ms(1);
        SSPBUF = 1;
        valorA = spiRead();
        __delay_ms(1);
        PORTCbits.RC1 = 1;

        __delay_ms(200);
        PORTCbits.RC2 = 0;
        __delay_ms(1);
        SSPBUF = 2;
        valorC = spiRead();
        __delay_ms(1);
        PORTCbits.RC2 = 1;
        __delay_ms(200);


        //        v = conversor(valorA);
        //        sprintf(s, "%3.2fV", v); //guardamos los valores de la converison
        //        //        //en el array s con un formato de 2 decimales en caracter flotante
        //        Lcd_Write_String(s); //escribimos los valores en la LCD
        //        __delay_ms(200);
        //        PORTBbits.RB1 = 0;
        //        spiWrite(2);
        //        valorA = spiRead();
        //        enviar(s);
        //        __delay_ms(50);
        //        PORTBbits.RB1 = 1;
        //        Lcd_Set_Cursor(2, 7);
        //        sprintf(s, "%d", valorC);
        //        Lcd_Write_String(s);
        //        __delay_ms(200);
        //        PORTBbits.RB2 = 0;
        //        spiWrite(3);
        //        valorC = spiRead();
        //        enviar(s);
        //        __delay_ms(50);
        //        //        enviar(s);
        //        sprintf(s, "%3.2fC", valorT); //se guarda el valor de contador en s con
        //        //        //formado de decimal
        //        Lcd_Set_Cursor(2, 12);
        //        Lcd_Write_String(s);
        //
        //        __delay_ms(200);
        //        PORTBbits.RB0 = 0;
        //        PORTBbits.RB1 = 1;
        //        PORTBbits.RB2 = 1;
        //        spiWrite(1);
        //        valorT = spiRead();
        //        enviar(s);
        //        __delay_ms(50);











    }
}
//******************************************************************************
//  Configuración
//******************************************************************************

void setup(void) {
    TRISD = 0b00000000; // puerto D como salida
    TRISC = 0b10010000; //activamos el RX como entrada
    PORTCbits.RC0 = 1;
    PORTCbits.RC1 = 1;
    PORTCbits.RC2 = 1;
    TRISE = 0b00000000;
    TRISB = 0b00000000;
    ANSEL = 0b00000011;
    PORTC = 0; //limpiamos puertos
    PORTD = 0;
    PORTE = 0;
    PORTB = 0b11111111;
    INTCON = 0b11101000; //se configuran las interrupciones GIE, PIE, T0IE y RBIE
}


//******************************************************************************
//  Funciones
//******************************************************************************
//

float conversor(uint8_t val) {
    x = 0.0195 * val; //5V/256bits=0.0195 convertir de bits a voltaje
    return (x);
}
float conversor2(uint8_t val) {
    vv = 1.95 * val;
    return (vv);
}

//******************************************************************************
//  Interrupciones
//******************************************************************************

void __interrupt() ISR(void) {
    if (PIR1bits.RCIF == 1) {//verificamos si fue interrupcion de la recepción USART
        if (RCSTAbits.OERR == 1) {//verificamos si hubo algún error de overrun
            RCSTAbits.CREN = 0;
            __delay_us(300);
        } else {
            recibido = RCREG; //guardamos el valor recibido en una variable
        }
    }
}
