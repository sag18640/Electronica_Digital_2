/*
 * File:   Proyecto 2.c
 * Author: Cristopher Sagastume 18640
 *
 * Created on 20 de marzo de 2021, 08:46 AM
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
#include <stdio.h>
#include <xc.h>
#include<stdint.h>
#include "I2C.h"
#include "UART.h"
#include "sensor.h"
#define _XTAL_FREQ 8000000
uint8_t Temp=0;
uint8_t recibido=0;
void setup(void);
void __interrupt() ISR(void);

void main(void) {
    //configuración de pines, comunicación I2C y comunicación UART
    setup();
    Config_USARTT();
    recibir();
    sensor_init();
    while(1){
        //se lee la temperatura del sensor
        Temp=read(TEMPERATURE_ADDRESS);
        //se envia la temperatura por uart al esp32
        enviar(Temp);
        //si el dato recibido de la uart es "+" se enciende un led RD0
        if (recibido == 0x2B){
            PORTDbits.RD0=1;
        }
        //si el dato recibido de la uart es "-" se apaga un led RD0
        else if(recibido == 0x2D){
            PORTDbits.RD0=0;
        }
        //si el dato recibido de la uart es "/" se apaga un led RD1
        if (recibido == 0x2F){
            PORTDbits.RD1=1;
        }
        //si el dato recibido de la uart es "&" se apaga un led RD1
        else if (recibido ==0x26){
            PORTDbits.RD1=0;
        }
    }
}

void setup(void) {
    TRISD = 0b00000000;// puerto D como salida
    TRISC = 0b10000000; //activamos el RX como entrada
    PORTC = 0;//limpiamos puertos
    PORTD = 0;
}

void __interrupt() ISR(void) {
    if (PIR1bits.RCIF == 1){//verificamos si fue interrupcion de la recepción USART
        if (RCSTAbits.OERR == 1) {//verificamos si hubo algún error de overrun
            RCSTAbits.CREN = 0; 
            __delay_us(300);
        } else {
            recibido = RCREG; //guardamos el valor recibido en una variable
        }
    }
}
