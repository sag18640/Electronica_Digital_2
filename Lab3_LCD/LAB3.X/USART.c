/*
 * File:   USART.c
 * Author: SAGASTUME
 *
 * Created on 9 de febrero de 2021, 12:41 AM
 */


#include <xc.h>
#define _XTAL_FREQ 8000000
void Config_USARTT(void) {
    TXSTAbits.SYNC = 0; //modo asincrono
    TXSTAbits.TXEN = 1; //activamos la transmisión
    TXSTAbits.BRGH = 0; //velocidad baja de baud rate (velocidad de trabajo de la comuniación)
    BAUDCTLbits.BRG16 = 1; //geneardor de 16 bits de baud rate
    SPBRG = 25; // tener un baudrate a 19230
    RCSTAbits.SPEN = 1; //se activa la comunación del RX/TX
    INTCONbits.GIE = 1; //se activan la interrupción global
    return;
}

void enviar(char *valor) { 
    TXREG = valor[0];
    while (TRMT == 0) {//bucle si TRMT sigue lleno 
    }
    TXREG = valor[1];//si TRMT esta vacio le cargamos otro valor al TXREG->TRMT
    while (TRMT == 0) {
    }
    TXREG = valor[2];
    while (TRMT == 0) {
    }
    TXREG = valor[3];
    while (TRMT == 0) {
    }
    TXREG = 0x20;
    while (TRMT == 0) {
    }
}

void recibir(void) {
    RCSTAbits.CREN = 1; //activamos el recibidor contunio asincrono
    RCSTAbits.FERR = 0; //apagamos el error de frame bit
    PIE1bits.RCIE = 1; //ecendemos la interrupción de recepción de la comunicación USART
    RCSTAbits.OERR = 0; //apagamos el overrun error

}
