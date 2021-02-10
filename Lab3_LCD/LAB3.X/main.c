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
#include <stdio.h>
#include <xc.h>
#include<stdint.h>
#include "ADC.h"
#include "LCD.h"
#include "USART.h"
#define _XTAL_FREQ 8000000
//******************************************************************************
//  Variables
//******************************************************************************
uint8_t flag = 1;
uint8_t turno = 1;
uint8_t retorno;
uint8_t valor_AN0;
uint8_t valor_AN1;
double valor1;
double valor2;
float x;
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

//******************************************************************************
//  Ciclo Principal
//******************************************************************************

void main(void) {
    setup();
    Config_USARTT();
    recibir();
    Lcd_Init();
    Lcd_Clear();
    while (1) {
        Lcd_Set_Cursor(1, 1); //colocamos el cursor en posicón 1,1
        Lcd_Write_String("S1:   S2:    S3:"); //escribimos los encabezados
        retorno = ADC_con(flag, turno); //convertimos los valores de ADC
        valor1 = conversor(valor_AN0); //convertimos la resolución del ADC a
        //valores de voltaje en decimal
        valor2 = conversor(valor_AN1);
        Lcd_Set_Cursor(2, 1);//colocamos el cursor en posición 2,1
        sprintf(s, "%3.2fV", valor1); //guardamos los valores de la converison
        //en el array s con un formato de 2 decimales en caracter flotante
        Lcd_Write_String(s); //escribimos los valores en la LCD
        //        enviar(s);
        Lcd_Set_Cursor(2, 7);
        sprintf(s, "%3.2fV", valor2);
        Lcd_Write_String(s);
        enviar(s);
        __delay_ms(300);
        if (recibido == 0x2B) { //0x2B == signo "+" revisamos si lo recibimos
            count++; //aumentamos el contador
            recibido = 0;//vaciamos la variable de recibido
        }
        else if (recibido == 0x2D){//0x2D == signo "-" revisamos si lo recibimos
            count--;//disminuimos el contador
            recibido = 0; //vaciamos la variable de recibido
        }
        sprintf(s, "%d", count);//se guarda el valor de contador en s con
        //formado de decimal
        Lcd_Set_Cursor(2, 14);
        Lcd_Write_String(s);
        //        enviar(s);
        if (count >= 0 && count < 10) {//limpiamos los espacios de decenas y 
            //centenas si en caso no se necesitaran
            Lcd_Set_Cursor(2, 15); 
            Lcd_Write_String("   ");

        }
    }
}
//******************************************************************************
//  Configuración
//******************************************************************************

void setup(void) {
    TRISD = 0b00000000;// puerto D como salida
    TRISC = 0b10000000; //activamos el RX como entrada
    TRISE = 0b00000000;
    ANSEL = 0b00000011;
    PORTC = 0;//limpiamos puertos
    PORTD = 0;
    PORTE = 0;
}


//******************************************************************************
//  Funciones
//******************************************************************************

double conversor(uint8_t val) {
    x = 0.0195 * val; //5V/256bits=0.0195 convertir de bits a voltaje
    return (x);
}


//******************************************************************************
//  Interrupciones
//******************************************************************************

void __interrupt() ISR(void) {
    if (PIR1bits.ADIF == 1) {//verificamos si fue interrupt ADC
        if (retorno == 0) { //verificamos si fue del AN0
            valor_AN0 = ADRESH; //guardamos el valor
            turno = 0;//cambiamos a conversión de AN1
            flag = 1;
        } else if (retorno == 1) {//verificamos si fue del AN1
            valor_AN1 = ADRESH;
            turno = 1;//cambiamos a conversión de AN0
            flag = 1;//se setea flag para indicar que puede volver a convertir
        }
//        flag = 1;

        PIR1bits.ADIF = 0; //apagamos la bandera de ADC
    }
    if (PIR1bits.RCIF == 1){//verificamos si fue interrupcion de la recepción USART
        if (RCSTAbits.OERR == 1) {//verificamos si hubo algún error de overrun
            RCSTAbits.CREN = 0;
            __delay_us(300);
        } else {
            recibido = RCREG; //guardamos el valor recibido en una variable
        }
    }
}