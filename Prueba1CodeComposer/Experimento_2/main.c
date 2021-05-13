//Cristopher Sagastume 18640

#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "inc/hw_ints.h"
#include "driverlib/pin_map.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include "driverlib/systick.h"
#include "driverlib/uart.h"


uint32_t dato = 0x67;
bool c = 0;


int main (void){
    //Se configura el reloj a 40Mhz
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    //Se habilitan los led como salidas (Puerto F)
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_3);

    //Se configura el TMR0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC); //configuro el TMR0 de manera periodica
    TimerLoadSet(TIMER0_BASE, TIMER_BOTH, 20000000-1); //Se confitura el TMR0 a 32 bits medio ciclo
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT); //habilitamos la interrupcion cada timeout del tmr0
    IntEnable(INT_TIMER0A);
    TimerEnable(TIMER0_BASE, TIMER_A); //habilitamos el TMR0

    //Se configura la comunicacion UART
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);//Se habilita el UART0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);//Se habilita el puerto del UART0
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 |GPIO_PIN_1); //definicion de los pines del UART0
    //Configuracion de UART0 a 115200 con 8 bits de datos y 1 de stop sin pariedad
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |UART_CONFIG_PAR_NONE));

    //Se configuran las interrupciones de UART
    IntEnable(INT_UART0);
    UARTIntEnable(UART0_BASE, UART_INT_RX);
    UARTIntDisable(UART0_BASE, UART_INT_9BIT|UART_INT_TX |UART_INT_OE|UART_INT_BE|UART_INT_PE|UART_INT_FE|UART_INT_RT|UART_INT_DSR|UART_INT_DCD |UART_INT_CTS | UART_INT_RI);
    UARTFIFOLevelSet(UART0_BASE ,  UART_FIFO_TX1_8,  UART_FIFO_RX1_8);
    UARTEnable(UART0_BASE);

    IntMasterEnable();
    while(1){}

}

void Timer0IntHandler(void){
    //Se verifica el dato recibido
    if (c==0){
        //Si se recibe la letra r se enciende el led rojo
        if (dato == 'r'){
            GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1,0x02);
        }
        //Si se recibe la letra r se enciende el led azul
        if (dato == 'b'){// MANDO AZUL
            GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1,0x04);
        }
        //Si se recibe la letra r se enciende el led verde
        if (dato == 'g'){
            GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1,0x08);
        }
    c = 1;
    }else{
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x00);
        c = 0;
        }
    //si se genero una interrupcion se limpia la fuente de interrupcion para evitar que vuelva a entrar a la misma generando un buble infinito
        TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
}
void UART0IntHandler(void){
        //se verifica si hubo una recepcion de dato en el UART0
        if (UARTCharsAvail(UART0_BASE)==true){
            //Se guarda el dato recibido en la variable dato
            dato = UARTCharGet(UART0_BASE);
        }
        UARTRxErrorClear(UART0_BASE);
        //Se limpia la fuente de interrupcion de la interrpcion de la UART
            UARTIntClear(UART0_BASE, UART_INT_RX);
}
