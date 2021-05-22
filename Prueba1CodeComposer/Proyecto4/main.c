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


bool flag = 0;
bool flag2=0;
bool flag3=0;
bool flag4=0;
void DatosUart(char *Dat);

int main (void){
    //Se configura el reloj a 40Mhz
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    //Se habilitan los botones como entradas
    GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_2);
    GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_2, GPIO_STRENGTH_2MA,
                     GPIO_PIN_TYPE_STD_WPU);
    GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_3);
    GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_3, GPIO_STRENGTH_2MA,
                     GPIO_PIN_TYPE_STD_WPU);
    GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_6);
    GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_6, GPIO_STRENGTH_2MA,
                     GPIO_PIN_TYPE_STD_WPU);
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA,
                     GPIO_PIN_TYPE_STD_WPU);



    //Se habilitan los led como salidas
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_2);
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_3);
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_4);
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_5);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1);
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_7);
    GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_0);
    GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_1);


    //Se configura la comunicacion UART
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);//Se habilita el UART0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);//Se habilita el puerto del UART0
    GPIOPinConfigure(GPIO_PB0_U1RX);
    GPIOPinConfigure(GPIO_PB1_U1TX);
    GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 |GPIO_PIN_1); //definicion de los pines del UART0
    UARTClockSourceSet(UART1_BASE, UART_CLOCK_PIOSC);
    //Configuracion de UART0 a 115200 con 8 bits de datos y 1 de stop sin pariedad
    UARTConfigSetExpClk(UART1_BASE, 16000000, 115200, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |UART_CONFIG_PAR_NONE));


    UARTEnable(UART1_BASE);

    //Se coloca como estado inicial los led como Libres
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0xFF);
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_7, 0xFF);
    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0, 0xFF);
    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1, 0xFF);
    while (1)
    {   //Se verifica si se presionó el botón Parqueo1
        if (!GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_2))
        {
            SysCtlDelay(500000);    //ESPERO 500 MS PARA EL ANTIRREBOTE
            while (!GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_2)){}
            //Si El parqueo esta desocupado se pone Libre
            if (flag == 1)
            {
                GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2, 0x00);
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0xFF);
                SysCtlDelay(500000);
                DatosUart("a"); //Se envia por UART una cadena diferente para cada estado de cada semaforo
                SysCtlDelay(500000);
                flag = 0;
            } //si el parqueo esta libre se coloca ocupado
            else if (flag == 0)
            {
                GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2, 0xFF);
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0x00);
                SysCtlDelay(500000);
                DatosUart("aa");//Se envia por UART una cadena diferente para cada estado de cada semaforo
                SysCtlDelay(500000);
                flag = 1;
            }

        }//Se verifica si se presionó el botón Parqueo2
        else if (!GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_3))
        {
            SysCtlDelay(500000);    //ESPERO 500 MS PARA EL ANTIRREBOTE
            while (!GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_3))
            {
            }//Si El parqueo esta desocupado se pone Libre
            if (flag2 == 1)
            {
                GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, 0x00);
                GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_7, 0xFF);
                SysCtlDelay(500000);
                DatosUart("aaa");//Se envia por UART una cadena diferente para cada estado de cada semaforo
                SysCtlDelay(500000);
                flag2 = 0;
            }//si el parqueo esta libre se coloca ocupado
            else if (flag2 == 0)
            {
                GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, 0xFF);
                GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_7, 0x00);
                SysCtlDelay(500000);
                DatosUart("aaaa");//Se envia por UART una cadena diferente para cada estado de cada semaforo
                SysCtlDelay(500000);
                flag2 = 1;
                        }
        }//Se verifica si se presionó el botón Parqueo3
        else if (!GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_6))
        {
            SysCtlDelay(500000);    //ESPERO 500 MS PARA EL ANTIRREBOTE
            while (!GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_6))
            {
            }//Si El parqueo esta desocupado se pone Libre
            if (flag3 == 1)
            {
                GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4, 0x00);
                GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0, 0xFF);
                SysCtlDelay(500000);
                DatosUart("aaaaa");//Se envia por UART una cadena diferente para cada estado de cada semaforo
                SysCtlDelay(500000);
                flag3 = 0;
            }//si el parqueo esta libre se coloca ocupado
            else if (flag3 == 0)
            {
                GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4, 0xFF);
                GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0, 0x00);
                SysCtlDelay(500000);
                DatosUart("aaaaaa");//Se envia por UART una cadena diferente para cada estado de cada semaforo
                SysCtlDelay(500000);
                flag3 = 1;
            }

        }//Se verifica si se presionó el botón Parqueo4
        else if (!GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4))
        {
            SysCtlDelay(500000);    //ESPERO 500 MS PARA EL ANTIRREBOTE
            while (!GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4))
            {
            }//Si El parqueo esta desocupado se pone Libre
            if (flag4 == 1)
            {
                GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_5, 0x00);
                GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1, 0xFF);
                SysCtlDelay(500000);
                DatosUart("aaaaaaa");//Se envia por UART una cadena diferente para cada estado de cada semaforo
                SysCtlDelay(500000);
                flag4 = 0;
            }//si el parqueo esta libre se coloca ocupado
            else if (flag4 == 0)
            {
                GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_5, 0xFF);
                GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1, 0x00);
                SysCtlDelay(500000);
                DatosUart("aaaaaaaa");//Se envia por UART una cadena diferente para cada estado de cada semaforo
                SysCtlDelay(500000);
                flag4 = 1;
            }

        }

    }

}//Funcion que nos servíra para enviar datos (verifica las banderas de la uart y envia datos)
void DatosUart(char *Dat){
    while(UARTBusy(UART1_BASE));
    while(*Dat != '\0')
    {
        UARTCharPut(UART1_BASE, *Dat++);
    }
}

